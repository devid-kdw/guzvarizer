# Release Targets

## Format and platforms

- Plugin format: `VST3 (.vst3)`
- Supported OS: `macOS` + `Windows`
- Product name (user-facing): `Gužvarizer`

## Output artifacts

- macOS artifact: `Gužvarizer.vst3` (bundle)
- Windows artifact: `Gužvarizer.vst3`

## Build matrix

- `macOS`:
  - target: VST3
  - architecture: arm64 + x86_64 (universal ako build env dozvoljava)
- `Windows`:
  - target: VST3
  - architecture: x64

## Validation checklist

- plugin se ucitava u barem jedan host na macOS
- plugin se ucitava u barem jedan host na Windows
- automation, preset restore i metering rade na obje platforme
- `Gužvanje` i LFO sync rade na obje platforme

## Note on naming

- User-facing komunikacija moze koristiti izraz "VST format".
- Tehnicki deliverable u ovom projektu je `VST3` artefakt (`.vst3`).
