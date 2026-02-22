# GD-1 report

## faza

- `GD-1` - shell/frame

## sto je isporuceno

- Isporucen modularni shell/frame export set u `assets/ui/exports/shell`:
  - outer frame 9-slice kompatibilni elementi (4 kuta, 4 brid trake, central fill)
  - sekcijski frameovi (`header`, `main_section`, `tone_shaper_section`, `footer`)
  - side neon deco elementi (`left`, `right`)
  - background atmosphere
  - logo plate s nazivom `Gužvarizer` (sa slovom `ž`)
- Sve runtime shell datoteke isporucene u `@1x` i `@2x` varijanti.
- Dodan source master SVG za shell/frame.
- Dodan QA preview kompozit za brzu provjeru izgleda.
- Dodan GD-1 shell manifest s popisom svih exporta.

## koje datoteke su dodane/izmijenjene

- `scripts/generate_gd1_shell_assets.py` (dodano)
- `assets/ui/source/ai/gz_shell_frame_master_gd1.svg` (dodano)
- `assets/ui/manifests/gd1_shell_manifest.json` (dodano)
- `assets/ui/qa/gz_qa_gd1_shell_preview_1180x760.png` (dodano)
- `assets/ui/qa/gz_qa_gd1_shell_preview_2360x1520.png` (dodano)
- `assets/ui/exports/shell/gz_shell_background_atmosphere_base_1180x760.png` (dodano)
- `assets/ui/exports/shell/gz_shell_background_atmosphere_base_2360x1520.png` (dodano)
- `assets/ui/exports/shell/gz_shell_footer_frame_base_1148x88.png` (dodano)
- `assets/ui/exports/shell/gz_shell_footer_frame_base_2296x176.png` (dodano)
- `assets/ui/exports/shell/gz_shell_header_frame_base_1148x78.png` (dodano)
- `assets/ui/exports/shell/gz_shell_header_frame_base_2296x156.png` (dodano)
- `assets/ui/exports/shell/gz_shell_logo_plate_base_720x96.png` (dodano)
- `assets/ui/exports/shell/gz_shell_logo_plate_base_1440x192.png` (dodano)
- `assets/ui/exports/shell/gz_shell_main_section_frame_base_1148x264.png` (dodano)
- `assets/ui/exports/shell/gz_shell_main_section_frame_base_2296x528.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_corner_tl_base_96x96.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_corner_tl_base_192x192.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_corner_tr_base_96x96.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_corner_tr_base_192x192.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_corner_bl_base_96x96.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_corner_bl_base_192x192.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_corner_br_base_96x96.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_corner_br_base_192x192.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_edge_top_base_988x96.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_edge_top_base_1976x192.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_edge_bottom_base_988x96.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_edge_bottom_base_1976x192.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_edge_left_base_96x568.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_edge_left_base_192x1136.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_edge_right_base_96x568.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_edge_right_base_192x1136.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_fill_center_base_988x568.png` (dodano)
- `assets/ui/exports/shell/gz_shell_outer_fill_center_base_1976x1136.png` (dodano)
- `assets/ui/exports/shell/gz_shell_side_deco_left_glow_64x520.png` (dodano)
- `assets/ui/exports/shell/gz_shell_side_deco_left_glow_128x1040.png` (dodano)
- `assets/ui/exports/shell/gz_shell_side_deco_right_glow_64x520.png` (dodano)
- `assets/ui/exports/shell/gz_shell_side_deco_right_glow_128x1040.png` (dodano)
- `assets/ui/exports/shell/gz_shell_tone_shaper_section_frame_base_1148x220.png` (dodano)
- `assets/ui/exports/shell/gz_shell_tone_shaper_section_frame_base_2296x440.png` (dodano)

## sto je otvoreno/sto nedostaje

- `GD-2`: knob asset setovi za svih 11 knob ID-eva (ring/cap/pointer/ticks/glow + @1x/@2x).
- `GD-3`: controls asseti (`bypass`, `vibe`, `guzvanje`, `sync`, `led`).
- `GD-4`: metering, strip i overlay asseti.
- `GD-5`: final source paket (`figma/psd/ai` komplet), final QA preview i final manifest verifikacija.
