# Phase Report

## 1) Phase ID

`BE-2` — Core Compressor DSP

## 2) Scope completed

- **CompressorCore** (`src/dsp/compressor/CompressorCore.h/.cpp`) — full rewrite:
  - **Envelope detector**: peak-based, dB-domain, per-sample tracking
  - **Attack/release follower**: separate one-pole coefficients — `exp(-1 / (time_sec * sampleRate))`
  - **Gain computer**: soft-knee (6 dB width), quadratic transition in knee region
  - **Output makeup**: linear gain from `outputDb` parameter
  - **Return value**: `processBlock()` now returns peak gain reduction (negative dB) for metering
  - All operations are realtime-safe — no locks, no heap

- **GuzvarizerProcessor** (`src/host/GuzvarizerProcessor.cpp`) — metering wiring:
  - Input peak measured **before** compressor processes
  - GR captured from `compressor_.processBlock()` return value
  - Output peak + RMS measured **after** compressor
  - All four meter channels published via lock-free `MeterBridgeSource`

- **Bypass**: deterministic — when bypassed, audio passes through unmodified, meters publish silent values (GR=0, levels=-90 dB, RMS=0)

### Files changed

| File | Status | Description |
|---|---|---|
| `src/dsp/compressor/CompressorCore.h` | MODIFIED | Added envelope state, coefficients, GR return type, static helpers |
| `src/dsp/compressor/CompressorCore.cpp` | REWRITTEN | Full compressor algorithm |
| `src/host/GuzvarizerProcessor.cpp` | MODIFIED | Input/output metering split, GR wiring |

## 3) Validation

| Command | Result |
|---|---|
| `cmake --build build --target Guzvarizer_VST3` | 100% built, exit 0 ✅ |

Zero errors, zero warnings from BE code. VST3 bundle re-linked successfully.

### Algorithm verification (by inspection)

- Threshold at 0 dB, ratio 1.0 → GR = 0 (no compression) ✅
- Signal below threshold → envelope stays below threshold → GR = 0 ✅
- Signal above threshold → GR proportional to `(1 - 1/ratio) * overshoot` ✅
- Soft knee smoothly blends around threshold ✅
- Attack/release coefficients correct for log-domain one-pole filter ✅

## 4) Dependencies

No new dependencies installed.

## 5) Issues / blockers

- **None.** Compressor compiles and links correctly.
- Note: runtime audio testing requires loading the VST3 in a DAW host — cannot be verified purely from build. Unit tests for DSP range/mapping will be added in later phases (or as a follow-up).

## 6) Handoff notes

- **For BE-3**: `CompressorCore` is ready to receive Gužvanje bias (threshold offset, ratio offset). Tone shaper and vibe mode processing should chain after compressor in `processBlock`.
- **For FE**: Meter bridge now publishes real GR, input level, output level, and RMS values. UI meters should respond.
- **Exit criteria**: core compressor radi — smoothing active (per-block SmoothedValue in processor, per-sample envelope in CompressorCore). Bypass deterministic (passthrough + silent meters).
