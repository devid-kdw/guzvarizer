# GD-2 report

## faza

- `GD-2` - knobs

## sto je isporuceno

- Isporuceni kompletni modularni knob asset setovi za svih 11 obaveznih knob ID-eva:
  - main: `threshold`, `attack`, `release`, `ratio`, `output`
  - tone: `saturation`, `harmonics`, `grit`, `air`, `color`
  - lfo: `lfo_rate`
- Za svaki knob isporuceni slojevi:
  - `ring_base`
  - `ring_active`
  - `ticks_base`
  - `cap_base`
  - `pointer_base` (default orijentacija 12h)
  - `glow_base`
- Export rezolucije:
  - main: `256x256` + `512x512`
  - tone: `224x224` + `448x448`
  - lfo: `192x192` + `384x384`
- Ukupno izvezeno `132` PNG knob asseta:
  - `60` main + `60` tone + `12` lfo
- Dodan GD-2 knob manifest i QA preview slike.
- Dodan source master SVG za knob sistem.

## koje datoteke su dodane/izmijenjene

- `scripts/generate_gd2_knob_assets.py` (dodano)
- `assets/ui/manifests/gd2_knob_manifest.json` (dodano)
- `assets/ui/source/ai/gz_knob_system_master_gd2.svg` (dodano)
- `assets/ui/qa/gz_qa_gd2_knobs_main_preview_1496x366.png` (dodano)
- `assets/ui/qa/gz_qa_gd2_knobs_main_preview_2992x732.png` (dodano)
- `assets/ui/qa/gz_qa_gd2_knobs_tone_preview_1336x334.png` (dodano)
- `assets/ui/qa/gz_qa_gd2_knobs_tone_preview_2672x668.png` (dodano)
- `assets/ui/qa/gz_qa_gd2_knobs_lfo_preview_264x302.png` (dodano)
- `assets/ui/qa/gz_qa_gd2_knobs_lfo_preview_528x604.png` (dodano)
- `assets/ui/exports/knobs/main/*` (dodano 60 PNG datoteka, pattern `gz_knob_<main_id>_<layer>_<state>_<size>.png`)
- `assets/ui/exports/knobs/tone/*` (dodano 60 PNG datoteka, pattern `gz_knob_<tone_id>_<layer>_<state>_<size>.png`)
- `assets/ui/exports/knobs/lfo/*` (dodano 12 PNG datoteka, pattern `gz_knob_lfo_rate_<layer>_<state>_<size>.png`)

## sto je otvoreno/sto nedostaje

- `GD-3`: controls asset setovi (`bypass`, `vibe`, `guzvanje`, `sync`, `led`) s punim state varijantama.
- `GD-4`: metering + strip + overlay asset setovi (`grain/scanline/vignette`).
- `GD-5`: final paket (source datoteke komplet, final QA preview, finalni layout/manifest QA lock).
