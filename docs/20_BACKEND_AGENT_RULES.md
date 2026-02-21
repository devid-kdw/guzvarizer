# Backend Agent Rules

## Mission

Implementirati stabilan audio engine, host integracije i thread-safe telemetry za UI.

## Stack

- JUCE AudioProcessor + APVTS
- C++20
- lock-free DSP-to-UI bridge

## Release target

- Build output mora biti `VST3 (.vst3)`.
- Podrzane platforme: `macOS` + `Windows`.

## Scope ownership

Backend agent je vlasnik:

- `src/dsp`
- `src/host`
- `src/shared` (uz koordinaciju orkestratora)

## Hard rules

1. Audio callback ne smije alocirati memoriju.
2. Parametri za audio moraju imati smoothing gdje je potrebno (anti-zipper).
3. Meter publish mora biti lock-free (atomics ili ring buffer).
4. UI polling nikad ne smije blokirati audio thread.
5. Bypass implementacija mora biti deterministicna (soft ili hard prema dogovoru).
6. Vibe mode mora mijenjati stvarne DSP konstante, ne samo UI state.
7. `Gužvanje` OFF/ON prijelaz mora biti bez klikova (smooth crossfade/ramp).
8. Preset serialization mora ukljuciti sve host-automatabilne parametre.
9. Build scripts i CI moraju imati `macOS` i `Windows` VST3 job.

## DSP requirements

- Compressor: threshold, attack(log), release(log), ratio, output gain.
- Tone shaper: saturation, harmonics, grit, air, air_freq/color_shift.
- Vibe modes:
  - SMOOTH: mekse koljeno, sporiji release bias.
  - PUNCH: brzi attack/release i agresivniji detector.
  - DIRTY: vise harmonics/saturation i cvrsci karakter.
- `Gužvanje` mode:
  - OFF: standard compressor behaviour.
  - ON: pojacana kompresija (npr. interni threshold bias + ratio bias) + aktivna LFO modulacija.

## LFO requirements

- Parametri: `lfo_rate_hz`, `lfo_sync_enabled`, `lfo_sync_division`.
- Ako je `lfo_sync_enabled = 0`: koristi se `lfo_rate_hz`.
- Ako je `lfo_sync_enabled = 1`: frekvencija dolazi iz host BPM + division tablice.
- Ako host tempo nije dostupan, fallback na `lfo_rate_hz`.
- LFO path je aktivan samo kad je `guzvanje_enabled = 1`.

Formula (sync mode):

`freq_hz = (bpm / 60.0f) / beatsPerCycle(lfo_sync_division)`

## Host requirements

- stable `ParameterRegistry` IDs
- automation read/write
- state save/load
- reset-safe init values
- host tempo/playhead access za LFO sync

## Testing minimum

- DSP unit testovi za range clamp i mapping
- Integration test: automation playback reproducibilnost
- Meter bridge test: bez data race-a i bez out-of-range skokova
- `Gužvanje` ON/OFF transition test bez klikova
- LFO sync test (BPM promjena -> ocekivani `freq_hz`)
- build smoke za `Gužvarizer.vst3` na `macOS` i `Windows`
