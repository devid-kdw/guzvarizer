#!/usr/bin/env python3
"""Generate GD-3 control assets for Gužvarizer."""

from __future__ import annotations

import json
from pathlib import Path

from PIL import Image, ImageDraw, ImageFilter, ImageFont


ROOT = Path(__file__).resolve().parents[1]
EXPORT_DIR = ROOT / "assets" / "ui" / "exports" / "controls"
QA_DIR = ROOT / "assets" / "ui" / "qa"
MANIFEST_PATH = ROOT / "assets" / "ui" / "manifests" / "gd3_controls_manifest.json"


def blank(size: tuple[int, int]) -> Image.Image:
    return Image.new("RGBA", size, (0, 0, 0, 0))


def get_font(size: int, bold: bool = False) -> ImageFont.FreeTypeFont | ImageFont.ImageFont:
    candidates = [
        "/System/Library/Fonts/Supplemental/Arial Unicode.ttf",
        "/System/Library/Fonts/Supplemental/Arial Bold.ttf" if bold else "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Supplemental/Helvetica.ttc",
    ]
    for path in candidates:
        try:
            return ImageFont.truetype(path, size=size)
        except OSError:
            continue
    return ImageFont.load_default()


def draw_center_text(
    img: Image.Image, text: str, y: int, color: tuple[int, int, int, int], size: int, bold: bool = True
) -> None:
    d = ImageDraw.Draw(img, "RGBA")
    font = get_font(size, bold=bold)
    box = d.textbbox((0, 0), text, font=font)
    tw, th = box[2] - box[0], box[3] - box[1]
    x = (img.width - tw) // 2
    d.text((x, y - th // 2), text, font=font, fill=color)


def save_pair(img: Image.Image, base_name: str) -> tuple[str, str]:
    EXPORT_DIR.mkdir(parents=True, exist_ok=True)
    file_1x = f"{base_name}_{img.width}x{img.height}.png"
    file_2x = f"{base_name}_{img.width * 2}x{img.height * 2}.png"
    img.save(EXPORT_DIR / file_1x, format="PNG")
    img.resize((img.width * 2, img.height * 2), Image.Resampling.LANCZOS).save(EXPORT_DIR / file_2x, format="PNG")
    return file_1x, file_2x


def bypass_button(state: str) -> Image.Image:
    size = (320, 96)
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")

    bg = {
        "off": (18, 28, 54, 232),
        "on": (12, 64, 76, 236),
        "hover": (20, 40, 74, 238),
        "pressed": (10, 22, 38, 244),
        "disabled": (20, 24, 34, 195),
    }[state]
    stroke = {
        "off": (255, 80, 156, 220),
        "on": (72, 238, 188, 228),
        "hover": (84, 215, 255, 230),
        "pressed": (255, 150, 62, 225),
        "disabled": (108, 120, 140, 140),
    }[state]
    text = {
        "off": (245, 215, 225, 245),
        "on": (220, 255, 245, 250),
        "hover": (234, 248, 255, 250),
        "pressed": (255, 240, 214, 252),
        "disabled": (164, 174, 188, 170),
    }[state]

    d.rounded_rectangle((0, 0, size[0] - 1, size[1] - 1), radius=18, fill=bg, outline=(60, 86, 126, 236), width=2)
    d.rounded_rectangle((6, 6, size[0] - 7, size[1] - 7), radius=14, outline=stroke, width=3)
    glow = blank(size)
    gd = ImageDraw.Draw(glow, "RGBA")
    gd.rounded_rectangle((8, 8, size[0] - 9, size[1] - 9), radius=14, outline=(stroke[0], stroke[1], stroke[2], 130), width=4)
    img.alpha_composite(glow.filter(ImageFilter.GaussianBlur(2.3)))
    draw_center_text(img, "BY-PASS", size[1] // 2 + 1, text, size=36)
    return img


def vibe_shell() -> Image.Image:
    size = (640, 120)
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    d.rounded_rectangle((0, 0, size[0] - 1, size[1] - 1), radius=24, fill=(15, 24, 46, 230), outline=(70, 98, 140, 240), width=2)
    d.rounded_rectangle((10, 10, size[0] - 11, size[1] - 11), radius=18, outline=(63, 205, 255, 150), width=2)
    d.line((214, 18, 214, size[1] - 18), fill=(92, 124, 170, 170), width=2)
    d.line((426, 18, 426, size[1] - 18), fill=(92, 124, 170, 170), width=2)
    draw_center_text(img, "VIBE MODE", 22, (190, 212, 242, 225), size=22, bold=True)
    return img


def vibe_item(mode: str, state: str) -> Image.Image:
    size = (180, 88)
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    label = mode.upper()
    on = state == "on"

    accent_map = {
        "smooth": (83, 232, 255),
        "punch": (255, 88, 188),
        "dirty": (255, 172, 74),
    }
    accent = accent_map[mode]

    bg = (12, 22, 44, 235) if on else (14, 20, 36, 210)
    stroke = (accent[0], accent[1], accent[2], 235 if on else 118)
    txt = (242, 250, 255, 250) if on else (180, 196, 220, 212)

    d.rounded_rectangle((0, 0, size[0] - 1, size[1] - 1), radius=14, fill=bg, outline=(76, 104, 150, 230), width=2)
    d.rounded_rectangle((6, 6, size[0] - 7, size[1] - 7), radius=10, outline=stroke, width=3 if on else 2)

    if on:
        glow = blank(size)
        gd = ImageDraw.Draw(glow, "RGBA")
        gd.rounded_rectangle((8, 8, size[0] - 9, size[1] - 9), radius=10, outline=(accent[0], accent[1], accent[2], 145), width=4)
        img.alpha_composite(glow.filter(ImageFilter.GaussianBlur(2.0)))

    draw_center_text(img, label, size[1] // 2 + 2, txt, size=32)
    return img


def guzvanje_toggle(state: str) -> Image.Image:
    size = (260, 94)
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")
    on = state == "on"

    if on:
        bg = (18, 56, 62, 236)
        stroke = (64, 244, 194, 234)
        txt = (224, 255, 244, 252)
    elif state == "off":
        bg = (28, 22, 42, 232)
        stroke = (255, 96, 176, 228)
        txt = (246, 219, 236, 245)
    elif state == "hover":
        bg = (22, 38, 58, 236)
        stroke = (96, 225, 255, 232)
        txt = (232, 246, 255, 250)
    else:  # pressed
        bg = (12, 20, 34, 244)
        stroke = (255, 180, 86, 234)
        txt = (255, 240, 212, 250)

    d.rounded_rectangle((0, 0, size[0] - 1, size[1] - 1), radius=16, fill=bg, outline=(63, 92, 138, 235), width=2)
    d.rounded_rectangle((6, 6, size[0] - 7, size[1] - 7), radius=12, outline=stroke, width=3)

    pill_w, pill_h = 62, 34
    py = (size[1] - pill_h) // 2
    px = size[0] - pill_w - 16 if on else 16
    d.rounded_rectangle((16, py, size[0] - 16, py + pill_h), radius=17, fill=(9, 16, 30, 215), outline=(70, 96, 130, 210), width=2)
    d.rounded_rectangle((px, py, px + pill_w, py + pill_h), radius=15, fill=stroke, outline=(240, 248, 255, 220), width=1)

    draw_center_text(img, "GUŽVANJE", size[1] // 2 + 1, txt, size=28, bold=True)
    return img


def sync_checkbox(state: str) -> Image.Image:
    size = (88, 88)
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")

    bg = {
        "off": (14, 20, 36, 230),
        "on": (14, 48, 58, 238),
        "disabled": (20, 22, 30, 180),
    }[state]
    stroke = {
        "off": (110, 132, 170, 220),
        "on": (85, 228, 255, 236),
        "disabled": (102, 110, 124, 150),
    }[state]
    d.rounded_rectangle((0, 0, size[0] - 1, size[1] - 1), radius=14, fill=bg, outline=(64, 90, 130, 220), width=2)
    d.rounded_rectangle((10, 10, size[0] - 11, size[1] - 11), radius=10, outline=stroke, width=3)

    if state == "on":
        d.line((22, 46, 36, 60), fill=(232, 255, 255, 245), width=5)
        d.line((36, 60, 66, 28), fill=(232, 255, 255, 245), width=5)
    draw_center_text(img, "SYNC", 12, (208, 224, 246, 240 if state != "disabled" else 165), size=15, bold=True)
    return img


def status_led(state: str) -> Image.Image:
    size = (48, 48)
    img = blank(size)
    d = ImageDraw.Draw(img, "RGBA")

    d.ellipse((4, 4, 44, 44), fill=(8, 14, 24, 220), outline=(75, 95, 128, 220), width=2)
    if state == "on":
        color = (64, 255, 146, 244)
        glow_a = 170
    else:
        color = (255, 64, 122, 198)
        glow_a = 95

    glow = blank(size)
    gd = ImageDraw.Draw(glow, "RGBA")
    gd.ellipse((8, 8, 40, 40), fill=(color[0], color[1], color[2], glow_a))
    img.alpha_composite(glow.filter(ImageFilter.GaussianBlur(3.4)))
    d.ellipse((12, 12, 36, 36), fill=color, outline=(240, 248, 255, 195), width=1)
    return img


def generate_assets() -> dict[str, list[str]]:
    manifest_assets: dict[str, list[str]] = {
        "bypass": [],
        "vibe_shell": [],
        "vibe_items": [],
        "guzvanje": [],
        "sync": [],
        "led": [],
    }

    for state in ("off", "on", "hover", "pressed", "disabled"):
        files = save_pair(bypass_button(state), f"gz_control_bypass_button_{state}")
        manifest_assets["bypass"].extend(files)

    files = save_pair(vibe_shell(), "gz_control_vibe_segmented_shell_base")
    manifest_assets["vibe_shell"].extend(files)

    for mode in ("smooth", "punch", "dirty"):
        for state in ("off", "on"):
            files = save_pair(vibe_item(mode, state), f"gz_control_vibe_{mode}_{state}")
            manifest_assets["vibe_items"].extend(files)

    for state in ("off", "on", "hover", "pressed"):
        files = save_pair(guzvanje_toggle(state), f"gz_control_guzvanje_toggle_{state}")
        manifest_assets["guzvanje"].extend(files)

    for state in ("off", "on", "disabled"):
        files = save_pair(sync_checkbox(state), f"gz_control_sync_checkbox_{state}")
        manifest_assets["sync"].extend(files)

    for state in ("off", "on"):
        files = save_pair(status_led(state), f"gz_control_led_status_{state}")
        manifest_assets["led"].extend(files)

    return manifest_assets


def make_preview(manifest_assets: dict[str, list[str]]) -> None:
    QA_DIR.mkdir(parents=True, exist_ok=True)
    canvas = Image.new("RGBA", (1180, 360), (8, 14, 30, 255))
    d = ImageDraw.Draw(canvas, "RGBA")
    d.rounded_rectangle((8, 8, 1172, 352), radius=18, outline=(64, 98, 146, 220), width=2)

    # Bypass states row (preview thumbs, non-overlapping)
    x = 20
    y = 24
    for state in ("off", "on", "hover", "pressed", "disabled"):
        img = (
            Image.open(EXPORT_DIR / f"gz_control_bypass_button_{state}_320x96.png")
            .convert("RGBA")
            .resize((220, 66), Image.Resampling.LANCZOS)
        )
        canvas.alpha_composite(img, (x, y))
        x += 228

    # Vibe controls
    shell = Image.open(EXPORT_DIR / "gz_control_vibe_segmented_shell_base_640x120.png").convert("RGBA")
    canvas.alpha_composite(shell, (24, 140))
    for i, mode in enumerate(("smooth", "punch", "dirty")):
        itm = Image.open(EXPORT_DIR / f"gz_control_vibe_{mode}_on_180x88.png").convert("RGBA")
        canvas.alpha_composite(itm, (42 + i * 206, 168))

    # Other controls
    positions = {
        "off": (700, 138),
        "on": (900, 138),
        "hover": (700, 242),
        "pressed": (900, 242),
    }
    for state in ("off", "on", "hover", "pressed"):
        tog = Image.open(EXPORT_DIR / f"gz_control_guzvanje_toggle_{state}_260x94.png").convert("RGBA")
        canvas.alpha_composite(tog, positions[state])

    canvas.alpha_composite(Image.open(EXPORT_DIR / "gz_control_sync_checkbox_on_88x88.png").convert("RGBA"), (1054, 150))
    canvas.alpha_composite(Image.open(EXPORT_DIR / "gz_control_led_status_on_48x48.png").convert("RGBA"), (1074, 258))

    p1 = QA_DIR / "gz_qa_gd3_controls_preview_1180x360.png"
    p2 = QA_DIR / "gz_qa_gd3_controls_preview_2360x720.png"
    canvas.save(p1, format="PNG")
    canvas.resize((2360, 720), Image.Resampling.LANCZOS).save(p2, format="PNG")


def write_manifest(assets: dict[str, list[str]]) -> None:
    manifest = {
        "phase": "GD-3",
        "group": "control",
        "exports_dir": "assets/ui/exports/controls",
        "notes": [
            "All controls exported as PNG RGBA.",
            "Runtime assets include @1x and @2x variants.",
            "GUŽVANJE label appears in control artwork (filename remains ASCII guzvanje).",
        ],
        "assets": assets,
        "total_exports": sum(len(v) for v in assets.values()),
    }
    MANIFEST_PATH.write_text(json.dumps(manifest, indent=2, ensure_ascii=True) + "\n", encoding="utf-8")


def main() -> None:
    assets = generate_assets()
    make_preview(assets)
    write_manifest(assets)


if __name__ == "__main__":
    main()
