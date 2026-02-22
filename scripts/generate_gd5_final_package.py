#!/usr/bin/env python3
"""Generate GD-5 final package artifacts (QA preview + final manifest)."""

from __future__ import annotations

import json
import re
from pathlib import Path
from typing import Any

from PIL import Image, ImageDraw, ImageFont


ROOT = Path(__file__).resolve().parents[1]
EXPORTS = ROOT / "assets" / "ui" / "exports"
MANIFESTS = ROOT / "assets" / "ui" / "manifests"
QA = ROOT / "assets" / "ui" / "qa"
SOURCE = ROOT / "assets" / "ui" / "source"


def load_rgba(path: Path) -> Image.Image:
    return Image.open(path).convert("RGBA")


def compose_outer_frame() -> Image.Image:
    size = (1180, 760)
    c = 96
    frame = Image.new("RGBA", size, (0, 0, 0, 0))
    shell = EXPORTS / "shell"

    frame.alpha_composite(load_rgba(shell / "gz_shell_outer_corner_tl_base_96x96.png"), (0, 0))
    frame.alpha_composite(load_rgba(shell / "gz_shell_outer_corner_tr_base_96x96.png"), (size[0] - c, 0))
    frame.alpha_composite(load_rgba(shell / "gz_shell_outer_corner_bl_base_96x96.png"), (0, size[1] - c))
    frame.alpha_composite(load_rgba(shell / "gz_shell_outer_corner_br_base_96x96.png"), (size[0] - c, size[1] - c))
    frame.alpha_composite(load_rgba(shell / "gz_shell_outer_edge_top_base_988x96.png"), (c, 0))
    frame.alpha_composite(load_rgba(shell / "gz_shell_outer_edge_bottom_base_988x96.png"), (c, size[1] - c))
    frame.alpha_composite(load_rgba(shell / "gz_shell_outer_edge_left_base_96x568.png"), (0, c))
    frame.alpha_composite(load_rgba(shell / "gz_shell_outer_edge_right_base_96x568.png"), (size[0] - c, c))
    return frame


def draw_text(
    image: Image.Image, text: str, xy: tuple[int, int], color=(232, 244, 255, 235), size=26, bold=False
) -> None:
    d = ImageDraw.Draw(image, "RGBA")
    font_candidates = [
        "/System/Library/Fonts/Supplemental/Arial Unicode.ttf",
        "/System/Library/Fonts/Supplemental/Arial Bold.ttf" if bold else "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Supplemental/Helvetica.ttc",
    ]
    f = None
    for p in font_candidates:
        try:
            f = ImageFont.truetype(p, size=size)
            break
        except OSError:
            continue
    if f is None:
        f = ImageFont.load_default()
    d.text(xy, text, font=f, fill=color)


def load_layout_map() -> dict[str, Any]:
    return json.loads((MANIFESTS / "layout_map_1180x760.json").read_text(encoding="utf-8"))


def compose_knob(knob_id: str, family: str, export_size: int, draw_size: int) -> Image.Image:
    folder = EXPORTS / "knobs" / family
    layers = [
        f"gz_knob_{knob_id}_glow_base_{export_size}x{export_size}.png",
        f"gz_knob_{knob_id}_ring_base_{export_size}x{export_size}.png",
        f"gz_knob_{knob_id}_ring_active_{export_size}x{export_size}.png",
        f"gz_knob_{knob_id}_ticks_base_{export_size}x{export_size}.png",
        f"gz_knob_{knob_id}_cap_base_{export_size}x{export_size}.png",
        f"gz_knob_{knob_id}_pointer_base_{export_size}x{export_size}.png",
    ]
    image = Image.new("RGBA", (export_size, export_size), (0, 0, 0, 0))
    for l in layers:
        image.alpha_composite(load_rgba(folder / l))
    if draw_size != export_size:
        image = image.resize((draw_size, draw_size), Image.Resampling.LANCZOS)
    return image


def place_centered(canvas: Image.Image, element: Image.Image, cx: int, cy: int) -> None:
    x = int(cx - element.width / 2)
    y = int(cy - element.height / 2)
    canvas.alpha_composite(element, (x, y))


