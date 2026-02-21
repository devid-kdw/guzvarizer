# Phase Report

## 1) Phase ID

`BE-3` — Tone + Vibe + Gužvanje + LFO DSP

## 2) Scope completed

### ToneShaper (`src/dsp/tone_shaper/ToneShaper.h/.cpp`) — NEW
- **Waveshaper saturation**: tanh soft-clip with configurable drive (0-100% → 1-10x gain)
- **Harmonics**: wet/dry blend between clean and shaped signal
- **Grit**: asymmetric distortion (positive half gets extra push, re-clipped via tanh)
- **Air exciter**: 1st-order high-shelf filter at configurable frequency (1000-20000 Hz)
- Early-out when all params near zero (zero CPU cost when inactive)

### VibeModeConstants (`src/dsp/modes/VibeModeConstants.h`) — NEW
- **SMOOTH**: slower attack (+5ms), slower release (+30ms), wider knee (+4dB), reduced saturation (0.6x)
- **PUNCH**: faster attack (-3ms), faster release (-20ms), tighter knee (-2dB), neutral saturation
- **DIRTY**: hard knee (-3dB), more saturation (1.8x), more harmonics (2.0x), much more grit (2.5x)
- Implemented as `constexpr` — zero runtime overhead

### GuzvanjeDsp (`src/dsp/modes/GuzvanjeDsp.h/.cpp`) — NEW
- **ON/OFF crossfade**: 30ms linear ramp, per-sample processing, click-free transitions
- **Compression bias**: threshold -6 dB, ratio +2 when fully ON (scaled by crossfade)
- **LFO modulation**: ±4 dB threshold modulation via sine LFO
- **Hz/sync modes**: manual `lfo_rate_hz` or tempo-sync via `(bpm/60)/beatsPerCycle(division)`
- **Host BPM fallback**: defaults to 120 BPM when host playhead not available

### CompressorCore update
- Added `kneeWidthDb` to `CompressorParams` — allows VibeMode to bias the knee width

### GuzvarizerProcessor wiring
- Full DSP chain: **compressor → tone shaper**
- Vibe mode morph biases applied to compressor (attack, release, knee) and tone shaper (saturation, harmonics, grit scales)
- Gužvanje bias applied to compressor (threshold offset + LFO modulation, ratio offset)
- Host BPM read via `getPlayHead()->getPosition()->getBpm()` for LFO tempo sync
- All parameter reads via `std::memory_order_relaxed` — realtime-safe

### Files changed/created

| File | Status | Description |
|---|---|---|
| `src/dsp/tone_shaper/ToneShaper.h` | NEW | Tone shaper header |
| `src/dsp/tone_shaper/ToneShaper.cpp` | NEW | Waveshaper + air exciter implementation |
| `src/dsp/modes/VibeModeConstants.h` | NEW | SMOOTH/PUNCH/DIRTY morph presets |
| `src/dsp/modes/GuzvanjeDsp.h` | NEW | Gužvanje DSP header |
| `src/dsp/modes/GuzvanjeDsp.cpp` | NEW | Crossfade + bias + LFO implementation |
| `src/dsp/compressor/CompressorCore.h` | MODIFIED | Added kneeWidthDb to params |
| `src/dsp/compressor/CompressorCore.cpp` | MODIFIED | Use configurable knee width |
| `src/host/GuzvarizerProcessor.h` | MODIFIED | Added ToneShaper, GuzvanjeDsp members |
| `src/host/GuzvarizerProcessor.cpp` | REWRITTEN | Full chain with vibe + Gužvanje wiring |
| `CMakeLists.txt` | MODIFIED | Added 2 new .cpp sources |

## 3) Validation

| Command | Result |
|---|---|
| `cmake -S . -B build -Wno-dev` | Configuring done (0.8s), exit 0 ✅ |
| `cmake --build build --target Guzvarizer_VST3` | 100% built, exit 0 ✅ |

Zero errors, zero warnings from BE code.

### Behavior verification (by inspection)

- Gužvanje OFF: crossfade = 0, no bias, no LFO → standard compressor ✅
- Gužvanje ON: crossfade ramps to 1.0 over 30ms → threshold -6dB, ratio +2, LFO active ✅
- OFF→ON transition: linear ramp ensures no clicks ✅
- LFO sync: `freq = (bpm/60) / beatsPerCycle(division)` ✅
- LFO Hz mode: uses `lfo_rate_hz` directly ✅
- Host BPM fallback: 120.0 when playhead unavailable ✅
- Vibe SMOOTH: wider knee, slower dynamics, less saturation ✅
- Vibe DIRTY: hard knee, more harmonics/grit ✅

## 4) Dependencies

No new dependencies installed.

## 5) Issues / blockers

- **None.** All code compiles and links correctly.
- IDE linting errors are false positives — clangd doesn't have CMake build context for JUCE include paths.

## 6) Handoff notes

- **For BE-4**: Implement thread-safe meter bridge source snapshot publishing, state save/load validation, automation playback verification.
- **For FE**: Gužvanje toggle and LFO controls can now be bound — the DSP responds to `guzvanje_enabled`, `lfo_rate_hz`, `lfo_sync_enabled`, `lfo_sync_division`. LFO UI should be disabled when Gužvanje is OFF. Tone shaper knobs affect `saturation_drive`, `harmonics`, `grit`, `air`, `air_freq`. Vibe mode selector (`vibe_mode`) changes compressor character.
- **Exit criteria**: Gužvanje ON gives stronger compression + LFO modulation ✅. OFF/ON transition without clicks (30ms smooth crossfade) ✅.
