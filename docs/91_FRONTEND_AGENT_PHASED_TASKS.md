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
8. `docs/90_MASTER_IMPLEMENTATION_PLAN.md`
9. `docs/95_PHASE_REPORT_TEMPLATE.md`

## FE-0 Environment Bootstrap

Tasks:

- provjeri dostupnost alata (`cmake`, compiler, optional `ninja`)
- instaliraj nedostajuce dependency-je potrebne za FE rad
- potvrdi da mozes konfigurirati project build tree

Exit criteria:

- documented toolchain status
- documented install actions

## FE-1 UI Foundation + Attachments

Tasks:

- auditiraj `src/ui` komponente i attachment wiring
- osiguraj APVTS attachment integraciju za sve postojece kontrole
- ukloni preostale thread-unsafe UI write putanje

Exit criteria:

- controls rade kroz attachment pattern
- nema custom direct write callback patha za audio parametre

## FE-2 Core Controls UX

Tasks:

- finaliziraj gornje knobove i bypass UX states
- potvrdi preset restore prikaz i automation mirror u UI
- doradi label/readout consistency

Exit criteria:

- THRESHOLD/ATTACK/RELEASE/RATIO/OUTPUT + BYPASS UX stabilni

## FE-3 Gužvanje + LFO Controls

Tasks:

- implementiraj `Gužvanje` toggle komponentu
- implementiraj `LfoRateControl` (Hz/sync + division)
- enforce disable state kad je `guzvanje_enabled = 0`

Exit criteria:

- LFO controls tocno prate `lfo_sync_enabled` i `guzvanje_enabled`

## FE-4 Metering + Reactive Visuals

Tasks:

- spoji UI meter komponente na `MeterBridgeClient`
- stabiliziraj smoothing/clamp/fps update
- spoji reactive color strip na meter input

Exit criteria:

- meter i vizuali rade bez direct audio buffer pristupa

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
