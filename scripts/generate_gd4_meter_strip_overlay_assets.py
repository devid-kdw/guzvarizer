#!/usr/bin/env python3
"""Generate GD-4 meter, strip, and overlay assets for Gužvarizer."""

from __future__ import annotations

import json
import random
from pathlib import Path

from PIL import Image, ImageDraw, ImageFilter, ImageFont


ROOT = Path(__file__).resolve().parents[1]
METERS_DIR = ROOT / "assets" / "ui" / "exports" / "meters"
STRIPS_DIR = ROOT / "assets" / "ui" / "exports" / "strips"
OVERLAYS_DIR = ROOT / "assets" / "ui" / "exports" / "overlays"
QA_DIR = ROOT / "assets" / "ui" / "qa"
MANIFEST_PATH = ROOT / "assets" / "ui" / "manifests" / "gd4_meter_strip_overlay_manifest.json"


def blank(size: tuple[int, int], color=(0, 0, 0, 0)) -> Image.Image:
    return Image.new("RGBA", size, color)


def font(size: int, bold: bool = False) -> ImageFont.FreeTypeFont | ImageFont.ImageFont:
    candidates = [
        "/System/Library/Fonts/Supplemental/Arial Bold.ttf" if bold else "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Supplemental/Helvetica.ttc",
    ]
    for path in candidates:
        try:
            return ImageFont.truetype(path, size=size)
        except OSError:
            continue
    return ImageFont.load_default()


def save_pair(img: Image.Image, out_dir: Path, base_name: str) -> tuple[str, str]:
    out_dir.mkdir(parents=True, exist_ok=True)
    file_1x = f"{base_name}_{img.width}x{img.height}.png"
    file_2x = f"{base_name}_{img.width * 2}x{img.height * 2}.png"
    img.save(out_dir / file_1x, format="PNG")
    img.resize((img.width * 2, img.height * 2), Image.Resampling.LANCZOS).save(
        out_dir / file_2x, format="PNG"
    )
    return file_1x, file_2x


def meter_top_visualizer_panel_frame() -> Image.Image:
    size = (220, 96)
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    d.rounded_rectangle((0, 0, 219, 95), radius=14, fill=(11, 18, 36, 226), outline=(68, 100, 146, 240), width=2)
    d.rounded_rectangle((6, 6, 213, 89), radius=10, outline=(70, 226, 255, 144), width=2)
    d.line((16, 70, 204, 70), fill=(98, 122, 168, 170), width=1)
    d.text((12, 10), "LEVEL", font=font(16, bold=True), fill=(205, 228, 254, 220))
    return img


def meter_bar_segment(accent: tuple[int, int, int]) -> Image.Image:
    size = (18, 64)
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    d.rounded_rectangle((1, 1, 17, 63), radius=4, fill=(11, 18, 30, 230), outline=(86, 112, 150, 210), width=1)
    for y in range(4, 60):
        t = y / 63.0
        r = int(accent[0] * (0.45 + 0.55 * (1.0 - t)))
        g = int(accent[1] * (0.45 + 0.55 * (1.0 - t)))
        b = int(accent[2] * (0.45 + 0.55 * (1.0 - t)))
        d.line((3, y, 15, y), fill=(r, g, b, 230))
    glow = blank(size)
    gd = ImageDraw.Draw(glow, "RGBA")
    gd.rounded_rectangle((2, 2, 16, 62), radius=4, outline=(accent[0], accent[1], accent[2], 125), width=2)
    img.alpha_composite(glow.filter(ImageFilter.GaussianBlur(1.2)))
    return img


def meter_gr_panel_frame() -> Image.Image:
    size = (220, 200)
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    d.rounded_rectangle((0, 0, 219, 199), radius=16, fill=(10, 16, 33, 230), outline=(67, 98, 142, 240), width=2)
    d.rounded_rectangle((8, 8, 211, 191), radius=12, outline=(255, 94, 176, 142), width=2)
    d.text((14, 12), "GAIN REDUCTION", font=font(15, bold=True), fill=(238, 224, 239, 232))
    return img


def meter_gr_segment_sprite() -> Image.Image:
    size = (16, 48)
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    d.rounded_rectangle((1, 1, 15, 47), radius=4, fill=(11, 18, 30, 230), outline=(84, 110, 150, 220), width=1)
    for y in range(4, 44):
        t = y / 47.0
        # Pink to orange gradient for GR movement.
        r = int(255 - 18 * t)
        g = int(84 + 110 * t)
        b = int(176 - 112 * t)
        d.line((3, y, 13, y), fill=(r, g, b, 235))
    glow = blank(size)
    gd = ImageDraw.Draw(glow, "RGBA")
    gd.rounded_rectangle((2, 2, 14, 46), radius=4, outline=(255, 110, 180, 138), width=2)
    img.alpha_composite(glow.filter(ImageFilter.GaussianBlur(1.0)))
    return img


