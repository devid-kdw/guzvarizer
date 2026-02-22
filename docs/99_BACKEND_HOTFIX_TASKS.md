# Backend Hotfix Tasks (BHF)

## Scope

Ovaj hotfix krug je fokusiran na funkcionalne probleme prijavljene nakon DAW testiranja:

- UI izgled nije backend scope, ali backend mora osigurati da svi Tone Shaper parametri imaju jasno cujan efekt.
- Problematicni parametri: `saturation_drive`, `harmonics`, `grit`, `air_freq` (`COLOR`).
- Cilj: cujna, stabilna i predvidiva DSP reakcija bez regresija i bez kršenja realtime pravila.

## Hard constraints

- Radi iskljucivo unutar: `/Users/grzzi/Desktop/Kompresor`
- Ne citati/pisati izvan `Kompresor`.
- Nakon svake faze: STOP + report u `docs/reports/BHF-X_report.md`
- Audio callback mora ostati realtime-safe:
  - bez lockova
  - bez heap alokacija unutar audio petlje

## Mandatory reading

Procitati prije BHF-0:

1. `README.md`
2. `docs/20_BACKEND_AGENT_RULES.md`
3. `docs/30_ORCHESTRATOR_RULES.md`
4. `docs/40_PARAMETER_SPEC.md`
5. `docs/50_TECHSTACK_DECISION.md`
6. `docs/70_IMPLEMENTATION_STRATEGY.md`
7. `docs/95_PHASE_REPORT_TEMPLATE.md`
8. `docs/96_AGENT_EXECUTION_RULES.md`
9. `docs/reports/BF2-1_report.md`
10. `docs/reports/BF2-2_report.md`
11. `docs/reports/BF2-3_report.md`
12. `docs/reports/BF2-4_5_report.md`
13. `docs/99_BACKEND_HOTFIX_TASKS.md`

## BHF-0 Baseline + repro setup

Tasks:

- potvrdi baseline build (`cmake -S . -B build -Wno-dev` + `cmake --build build --target Guzvarizer_VST3 -j4`)
- pregledaj `ToneShaper` i host param wiring (`GuzvarizerProcessor`)
- dokumentiraj hipotezu zasto su `SATURATION/HARMONICS/COLOR` slabo cujni

Exit criteria:

- reproduciran i dokumentiran problem
- plan izmjena spreman

## BHF-1 Saturation + Harmonics audibility fix

Tasks:

- ispraviti drive mapiranje tako da odgovara realnom rasponu (npr. 1.0 -> 10.0 ili slicno dokumentirano)
- osigurati da `harmonics` ima jasan perceptualni raspon kroz cijeli 0-100
- dodati/azurirati inline komentare s jasnim mapiranjem i razlogom

Exit criteria:

- `SATURATION` i `HARMONICS` su jasno cujni u DAW smoke testu
- nema clipping/NaN regresija

## BHF-2 Grit character fix

Tasks:

- pojacati i stabilizirati `GRIT` karakter da bude konzistentno cujan (ali kontroliran)
- provjeriti ponasanje pri ekstremnim vrijednostima (`0`, `100`)
- osigurati da grit ne razbije stereo stabilnost i ne uvodi DC drift

Exit criteria:

- `GRIT` ima jasan efekt i upotrebljiv raspon
- nema nestabilnosti ili nenormalnih izlaza

## BHF-3 Color (air_freq) behavior fix

Tasks:

- osigurati da `COLOR` (`air_freq`) ima cujnu i smisleno mapiranu promjenu karaktera
- ako je `COLOR` ovisan o `AIR` kolicini, to eksplicitno dokumentirati i kalibrirati da korisnicko ponasanje bude intuitivno
- provjeriti filter koeficijent bounds i stabilnost

Exit criteria:

- `COLOR` ima cujan efekt u realnom radu
- dokumentirano ponasanje `AIR` + `COLOR` interakcije

## BHF-4 Realtime safety + regression

Tasks:

- audit da nema novih heap/lock operacija u audio callbacku
- ako postoje postojeci defensivni `resize` pozivi u callbacku, minimizirati rizik i dokumentirati tradeoff
- full build + static code sanity check

Exit criteria:

- build green
- realtime safety status transparentno dokumentiran

## BHF-5 DAW validation + handoff

Tasks:

- backend smoke checklist za Tone Shaper parametre (kratki test-protokol)
- zapisati konkretne postavke koje dokazuju da svaki knob ima cujan efekt
- predati handoff napomene frontend agentu (sto je promijenjeno u sonic karakteru)

Exit criteria:

- jasna potvrda da `SATURATION`, `HARMONICS`, `GRIT`, `COLOR`, `AIR` rade perceptualno smisleno
- backend hotfix spreman za FE nastavak
