#!/usr/bin/env python3
"""Generate GD-1 shell/frame assets for Gužvarizer UI."""

from __future__ import annotations

import math
import random
from pathlib import Path

from PIL import Image, ImageChops, ImageDraw, ImageFilter, ImageFont


ROOT = Path(__file__).resolve().parents[1]
EXPORT_DIR = ROOT / "assets" / "ui" / "exports" / "shell"


def make_canvas(size: tuple[int, int], color=(0, 0, 0, 0)) -> Image.Image:
    return Image.new("RGBA", size, color)


def add_soft_noise(img: Image.Image, amount: int, seed: int) -> None:
    random.seed(seed)
    noise = Image.new("L", img.size, 0)
    px = noise.load()
    w, h = noise.size
    for y in range(h):
        for x in range(w):
            px[x, y] = random.randint(0, amount)
    noise_rgba = Image.merge("RGBA", (noise, noise, noise, noise.point(lambda v: v // 3)))
    img.alpha_composite(noise_rgba)


def add_vertical_glow(
    img: Image.Image, x: int, color: tuple[int, int, int], width: int, alpha: int
) -> None:
    glow = make_canvas(img.size)
    g = ImageDraw.Draw(glow, "RGBA")
    h = img.height
    for i in range(width):
        a = max(0, alpha - i * (alpha // max(1, width)))
        g.line([(x - i, 0), (x - i, h)], fill=(color[0], color[1], color[2], a), width=1)
        g.line([(x + i, 0), (x + i, h)], fill=(color[0], color[1], color[2], a), width=1)
    img.alpha_composite(glow.filter(ImageFilter.GaussianBlur(6)))


def make_background() -> Image.Image:
    w, h = 1180, 760
    img = make_canvas((w, h), (6, 12, 28, 255))
    px = img.load()
    for y in range(h):
        t = y / max(1, h - 1)
        for x in range(w):
            x_t = x / max(1, w - 1)
            r = int(6 + 6 * t + 16 * (x_t * (1 - x_t)))
            g = int(12 + 22 * t)
            b = int(28 + 36 * t)
            px[x, y] = (r, g, b, 255)

    add_vertical_glow(img, 74, (255, 74, 168), 12, 130)
    add_vertical_glow(img, 1104, (47, 226, 255), 12, 130)
    add_vertical_glow(img, 590, (255, 164, 48), 7, 80)

    skyline = make_canvas((w, h))
    d = ImageDraw.Draw(skyline, "RGBA")
    random.seed(1337)
    for _ in range(210):
        bw = random.randint(3, 9)
        bh = random.randint(4, 44)
        x0 = random.randint(0, w - bw - 1)
        y0 = random.randint(h - 118, h - 12)
        color = random.choice(
            [(255, 98, 180, 76), (64, 234, 255, 74), (255, 170, 52, 68), (190, 120, 255, 65)]
        )
        d.rectangle([x0, y0, x0 + bw, y0 + bh], fill=color)

    img.alpha_composite(skyline.filter(ImageFilter.GaussianBlur(0.6)))
    add_soft_noise(img, amount=20, seed=99)
    return img


def make_outer_frame_master() -> Image.Image:
    w, h = 1180, 760
    img = make_canvas((w, h))
    d = ImageDraw.Draw(img, "RGBA")

    d.rounded_rectangle([0, 0, w - 1, h - 1], radius=24, fill=(15, 24, 44, 225))
    d.rounded_rectangle([2, 2, w - 3, h - 3], radius=24, outline=(66, 93, 136, 255), width=3)
    d.rounded_rectangle([10, 10, w - 11, h - 11], radius=18, outline=(15, 214, 255, 130), width=2)
    d.rounded_rectangle([20, 20, w - 21, h - 21], radius=14, outline=(255, 80, 166, 110), width=2)

    corners = [
        [(0, 0), (136, 0), (0, 136)],
        [(w - 1, 0), (w - 137, 0), (w - 1, 136)],
        [(0, h - 1), (136, h - 1), (0, h - 137)],
        [(w - 1, h - 1), (w - 137, h - 1), (w - 1, h - 137)],
    ]
    corner_colors = [(255, 80, 166, 145), (68, 220, 255, 145), (74, 236, 255, 122), (255, 175, 54, 145)]
    for pts, c in zip(corners, corner_colors):
        d.polygon(pts, fill=c)

    glow = img.filter(ImageFilter.GaussianBlur(8))
    img.alpha_composite(glow)
    return img


def make_section_frame(size: tuple[int, int], accent: tuple[int, int, int]) -> Image.Image:
    w, h = size
    img = make_canvas((w, h))
    d = ImageDraw.Draw(img, "RGBA")
    d.rounded_rectangle([0, 0, w - 1, h - 1], radius=14, fill=(9, 16, 34, 214))
    d.rounded_rectangle([1, 1, w - 2, h - 2], radius=14, outline=(52, 78, 122, 245), width=2)
    d.rounded_rectangle([7, 7, w - 8, h - 8], radius=10, outline=(accent[0], accent[1], accent[2], 150), width=2)

    notch_w = max(28, w // 7)
    d.polygon(
        [(w // 2 - notch_w, 0), (w // 2 + notch_w, 0), (w // 2 + notch_w // 2, 12), (w // 2 - notch_w // 2, 12)],
        fill=(accent[0], accent[1], accent[2], 170),
    )
    return img


def make_side_deco(size: tuple[int, int], side: str) -> Image.Image:
    w, h = size
    img = make_canvas((w, h))
    d = ImageDraw.Draw(img, "RGBA")
    base = (255, 88, 170) if side == "left" else (67, 225, 255)

    d.rounded_rectangle([0, 0, w - 1, h - 1], radius=10, fill=(15, 22, 45, 188), outline=(60, 88, 132, 220), width=2)
    for i in range(6):
        y0 = 12 + i * ((h - 24) // 6)
        y1 = y0 + max(8, (h // 14))
        alpha = 120 + i * 18
        d.rectangle([10, y0, w - 10, y1], fill=(base[0], base[1], base[2], alpha))

    return img.filter(ImageFilter.GaussianBlur(0.4))


def get_font(size: int) -> ImageFont.FreeTypeFont | ImageFont.ImageFont:
    candidates = [
        "/System/Library/Fonts/Supplemental/Arial Unicode.ttf",
        "/System/Library/Fonts/Supplemental/Arial Bold.ttf",
        "/System/Library/Fonts/Supplemental/Helvetica.ttc",
    ]
    for path in candidates:
        try:
            return ImageFont.truetype(path, size=size)
        except OSError:
            continue
    return ImageFont.load_default()


def make_logo_plate() -> Image.Image:
    w, h = 720, 96
    img = make_canvas((w, h))
    d = ImageDraw.Draw(img, "RGBA")
    d.rounded_rectangle([0, 0, w - 1, h - 1], radius=18, fill=(13, 22, 46, 235), outline=(77, 108, 156, 255), width=2)
    d.rounded_rectangle([8, 8, w - 9, h - 9], radius=14, outline=(255, 84, 172, 155), width=2)

    text = "Gužvarizer"
    font = get_font(56)
    text_box = d.textbbox((0, 0), text, font=font)
    tw, th = text_box[2] - text_box[0], text_box[3] - text_box[1]
    tx, ty = (w - tw) // 2, (h - th) // 2 - 5

    glow = make_canvas((w, h))
    gd = ImageDraw.Draw(glow, "RGBA")
    gd.text((tx, ty), text, font=font, fill=(79, 235, 255, 180))
    glow = glow.filter(ImageFilter.GaussianBlur(4))
    img.alpha_composite(glow)

    d.text((tx, ty), text, font=font, fill=(243, 251, 255, 255))
    return img


def save_with_2x(image: Image.Image, name_1x: str, name_2x: str) -> None:
    EXPORT_DIR.mkdir(parents=True, exist_ok=True)
    image.save(EXPORT_DIR / name_1x, format="PNG")
    image.resize((image.width * 2, image.height * 2), resample=Image.Resampling.LANCZOS).save(
        EXPORT_DIR / name_2x, format="PNG"
    )


def crop_and_save_9slice(master: Image.Image) -> None:
    c = 96
    w, h = master.size
    slices = {
        "gz_shell_outer_corner_tl_base_96x96.png": master.crop((0, 0, c, c)),
        "gz_shell_outer_corner_tr_base_96x96.png": master.crop((w - c, 0, w, c)),
        "gz_shell_outer_corner_bl_base_96x96.png": master.crop((0, h - c, c, h)),
        "gz_shell_outer_corner_br_base_96x96.png": master.crop((w - c, h - c, w, h)),
        "gz_shell_outer_edge_top_base_988x96.png": master.crop((c, 0, w - c, c)),
        "gz_shell_outer_edge_bottom_base_988x96.png": master.crop((c, h - c, w - c, h)),
        "gz_shell_outer_edge_left_base_96x568.png": master.crop((0, c, c, h - c)),
        "gz_shell_outer_edge_right_base_96x568.png": master.crop((w - c, c, w, h - c)),
        "gz_shell_outer_fill_center_base_988x568.png": master.crop((c, c, w - c, h - c)),
    }

    for name_1x, asset in slices.items():
        size_token = name_1x.rsplit("_", 1)[-1].replace(".png", "")
        sw, sh = map(int, size_token.split("x"))
        name_2x = name_1x.replace(f"_{size_token}.png", f"_{sw * 2}x{sh * 2}.png")
        save_with_2x(asset, name_1x, name_2x)


def main() -> None:
    background = make_background()
    save_with_2x(
        background,
        "gz_shell_background_atmosphere_base_1180x760.png",
        "gz_shell_background_atmosphere_base_2360x1520.png",
    )

    outer_master = make_outer_frame_master()
    crop_and_save_9slice(outer_master)

    save_with_2x(
        make_section_frame((1148, 78), (72, 225, 255)),
        "gz_shell_header_frame_base_1148x78.png",
        "gz_shell_header_frame_base_2296x156.png",
    )
    save_with_2x(
        make_section_frame((1148, 264), (255, 94, 180)),
        "gz_shell_main_section_frame_base_1148x264.png",
        "gz_shell_main_section_frame_base_2296x528.png",
    )
    save_with_2x(
        make_section_frame((1148, 220), (186, 119, 255)),
        "gz_shell_tone_shaper_section_frame_base_1148x220.png",
        "gz_shell_tone_shaper_section_frame_base_2296x440.png",
    )
    save_with_2x(
        make_section_frame((1148, 88), (255, 164, 53)),
        "gz_shell_footer_frame_base_1148x88.png",
        "gz_shell_footer_frame_base_2296x176.png",
    )

    save_with_2x(
        make_side_deco((64, 520), "left"),
        "gz_shell_side_deco_left_glow_64x520.png",
        "gz_shell_side_deco_left_glow_128x1040.png",
    )
    save_with_2x(
        make_side_deco((64, 520), "right"),
        "gz_shell_side_deco_right_glow_64x520.png",
        "gz_shell_side_deco_right_glow_128x1040.png",
    )

    save_with_2x(
        make_logo_plate(),
        "gz_shell_logo_plate_base_720x96.png",
        "gz_shell_logo_plate_base_1440x192.png",
    )


if __name__ == "__main__":
    main()
