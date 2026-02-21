# Backend Agent Phased Tasks

## Hard Constraints

- Radi iskljucivo u: `/Users/grzzi/Desktop/Kompresor`
- Ne citati/pisati izvan `Kompresor` foldera.
- Nakon svake faze: STOP + report prema `docs/95_PHASE_REPORT_TEMPLATE.md`.

## Mandatory Reading

Procitati prije Phase BE-0:

1. `README.md`
2. `docs/00_PROJECT_PLAN.md`
3. `docs/20_BACKEND_AGENT_RULES.md`
4. `docs/30_ORCHESTRATOR_RULES.md`
5. `docs/40_PARAMETER_SPEC.md`
6. `docs/50_TECHSTACK_DECISION.md`
7. `docs/70_IMPLEMENTATION_STRATEGY.md`
8. `docs/80_RELEASE_TARGETS.md`
9. `docs/90_MASTER_IMPLEMENTATION_PLAN.md`
10. `docs/95_PHASE_REPORT_TEMPLATE.md`

## BE-0 Environment Bootstrap

Tasks:

- provjeri/instaliraj dependency-je potrebne za CMake + JUCE + compiler toolchain
- potvrdi `cmake -S . -B build` i prvi build attempt
- dokumentiraj blockers ako alat nedostaje

Exit criteria:

- build configure path je reproducibilan

## BE-1 Processor + Parameter Contracts

Tasks:

- finaliziraj `GuzvarizerProcessor` skeleton
- finaliziraj APVTS parameter layout prema `docs/40_PARAMETER_SPEC.md`
- provjeri stable parameter IDs/defaults/ranges

Exit criteria:

- Gate A zadovoljen

## BE-2 Core Compressor DSP

Tasks:

- implementiraj core kompresor processing path
- smoothing za audio parametre
- bypass behavior deterministican

Exit criteria:

- core compressor radi bez zipper noise

## BE-3 Tone + Vibe + Gužvanje + LFO DSP

Tasks:

- implementiraj tone shaper obradu
- implementiraj vibe-mode morph konstante
- implementiraj Gužvanje ON/OFF branch
- implementiraj LFO Hz/sync engine + host tempo fallback

Exit criteria:

- Gužvanje ON daje jacu kompresiju + LFO modulaciju
- OFF/ON transition bez klikova

## BE-4 Meter Bridge + Host State

Tasks:

- implementiraj thread-safe meter bridge source
- state save/load i preset restore putanje
- automation playback validacija

Exit criteria:

- UI polling ne blokira audio thread
- state lifecycle stabilan

## BE-5 Cross-Platform VST3 Matrix

Tasks:

- osiguraj VST3 build target na macOS i Windows
- CI/job matrix (ako je u scope-u) ili documented local matrix
- smoke validation u barem jednom hostu po platformi

Exit criteria:

- `Gužvarizer.vst3` build i smoke pass na obje platforme