def meter_scale_overlay_texture() -> Image.Image:
    size = (220, 200)
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    tick_x = [26, 68, 110, 152, 194]
    labels = ["0", "-10", "-20", "-30", "dB"]
    for x in tick_x:
        d.line((x, 58, x, 168), fill=(194, 214, 241, 120), width=1)
    f = font(13, bold=True)
    for x, label in zip(tick_x, labels):
        bbox = d.textbbox((0, 0), label, font=f)
        tw = bbox[2] - bbox[0]
        d.text((x - tw // 2, 172), label, font=f, fill=(201, 221, 247, 182))
    return img


def strip_color_strip_base() -> Image.Image:
    size = (980, 22)
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    d.rounded_rectangle((0, 0, 979, 21), radius=10, fill=(10, 16, 30, 235), outline=(62, 95, 140, 230), width=2)
    # Thin neon lane.
    d.rounded_rectangle((8, 7, 971, 15), radius=4, fill=(22, 34, 58, 225), outline=(84, 121, 172, 160), width=1)
    return img


def strip_segment_mask() -> Image.Image:
    size = (980, 22)
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    x = 10
    while x < 968:
        d.rounded_rectangle((x, 6, x + 14, 16), radius=2, fill=(255, 255, 255, 218))
        x += 18
    return img


def strip_hotspot_glow() -> Image.Image:
    size = (72, 32)
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    d.ellipse((12, 4, 60, 28), fill=(255, 174, 58, 175))
    d.ellipse((20, 8, 52, 24), fill=(255, 236, 136, 220))
    return img.filter(ImageFilter.GaussianBlur(2.2))


def overlay_grain() -> Image.Image:
    size = (1180, 760)
    img = blank(size)
    px = img.load()
    random.seed(410)
    for y in range(size[1]):
        for x in range(size[0]):
            n = random.randint(0, 255)
            a = 14 if random.random() > 0.6 else 7
            px[x, y] = (n, n, n, a)
    return img


def overlay_scanline() -> Image.Image:
    size = (1180, 760)
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    for y in range(0, size[1], 3):
        alpha = 22 if (y // 3) % 2 == 0 else 10
        d.line((0, y, size[0], y), fill=(164, 198, 236, alpha), width=1)
    return img


def overlay_vignette() -> Image.Image:
    size = (1180, 760)
    base = blank(size)
    d = ImageDraw.Draw(base, "RGBA")
    # Dark edge bands.
    d.rectangle((0, 0, size[0], 46), fill=(0, 0, 0, 120))
    d.rectangle((0, size[1] - 46, size[0], size[1]), fill=(0, 0, 0, 120))
    d.rectangle((0, 0, 46, size[1]), fill=(0, 0, 0, 120))
    d.rectangle((size[0] - 46, 0, size[0], size[1]), fill=(0, 0, 0, 120))

    feather = blank(size)
    fd = ImageDraw.Draw(feather, "RGBA")
    fd.rounded_rectangle((16, 16, size[0] - 16, size[1] - 16), radius=32, outline=(0, 0, 0, 110), width=32)
    return Image.alpha_composite(base, feather.filter(ImageFilter.GaussianBlur(12)))


def generate_assets() -> dict[str, list[str]]:
    assets: dict[str, list[str]] = {
        "meters": [],
        "strips": [],
        "overlays": [],
    }

    assets["meters"].extend(save_pair(meter_top_visualizer_panel_frame(), METERS_DIR, "gz_meter_top_visualizer_panel_frame_base"))
    assets["meters"].extend(save_pair(meter_bar_segment((64, 230, 255)), METERS_DIR, "gz_meter_in_bar_segment_sprite_base"))
    assets["meters"].extend(save_pair(meter_bar_segment((255, 172, 76)), METERS_DIR, "gz_meter_out_bar_segment_sprite_base"))
    assets["meters"].extend(save_pair(meter_gr_panel_frame(), METERS_DIR, "gz_meter_gr_panel_frame_base"))
    assets["meters"].extend(save_pair(meter_gr_segment_sprite(), METERS_DIR, "gz_meter_gr_segment_sprite_base"))
    assets["meters"].extend(save_pair(meter_scale_overlay_texture(), METERS_DIR, "gz_meter_scale_overlay_texture_base"))

    assets["strips"].extend(save_pair(strip_color_strip_base(), STRIPS_DIR, "gz_strip_color_strip_base_base"))
    assets["strips"].extend(save_pair(strip_segment_mask(), STRIPS_DIR, "gz_strip_color_strip_segment_mask_base"))
    assets["strips"].extend(save_pair(strip_hotspot_glow(), STRIPS_DIR, "gz_strip_color_strip_hotspot_glow"))

    assets["overlays"].extend(save_pair(overlay_grain(), OVERLAYS_DIR, "gz_overlay_grain_full_base"))
    assets["overlays"].extend(save_pair(overlay_scanline(), OVERLAYS_DIR, "gz_overlay_scanline_full_base"))
    assets["overlays"].extend(save_pair(overlay_vignette(), OVERLAYS_DIR, "gz_overlay_vignette_full_base"))
    return assets


def write_manifest(assets: dict[str, list[str]]) -> None:
    manifest = {
        "phase": "GD-4",
        "groups": ["meter", "strip", "overlay"],
        "exports_dirs": {
            "meters": "assets/ui/exports/meters",
            "strips": "assets/ui/exports/strips",
            "overlays": "assets/ui/exports/overlays",
        },
        "assets": assets,
        "total_exports": sum(len(v) for v in assets.values()),
        "notes": [
            "All runtime assets are PNG RGBA with @1x and @2x variants.",
            "Overlays are intended as top-layer passes (grain/scanline/vignette).",
        ],
    }
    MANIFEST_PATH.write_text(json.dumps(manifest, indent=2, ensure_ascii=True) + "\n", encoding="utf-8")


def make_preview() -> None:
    QA_DIR.mkdir(parents=True, exist_ok=True)
    canvas = blank((1180, 420), (8, 14, 30, 255))
    d = ImageDraw.Draw(canvas, "RGBA")
    d.rounded_rectangle((8, 8, 1172, 412), radius=16, outline=(66, 98, 142, 220), width=2)

    top_panel = Image.open(METERS_DIR / "gz_meter_top_visualizer_panel_frame_base_220x96.png").convert("RGBA")
    in_seg = Image.open(METERS_DIR / "gz_meter_in_bar_segment_sprite_base_18x64.png").convert("RGBA")
    out_seg = Image.open(METERS_DIR / "gz_meter_out_bar_segment_sprite_base_18x64.png").convert("RGBA")
    gr_panel = Image.open(METERS_DIR / "gz_meter_gr_panel_frame_base_220x200.png").convert("RGBA")
    gr_seg = Image.open(METERS_DIR / "gz_meter_gr_segment_sprite_base_16x48.png").convert("RGBA")
    scale = Image.open(METERS_DIR / "gz_meter_scale_overlay_texture_base_220x200.png").convert("RGBA")
    strip_base = Image.open(STRIPS_DIR / "gz_strip_color_strip_base_base_980x22.png").convert("RGBA")
    strip_mask = Image.open(STRIPS_DIR / "gz_strip_color_strip_segment_mask_base_980x22.png").convert("RGBA")
    hotspot = Image.open(STRIPS_DIR / "gz_strip_color_strip_hotspot_glow_72x32.png").convert("RGBA")

    canvas.alpha_composite(top_panel, (38, 42))
    for i in range(8):
        canvas.alpha_composite(in_seg, (58 + i * 20, 62))
    for i in range(8):
        canvas.alpha_composite(out_seg, (58 + i * 20, 62 - i * 2))

    canvas.alpha_composite(gr_panel, (292, 42))
    for i in range(10):
        canvas.alpha_composite(gr_seg, (312 + i * 19, 92 + i * 3))
    canvas.alpha_composite(scale, (292, 42))

    canvas.alpha_composite(strip_base, (38, 282))
    canvas.alpha_composite(strip_mask, (38, 282))
    for i in range(6):
        canvas.alpha_composite(hotspot, (88 + i * 156, 276))

    # Small overlay swatches.
    grain = Image.open(OVERLAYS_DIR / "gz_overlay_grain_full_base_1180x760.png").convert("RGBA").crop((0, 0, 320, 180))
    scan = Image.open(OVERLAYS_DIR / "gz_overlay_scanline_full_base_1180x760.png").convert("RGBA").crop((0, 0, 320, 180))
    vignette = Image.open(OVERLAYS_DIR / "gz_overlay_vignette_full_base_1180x760.png").convert("RGBA").crop((0, 0, 320, 180))

    for idx, img in enumerate((grain, scan, vignette)):
        x = 820
        y = 38 + idx * 124
        sw = blank((332, 104), (12, 20, 38, 240))
        sw.alpha_composite(img.resize((320, 96), Image.Resampling.LANCZOS), (6, 4))
        sw_d = ImageDraw.Draw(sw, "RGBA")
        sw_d.rounded_rectangle((0, 0, 331, 103), radius=10, outline=(80, 112, 162, 220), width=2)
        canvas.alpha_composite(sw, (x, y))

    p1 = QA_DIR / "gz_qa_gd4_meter_strip_overlay_preview_1180x420.png"
    p2 = QA_DIR / "gz_qa_gd4_meter_strip_overlay_preview_2360x840.png"
    canvas.save(p1, format="PNG")
    canvas.resize((2360, 840), Image.Resampling.LANCZOS).save(p2, format="PNG")


def main() -> None:
    assets = generate_assets()
    write_manifest(assets)
    make_preview()


if __name__ == "__main__":
    main()
