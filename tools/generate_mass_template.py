from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parent.parent
SOURCE_DIR = PROJECT_ROOT / "src"
OUTPUT_FILE = PROJECT_ROOT / "files.md"
EXTENSIONS = {".cpp", ".h"}

TEMPLATE_HEADER = """# Mass workflow

## Instructions

Replace this placeholder with the actions to perform on each claimed file.

## Files

"""


def main() -> None:
    files = sorted(
        path.relative_to(PROJECT_ROOT).as_posix()
        for path in SOURCE_DIR.rglob("*")
        if path.is_file() and path.suffix in EXTENSIONS
    )

    checklist = "".join(f"[ ] {path}\n" for path in files)
    content = TEMPLATE_HEADER + checklist
    OUTPUT_FILE.write_text(content, encoding="utf-8")


if __name__ == "__main__":
    main()