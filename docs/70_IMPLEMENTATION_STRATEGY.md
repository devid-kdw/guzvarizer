# Implementation Strategy (Coding Approach)

## Goal

Isporuciti funkcionalan plugin bez arhitekturnih prepravki kasnije.

## Recommended coding approach

1. Contract-first
- prvo zakljucati parameter IDs/range/defaults
- zatim API izmedju `src/host`, `src/dsp` i `src/ui`

2. Vertical slices
- implementirati end-to-end po featureu:
  - parameter
  - DSP behaviour
  - UI kontrola
  - automation test

3. Realtime safety first
- ako postoji tradeoff izmedju audio sigurnosti i UI efekta, prednost ima audio sigurnost

4. Visual complexity staged
- prvo funkcionalni layout/controls
- zatim neon FX i dekoracije u odvojenom pass-u

5. Cross-platform by default
- svaka build promjena mora biti testabilna na `macOS` i `Windows`
- release artefakt je uvijek `VST3 (.vst3)`

## Suggested sequence

### Phase 1 - Contracts and registry
- `ParameterRegistry` + APVTS layout
- `MeterBridge` source/client
- editor skeleton uvezan na registry
- zakljucati `Gužvanje` + LFO parameter contract

### Phase 2 - Core compressor E2E
- threshold/attack/release/ratio/output kroz DSP i UI
- bypass path + host automation
- GR meter publish + display

### Phase 3 - Tone, vibe i Gužvanje E2E
- tone shaper DSP
- vibe mode enum + morph constants
- `Gužvanje` ON/OFF DSP branch
- LFO modulacija (Hz + tempo sync)
- UI segmented/toggle/knob binding
- disable logic za LFO UI kad je `Gužvanje` OFF

### Phase 4 - Theme and polish
- theme knobs i runtime update
- frame textures, glow passes, scanlines/grain
- final responsive tuning

### Phase 5 - Host validation and release matrix
- preset serialization
- automation playback reproducibility
- CPU and stability smoke u ciljanim DAW-ovima
- tempo sync interoperabilnost medju hostovima
- build i smoke test `Gužvarizer.vst3` za `macOS` i `Windows`

## Definition of quality

- nema audio artefakata zbog automation-a
- param restore tocan nakon reopen projekta
- UI update ne destabilizira audio thread
- `Gužvanje` OFF/ON ne proizvodi klikove
- LFO sync i Hz mod daju predvidljivo ponasanje
- `Gužvarizer.vst3` build prolazi na `macOS` i `Windows`
- vizualno podudaranje s referentnom slikom
