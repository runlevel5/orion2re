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
    row_groups: list[list[str]]
    columns: list[dict]
    column_names: list[str]
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


def Is_Ratio_Literal_(text: str) -> bool:
    if "/" not in text:
        return False
    left, right = text.split("/", 1)
    return bool(left) and bool(right) and Is_Decimal_Integer_(left) and Is_Decimal_Integer_(right)


def Is_Skip_Token_(token: s_token) -> bool:
    return (not token.is_quoted) and token.text == "-"


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

        row_groups: list[list[str]] = []
        raw_rows = entry.get("rows", [])
        if not isinstance(raw_rows, list):
            raise ValueError(f"parameter '{name}' has non-list rows")
        for row_group_index, row_group in enumerate(raw_rows):
            if not isinstance(row_group, list):
                raise ValueError(f"parameter '{name}' row group {row_group_index} is not a list")
            parsed_group: list[str] = []
            for row_name in row_group:
                if not isinstance(row_name, str):
                    raise ValueError(f"parameter '{name}' row group {row_group_index} contains a non-string selector")
                parsed_group.append(row_name)
            row_groups.append(parsed_group)

        columns: list[dict] = []
        column_names: list[str] = []
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
            columns.append(column)
            column_names.append(column_name)
            column_map[column_name] = column_index

        output[name] = s_schema_parameter(
            parameter_id=parameter_id,
            name=name,
            parameter_type=parameter_type,
            row_groups=row_groups,
            columns=columns,
            column_names=column_names,
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


def Selector_Can_Decode_(token: s_token, domain: list[str]) -> bool:
    if token.is_quoted:
        return False
    if Is_Integer_Literal_(token.text):
        return True
    return token.text in domain


def Value_Can_Decode_By_Column_(token: s_token, column: dict) -> bool:
    if Is_Skip_Token_(token):
        return True

    column_type = column.get("type")
    if column_type == "integer":
        return (not token.is_quoted) and Is_Integer_Literal_(token.text)
    if column_type == "enum":
        values = column.get("values", [])
        return (not token.is_quoted) and token.text in values
    if column_type == "mask":
        if token.is_quoted:
            return False
        if Is_Integer_Literal_(token.text):
            return True
        if token.text == "none":
            return True
        flags = column.get("flags", [])
        valid_codes = {flag.get("code") for flag in flags if isinstance(flag, dict) and isinstance(flag.get("code"), str)}
        parts = [part for part in token.text.split(",") if part]
        return bool(parts) and all(part in valid_codes for part in parts)
    if column_type == "string":
        return True
    return False


def Scalar_Value_Can_Decode_(token: s_token, parameter: s_schema_parameter) -> bool:
    if Is_Skip_Token_(token):
        return False
    parameter_type = parameter.parameter_type
    if parameter_type == "integer":
        return (not token.is_quoted) and Is_Integer_Literal_(token.text)
    if parameter_type == "enum":
        values = parameter.columns[0].get("values", []) if parameter.columns else []
        return (not token.is_quoted) and token.text in values
    if parameter_type == "string":
        return True
    return False


def Try_Validate_Table_Assignment_(parameter: s_schema_parameter, assignment: s_assignment) -> tuple[bool, str | None]:
    selector_count = len(assignment.selectors)
    row_group_count = len(parameter.row_groups)
    column_count = len(parameter.columns)

    if selector_count > row_group_count + 1:
        return False, f"too many selectors ({selector_count}) for table with {row_group_count} row groups"

    for selector_index in range(min(selector_count, row_group_count)):
        if not Selector_Can_Decode_(assignment.selectors[selector_index], parameter.row_groups[selector_index]):
            return False, f"selector {selector_index}='{assignment.selectors[selector_index].text}' does not decode in row group {selector_index}"

    start_column_index = 0
    if selector_count == row_group_count + 1:
        column_token = assignment.selectors[-1]
        if not Selector_Can_Decode_(column_token, parameter.column_names):
            return False, f"column selector '{column_token.text}' does not decode"
        if Is_Integer_Literal_(column_token.text):
            start_column_index = int(column_token.text, 0)
        else:
            start_column_index = parameter.column_map[column_token.text]

    if start_column_index < 0 or start_column_index >= column_count:
        return False, f"start column index {start_column_index} out of range"

    if len(assignment.values) > column_count - start_column_index:
        return False, f"rhs value count {len(assignment.values)} exceeds remaining column span {column_count - start_column_index}"

    for value_index, token in enumerate(assignment.values):
        column = parameter.columns[start_column_index + value_index]
        if not Value_Can_Decode_By_Column_(token, column):
            return False, f"rhs value '{token.text}' does not decode for column '{column['name']}' of type '{column.get('type')}'"

    return True, None


def Validate_Assumptions_(schema: dict[str, s_schema_parameter], assignments: list[s_assignment]) -> int:
    assignment_count = 0
    selector_count = 0
    missing_parameter_count = 0
    unresolved_selector_count = 0
    rhs_decode_error_count = 0
    table_assignment_count = 0
    table_bulk_assignment_count = 0
    table_skip_assignment_count = 0

    for assignment in assignments:
        assignment_count += 1
        selector_count += len(assignment.selectors)

        parameter = schema.get(assignment.name)
        if parameter is None:
            print(f"ERROR {assignment.file_path}:{assignment.line}: unknown parameter '{assignment.name}'")
            missing_parameter_count += 1
            continue

        if parameter.parameter_type != "table":
            if assignment.selectors:
                print(f"ERROR {assignment.file_path}:{assignment.line}: scalar parameter '{assignment.name}' does not accept selectors")
                unresolved_selector_count += 1
                continue
            if len(assignment.values) != 1:
                print(f"ERROR {assignment.file_path}:{assignment.line}: scalar parameter '{assignment.name}' expects exactly one value")
                rhs_decode_error_count += 1
                continue
            if not Scalar_Value_Can_Decode_(assignment.values[0], parameter):
                print(f"ERROR {assignment.file_path}:{assignment.line}: scalar value '{assignment.values[0].text}' does not decode for parameter '{assignment.name}'")
                rhs_decode_error_count += 1
                continue
            continue

        table_assignment_count += 1
        if len(assignment.values) > 1:
            table_bulk_assignment_count += 1
        if any(Is_Skip_Token_(token) for token in assignment.values):
            table_skip_assignment_count += 1

        ok, error = Try_Validate_Table_Assignment_(parameter, assignment)
        if not ok:
            print(f"ERROR {assignment.file_path}:{assignment.line}: parameter '{assignment.name}': {error}")
            if error and "selector" in error:
                unresolved_selector_count += 1
            else:
                rhs_decode_error_count += 1

    print()
    print("Summary")
    print(f"  assignments checked:          {assignment_count}")
    print(f"  selectors checked:            {selector_count}")
    print(f"  table assignments:            {table_assignment_count}")
    print(f"  table bulk assignments:       {table_bulk_assignment_count}")
    print(f"  table skip assignments:       {table_skip_assignment_count}")
    print(f"  unknown parameters:           {missing_parameter_count}")
    print(f"  selector/decode errors:       {unresolved_selector_count}")
    print(f"  rhs decode/shape errors:      {rhs_decode_error_count}")

    return 1 if (missing_parameter_count or unresolved_selector_count or rhs_decode_error_count) else 0


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Validate CFG assignments against config.json, including selector decoding, bulk notation, and skip-aware table traversal.")
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