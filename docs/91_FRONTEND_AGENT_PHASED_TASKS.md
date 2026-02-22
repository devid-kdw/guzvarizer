# Frontend Agent Phased Tasks

## Hard Constraints

- Radi iskljucivo u: `/Users/grzzi/Desktop/Kompresor`
- Ne citati/pisati izvan `Kompresor` foldera.
- Nakon svake faze: STOP + report prema `docs/95_PHASE_REPORT_TEMPLATE.md`.

## Mandatory Reading

Procitati prije Phase FE-0:

1. `README.md`
2. `docs/00_PROJECT_PLAN.md`
3. `docs/10_FRONTEND_AGENT_RULES.md`
4. `docs/30_ORCHESTRATOR_RULES.md`
5. `docs/40_PARAMETER_SPEC.md`
6. `docs/60_UI_COMPONENT_BLUEPRINT.md`
7. `docs/70_IMPLEMENTATION_STRATEGY.md`
8. `docs/80_RELEASE_TARGETS.md`
9. `docs/90_MASTER_IMPLEMENTATION_PLAN.md`
10. `docs/95_PHASE_REPORT_TEMPLATE.md`
11. `docs/96_AGENT_EXECUTION_RULES.md`
12. `docs/reports/BF2-1_report.md`
13. `docs/reports/BF2-2_report.md`
14. `docs/reports/BF2-3_report.md`
15. `docs/reports/BF2-4_5_report.md`
16. `assets/images/guzvarizer-ui-reference.png`

## FE-0 Backend Handoff Validation

Tasks:

- potvrdi da backend trenutno builda (`cmake -S . -B build -Wno-dev` + `cmake --build build --target Guzvarizer_VST3 -j4`)
- auditiraj backend->UI handoff tocke:
  - `GuzvarizerProcessor::meterSource()`
  - `PluginEditor` timer polling (`AtomicMeterBridgeClient`)
  - APVTS dostupnost za sve parametre iz speca
- dokumentiraj FE pretpostavke i otvorena pitanja prije UI implementacije

Exit criteria:

- backend handoff potvrden
- FE implementacija moze krenuti bez blokera

## FE-1 UI Foundation + Attachments

Tasks:

- auditiraj `src/ui` komponente i attachment wiring
- osiguraj APVTS attachment integraciju za sve postojece i nove kontrole
- ukloni preostale direct param write putanje koje ne koriste attachment pattern

Exit criteria:

- controls rade kroz attachment pattern
- nema custom direct write callback patha za audio parametre

## FE-2 Core Controls UX

Tasks:

- finaliziraj gornje knobove i bypass UX states
- potvrdi preset restore prikaz i automation mirror u UI
- doradi label/readout consistency
- primijeni reference correction: 2x `AIR` iz slike nije validan; final mora biti `AIR` + `COLOR`

Exit criteria:

- THRESHOLD/ATTACK/RELEASE/RATIO/OUTPUT + BYPASS UX stabilni

## FE-3 Gužvanje + LFO Controls

Tasks:

- implementiraj `Gužvanje` toggle komponentu i bind na `guzvanje_enabled`
- implementiraj `LfoRateControl` (rate + sync mode + division) i bind na:
  - `lfo_rate_hz`
  - `lfo_sync_enabled`
  - `lfo_sync_division`
- enforce disable state:
  - cijeli LFO control disabled kad je `guzvanje_enabled = 0`
  - division UI aktivan samo kad je `lfo_sync_enabled = 1`

Exit criteria:

- Gužvanje i LFO kontrole potpuno bindane
- disable/enable logika tocno prati parametre

## FE-4 Metering + Reactive Visuals

Tasks:

- spoji `GainReductionMeter` i `LevelVisualizer` na backend meter snapshot (bez direct audio buffer pristupa)
- spoji `ColorStripIndicator` na `rms_normalized` (ili dogovoreni reactive signal)
- stabiliziraj smoothing/clamp/fps update bez prekomjernog CPU troska

Exit criteria:

- meter i reactive strip rade stabilno
- nema UI thread blokiranja ni DSP thread pristupa

## FE-5 Theme + Final Polish

Tasks:

- runtime theme propagation za sve bitne kontrole
- smanji hardcoded boje gdje je practical
- zavrsni responsive i HiDPI tuning

Exit criteria:

- theme promjene su vidljive runtime
- vizualno podudaranje s referencom je prihvatljivo

## FE-6 Release Smoke

Tasks:

- FE smoke checklist na macOS i Windows build artefaktu
- documented UI issues/regressions (ako postoje)

Exit criteria:

- FE dio `docs/80_RELEASE_TARGETS.md` checklist zatvoren
