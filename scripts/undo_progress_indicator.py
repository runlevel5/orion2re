from pathlib import Path
import sys


def main() -> int:
    path = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("validate.txt")
    start_line = int(sys.argv[2]) if len(sys.argv) > 2 else 1240

    lines = path.read_text(encoding="utf-8").splitlines(keepends=True)

    for index in range(start_line - 1, len(lines)):
        lines[index] = lines[index].replace("[.]", "[ ]")

    path.write_text("".join(lines), encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())