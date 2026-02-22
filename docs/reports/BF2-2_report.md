# Phase Report

## 1) Phase ID

`BF2-2` — Buffer safety + state restore

## 2) Scope completed

- Prepoznata su dva ključna rizika vezana uz stanje plugin instanci:
  1.  **Buffer overflows:** DAW (npr. FL Studio) može varirati `numSamples` u `processBlock`-u i poslat blok veći od onog obećanog u `prepareToPlay` (`samplesPerBlock`). Pošto su sada polja `sampleThresholds_`, `sampleRatios_` i `sampleOutputGains_` kritična, dodao sam `if (array.size() < numSamples)` provjeru na vrh callbacka. Iako `std::vector::resize` ovdje alocira, to će se dogoditi *isključivo jednom* ako host drastično prekrši dogovor, sprječavajući fatalni pad na način prihvaćen u sličnim defenzivnim patternima.
  2.  **Stale smoothed values:** Kod izmjene APVTS state-a kroz `setStateInformation` (tj. učitavanja preseta ili projekta iz DAW-a), smootheri su polagano nastavili putovanje s prethodne vrijednosti do nove umjesto da idu instanstno i bez zipperinga. Poslije de-serijalizacije iz XML staba izravno čitam atribute (`pThreshold_->load(...)`) i zovem `setCurrentAndTargetValue` na svih pet glavnih smoothera za instantni snap (reset).

### Files changed/created
- `src/host/GuzvarizerProcessor.cpp` (MODIFIED)
- `docs/reports/BF2-2_report.md` (NEW)

## 3) Validation

| Command | Result |
|---|---|
| `cmake --build build --target Guzvarizer_VST3 --clean-first` | 100% build success ✅ |

Build prolazi regularno. Dinamička alokacija `resize` stavljena je pod uvjet `requiredSize > size()` što minimizira njenu interakciju i ostavlja generalni `processBlock` performantnim.

## 4) Dependencies

Bez novih ovisnosti.

## 5) Issues / blockers

- Ne postoje daljnje blokade.

## 6) Handoff notes

- Sigurnosni temelj per-sample bufferinga je osiguran. Prelazimo na internu DSP logiku komponenata u fazi **BF2-3**, gdje treba ujediniti "Air" putanju (`ToneShaper`) i provjeriti `LFO Sync` division clamp.
