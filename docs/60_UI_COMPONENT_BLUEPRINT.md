# UI Component Blueprint

## Summary

Ovaj dokument mapira trazene UI elemente na konkretne C++ komponente u `src/ui`.

## Component map

1. Plugin shell:
- `src/ui/components/shell/MainPanel.h`
- `src/ui/components/shell/HeaderBar.h`
- `src/ui/components/shell/MainSection.h`
- `src/ui/components/shell/ToneShaperSection.h`
- `src/ui/components/shell/FooterBar.h`

2. Gornji kompresor knobovi:
- `src/ui/components/controls/NeonKnob.h`

3. Bypass:
- `src/ui/components/controls/BypassToggle.h`

4. Metering:
- `src/ui/components/metering/GainReductionMeter.h`
- `src/ui/components/metering/LevelVisualizer.h`

5. LED/neon strip:
- `src/ui/components/theme/ColorStripIndicator.h`

6. Tone shaper knobovi:
- `src/ui/components/controls/NeonKnob.h` (reuse)

7. Vibe mode switch:
- `src/ui/components/controls/VibeModeSegmentedControl.h`

8. Gužvanje controls (planned next):
- `src/ui/components/controls/GuzvanjeToggle.h`
- `src/ui/components/controls/LfoRateControl.h` (composite: knob + Hz/Sync mode + division)

9. Theme engine:
- `src/ui/theme/ThemeManager.h`
- `src/ui/theme/ThemeState.h`

10. Text system:
- `src/ui/components/text/SectionTitle.h`

11. Editor composition:
- `src/ui/app/PluginEditor.h`

12. Binding contracts:
- `src/ui/bindings/ParameterBindingAdapter.h`
- `src/ui/bindings/MeterBridgeClient.h`

## Layout and layering

Render order:

1. background/frame
2. section panels
3. controls/meters
4. glow pass
5. labels/text
6. overlay FX (grain/scanline)

## Interaction contract for Gužvanje

- `Gužvanje` toggle ON/OFF kontrolira `guzvanje_enabled`.
- `LfoRateControl` je disabled kada je `guzvanje_enabled=0`.
- `LfoRateControl` radi u dva prikaza:
  - Hz prikaz (`lfo_sync_enabled=0`, koristi `lfo_rate_hz`)
  - Sync prikaz (`lfo_sync_enabled=1`, koristi `lfo_sync_division`)

## Binding contracts

- UI controls su vezane na APVTS preko JUCE attachment klasa (`SliderAttachment`, `ButtonAttachment`, `ComboBoxAttachment`).
- `ParameterBindingAdapter` je thin wrapper oko attachment patterna i ne koristi custom thread-unsafe callback write/read mehanizam.
- meter komponente citaju samo bridge snapshot, nikad audio buffer.
- theme kontrole diraju samo UI state (`ThemeManager`).

## Next implementation step

- dodati `Gužvanje` toggle i `LfoRateControl` komponente
- uvezati komponente na `AudioProcessorValueTreeState` attachment sloj
- dodati asset loading (font/frame/texture) u `ThemeManager` i `MainPanel`
- finalizirati resizing metrics prema referentnoj slici
