#!/usr/bin/env python3

import argparse
import json
import sys
from dataclasses import dataclass
from pathlib import Path


@dataclass
class s_token:
    text: str
    is_quoted: bool


@dataclass
class s_statement:
    file_path: str
    line: int
    text: str


@dataclass
class s_assignment:
    file_path: str
    line: int
    name: str
    selectors: list[s_token]
    values: list[s_token]


@dataclass
class s_schema_parameter:
    parameter_id: int
    name: str
    parameter_type: str
    row_groups: list[dict[str, int]]
    column_map: dict[str, int]


def Is_Decimal_Integer_(text: str) -> bool:
    if text == "0":
        return True
    if text.startswith("-"):
        return len(text) > 1 and text[1:].isdigit()
    return text.isdigit()


def Is_Hex_Integer_(text: str) -> bool:
    if text.startswith(("0x", "0X")):
        digits = text[2:]
    elif text.startswith(("-0x", "-0X")):
        digits = text[3:]
    else:
        return False
    return len(digits) > 0 and all(ch in "0123456789abcdefABCDEF" for ch in digits)


def Is_Binary_Integer_(text: str) -> bool:
    if text.startswith("-"):
        digits = text[1:]
    else:
        digits = text

    if digits == "0":
        return True
    if not digits.startswith("0"):
        return False
    if digits.startswith(("0x", "0X")):
        return False
    return all(ch in "01" for ch in digits)


def Is_Integer_Literal_(text: str) -> bool:
    return Is_Hex_Integer_(text) or Is_Binary_Integer_(text) or Is_Decimal_Integer_(text)


def Load_Schema_(schema_path: Path) -> dict[str, s_schema_parameter]:
    with schema_path.open("r", encoding="utf-8") as input_file:
        root = json.load(input_file)

    parameters = root.get("parameters")
    if not isinstance(parameters, list):
        raise ValueError("config.json does not contain a top-level 'parameters' list")

    output: dict[str, s_schema_parameter] = {}
    for parameter_id, entry in enumerate(parameters):
        if not isinstance(entry, dict):
            raise ValueError(f"parameters[{parameter_id}] is not an object")

        name = entry.get("name")
        parameter_type = entry.get("type")
        if not isinstance(name, str) or not isinstance(parameter_type, str):
            raise ValueError(f"parameters[{parameter_id}] is missing string name/type")

        row_groups: list[dict[str, int]] = []
        raw_rows = entry.get("rows", [])
        if not isinstance(raw_rows, list):
            raise ValueError(f"parameter '{name}' has non-list rows")

        for row_group_index, row_group in enumerate(raw_rows):
            if not isinstance(row_group, list):
                raise ValueError(f"parameter '{name}' row group {row_group_index} is not a list")

            row_map: dict[str, int] = {}
            for row_index, row_name in enumerate(row_group):
                if not isinstance(row_name, str):
                    raise ValueError(f"parameter '{name}' row group {row_group_index} contains a non-string selector")
                if row_name in row_map:
                    raise ValueError(f"parameter '{name}' row group {row_group_index} contains duplicate selector '{row_name}'")
                row_map[row_name] = row_index
            row_groups.append(row_map)

        column_map: dict[str, int] = {}
        raw_columns = entry.get("columns", [])
        if not isinstance(raw_columns, list):
            raise ValueError(f"parameter '{name}' has non-list columns")

        for column_index, column in enumerate(raw_columns):
            if not isinstance(column, dict):
                raise ValueError(f"parameter '{name}' column {column_index} is not an object")
            column_name = column.get("name")
            if not isinstance(column_name, str):
                raise ValueError(f"parameter '{name}' column {column_index} is missing string name")
            if column_name in column_map:
                raise ValueError(f"parameter '{name}' contains duplicate column selector '{column_name}'")
            column_map[column_name] = column_index

        output[name] = s_schema_parameter(
            parameter_id=parameter_id,
            name=name,
            parameter_type=parameter_type,
            row_groups=row_groups,
            column_map=column_map,
        )

    return output


