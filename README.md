# Gužvarizer - Project Skeleton

Ovaj repozitorij je postavljen za paralelan rad vise agenata (frontend, backend, orchestrator) na VST pluginu "Gužvarizer".

## Branding rule

- User-facing naziv plugina mora biti tocno: `Gužvarizer` (sa slovom `ž`).
- Naziv moda mora biti tocno: `Gužvanje` (sa slovom `ž`).
- Interni ID-evi u kodu ostaju ASCII (`guzvanje_*`, `lfo_*`) radi host kompatibilnosti.

## Zakljucani tech stack

- `C++20`
- `JUCE 8.x`
- `CMake 3.25+`
- Plugin target: `VST3 (.vst3)`
- Platforme: `macOS` + `Windows`

## Release target

- Isporucuje se VST format kroz `VST3` artefakt (`.vst3`) za obje platforme.
- Reference: `docs/80_RELEASE_TARGETS.md`

Detaljna analiza i tradeoffi:

- `docs/50_TECHSTACK_DECISION.md`

## Folder struktura

```text
Kompresor/
  docs/
    00_PROJECT_PLAN.md
    10_FRONTEND_AGENT_RULES.md
    20_BACKEND_AGENT_RULES.md
    30_ORCHESTRATOR_RULES.md
    40_PARAMETER_SPEC.md
    50_TECHSTACK_DECISION.md
    60_UI_COMPONENT_BLUEPRINT.md
    70_IMPLEMENTATION_STRATEGY.md
    80_RELEASE_TARGETS.md
    90_MASTER_IMPLEMENTATION_PLAN.md
    91_FRONTEND_AGENT_PHASED_TASKS.md
    92_BACKEND_AGENT_PHASED_TASKS.md
    93_FRONTEND_AGENT_PROMPT.md
    94_BACKEND_AGENT_PROMPT.md
    95_PHASE_REPORT_TEMPLATE.md
    96_AGENT_EXECUTION_RULES.md
  assets/
    fonts/
    images/
    textures/
    frames/
    glow/
  src/
    ui/
      app/
      components/
        shell/
        controls/
        metering/
        tone_shaper/
        theme/
        text/
        common/
      layout/
      theme/
      fx/
      bindings/
    dsp/
      compressor/
      tone_shaper/
      modes/
      metering/
      util/
    host/
      parameters/
      automation/
      presets/
      state/
      bridge/
    shared/
      types/
      constants/
      math/
  presets/
  tests/
    ui/
    dsp/
    integration/
  scripts/
  .github/
    workflows/
```

## Ownership

- Frontend agent: `src/ui`, UI asset wiring, theme pipeline.
- Backend agent: `src/dsp`, `src/host`, DSP i host integracije.
- Orchestrator: `docs`, interface contracti, integration gate i release redoslijed.

## Implementirano u ovom koraku (foundation skeleton)

- plugin shell komponente:
  - `src/ui/components/shell/MainPanel.h`
  - `src/ui/components/shell/HeaderBar.h`
  - `src/ui/components/shell/MainSection.h`
  - `src/ui/components/shell/ToneShaperSection.h`
  - `src/ui/components/shell/FooterBar.h`
- kontrolne komponente:
  - `src/ui/components/controls/NeonKnob.h`
  - `src/ui/components/controls/BypassToggle.h`
  - `src/ui/components/controls/VibeModeSegmentedControl.h`
- metering i vizual:
  - `src/ui/components/metering/GainReductionMeter.h`
  - `src/ui/components/metering/LevelVisualizer.h`
  - `src/ui/components/theme/ColorStripIndicator.h`
- theme i editor:
  - `src/ui/theme/ThemeManager.h`
  - `src/ui/app/PluginEditor.h`
- UI bridge contracts:
  - `src/ui/bindings/MeterBridgeClient.h`
  - `src/ui/bindings/ParameterBindingAdapter.h`
- build i host foundation:
  - `CMakeLists.txt`
  - `src/host/GuzvarizerProcessor.h`
  - `src/host/parameters/ParameterLayout.h`
  - `src/shared/types/LfoSyncDivision.h`

Prije implementacije obavezno procitati:

1. `docs/00_PROJECT_PLAN.md`
2. `docs/10_FRONTEND_AGENT_RULES.md`
3. `docs/20_BACKEND_AGENT_RULES.md`
4. `docs/30_ORCHESTRATOR_RULES.md`
5. `docs/40_PARAMETER_SPEC.md`
6. `docs/50_TECHSTACK_DECISION.md`
7. `docs/60_UI_COMPONENT_BLUEPRINT.md`
8. `docs/70_IMPLEMENTATION_STRATEGY.md`
9. `docs/80_RELEASE_TARGETS.md`
10. `docs/90_MASTER_IMPLEMENTATION_PLAN.md`
11. `docs/91_FRONTEND_AGENT_PHASED_TASKS.md`
12. `docs/92_BACKEND_AGENT_PHASED_TASKS.md`
13. `docs/95_PHASE_REPORT_TEMPLATE.md`
14. `docs/96_AGENT_EXECUTION_RULES.md`
