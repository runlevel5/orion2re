from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
SOURCE_DIR = ROOT / "src" / "game"
OUTPUT_FILE = ROOT / "files.md"
EXTENSIONS = {".cpp", ".h"}


def main() -> None:
    files = sorted(
        path.relative_to(ROOT).as_posix()
        for path in SOURCE_DIR.rglob("*")
        if path.is_file() and path.suffix in EXTENSIONS
    )

    content = "".join(f"[ ] {path}\n" for path in files)
    OUTPUT_FILE.write_text(content, encoding="utf-8")


if __name__ == "__main__":
    main()