def Scan_Statements_(file_path: Path) -> list[s_statement]:
    text = file_path.read_text(encoding="utf-8", errors="replace")

    statements: list[s_statement] = []
    current_chars: list[str] = []
    current_line = 1
    statement_line = 1
    have_statement_text = False

    in_string = False
    escape_next = False
    in_comment = False

    for ch in text:
        if in_comment:
            if ch == "\n":
                in_comment = False
                current_line += 1
            continue

        if in_string:
            current_chars.append(ch)
            if escape_next:
                escape_next = False
            elif ch == "^":
                escape_next = True
            elif ch == '"':
                in_string = False
            elif ch == "\n":
                current_line += 1
            continue

        if ch == '#':
            in_comment = True
            continue

        if ch == '"':
            if not have_statement_text and not ch.isspace():
                statement_line = current_line
                have_statement_text = True
            current_chars.append(ch)
            in_string = True
            continue

        if ch == ';':
            statement_text = "".join(current_chars).strip()
            if statement_text:
                statements.append(s_statement(str(file_path), statement_line, statement_text))
            current_chars.clear()
            have_statement_text = False
            continue

        if ch == "\n":
            if not have_statement_text and current_chars:
                current_chars.clear()
            current_chars.append(ch)
            current_line += 1
            continue

        if not have_statement_text and not ch.isspace():
            statement_line = current_line
            have_statement_text = True

        current_chars.append(ch)

    trailing_text = "".join(current_chars).strip()
    if trailing_text:
        statements.append(s_statement(str(file_path), statement_line, trailing_text))

    return statements


def Tokenize_(text: str) -> list[s_token]:
    tokens: list[s_token] = []
    current_chars: list[str] = []
    in_string = False
    escape_next = False

    def Flush_Atom_() -> None:
        if current_chars:
            tokens.append(s_token("".join(current_chars), False))
            current_chars.clear()

    for ch in text:
        if in_string:
            if escape_next:
                current_chars.append(ch)
                escape_next = False
            elif ch == '^':
                escape_next = True
            elif ch == '"':
                tokens.append(s_token("".join(current_chars), True))
                current_chars.clear()
                in_string = False
            else:
                current_chars.append(ch)
            continue

        if ch.isspace():
            Flush_Atom_()
            continue

        if ch == '=':
            Flush_Atom_()
            tokens.append(s_token("=", False))
            continue

        if ch == '"':
            Flush_Atom_()
            in_string = True
            continue

        current_chars.append(ch)

    if in_string:
        raise ValueError("unterminated quoted string")

    Flush_Atom_()
    return tokens


def Parse_Assignments_(statements: list[s_statement]) -> tuple[list[s_assignment], list[str]]:
    assignments: list[s_assignment] = []
    errors: list[str] = []

    for statement in statements:
        try:
            tokens = Tokenize_(statement.text)
        except ValueError as exc:
            errors.append(f"{statement.file_path}:{statement.line}: {exc}")
            continue

        if not tokens:
            continue

        eq_index = -1
        for index, token in enumerate(tokens):
            if token.text == "=":
                eq_index = index
                break

        if eq_index < 0:
            continue

        lhs_tokens = tokens[:eq_index]
        rhs_tokens = tokens[eq_index + 1:]

        if not lhs_tokens:
            errors.append(f"{statement.file_path}:{statement.line}: missing assignment name")
            continue
        if not rhs_tokens:
            errors.append(f"{statement.file_path}:{statement.line}: missing assignment value")
            continue

        assignments.append(s_assignment(
            file_path=statement.file_path,
            line=statement.line,
            name=lhs_tokens[0].text,
            selectors=lhs_tokens[1:],
            values=rhs_tokens,
        ))

    return assignments, errors


def Get_Selector_Shapes_(parameter: s_schema_parameter) -> list[list[dict[str, int]]]:
    shapes: list[list[dict[str, int]]] = []
    row_groups = parameter.row_groups
    column_map = parameter.column_map

    if not row_groups and column_map:
        shapes.append([column_map])
        return shapes

    if row_groups:
        shapes.append(list(row_groups))
        if column_map:
            shapes.append(list(row_groups) + [column_map])

    return shapes


def Build_Selector_Vocabulary_(parameter: s_schema_parameter) -> set[str]:
    vocabulary: set[str] = set()
    for row_group in parameter.row_groups:
        vocabulary.update(row_group.keys())
    vocabulary.update(parameter.column_map.keys())
    return vocabulary


