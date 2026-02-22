# Backend Fix Tasks (BF2)

## Scope

Ovaj dokument definira drugi val backend fixeva prije pune FE integracije.
Cilj: ukloniti audio/artifact rizike i osigurati stabilan host/runtime behavior.

## Hard constraints

- Radi iskljucivo unutar: `/Users/grzzi/Desktop/Kompresor`
- Nakon svake faze: STOP + report u `docs/reports/BF2-X_report.md`
- Audio callback mora ostati realtime-safe: bez lockova i bez heap alokacija

## Phase BF2-0: Baseline + scope lock

Tasks:

- procitati obaveznu dokumentaciju i sve BE/BF reportove
- potvrditi baseline build (`cmake -S . -B build -Wno-dev` + `cmake --build build --target Guzvarizer_VST3 -j4`)
- zapisati tocne ciljeve BF2 faza

Exit criteria:

- baseline build reproducibilan
- scope i prioritizacija jasni

## Phase BF2-1: Audio smoothing (kriticni bug)

Tasks:

- ukloniti block-step smoothing za kriticne parametre (`threshold`, `output`)
- zamijeniti `SmoothedValue::skip(numSamples)` sample-accurate pristupom (`getNextValue()` per-sample)
- po potrebi prosiriti DSP API tako da podrzi per-sample vrijednosti bez narusavanja realtime safety

Exit criteria:

- nema block-step skokova za `threshold`/`output`
- smoothing je sample-accurate u audio putanji

## Phase BF2-2: Buffer safety + state restore

Tasks:

- zastititi `guzThresholdBiases_` i `guzRatioBiases_` od out-of-bounds upisa kad `numSamples` premaši inicijalni kapacitet
- nakon `setStateInformation` napraviti reinit svih smoothera (`setCurrentAndTargetValue`) prema novim APVTS vrijednostima

Exit criteria:

- nema OOB rizika u bias bufferima
- preset/project restore ne uzrokuje cujne prijelaze zbog stale smoother state-a

## Phase BF2-3: DSP korekcije (ToneShaper + LFO sync)

Tasks:

- ispraviti `ToneShaper` air path u pravi LP state + HP ekstrakciju:
  - `low = alpha * low + (1 - alpha) * x`
  - `high = x - low`
- u `GuzvanjeDsp::setLfoSync`:
  - sigurno clampati `division` na `[0..7]` (ukljucujuci negativne)
  - ne zvati `setFrequency` ako efektivna frekvencija nije promijenjena (epsilon compare/cache)

Exit criteria:

- air path odgovara stabilnom high-frequency enhancement ponasanju
- LFO sync robustan i bez nepotrebnog reset/set churn-a

## Phase BF2-4: Final verification + known limitations

Tasks:

- full configure + build validacija
- dokumentirati poznata ogranicenja koja ostaju (ako ih ima, npr. attack/release smoothing granularnost)
- reportati tocno sto je DONE, a sto BLOCKED

Exit criteria:

- build green
- status transparentan i provjerljiv

## Phase BF2-5: Nice-to-have (ne blokira FE)

Tasks:

- branding audit za user-facing stringove (`Gužvarizer` gdje je primjenjivo)
- ako je u scope-u: spojiti `ColorStripIndicator` na RMS/reaktivni meter signal

Exit criteria:

- branding konzistentnost dokumentirana
- dodatne FE-facing integracije spremne ili jasno oznacene kao pending
