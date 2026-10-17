#!/usr/bin/env python3
"""Convert pipe-delimited text grids into clean PNG images with square cells.

Example:
    python3 grid_to_image.py grid1.txt grid2.txt
    python3 grid_to_image.py *.txt --output-dir images --cell-size 90
"""

import argparse
import re
import sys
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont


def parse_grid(
    path: Path,
    min_rows: int | None = None,
    min_cols: int | None = None,
) -> list[list[str]]:
    """Read a grid made from rows such as '|  2  | 17 |'."""
    rows = []
    separator_col_counts = []

    try:
        lines = path.read_text(encoding="utf-8").splitlines()
    except OSError as exc:
        raise ValueError(f"could not read file: {exc}") from exc

    for line in lines:
        line = line.strip()
        if "|" not in line:
            continue

        # Strip only the outer border pipes so empty border cells are not discarded.
        if line.startswith("|"):
            line = line[1:]
        if line.endswith("|"):
            line = line[:-1]

        if not line:
            continue

        cells = [part.strip() for part in line.split("|")]

        # Check if this line is a horizontal separator row (e.g. |------|------|).
        # A separator must contain at least one separator character (-, =, +, _)
        # and consist entirely of separator characters and whitespace.
        is_separator = (
            bool(cells)
            and any(re.search(r"[-=+_]", cell) for cell in cells)
            and all(not cell or re.fullmatch(r"[-=+_\s]+", cell) for cell in cells)
        )

        if is_separator:
            separator_col_counts.append(len(cells))
            continue

        rows.append(cells)

    if not rows:
        raise ValueError("no grid rows found; expected rows containing '|' separators")

    column_count = max([len(row) for row in rows] + separator_col_counts, default=0)
    if min_cols is not None:
        column_count = max(column_count, min_cols)

    # Pad short rows so every row has the same number of columns.
    rows = [row + [""] * (column_count - len(row)) for row in rows]

    # Pad with blank rows if min_rows is specified and the grid has fewer rows.
    if min_rows is not None and len(rows) < min_rows:
        rows += [[""] * column_count for _ in range(min_rows - len(rows))]

    return rows


def load_font(size: int):
    """Use a readable system font when available, with a Pillow fallback."""
    candidates = [
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf",
        "/usr/share/fonts/truetype/freefont/FreeSans.ttf",
    ]
    for candidate in candidates:
        if Path(candidate).exists():
            return ImageFont.truetype(candidate, size=size)
    return ImageFont.load_default()


def render_grid(
    rows: list[list[str]],
    output_path: Path,
    cell_size: int = 80,
    line_width: int = 2,
    margin: int = 4,
) -> None:
    """Draw a grid as a PNG with square cells and centered labels."""
    row_count = len(rows)
    column_count = max(len(row) for row in rows)
    width = column_count * cell_size + 2 * margin
    height = row_count * cell_size + 2 * margin

    image = Image.new("RGB", (width, height), "white")
    draw = ImageDraw.Draw(image)

    font_size = max(12, int(cell_size * 0.28))
    font = load_font(font_size)
    line_color = (45, 45, 45)
    text_color = (20, 20, 20)

    # Draw cell labels, centered both horizontally and vertically.
    for r, row in enumerate(rows):
        for c, value in enumerate(row):
            if not value:
                continue

            x0 = margin + c * cell_size
            y0 = margin + r * cell_size
            bbox = draw.textbbox((0, 0), value, font=font)
            text_width = bbox[2] - bbox[0]
            text_height = bbox[3] - bbox[1]
            x = x0 + (cell_size - text_width) / 2 - bbox[0]
            y = y0 + (cell_size - text_height) / 2 - bbox[1]
            draw.text((x, y), value, fill=text_color, font=font)

    # Draw a complete, consistent grid.
    left, top = margin, margin
    right = margin + column_count * cell_size
    bottom = margin + row_count * cell_size

    for c in range(column_count + 1):
        x = left + c * cell_size
        draw.line((x, top, x, bottom), fill=line_color, width=line_width)

    for r in range(row_count + 1):
        y = top + r * cell_size
        draw.line((left, y, right, y), fill=line_color, width=line_width)

    output_path.parent.mkdir(parents=True, exist_ok=True)
    image.save(output_path)


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Convert pipe-delimited text grids into PNG images with square cells."
    )
    parser.add_argument("files", nargs="+", type=Path, help="one or more input text files")
    parser.add_argument(
        "-o", "--output-dir", type=Path, default=Path("images"),
        help="directory for generated PNGs (default: images)",
    )
    parser.add_argument(
        "--cell-size", type=int, default=80,
        help="width and height of each square cell in pixels (default: 80)",
    )
    parser.add_argument(
        "--line-width", type=int, default=2,
        help="grid line thickness in pixels (default: 2)",
    )
    parser.add_argument(
        "--rows", type=int, default=None,
        help="minimum number of rows (pads with blank rows if fewer)",
    )
    parser.add_argument(
        "--cols", type=int, default=None,
        help="minimum number of columns (pads with blank columns if fewer)",
    )
    parser.add_argument(
        "--size", type=int, default=None,
        help="sets both minimum rows and columns to this size (e.g. 10 for 10x10)",
    )
    args = parser.parse_args()

    min_rows = args.size if args.rows is None else args.rows
    min_cols = args.size if args.cols is None else args.cols

    if args.cell_size < 20:
        parser.error("--cell-size must be at least 20")
    if args.line_width < 1:
        parser.error("--line-width must be at least 1")

    failed = False
    for input_path in args.files:
        try:
            rows = parse_grid(input_path, min_rows=min_rows, min_cols=min_cols)
            output_path = args.output_dir / f"{input_path.stem}.png"
            render_grid(rows, output_path, args.cell_size, args.line_width)
            print(f"{input_path} -> {output_path} ({len(rows)} rows x {len(rows[0])} columns)")
        except ValueError as exc:
            print(f"Error: {input_path}: {exc}", file=sys.stderr)
            failed = True

    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(main())
