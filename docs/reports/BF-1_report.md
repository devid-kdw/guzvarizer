# Phase Report

## 1) Phase ID

`BF-1` — DSP ispravnost: Gužvanje/LFO per-sample

## 2) Scope completed

- Detektiran problem gdje je `CompressorCore::processBlock` prikupljao LFO bias samo jednom za cijeli buffer (na temelju zadnjeg procesiranog sample-a), stvarajući "stepeničaste" promjene thresholda i ratija.
- **`CompressorCore.h/.cpp`**: Promijenjen potpis za `processBlock` tako da sada (opcionalno) prima pointere na nizove (`float* thresholdBiases` i `float* ratioBiases`) koji nose per-sample modulacije.
- Unutar envelope follower i gain reduction loop-a u kompresoru svaka formula za prag i omjer dinamički izračunava trenutni iznos modulacije koristeći vrijednosti baznih parametara i per-sample biasa.
- **`GuzvarizerProcessor.h/.cpp`**: Dodani `std::vector<float>` spremnici (`guzThresholdBiases_` i `guzRatioBiases_`) koji se pre-alociraju u `prepareToPlay` na maksimalnu block size veličinu. Petlja za Gužvanje ispravno puni niz sample-po-sample s LFO / Crossfade evaluacijama i predaje pokazivače na te spremnike kompresoru. 

### Files changed/created
- `src/dsp/compressor/CompressorCore.h` (MODIFIED)
- `src/dsp/compressor/CompressorCore.cpp` (MODIFIED)
- `src/host/GuzvarizerProcessor.h` (MODIFIED)
- `src/host/GuzvarizerProcessor.cpp` (MODIFIED)
- `docs/reports/BF-1_report.md` (NEW)

## 3) Validation

| Command | Result |
|---|---|
| `cmake --build build --target Guzvarizer_VST3 --clean-first` | 100% built, exit 0 ✅ |

Nula C++ warninga, memory layout prilagođen (dodatne alokacije samo jednom u konstruktoru/prepare fazi) što garantira očuvanje realtime-safe `processBlock` filozofije. Step-wise LFO je popravljen u glatku krivulju unutar bloka.

## 4) Dependencies

Bez novih ovisnosti.

## 5) Issues / blockers

- **None**: Arhitektura spremnika za po-sample modulacije je uspješno integrirana bez dodavanja couplinga između DSP klasa.

## 6) Handoff notes

- **Za BF-2**: Spremni smo riješiti sigurnosne rubne slučajeve na CompressorCore i ToneShaper instancama. Posebice obrambeni DSP kod za extreme vrijednosti knobova.
- **Odobrenje**: BF-1 je rješen. Čekam odobrenje za pokretanje BF-2 radnog okvira.