def compose_final_preview() -> None:
    shell = EXPORTS / "shell"
    controls = EXPORTS / "controls"
    meters = EXPORTS / "meters"
    strips = EXPORTS / "strips"
    overlays = EXPORTS / "overlays"

    canvas = load_rgba(shell / "gz_shell_background_atmosphere_base_1180x760.png")
    canvas.alpha_composite(compose_outer_frame())

    canvas.alpha_composite(load_rgba(shell / "gz_shell_header_frame_base_1148x78.png"), (16, 16))
    canvas.alpha_composite(load_rgba(shell / "gz_shell_main_section_frame_base_1148x264.png"), (16, 108))
    canvas.alpha_composite(load_rgba(shell / "gz_shell_tone_shaper_section_frame_base_1148x220.png"), (16, 422))
    canvas.alpha_composite(load_rgba(shell / "gz_shell_footer_frame_base_1148x88.png"), (16, 656))
    canvas.alpha_composite(load_rgba(shell / "gz_shell_side_deco_left_glow_64x520.png"), (28, 120))
    canvas.alpha_composite(load_rgba(shell / "gz_shell_side_deco_right_glow_64x520.png"), (1088, 120))
    canvas.alpha_composite(load_rgba(shell / "gz_shell_logo_plate_base_720x96.png"), (230, 26))

    layout = load_layout_map()["knobs"]

    main_ids = ["threshold", "attack", "release", "ratio", "output"]
    tone_ids = ["saturation", "harmonics", "grit", "air", "color"]
    for kid in main_ids:
        knob = compose_knob(kid, "main", 256, layout[kid]["diameter"])
        place_centered(canvas, knob, layout[kid]["cx"], layout[kid]["cy"])
    for kid in tone_ids:
        knob = compose_knob(kid, "tone", 224, layout[kid]["diameter"])
        place_centered(canvas, knob, layout[kid]["cx"], layout[kid]["cy"])
    lfo = compose_knob("lfo_rate", "lfo", 192, max(64, layout["lfo_rate"]["diameter"]))
    place_centered(canvas, lfo, layout["lfo_rate"]["cx"], layout["lfo_rate"]["cy"])

    # Controls
    canvas.alpha_composite(load_rgba(controls / "gz_control_bypass_button_on_320x96.png").resize((198, 60), Image.Resampling.LANCZOS), (950, 26))
    canvas.alpha_composite(load_rgba(controls / "gz_control_vibe_segmented_shell_base_640x120.png").resize((430, 80), Image.Resampling.LANCZOS), (214, 664))
    canvas.alpha_composite(load_rgba(controls / "gz_control_vibe_smooth_off_180x88.png").resize((120, 58), Image.Resampling.LANCZOS), (232, 675))
    canvas.alpha_composite(load_rgba(controls / "gz_control_vibe_punch_on_180x88.png").resize((120, 58), Image.Resampling.LANCZOS), (370, 675))
    canvas.alpha_composite(load_rgba(controls / "gz_control_vibe_dirty_off_180x88.png").resize((120, 58), Image.Resampling.LANCZOS), (508, 675))
    canvas.alpha_composite(load_rgba(controls / "gz_control_guzvanje_toggle_on_260x94.png").resize((188, 68), Image.Resampling.LANCZOS), (670, 670))
    canvas.alpha_composite(load_rgba(controls / "gz_control_sync_checkbox_on_88x88.png").resize((54, 54), Image.Resampling.LANCZOS), (870, 676))
    canvas.alpha_composite(load_rgba(controls / "gz_control_led_status_on_48x48.png"), (939, 684))

    # Meter and strip
    canvas.alpha_composite(load_rgba(meters / "gz_meter_top_visualizer_panel_frame_base_220x96.png"), (928, 132))
    in_seg = load_rgba(meters / "gz_meter_in_bar_segment_sprite_base_18x64.png")
    out_seg = load_rgba(meters / "gz_meter_out_bar_segment_sprite_base_18x64.png")
    for i in range(8):
        canvas.alpha_composite(in_seg, (952 + i * 20, 150))
    for i in range(8):
        canvas.alpha_composite(out_seg, (952 + i * 20, 150 - i * 2))

    canvas.alpha_composite(load_rgba(meters / "gz_meter_gr_panel_frame_base_220x200.png"), (928, 238))
    gr_seg = load_rgba(meters / "gz_meter_gr_segment_sprite_base_16x48.png")
    for i in range(10):
        canvas.alpha_composite(gr_seg, (948 + i * 19, 290 + i * 2))
    canvas.alpha_composite(load_rgba(meters / "gz_meter_scale_overlay_texture_base_220x200.png"), (928, 238))

    canvas.alpha_composite(load_rgba(strips / "gz_strip_color_strip_base_base_980x22.png"), (100, 388))
    canvas.alpha_composite(load_rgba(strips / "gz_strip_color_strip_segment_mask_base_980x22.png"), (100, 388))
    hotspot = load_rgba(strips / "gz_strip_color_strip_hotspot_glow_72x32.png")
    for i in range(6):
        canvas.alpha_composite(hotspot, (132 + i * 164, 382))

    # Labels for readability in QA preview.
    draw_text(canvas, "TONE SHAPER", (118, 445), (255, 137, 206, 240), 38, bold=True)
    draw_text(canvas, "VIBE MODE", (218, 670), (188, 210, 238, 220), 22, bold=True)

    # Overlay pass
    canvas.alpha_composite(load_rgba(overlays / "gz_overlay_grain_full_base_1180x760.png"))
    canvas.alpha_composite(load_rgba(overlays / "gz_overlay_scanline_full_base_1180x760.png"))
    canvas.alpha_composite(load_rgba(overlays / "gz_overlay_vignette_full_base_1180x760.png"))

    QA.mkdir(parents=True, exist_ok=True)
    p1 = QA / "gz_qa_gd5_final_preview_1180x760.png"
    p2 = QA / "gz_qa_gd5_final_preview_2360x1520.png"
    canvas.save(p1, format="PNG")
    canvas.resize((2360, 1520), Image.Resampling.LANCZOS).save(p2, format="PNG")


