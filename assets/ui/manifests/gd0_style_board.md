# GD-0 Style Board (Gužvarizer)

## Phase

- `GD-0`

## Intent

- Visual direction: neon-industrial cyberpunk with modular metal framing.
- Runtime readability first: strong contrast for labels, meters, and active states.
- Frame + controls must stay layered and exportable as separate assets.

## Branding Lock

- Product name in artwork: `Gužvarizer` (with `ž`).
- Tone section labels must include `AIR` and `COLOR` (never `AIR` + `AIR`).

## Palette (Base)

| Token | Hex | Usage |
| --- | --- | --- |
| `panel_bg_900` | `#060B18` | outer background, deep shadows |
| `panel_bg_700` | `#0D1630` | panel midtone |
| `panel_stroke` | `#2B3F66` | frame edge lines |
| `text_primary` | `#E6F3FF` | labels, meter markings |
| `text_dim` | `#89A7C2` | secondary labels |
| `neon_cyan` | `#35E8FF` | threshold/air family accents |
| `neon_magenta` | `#FF4FAE` | release/harmonics accents |
| `neon_orange` | `#FF9E2C` | ratio/output/saturation accents |
| `neon_violet` | `#B879FF` | grit/color accents |
| `status_red` | `#FF365F` | bypass off/fault emphasis |
| `status_green` | `#43FF97` | enabled/active confirmations |

## Materials + FX

- Surface: brushed metal + subtle edge noise (low-frequency texture).
- Bloom: concentrated around active ring, pointer tip, and strip hotspots.
- Overlays:
  - grain target opacity: `8-16%`
  - scanline target opacity: `5-10%`
  - vignette target opacity: `12-20%`

## Typography Direction

- Primary display: geometric sci-fi uppercase (headline only).
- Control labels: compact uppercase with high x-height.
- Numeric ticks/value hints: narrow mono-like style for meter legibility.

## Knob Color Families

- Main row:
  - `threshold`: cyan
  - `attack`: mint-cyan
  - `release`: magenta
  - `ratio`: orange
  - `output`: amber-orange
- Tone row:
  - `saturation`: orange-pink
  - `harmonics`: magenta
  - `grit`: violet
  - `air`: cyan
  - `color`: violet-cyan
- Footer:
  - `lfo_rate`: yellow-cyan hybrid accent

## State Rules

- `base`: visible without bloom clipping.
- `hover`: +15-20% glow gain, no hue shift.
- `pressed`: slightly darker core + stronger outer ring.
- `disabled`: desaturated and lowered contrast, preserve silhouette.
- `on/off` toggles: color and luminance must both change.

## Export Readiness Targets

- All runtime assets split into modular layers (no monolithic flattened UI).
- Every runtime PNG exported at both `@1x` and `@2x`.
- Naming strictly follows `gz_<group>_<element>_<variant>_<state>_<size>.png`.
