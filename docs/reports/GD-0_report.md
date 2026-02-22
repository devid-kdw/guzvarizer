# GD-0 report

## faza

- `GD-0` - setup + style board + naming sanity

## sto je isporuceno

- Validiran i potvrden `assets/ui` folder layout prema `docs/103_GRAPHIC_ASSET_SPEC.md`.
- Dodan GD-0 style board s vizualnim smjerom, paletom, state pravilima i branding lockom (`Gužvarizer`, `AIR` + `COLOR`).
- Dodan naming sanity manifest sa canonical ID-evima i filename guardrailovima (`gz_`, ASCII snake_case).
- Kreiran obavezni bootstrap manifest `assets/ui/manifests/layout_map_1180x760.json` s centrima svih trazenih knobova.

## koje datoteke su dodane/izmijenjene

- `assets/ui/manifests/gd0_setup_status.md` (dodano)
- `assets/ui/manifests/gd0_style_board.md` (dodano)
- `assets/ui/manifests/gd0_naming_sanity.md` (dodano)
- `assets/ui/manifests/layout_map_1180x760.json` (dodano)

## sto je otvoreno/sto nedostaje

- `GD-1`: shell/frame export asseti (`assets/ui/exports/shell/*`) jos nisu producirani.
- `GD-2`: puni knob layer setovi (ring/cap/pointer/ticks/glow) za svih 11 knob ID-eva jos nisu producirani.
- `GD-3`: controls (`bypass`, `vibe`, `guzvanje`, `sync`, `led`) exporti jos nisu producirani.
- `GD-4`: meter/strip/overlay exporti jos nisu producirani.
- `GD-5`: final source paketi (`figma/psd/ai`) i QA preview slike jos nisu predani.