def collect_files(path: Path, suffix: str) -> list[str]:
    return sorted(str(p.relative_to(ROOT)) for p in path.rglob(f"*{suffix}") if p.is_file())


def count_pngs() -> dict[str, int]:
    return {
        "shell": len(list((EXPORTS / "shell").glob("*.png"))),
        "knobs_main": len(list((EXPORTS / "knobs" / "main").glob("*.png"))),
        "knobs_tone": len(list((EXPORTS / "knobs" / "tone").glob("*.png"))),
        "knobs_lfo": len(list((EXPORTS / "knobs" / "lfo").glob("*.png"))),
        "controls": len(list((EXPORTS / "controls").glob("*.png"))),
        "meters": len(list((EXPORTS / "meters").glob("*.png"))),
        "strips": len(list((EXPORTS / "strips").glob("*.png"))),
        "overlays": len(list((EXPORTS / "overlays").glob("*.png"))),
    }


def validate_naming() -> dict[str, Any]:
    patterns = {
        "shell": re.compile(r"^gz_shell_[a-z0-9_]+_(base|on|off|hover|pressed|disabled|glow)_[0-9]+x[0-9]+\.png$"),
        "knob": re.compile(r"^gz_knob_[a-z0-9_]+_[a-z0-9_]+_(base|active|on|off|hover|pressed|disabled|glow)_[0-9]+x[0-9]+\.png$"),
        "control": re.compile(r"^gz_control_[a-z0-9_]+_(base|on|off|hover|pressed|disabled)_[0-9]+x[0-9]+\.png$"),
        "meter": re.compile(r"^gz_meter_[a-z0-9_]+_(base|on|off|hover|pressed|disabled|glow)_[0-9]+x[0-9]+\.png$"),
        "strip": re.compile(r"^gz_strip_[a-z0-9_]+_(base|on|off|hover|pressed|disabled|glow)_[0-9]+x[0-9]+\.png$"),
        "overlay": re.compile(r"^gz_overlay_[a-z0-9_]+_(base|on|off|hover|pressed|disabled|glow)_[0-9]+x[0-9]+\.png$"),
    }
    invalid: list[str] = []
    for png in collect_files(EXPORTS, ".png"):
        name = Path(png).name
        group = name.split("_", 2)[1]
        key = group if group in patterns else None
        if key == "knobs":
            key = "knob"
        if key is None:
            invalid.append(png)
            continue
        if not patterns[key].match(name):
            invalid.append(png)
    return {"invalid_files": invalid, "valid": len(invalid) == 0}


def build_final_manifest() -> None:
    counts = count_pngs()
    naming = validate_naming()
    manifest = {
        "phase": "GD-5",
        "status": "final_package",
        "product_name": "Gu\u017evarizer",
        "canvas": {"w": 1180, "h": 760},
        "layout_manifest": "assets/ui/manifests/layout_map_1180x760.json",
        "exports_png_counts": counts,
        "exports_png_total": sum(counts.values()),
        "source_files": collect_files(SOURCE, ".svg"),
        "qa_previews": sorted(
            p for p in collect_files(QA, ".png") if Path(p).name.startswith("gz_qa_")
        ),
        "naming_validation": naming,
        "notes": [
            "Tone Shaper labels finalized as AIR + COLOR (no duplicate AIR).",
            "All generated runtime assets remain inside assets/ui.",
        ],
    }
    (MANIFESTS / "gd5_final_package_manifest.json").write_text(
        json.dumps(manifest, indent=2, ensure_ascii=True) + "\n", encoding="utf-8"
    )


def lock_layout_manifest() -> None:
    path = MANIFESTS / "layout_map_1180x760.json"
    layout = json.loads(path.read_text(encoding="utf-8"))
    layout["meta"] = {
        "phase": "GD-5",
        "status": "final_locked",
        "method": "Derived from current 1180x760 UI layout metrics and component bounds.",
    }
    # Fine alignment pass based on current section bounds.
    layout["knobs"]["saturation"]["cy"] = 546
    layout["knobs"]["harmonics"]["cy"] = 546
    layout["knobs"]["grit"]["cy"] = 546
    layout["knobs"]["air"]["cy"] = 546
    layout["knobs"]["color"]["cy"] = 546
    path.write_text(json.dumps(layout, indent=2, ensure_ascii=True) + "\n", encoding="utf-8")


def main() -> None:
    lock_layout_manifest()
    compose_final_preview()
    build_final_manifest()


if __name__ == "__main__":
    main()
