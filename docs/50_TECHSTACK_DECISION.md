# Tech Stack Decision - Gužvarizer

## Objective

Odabrati stack koji je najbolji za "pravi" VST plugin sa:

- realtime-safe DSP
- host automation/preset support
- custom cyberpunk UI sa kompleksnim drawing slojevima
- stabilnim build/deploy putem CMake-a
- `Gužvanje` modom koji koristi LFO rate u Hz i tempo-sync
- isporukom `VST3 (.vst3)` za `macOS` i `Windows`

## Evaluated options

### Option A: JUCE + C++20 + CMake (recommended)

Pros:

- industrijski standard za VST3 plugin development
- nativna integracija DSP + UI + host automation
- `AudioProcessorValueTreeState` i attachment pattern olaksavaju binding
- odlican control nad renderingom, HiDPI i custom paint pipeline-om
- lako odrzavanje lock-free DSP-to-UI kanala bez prelaska jezika/runtime-a
- pouzdan pristup host playhead podacima za tempo-synced LFO
- jednostavno cross-platform buildanje (`macOS` + `Windows`)

Cons:

- vise low-level koda u odnosu na web UI stack
- tim mora biti discipliniran oko realtime pravila

### Option B: iPlug2 + C++

Pros:

- lagan i performantan
- dobra kontrola nad low-level slojevima

Cons:

- manji ecosystem i manje standardiziran onboarding
- vise custom glue koda za neke host/UI workflowe

### Option C: C++ DSP + WebView UI (React/TS)

Pros:

- vrlo brz UI iteration
- jak frontend ecosystem

Cons:

- dodatna kompleksnost bridge-a i deploymenta
- rizik vecih latencija/jittera u UI update putanji
- veci operativni rizik za plugin distribuciju i cross-host stabilnost

## Final decision

**Primarni stack: JUCE + C++20 + CMake + Catch2 (testovi).**

Razlog: najbolji balans izmedju audio sigurnosti, host kompatibilnosti i custom UI kontrole za trazeni izgled, uz stabilan tempo-sync put za LFO i direktan put do VST3 builda na macOS/Windows.

## Concrete stack definition

- Language: `C++20`
- Framework: `JUCE 8.x` (AudioProcessor + GUI)
- Build: `CMake 3.25+`
- Plugin format: `VST3 (.vst3)`
- Platforme: `macOS` + `Windows`
- Testing:
  - `Catch2` za unit/integration testove
  - JUCE `UnitTest` opcionalno za UI smoke
- Static checks:
  - `clang-tidy`
  - `clang-format`

## Suggested architecture

- `src/dsp`: audio processing i mode logika
- `src/host`: parameter registry, automation, state/presets, meter bridge source
- `src/ui`: editor i komponente, look-and-feel, theme manager, meter bridge client
- `src/shared`: IDs, common math/types/constants

## Performance constraints

- audio thread: bez lockova i bez heap alokacija
- UI refresh: target 60 FPS, degradacija do 30 FPS ako host optereti GUI
- meter bridge: atomics ili lock-free ringbuffer
- LFO sync path: bez lockova i bez host query-a unutar sample-loopa

## Risks and mitigation

- Risk: UI predetaljan glow/shader moze gutati CPU/GPU.
  - Mitigation: multi-pass FX kao opcionalni quality level + cache slojeva.
- Risk: parametar mapping drift izmedju UI i DSP.
  - Mitigation: single source-of-truth u `src/shared` + `docs/40_PARAMETER_SPEC.md`.
- Risk: zipper noise pri automation playback-u.
  - Mitigation: per-parameter smoothing u DSP sloju.
- Risk: nekonzistentan tempo-sync medju hostovima.
  - Mitigation: fallback na `lfo_rate_hz` kad host tempo nije dostupan.
- Risk: OS-specific build regresije.
  - Mitigation: CI matrix i release smoke test za `macOS` i `Windows`.
