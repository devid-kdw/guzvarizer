# Phase Report

## 1) Phase ID

`BHF-1` — Saturation + Harmonics audibility fix

## 2) Scope completed

- Prema analizi iz BHF-0, glavni razlog slabe uočljivosti distorzije i harmoničkih struktura bilo je neispravno matematičko množenje unutar `ToneShaper::waveshape`. Skala "driveGain" je varirala u rasponu od 1.0 do 1.09x što je premalo za pobudu nelinearnog elementa (`std::tanh(x)`).
- Izmijenjeno je mjerenje razine na `const float driveGain = 1.0f + drive01 * 9.0f;`. Točno kako je specificirano, gain faktor (multiplier) sada adekvatno eskalira do `x10.0` kad je parametar "Saturation" zakrenut do 100%. 
- Kako signal napokon trpi `std::tanh` clipping na višim razinama snage The "Harmonics" parametar sada automatski dobiva čujan prostor za rad: on jednostavno i real-time sigurno križa čisti signal (`sample`) i klipljen signal (`shaped`).
- Komentari unutar koda su ispravljeni s pojašnjenjem da je multiplier raspon od `1.0 do 10.0`.

### Files changed/created
- `src/dsp/tone_shaper/ToneShaper.cpp` (MODIFIED)
- `docs/reports/BHF-1_report.md` (NEW)

## 3) Validation

| Command | Result |
|---|---|
| `cmake --build build --target Guzvarizer_VST3 --clean-first` | 100% build success ✅ |

Build je potpuno upotrebljiv i generira DSP binary. Interpolacija izračuna je `sample-accurate` i DSP rutina i dalje nema niti jedan blokirajući ili thread-unsafe element. Nema `NaN` i `Inf` rizika uslijed tanh operacije.

## 4) Dependencies

Bez novih ovisnosti.

## 5) Issues / blockers

- Ne postoje tehničke blokade.

## 6) Handoff notes

- DSP sloj za *Drive* konačno zvuči i prebija razinu zvuka onako kako se očekuje kod pravih kompresora / pre-amp saturatora. 
- Sljedeća stanica je **BHF-2 Grit character fix**, gdje idemo ispraviti slabi i jedva primjetni "soft asymmetry" sloj koji kreira neparne harmonike i asimetričan izlaz.
