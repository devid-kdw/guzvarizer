# Gužvarizer - Execution Plan

## Scope

Implementirati VST plugin sa cyberpunk UI izgledom i punom funkcionalnoscu:

- compressor core (threshold, attack, release, ratio, output)
- bypass + host automation
- gain reduction meter + level visualizer
- tone shaper sekcija
- 3-state Vibe mode (Smooth/Punch/Dirty)
- `Gužvanje` mod (ON/OFF)
- LFO kontrola za Gužvanje (Hz mode + tempo sync mode)
- runtime theme engine (UI-only parametri)
- DSP-to-UI thread-safe bridge

## Tech baseline (locked)

- Language: `C++20`
- Framework: `JUCE 8.x`
- Build: `CMake 3.25+`
- Plugin format: `VST3 (.vst3)`
- Platform support: `macOS` + `Windows`

Reference: `docs/50_TECHSTACK_DECISION.md` i `docs/80_RELEASE_TARGETS.md`

## Workstreams

- FE stream: layout, kontrole, metering UI, theme engine, frame rendering.
- BE stream: DSP, parameter registry, automation, state, meter publishing.
- Integration stream: contracts, test gateovi, merge redoslijed.

## Milestones

### M1 - Foundations

- definiran `ParameterRegistry` (IDs, range, defaults, mapping)
- base plugin shell layout
- reusable Knob komponenta i Toggle/Segmented kontrole
- lock-free meter bridge skeleton

### M2 - Core Compressor

- DSP kompresor aktivan sa standardnim parametrima
- UI binding svih gornjih knobova + bypass
- gain reduction meter (UI smoothing + clamp)

### M3 - Tone + Vibe + Gužvanje

- tone shaper DSP parametri aktivni
- Vibe mode preset morphing (detector/knee/release + tone bias)
- `Gužvanje` mode:
  - OFF: standard compressor behavior
  - ON: jaca kompresija + aktivna LFO modulacija
- LFO controls:
  - rate u Hz
  - tempo sync + note division
  - LFO kontrola disabled kada je `Gužvanje` OFF
- reactive color strip indikator

### M4 - Theme + Polish

- runtime theme parametri (accent hue, glow, brightness, saturation)
- visual FX sloj (scanlines/grain/flicker) bez destabilizacije FPS-a
- final font/label/glow tuning, HiDPI resize provjera

### M5 - Host Readiness

- automation verification u hostu
- preset save/load serialization test
- CPU and XRUN sanity test
- host tempo sync validacija za LFO
- cross-platform build i smoke test (`macOS` + `Windows`)

## Task map po komponentama

1. Plugin Shell (layout, z-index, responsiveness): FE
2. Standard compressor knobs: FE + BE (binding + DSP)
3. Bypass toggle: FE + BE
4. Metering/display: FE + BE (bridge)
5. LED neon strip: FE (+ optional BE reactive input)
6. Tone shaper sekcija: FE + BE
7. Vibe mode switch: FE + BE
8. Gužvanje switch (ON/OFF): FE + BE
9. LFO rate control (Hz/sync) + disable logic: FE + BE
10. Theming engine: FE
11. Frame/decor draw pipeline: FE
12. Text/labels/fonts: FE
13. Host integrations: BE
14. DSP-to-UI komunikacija: BE (+ FE polling)
15. Build/release matrix za VST3 (`macOS` + `Windows`): BE + Orchestrator

## Integration gates

- Gate A: svi parametri imaju stabilne ID-eve i default vrijednosti.
- Gate B: UI nikad direktno ne cita audio buffer.
- Gate C: bypass, automation i preset restore rade kroz host lifecycle.
- Gate D: `Gužvanje` OFF/ON transition nema klikove ni dropove.
- Gate E: VST3 build artefakti prolaze za `macOS` i `Windows`.
- Gate F: meter/update loop ne prelazi dogovoreni CPU budget.

## Definition of done

- svi user-facing parametri rade i host ih moze automatizirati
- nema zipper noise na audio parametrima
- `Gužvanje` OFF daje standardnu kompresiju
- `Gužvanje` ON daje jacu kompresiju i aktivira LFO modulaciju
- LFO knob je disabled kada je `Gužvanje` OFF
- build daje `Gužvarizer.vst3` za `macOS` i `Windows`
- UI odgovara referentnom izgledu i ostaje citljiv pri skaliranju
- metering je stabilan, bez treperenja i out-of-range vrijednosti
