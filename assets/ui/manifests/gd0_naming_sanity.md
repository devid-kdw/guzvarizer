# GD-0 Naming Sanity (Gužvarizer)

## Phase

- `GD-0`

## Naming Contract

- ASCII only in file names.
- `snake_case` only.
- Mandatory prefix: `gz_`.
- Canonical pattern:
  - `gz_<group>_<element>_<variant>_<state>_<size>.png`

## Group Vocabulary

- `shell`
- `knob`
- `control`
- `meter`
- `strip`
- `text`
- `overlay`

## Canonical Element IDs

- Main knobs:
  - `threshold`
  - `attack`
  - `release`
  - `ratio`
  - `output`
- Tone knobs:
  - `saturation`
  - `harmonics`
  - `grit`
  - `air`
  - `color`
- Footer knob:
  - `lfo_rate`
- Controls:
  - `bypass`
  - `vibe`
  - `guzvanje`
  - `sync`
  - `led`

## Guardrails

- `air_freq` parameter maps to visual ID `color` (not `air_2`).
- `guzvanje` stays ASCII in filenames; label text in artwork is `GUŽVANJE`.
- No spaces, hyphens, or locale-specific chars in filenames.
- `@1x` + `@2x` runtime exports are mandatory.

## Example Filenames (Valid)

- `gz_shell_main_frame_base_1180x760.png`
- `gz_knob_threshold_ring_base_256x256.png`
- `gz_knob_threshold_ring_base_512x512.png`
- `gz_knob_color_pointer_base_224x224.png`
- `gz_control_bypass_button_on_320x96.png`
- `gz_control_guzvanje_toggle_off_240x88.png`
- `gz_meter_gr_segment_base_16x48.png`
- `gz_strip_color_strip_hotspot_glow_64x32.png`
- `gz_overlay_scanline_full_base_1180x760.png`

## Naming QA Checklist

- [x] All required knob IDs include `color`.
- [x] No duplicated `air` semantic ID.
- [x] All sample names are ASCII snake_case with `gz_` prefix.
- [x] Pattern supports stateful control variants (`on/off/hover/pressed/disabled`).
