# Frontend Fix Tasks (FF)

## Scope

Ovaj fix krug je fokusiran na:

- vizualni parity s referentnom slikom (`assets/images/guzvarizer-ui-reference.png`)
- UX i layout korekcije koje su vidljive u DAW-u
- robustan state-sync UI logike (Gužvanje/LFO)
- bez mijenjanja DSP logike (osim ako je explicitno dogovoreno)

Napomena:

- Referentna slika ima 2x `AIR` labelu u Tone Shaperu.
- Finalni UI mora imati `AIR` + `COLOR` (ne 2x AIR).

## Hard constraints

- Radi iskljucivo u: `/Users/grzzi/Desktop/Kompresor`
- Ne citati/pisati izvan `Kompresor` foldera.
- Nakon svake faze: STOP + report u `docs/reports/FF-X_report.md`
- Koristi APVTS attachment pattern za sve audio parametre.
- UI ne smije citati audio buffer direktno.

## Mandatory reading

Procitati prije FF-0:

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
21. `assets/images/guzvarizer-ui-reference.png`

## Backend handoff contract (obavezno za FE)

- `SATURATION` mora imati jasan rast karaktera od cca 60% prema gore.
- `HARMONICS` validirati tek nakon sto je `SATURATION` >= 60% (inace je razlika prirodno manja).
- `GRIT` mora biti jasno cujan na visokim vrijednostima, bez DC artefakata i bez nestabilnosti.
- `AIR` + `COLOR` (`air_freq`) testirati zajedno: razlika mora biti ocita izmedu niskog i visokog `COLOR` kad je `AIR` znacajno podignut.
- Frontend ne smije "fejkati" da parametar radi kroz cistu animaciju. Ako audit ne prolazi, otvoriti backend regresiju umjesto DSP workarounda u UI sloju.

## FF-0 Baseline + reference gap audit

Tasks:

- potvrdi build (`cmake -S . -B build -Wno-dev` + `cmake --build build --target Guzvarizer_VST3 -j4`)
- napravi vizualni gap audit: trenutni UI vs referenca
- popisi tocke koje moraju biti "must-match" (frame, top bar, knob stil, meter blok, footer stil)
- izvrsi backend handoff smoke-check prije UI fixeva:
  - `SATURATION` >= 60%, zatim vrtjeti `HARMONICS`
  - `GRIT` na 100% i potvrditi jasan karakter
  - `AIR` visoko + `COLOR` low/high usporedba

Exit criteria:

- dokumentiran gap audit i prioriteti
- dokumentiran backend handoff smoke-check rezultat (PASS/FAIL po tocki)
- nema blocker-a za FF-1

## FF-1 Shell reference parity (frame + panel hierarchy)

Tasks:

- redizajniraj `MainPanel`, `HeaderBar`, `MainSection`, `ToneShaperSection`, `FooterBar` da prate referentnu kompoziciju
- uvedi ispravan layer redoslijed: background -> panel/frame -> controls -> glows -> text -> overlays
- ukloni "flat default" izgled; uvedi sci-fi frame i sekcijske obrube koji vizualno prate referencu

Exit criteria:

- layout i proporcije su prepoznatljivo reference-like u DAW prozoru
- nema clipping problema pri default i min velicini

## FF-2 Knob visual system parity

Tasks:

- doradi `NeonKnob` tako da izgleda kao referenca (ring, ticks, pointer/cap stil, glow intenzitet)
- uskladiti font, label i value readout estetiku
- zadrzati postojece param bindinge

Exit criteria:

- gornji i donji knobovi vizualno odgovaraju referenci
- svi knobovi i dalje rade kroz attachment

## FF-3 Footer UX fixes (Vibe + Gužvanje + LFO)

Tasks:

- doradi footer raspored da nema truncation (`SMO...`) i da prati referentni izgled segmentiranih kontrola
- popravi `LfoRateControl` relayout/visibility kada se mijenja sync mode
- osiguraj robustan enable/disable LFO stanja i kad parametar dode iz host automation/preset restore (ne samo onClick path)

Exit criteria:

- `Gužvanje` i `LFO` UI stanja su konzistentna u svim scenarijima
- footer je cist i citljiv bez truncation artefakata

## FF-4 Meter block + color strip reference parity

Tasks:

- doradi desni meter blok (top visualizer + gain reduction zona) da bude reference-like
- doradi reactive color strip vizual i animaciju prema referenci
- zadrzi performantne repaint pragove bez gubitka responzivnosti

Exit criteria:

- meter i strip izgledaju i ponasaju se kao referenca
- nema flicker/jitter regresija

## FF-5 Typography + theme polish

Tasks:

- uskladiti naslove i label tipografiju s referentnim stilom (bez default dojma)
- smanjiti hardcoded boje i centralizirati kroz ThemeManager gdje je practical
- finalni polish glow/contrast/readability

Exit criteria:

- UI ima konzistentan cyberpunk vizualni identitet
- runtime theme promjene i dalje rade

## FF-6 Final DAW smoke + handoff

Tasks:

- DAW smoke checklist (minimalno: bypass, main knobs, tone section, vibe/guzvanje/lfo, meter)
- ponoviti backend handoff smoke-check i potvrditi da FE promjene nisu maskirale/regresirale percepciju tone parametara
- dokumentirati eventualne preostale razlike od reference
- predati final handoff s listom known issues (ako postoje)

Exit criteria:

- UI spreman za korisnicki test
- preostale razlike (ako ih ima) jasno popisane i male
