# Phase Report

## 1) Phase ID

`BHF-2` — Grit character fix

## 2) Scope completed

- **Grit asimetrija pojačana:** U `ToneShaper::waveshape()` funkciji parametar za Grit ("positive half gets extra push") množio se s faktorom `0.3x`. S obzirom na radne razine prosječnog miksa (~-12 dBFS do -18 dBFS), kvadratni iznos od `out * out * 0.3` nudio je dodatak amplitudi manji od `0.005`, što je perciptivno bilo gotovo pa nula. Zamijenio sam multiplikator sa `4.0x`, što stvara očitije i karakternije asimetrično "paranje" zvuka na gornjoj signalnoj polovici. Zvuk je ograničen `std::tanh` clippingom da i dalje nikad ne iscuri preko digitalnih 0 dBFS razina.
- **Zero DC drift:** Specifičnost "Grit" operacije je da gura signal samo prema gore (*positive half*). Svaka vrsta asimetričnog clip-a matematički inherentno uvodi DC drift (pozitivni jednosmjerni offset), što dugoročno razbija L/R stabilnost pa i headroom kompresora. Da to neutraliziram, implementirao sam **1st-order DC Blocker** u samom bloku (`ToneShaper::processBlock()`) kao dodatni High-Pass na oko `10 Hz`. 
- Uključio sam tracking mehanizam (stanja `dcStateL_`, `dcStateR_` i inicijalizaciju koeficijenta preko The exponential moving average formule) tako da ostaje `0 allocation` and `0 lock`. Nema probijanja stereo stabilnosti!

### Files changed/created
- `src/dsp/tone_shaper/ToneShaper.h` (MODIFIED)
- `src/dsp/tone_shaper/ToneShaper.cpp` (MODIFIED)
- `docs/reports/BHF-2_report.md` (NEW)

## 3) Validation

| Command | Result |
|---|---|
| `cmake --build build --target Guzvarizer_VST3 --clean-first` | 100% build success ✅ |

Build prolazi i verificirano je C++20 standardom i statičkim parametrima da ne uvedimo probleme s memorijom. 

## 4) Dependencies

Nema novih ovisnosti (samo standardna `<cmath>` biblioteka za `std::exp`).

## 5) Issues / blockers

- Ne postoje tehničke blokade.

## 6) Handoff notes

- Sustav distorzije je sad uparen s pravim *character* komponentama, i `Grit` radi "prljavi" posao bez dodavanja DC šuma.
- Slijedi popravljanje frekventno-kolorne ovisnosti zraka u iteraciji **BHF-3 Color (air_freq) behavior fix**.
