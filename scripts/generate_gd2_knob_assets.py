#!/usr/bin/env python3
"""Generate GD-2 knob assets for Gužvarizer."""

from __future__ import annotations

import json
import math
from pathlib import Path

from PIL import Image, ImageDraw, ImageFilter, ImageFont


ROOT = Path(__file__).resolve().parents[1]
KNOB_EXPORT_ROOT = ROOT / "assets" / "ui" / "exports" / "knobs"
MANIFEST_PATH = ROOT / "assets" / "ui" / "manifests" / "gd2_knob_manifest.json"
QA_DIR = ROOT / "assets" / "ui" / "qa"

LAYERS = ("ring_base", "ring_active", "ticks_base", "cap_base", "pointer_base", "glow_base")


FAMILIES = {
    "main": {
        "sizes": [256, 512],
        "knobs": {
            "threshold": (58, 232, 255),
            "attack": (70, 255, 199),
            "release": (255, 88, 204),
            "ratio": (255, 164, 56),
            "output": (255, 204, 74),
        },
    },
    "tone": {
        "sizes": [224, 448],
        "knobs": {
            "saturation": (255, 120, 58),
            "harmonics": (255, 78, 180),
            "grit": (188, 108, 255),
            "air": (74, 233, 255),
            "color": (122, 192, 255),
        },
    },
    "lfo": {
        "sizes": [192, 384],
        "knobs": {
            "lfo_rate": (255, 235, 96),
        },
    },
}


def lerp(a: int, b: int, t: float) -> int:
    return int(a + (b - a) * t)


def mix(c1: tuple[int, int, int], c2: tuple[int, int, int], t: float) -> tuple[int, int, int]:
    return (lerp(c1[0], c2[0], t), lerp(c1[1], c2[1], t), lerp(c1[2], c2[2], t))


def blank(size: int) -> Image.Image:
    return Image.new("RGBA", (size, size), (0, 0, 0, 0))


