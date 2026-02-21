# Gužvarizer Master Implementation Plan

## Purpose

Ovaj dokument je centralni orkestratorski plan za paralelan rad frontend i backend agenta.
Cilj: isporuka `Gužvarizer.vst3` za `macOS` i `Windows` kroz fazni workflow sa obaveznim reportom nakon svake faze.

## Workspace Boundary (hard lock)

Svi agenti smiju citati/pisati iskljucivo unutar:

`/Users/grzzi/Desktop/Kompresor`

Zabranjeno:

- rad u drugim folderima
- kreiranje fajlova izvan `Kompresor`
- mijenjanje globalnih system konfiguracija bez eksplicitnog odobrenja

## Mandatory Reading Order (before any code)

1. `README.md`
2. `docs/00_PROJECT_PLAN.md`
3. `docs/10_FRONTEND_AGENT_RULES.md`
4. `docs/20_BACKEND_AGENT_RULES.md`
5. `docs/30_ORCHESTRATOR_RULES.md`
6. `docs/40_PARAMETER_SPEC.md`
7. `docs/50_TECHSTACK_DECISION.md`
8. `docs/60_UI_COMPONENT_BLUEPRINT.md`
9. `docs/70_IMPLEMENTATION_STRATEGY.md`
10. `docs/80_RELEASE_TARGETS.md`
11. `docs/91_FRONTEND_AGENT_PHASED_TASKS.md` (FE)
12. `docs/92_BACKEND_AGENT_PHASED_TASKS.md` (BE)
13. `docs/95_PHASE_REPORT_TEMPLATE.md`
14. `docs/96_AGENT_EXECUTION_RULES.md`

## Dependency Policy

Agenti moraju instalirati dependency-je koje trebaju za svoju fazu, ali uz pravila:

- prioritet je lokalna instalacija ili projektno scoped dependency
- ako alat nije dostupan, agent smije pokrenuti install korak
- ako install trazi admin/globalne promjene, agent mora stati i reportati sta tocno treba
- svi build artefakti i cache moraju ostati unutar `Kompresor` (npr. `build/`, `.deps/`)

## Global Phase Rules

- Faze se izvode redom.
- Nakon svake faze agent mora stati.
- Nakon svake faze agent mora poslati report po `docs/95_PHASE_REPORT_TEMPLATE.md`.
- Bez reporta faza nije zatvorena.

## Phase Plan

### Phase 0 - Environment Bootstrap

Goal:

- potvrditi toolchain i dependency readiness
- postaviti reproducibilan build workflow

FE output:

- potvrdjen local run/build setup za UI compile path

BE output:

- potvrdjen CMake configure/build path i JUCE fetch/add_subdirectory path

Gate:

- build configure prolazi ili je dokumentiran blocker sa tocnim errorom

### Phase 1 - Contracts and Binding Foundation

Goal:

- APVTS i shared contracts stabilni
- UI attachments spojeni bez thread-unsafe putanje

FE output:

- svi postojeci UI controls vezani attachmentima

BE output:

- parameter layout i processor state skeleton stabilni

Gate:

- Gate A zadovoljen (stable IDs + defaults)

### Phase 2 - Core Compressor E2E

Goal:

- threshold/attack/release/ratio/output + bypass rade end-to-end

FE output:

- top controls i bypass imaju ispravan prikaz + host sync ponasanje

BE output:

- core compressor DSP branch aktivan + smoothing

Gate:

- automation playback bez zipper noise

### Phase 3 - Tone + Vibe + Gužvanje E2E

Goal:

- tone shaper, vibe mode i Gužvanje logika rade end-to-end
- LFO Hz i sync path aktivni

FE output:

- Gužvanje toggle i LFO UI (disable when OFF)

BE output:

- jaca kompresija kad je Gužvanje ON + LFO modulation path

Gate:

- Gužvanje OFF/ON bez klikova

### Phase 4 - Meter Bridge + Reactive Visuals

Goal:

- thread-safe DSP-to-UI meter pipeline
- stabilni meter refresh i smoothing

FE output:

- GR meter, level visualizer, color strip reactive update

BE output:

- lock-free meter source i snapshot publish

Gate:

- UI ne cita audio buffer direktno

### Phase 5 - Theme, Frame and UX Polish

Goal:

- runtime theming i final visual polish bez FPS regressions

FE output:

- theme knobs runtime effect, frame/decor pipeline, labels/fonts

BE output:

- nema regressije audio patha zbog UI/promjena

Gate:

- target 60 FPS, fallback 30 FPS bez vidljivog jittera

### Phase 6 - Release Validation

Goal:

- VST3 release readiness za macOS i Windows

FE output:

- UI smoke pass na obje platforme

BE output:

- build smoke `Gužvarizer.vst3`, preset restore i automation validation

Gate:

- `docs/80_RELEASE_TARGETS.md` checklist zatvoren
