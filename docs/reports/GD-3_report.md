# GD-3 report

## faza

- `GD-3` - controls

## sto je isporuceno

- Isporucen kompletan controls set u `assets/ui/exports/controls`:
  - `BY-PASS` button states: `off/on/hover/pressed/disabled`
  - `VIBE MODE` segmented shell
  - vibe item states: `smooth/punch/dirty` u `on/off`
  - `GUŽVANJE` toggle states: `off/on/hover/pressed`
  - `SYNC` checkbox states: `off/on/disabled`
  - status `LED` states: `off/on`
- Sve control datoteke isporucene kao `@1x` + `@2x` parovi.
- Ukupno eksportirano `42` controls PNG datoteka.
- Dodan controls manifest i QA preview.
- Dodan source master SVG za controls.

## koje datoteke su dodane/izmijenjene

- `scripts/generate_gd3_control_assets.py` (dodano)
- `assets/ui/manifests/gd3_controls_manifest.json` (dodano)
- `assets/ui/source/ai/gz_controls_master_gd3.svg` (dodano)
- `assets/ui/qa/gz_qa_gd3_controls_preview_1180x360.png` (dodano)
- `assets/ui/qa/gz_qa_gd3_controls_preview_2360x720.png` (dodano)
- `assets/ui/exports/controls/*` (dodano 42 PNG datoteke, naming `gz_control_<element>_<variant/state>_<size>.png`)

## sto je otvoreno/sto nedostaje

- `GD-4`: metering + strip + overlay asseti (`meters`, `strips`, `overlays` export folderi).
- `GD-5`: final paket (source datoteke komplet, final QA preview paket, finalna manifest verifikacija).