def ring_base(size: int, accent: tuple[int, int, int]) -> Image.Image:
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    margin = int(size * 0.08)
    w = max(4, int(size * 0.035))
    d.ellipse((margin, margin, size - margin, size - margin), outline=(56, 75, 112, 210), width=w)
    inner_margin = margin + int(size * 0.035)
    d.ellipse((inner_margin, inner_margin, size - inner_margin, size - inner_margin), outline=(accent[0], accent[1], accent[2], 76), width=max(2, w // 2))
    return img


def ring_active(size: int, accent: tuple[int, int, int]) -> Image.Image:
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    margin = int(size * 0.08)
    w = max(5, int(size * 0.045))
    arc_box = (margin, margin, size - margin, size - margin)
    d.arc(arc_box, start=130, end=402, fill=(accent[0], accent[1], accent[2], 238), width=w)

    # Hot tip to suggest pointer destination.
    d.arc(arc_box, start=118, end=145, fill=(255, 255, 255, 235), width=max(3, w // 2))
    return img.filter(ImageFilter.GaussianBlur(max(0.5, size * 0.0025)))


def ticks_layer(size: int, accent: tuple[int, int, int]) -> Image.Image:
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    c = size / 2.0
    outer_r = size * 0.47
    inner_r_minor = size * 0.425
    inner_r_major = size * 0.392
    tick_count = 41
    start_a = math.radians(132)
    end_a = math.radians(408)
    for i in range(tick_count):
        t = i / max(1, tick_count - 1)
        a = start_a + (end_a - start_a) * t
        major = (i % 5) == 0
        ir = inner_r_major if major else inner_r_minor
        x0 = c + math.cos(a) * ir
        y0 = c + math.sin(a) * ir
        x1 = c + math.cos(a) * outer_r
        y1 = c + math.sin(a) * outer_r
        alpha = 220 if major else 130
        width = max(2, int(size * 0.012)) if major else max(1, int(size * 0.007))
        d.line((x0, y0, x1, y1), fill=(accent[0], accent[1], accent[2], alpha), width=width)
    return img


def cap_layer(size: int, accent: tuple[int, int, int]) -> Image.Image:
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    c = size / 2.0
    outer_r = int(size * 0.31)
    inner_r = int(size * 0.26)

    # Radial cap body.
    for r in range(outer_r, 0, -1):
        t = 1.0 - (r / max(1, outer_r))
        col = mix((16, 24, 41), (68, 78, 102), t)
        alpha = lerp(236, 180, t)
        d.ellipse((c - r, c - r, c + r, c + r), fill=(col[0], col[1], col[2], alpha))

    # Inner gloss.
    gloss = blank(size)
    gd = ImageDraw.Draw(gloss, "RGBA")
    gd.ellipse((c - inner_r, c - inner_r, c + inner_r, c + inner_r), fill=(155, 170, 196, 35))
    gloss = gloss.filter(ImageFilter.GaussianBlur(max(1.0, size * 0.01)))
    img.alpha_composite(gloss)

    # Accent rim.
    rim_r = int(size * 0.33)
    d.ellipse((c - rim_r, c - rim_r, c + rim_r, c + rim_r), outline=(accent[0], accent[1], accent[2], 118), width=max(2, int(size * 0.01)))
    return img


def pointer_layer(size: int, accent: tuple[int, int, int]) -> Image.Image:
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    c = size / 2.0
    tip_y = c - size * 0.345
    neck_y = c - size * 0.08
    half_w = size * 0.034

    pointer_pts = [
        (c, tip_y),
        (c + half_w * 1.2, neck_y),
        (c, neck_y + size * 0.06),
        (c - half_w * 1.2, neck_y),
    ]
    d.polygon(pointer_pts, fill=(accent[0], accent[1], accent[2], 245))
    d.polygon([(c, tip_y + 3), (c + half_w * 0.68, neck_y - 2), (c - half_w * 0.68, neck_y - 2)], fill=(255, 255, 255, 200))
    return img.filter(ImageFilter.GaussianBlur(max(0.3, size * 0.0016)))


def glow_layer(size: int, accent: tuple[int, int, int]) -> Image.Image:
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    margin = int(size * 0.05)
    d.ellipse((margin, margin, size - margin, size - margin), outline=(accent[0], accent[1], accent[2], 96), width=max(6, int(size * 0.06)))
    return img.filter(ImageFilter.GaussianBlur(max(2.0, size * 0.02)))


def get_font(size: int) -> ImageFont.FreeTypeFont | ImageFont.ImageFont:
    candidates = [
        "/System/Library/Fonts/Supplemental/Menlo.ttc",
        "/System/Library/Fonts/Supplemental/Arial Bold.ttf",
    ]
    for font_path in candidates:
        try:
            return ImageFont.truetype(font_path, size=size)
        except OSError:
            continue
    return ImageFont.load_default()


def render_layers(size: int, accent: tuple[int, int, int]) -> dict[str, Image.Image]:
    return {
        "ring_base": ring_base(size, accent),
        "ring_active": ring_active(size, accent),
        "ticks_base": ticks_layer(size, accent),
        "cap_base": cap_layer(size, accent),
        "pointer_base": pointer_layer(size, accent),
        "glow_base": glow_layer(size, accent),
    }


def file_name(knob_id: str, layer: str, size: int) -> str:
    variant, state = layer.split("_", 1)
    return f"gz_knob_{knob_id}_{variant}_{state}_{size}x{size}.png"


def export_knobs() -> dict:
    QA_DIR.mkdir(parents=True, exist_ok=True)
    output: dict[str, list[str]] = {"main": [], "tone": [], "lfo": []}
    preview_inputs: dict[str, list[tuple[str, int]]] = {"main": [], "tone": [], "lfo": []}

    for family_name, family in FAMILIES.items():
        family_dir = KNOB_EXPORT_ROOT / family_name
        family_dir.mkdir(parents=True, exist_ok=True)
        sizes = family["sizes"]
        knob_map: dict[str, tuple[int, int, int]] = family["knobs"]  # type: ignore[assignment]

        for knob_id, accent in knob_map.items():
            for size in sizes:
                layers = render_layers(size, accent)
                for layer_name, image in layers.items():
                    fname = file_name(knob_id, layer_name, size)
                    image.save(family_dir / fname, format="PNG")
                    output[family_name].append(fname)
                if size == sizes[0]:
                    preview_inputs[family_name].append((knob_id, size))

    generate_previews(preview_inputs)
    return output


def composed_knob_image(family: str, knob_id: str, size: int) -> Image.Image:
    base = blank(size)
    family_dir = KNOB_EXPORT_ROOT / family
    stack = [
        file_name(knob_id, "glow_base", size),
        file_name(knob_id, "ring_base", size),
        file_name(knob_id, "ring_active", size),
        file_name(knob_id, "ticks_base", size),
        file_name(knob_id, "cap_base", size),
        file_name(knob_id, "pointer_base", size),
    ]
    for fname in stack:
        base.alpha_composite(Image.open(family_dir / fname).convert("RGBA"))
    return base


def generate_previews(preview_inputs: dict[str, list[tuple[str, int]]]) -> None:
    for family, rows in preview_inputs.items():
        if not rows:
            continue
        item_size = rows[0][1]
        spacing = 36
        cols = len(rows)
        width = cols * item_size + (cols + 1) * spacing
        height = item_size + 110
        canvas = Image.new("RGBA", (width, height), (8, 14, 30, 255))
        d = ImageDraw.Draw(canvas, "RGBA")
        d.rounded_rectangle((8, 8, width - 8, height - 8), radius=20, outline=(68, 104, 162, 220), width=2)

        font = get_font(max(16, item_size // 11))
        for i, (knob_id, size) in enumerate(rows):
            x = spacing + i * (item_size + spacing)
            y = 24
            knob_img = composed_knob_image(family, knob_id, size)
            canvas.alpha_composite(knob_img, (x, y))
            label = knob_id.upper()
            text_box = d.textbbox((0, 0), label, font=font)
            tw = text_box[2] - text_box[0]
            d.text((x + (item_size - tw) // 2, y + item_size + 12), label, font=font, fill=(220, 236, 255, 230))

        out_1x = QA_DIR / f"gz_qa_gd2_knobs_{family}_preview_{width}x{height}.png"
        out_2x = QA_DIR / f"gz_qa_gd2_knobs_{family}_preview_{width * 2}x{height * 2}.png"
        canvas.save(out_1x, format="PNG")
        canvas.resize((width * 2, height * 2), Image.Resampling.LANCZOS).save(out_2x, format="PNG")


def write_manifest(exports: dict[str, list[str]]) -> None:
    manifest = {
        "phase": "GD-2",
        "group": "knob",
        "layers": list(LAYERS),
        "families": {
            "main": {
                "knobs": list(FAMILIES["main"]["knobs"].keys()),
                "sizes": FAMILIES["main"]["sizes"],
                "exports_dir": "assets/ui/exports/knobs/main",
                "count": len(exports["main"]),
            },
            "tone": {
                "knobs": list(FAMILIES["tone"]["knobs"].keys()),
                "sizes": FAMILIES["tone"]["sizes"],
                "exports_dir": "assets/ui/exports/knobs/tone",
                "count": len(exports["tone"]),
            },
            "lfo": {
                "knobs": list(FAMILIES["lfo"]["knobs"].keys()),
                "sizes": FAMILIES["lfo"]["sizes"],
                "exports_dir": "assets/ui/exports/knobs/lfo",
                "count": len(exports["lfo"]),
            },
        },
        "total_exports": len(exports["main"]) + len(exports["tone"]) + len(exports["lfo"]),
        "exports": exports,
    }
    MANIFEST_PATH.write_text(json.dumps(manifest, indent=2, ensure_ascii=True) + "\n", encoding="utf-8")


def main() -> None:
    exports = export_knobs()
    write_manifest(exports)


if __name__ == "__main__":
    main()
