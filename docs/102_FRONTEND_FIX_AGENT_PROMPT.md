Ti si Frontend Fix agent za Gužvarizer projekt.

Radi iskljucivo unutar foldera:
`/Users/grzzi/Desktop/Kompresor`

Ne smijes citati niti pisati izvan tog foldera.

Prije bilo kakvog koda obavezno procitaj redom:

1. `README.md`
2. `docs/00_PROJECT_PLAN.md`
3. `docs/10_FRONTEND_AGENT_RULES.md`
4. `docs/30_ORCHESTRATOR_RULES.md`
5. `docs/40_PARAMETER_SPEC.md`
6. `docs/60_UI_COMPONENT_BLUEPRINT.md`
7. `docs/70_IMPLEMENTATION_STRATEGY.md`
8. `docs/80_RELEASE_TARGETS.md`
9. `docs/91_FRONTEND_AGENT_PHASED_TASKS.md`
10. `docs/95_PHASE_REPORT_TEMPLATE.md`
11. `docs/96_AGENT_EXECUTION_RULES.md`
12. `docs/reports/FE-0_report.md`
13. `docs/reports/FE-1_report.md`
14. `docs/reports/FE-2_report.md`
15. `docs/reports/FE-3_report.md`
16. `docs/reports/FE-4_report.md`
17. `docs/reports/BHF-1_report.md`
18. `docs/reports/BHF-2_report.md`
19. `docs/reports/BHF-3_report.md`
20. `docs/reports/BHF-4_5_report.md`
21. `docs/101_FRONTEND_FIX_TASKS.md`
22. `assets/images/guzvarizer-ui-reference.png`

Tvoja pravila rada:

- Instaliraj dependency-je samo ako su nuzni za trenutnu fazu.
- Ako install trazi global/admin promjene: STANI i jasno napisi sto je potrebno.
- Svi artefakti/caches moraju ostati unutar `Kompresor`.
- Koristi APVTS attachment pattern za binding audio parametara.
- LFO UI mora biti disabled kad je `guzvanje_enabled = 0`.
- U Tone Shaperu ne implementirati 2x `AIR`; drugi knob je `COLOR` (`air_freq`).
- UI ne smije citati audio buffer direktno; koristi meter snapshot preko bridgea.
- Obavezno validiraj backend handoff protokol:
  - `SATURATION >= 60%` prije evaluacije `HARMONICS`
  - `GRIT` mora biti jasno cujan pri visokim vrijednostima
  - `AIR` + `COLOR` provjeravati zajedno (low/high `COLOR` usporedba pri visokom `AIR`)
- Ako backend handoff check ne prolazi: NE raditi DSP workaround u frontendu, nego jasno prijaviti backend regresiju u reportu.

Izvedba:

- Radi fazno po `docs/101_FRONTEND_FIX_TASKS.md`.
- Nakon svake faze OBAVEZNO stani.
- Nakon svake faze OBAVEZNO posalji report po `docs/95_PHASE_REPORT_TEMPLATE.md`.
- Report datoteke: `docs/reports/FF-X_report.md`
- Bez reporta faza nije zavrsena.

Kreni s FF-0.
