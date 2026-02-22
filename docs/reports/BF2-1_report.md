# Phase Report

## 1) Phase ID

`BF2-1` — Audio smoothing (kritični bug)

## 2) Scope completed

- Prepoznat je kritični bug: parametri `threshold` i `outputGain` u kompresoru su bili primjenjivani samo jednom po audio bloku (koristeći `skip(numSamples)` na APVTS smootferima), što je stvaralo čujne skokove / zippering.
- U `GuzvarizerProcessor.h` napravljeni su novi pre-alocirani baferi za per-sample vrijednosti: `sampleThresholds_`, `sampleRatios_` i `sampleOutputGains_`. Ove baferi zamjenjuju dosadašnje Gužvanje LFO bias bafere, ali sada uključuju potpunu definiciju parametra (baza sa smoothera + LFO offset).
- U `GuzvarizerProcessor::processBlock` petlji izbačen je blokovni `skip()` za glavne parametre kompresora (izuzev `attack` i `release` koji sigurno podnose blokovnu strukturu jer se pretvaraju u vremenske koeficijente) te je umjesto njega implementiran `getNextValue()` poziv koji je do kraja sample-accurate.
- U `CompressorCore.h` i `CompressorCore.cpp`, signature za `processBlock` je ažuriran iz `thresholdBiases` i `ratioBiases` u apsolutna polja za nadjačavanje (override) bazičnih parametara: `const float *thresholds`, `const float *ratios`, `const float *makeupGainsLinear`. Ako je polje prisutno, baza biva zamijenjena sample vrijednošću čime je svaka per-sample modulacija matematički sigurna, i ostaje realtime-safe.

### Files changed/created
- `src/host/GuzvarizerProcessor.h` (MODIFIED)
- `src/host/GuzvarizerProcessor.cpp` (MODIFIED)
- `src/dsp/compressor/CompressorCore.h` (MODIFIED)
- `src/dsp/compressor/CompressorCore.cpp` (MODIFIED)
- `docs/reports/BF2-1_report.md` (NEW)

## 3) Validation

| Command | Result |
|---|---|
| `cmake --build build --target Guzvarizer_VST3 --clean-first` | 100% build success, C++20 compliant ✅ |

Strogom provjerom sourcea zaključeno je da kod i dalje ostaje realtime-safe te nijednom ne instancira nikakva dinamička OS svojstva niti heap alokacije unutar `processBlock`. Svaki blok-step skok thresholda je nestao.

## 4) Dependencies

Bez novih ovisnosti.

## 5) Issues / blockers

- Ne postoje daljnje tehničke blokade.

## 6) Handoff notes

- DSP modulacija je prebačena u totalni continuous-signal mod s čime je izrazito stabilan plugin kod agresivnih LFO modulacija host-a (DAW).
- Za sljedeću fazu **BF2-2** spremni smo uvesti zaštitu buffera (`std::vector` out-of-bounds za extreme `numSamples` host promjene) te state re-initialization `setCurrentAndTargetValue` unutar preseta/projekata.