def Validate_Assumptions_(schema: dict[str, s_schema_parameter], assignments: list[s_assignment]) -> int:
    assignment_count = 0
    selector_count = 0
    numeric_literal_count = 0
    mapped_selector_count = 0
    missing_parameter_count = 0
    shape_error_count = 0
    unresolved_selector_count = 0

    for assignment in assignments:
        assignment_count += 1
        parameter = schema.get(assignment.name)
        if parameter is None:
            print(f"ERROR {assignment.file_path}:{assignment.line}: unknown parameter '{assignment.name}'")
            missing_parameter_count += 1
            continue

        if not assignment.selectors:
            continue

        selector_count += len(assignment.selectors)

        if parameter.parameter_type != "table":
            print(
                f"ERROR {assignment.file_path}:{assignment.line}: parameter '{assignment.name}' has selectors "
                f"but schema type is '{parameter.parameter_type}'")
            shape_error_count += 1
            continue

        shapes = Get_Selector_Shapes_(parameter)
        matching_shape = None
        for shape in shapes:
            if len(shape) != len(assignment.selectors):
                continue

            shape_matches = True
            for selector_token, selector_map in zip(assignment.selectors, shape):
                if selector_token.is_quoted:
                    shape_matches = False
                    break
                if Is_Integer_Literal_(selector_token.text):
                    continue
                if selector_token.text not in selector_map:
                    shape_matches = False
                    break
            if shape_matches:
                matching_shape = shape
                break

        if matching_shape is not None:
            for selector_token, selector_map in zip(assignment.selectors, matching_shape):
                if Is_Integer_Literal_(selector_token.text):
                    numeric_literal_count += 1
                elif selector_token.text in selector_map:
                    mapped_selector_count += 1
            continue

        vocabulary = Build_Selector_Vocabulary_(parameter)
        selector_errors: list[str] = []
        for selector_token in assignment.selectors:
            if selector_token.is_quoted:
                selector_errors.append(f"quoted selector '{selector_token.text}'")
                unresolved_selector_count += 1
                continue
            if Is_Integer_Literal_(selector_token.text):
                numeric_literal_count += 1
                continue
            if selector_token.text in vocabulary:
                mapped_selector_count += 1
            else:
                selector_errors.append(f"unresolved selector '{selector_token.text}'")
                unresolved_selector_count += 1

        if selector_errors:
            print(
                f"ERROR {assignment.file_path}:{assignment.line}: parameter '{assignment.name}' selectors "
                f"could not be fully decoded: {', '.join(selector_errors)}")
        else:
            print(
                f"ERROR {assignment.file_path}:{assignment.line}: parameter '{assignment.name}' selector shape "
                f"does not match schema (selector_count={len(assignment.selectors)}, "
                f"expected one of {[len(shape) for shape in shapes]})")
            shape_error_count += 1

    print()
    print("Summary")
    print(f"  assignments checked:      {assignment_count}")
    print(f"  selectors checked:        {selector_count}")
    print(f"  numeric literal selectors:{numeric_literal_count}")
    print(f"  schema-mapped selectors:  {mapped_selector_count}")
    print(f"  unknown parameters:       {missing_parameter_count}")
    print(f"  selector shape errors:    {shape_error_count}")
    print(f"  unresolved selectors:     {unresolved_selector_count}")

    return 1 if (missing_parameter_count or shape_error_count or unresolved_selector_count) else 0


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Validate that every CFG assignment selector after the primary parameter name can decode to a numeric id.")
    parser.add_argument("--schema", required=True, help="Path to config.json schema file")
    parser.add_argument("cfg", nargs="+", help="One or more CFG files to validate")
    args = parser.parse_args()

    try:
        schema = Load_Schema_(Path(args.schema))
    except Exception as exc:
        print(f"Failed to load schema: {exc}", file=sys.stderr)
        return 2

    all_assignments: list[s_assignment] = []
    parse_errors: list[str] = []
    for cfg_path_text in args.cfg:
        cfg_path = Path(cfg_path_text)
        try:
            statements = Scan_Statements_(cfg_path)
        except Exception as exc:
            print(f"Failed to read {cfg_path}: {exc}", file=sys.stderr)
            return 2

        assignments, assignment_errors = Parse_Assignments_(statements)
        all_assignments.extend(assignments)
        parse_errors.extend(assignment_errors)

    for error in parse_errors:
        print(f"ERROR {error}")

    validation_result = Validate_Assumptions_(schema, all_assignments)
    if parse_errors:
        return 1
    return validation_result


if __name__ == "__main__":
    sys.exit(main())