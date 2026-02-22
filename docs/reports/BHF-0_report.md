# Phase Report

## 1) Phase ID

`BHF-0` — Baseline + repro setup

## 2) Scope completed

- Potvrđen ispravan baseline build (`cmake -S . -B build` ... `Guzvarizer_VST3`), bez novih upozorenja ili grešaka.
- Pregledan je kod unutar `GuzvarizerProcessor::processBlock` i `ToneShaper::waveshape` / `ToneShaper::processBlock`.
- Pronađen je temeljni matematički razlog za slabu čujnost `SATURATION`, `HARMONICS` i `GRIT` parametara.
- Dokumentirana hipoteza kako bi se u kasnijim **BHF** koracima moglo ciljano fiksirati skalu i interakciju `COLOR` filtera.

### Files changed/created
- `docs/reports/BHF-0_report.md` (NEW)
- Ažurirana lista zadataka u `.gemini/antigravity/brain/.../task.md`.

## 3) Validation

| Command | Result |
|---|---|
| `cmake -S . -B build -Wno-dev` | Configuring/Generating done ✅ |
| `cmake --build build --target Guzvarizer_VST3 -j4` | 100% build success ✅ |

### Hipoteza o slaboj čujnosti parametara:
1. **Saturation/Drive:** Formulacija `const float driveGain = 1.0f + drive01 * 0.09f;` na maksimalnih 100% generira tek faktor od `1.09x` (~0.75 dB boost-a). To je apsolutno preslabo tjeranje signala u `std::tanh` soft-clip granice, tako da praktički nema clippinga na tipičnim razinama signala. Bilo bi ispravno `1.0f + drive01 * 9.0f` (multiplier x10 kako sugeriraju i prijašnji komentari).
2. **Harmonics:** Pošto `driveGain` skoro pa ne uvodi distorziju, razlika `(shaped - sample)` iznosi ~0. Parametar `harmonics` množi efektivno nulu i miješa je natrag sa glavnim signalom. 
3. **Grit:** Kvadratna asimetrija je implementirana kao `out += gritAmount * out * out`. Na prosječnoj niskoj razini audia od oko `-18 dBFS` (`out` oko ~0.125), `out * out` dobiva minoran broj od `0.015`. Množeno s maksimalnim `gritAmount` = `0.3`, dobiva se dodatak na signalnoj envelopi od oko `0.0045`. Slušatelj to ne percipira kao karakter.
4. **Color / Air Freq:** Crossover filter ovisi o tome postoji li iole high-frequency sadržaj koji on može pojačati. Bez saturacije (koja inače kreira harmoničke "više prelive"), i uz maksimalni Air gain od pukih `2.0x`, Air sekcija i "Color" frekvencija reagiraju izuzetno pitomo po visokom registru te jedva da je čujno. 

## 4) Dependencies

Bez novih ovisnosti.

## 5) Issues / blockers

- Ne postoje blokade. 

## 6) Handoff notes

- Krećemo u fazu **BHF-1**, prelazimo u EXECUTION mode i počinjemo aktivno mijenjati gain skalu i clipping stageunutar `ToneShaper.cpp`!
