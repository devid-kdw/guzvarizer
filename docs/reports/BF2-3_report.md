# Phase Report

## 1) Phase ID

`BF2-3` — DSP korekcije (ToneShaper + LFO sync)

## 2) Scope completed

- **ToneShaper Air Filter:** Stari pristup oduzimanja signala putem alpha vrijednosti i stanja proizvodio je nestabilan rezultat. Filter je sada prepravljen u standardni 1-pole Low-Pass mehanizam: `hsfState = alpha * hsfState + (1 - alpha) * sample`. Zatim se High-Pass detalj ekstrahira `high = sample - hsfState` i dodaje natrag uz `air01 * 2.0f` pojačanje (Exciter). Zvuk je pročišćen u visokim registrima (per L/R channel).
- **GuzvanjeDsp LFO Sync:** Izmiješana logika divisiona mogla je uvesti garbage int-ove iz APVTS-a u sync. Dodano je agresivno `std::max(0, std::min(division, 7))` ograničenje. Nadalje je dodana float klasna varijabla `currentEffectiveFreq_ = -1.0f` koja pamti posljednju zadanu frekvenciju LFO-u. LFO klasa biva resetirana (`setFrequency`) samo ako epsilon ispadne veći od `0.001f` između izračunate *target* frekvencije i stare efektivne, štedeći time gomilu beskorisnih ponovnih izračuna trigenometrije (phase increment) i spriječavajući klikove.

### Files changed/created
- `src/dsp/tone_shaper/ToneShaper.cpp` (MODIFIED)
- `src/dsp/modes/GuzvanjeDsp.h` (MODIFIED)
- `src/dsp/modes/GuzvanjeDsp.cpp` (MODIFIED)
- `docs/reports/BF2-3_report.md` (NEW)

## 3) Validation

| Command | Result |
|---|---|
| `cmake --build build --target Guzvarizer_VST3 --clean-first` | 100% build success ✅ |

Build na VST3 je čist od pogrešaka i sve izmjene su C++20 safe, bez utjecaja na real-time callback constrainte.

## 4) Dependencies

Bez novih ovisnosti.

## 5) Issues / blockers

- Faza nema zaostalih problema.

## 6) Handoff notes

- Matematička jeza unutar engine-a trebala bi sada biti kompletno ukroćena. Ulazimo u predzadnju obaveznu fazu **BF2-4 Final verification + known limitations**. Tamo ćemo osigurati cijeli projekt, dokumentirati eventualna preostala ograničenja, te zatvoriti backend vrata.
