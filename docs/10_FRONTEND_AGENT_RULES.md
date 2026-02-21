# Frontend Agent Rules

## Mission

Implementirati UI koji vizualno prati referencu i tehnicki je odvojen od DSP threada.

## Stack

- JUCE `Component` architecture
- C++20
- custom paint + layout (`FlexBox`/manual grids)

## Naming and labels

- User-facing naziv plugina: `Gužvarizer`.
- Naziv moda: `Gužvanje`.
- UI mora prikazivati `ž` u svim vidljivim labelama gdje je definirano.
- Interni param IDs ostaju ASCII.

## Scope ownership

Frontend agent je vlasnik:

- `src/ui/app`
- `src/ui/components`
- `src/ui/layout`
- `src/ui/theme`
- `src/ui/fx`
- `src/ui/bindings` (UI strana bindinga)

## Hard rules

1. Ne implementirati DSP obradu unutar UI sloja.
2. Svaka kontrola mora koristiti parametar iz `docs/40_PARAMETER_SPEC.md`.
3. UI smoothing je dozvoljen samo za prikaz (meter/knob animation), ne za audio.
4. UI ne smije citati audio buffer direktno; koristi meter bridge client.
5. Layout mora ostati funkcionalan pri resize/HiDPI skaliranju.
6. Z-index pipeline: background -> panels -> controls -> glows -> text -> overlays.
7. Potrosnja: cilj 60 FPS, fallback 30 FPS bez vidljivog jittera.
8. `LFO` control mora biti disabled kada je `guzvanje_enabled = 0`.

## Required components

- `NeonKnob`: drag, wheel, double-click reset, ring arc, tick marks, label, optional readout.
- `BypassToggle`: jasno ON/OFF stanje + active glow.
- `VibeModeSegmentedControl`: 3-state enum, jasno aktivna sekcija.
- `GainReductionMeter`: clamp + smoothing + scale labels.
- `ThemeManager`: runtime update varijabli bez restarta UI-a.
- `Gužvanje` toggle (ON/OFF).
- `LfoRateControl`:
  - rate knob
  - Hz/sync prikaz
  - sync division UI (kad je sync ukljucen)

## Binding rules

- Param write ide kroz binding adapter ili APVTS attachments.
- Theme parametri ne idu u audio path.
- Meter komponente citaju samo snapshot (`MeterBridgeClient`).
- LFO prikaz mod (`Hz`/`sync`) mora pratiti `lfo_sync_enabled`.

## Coding style

- Male, ponovljive komponente.
- Bez magic numbers: koristiti konstante u `src/shared/constants`.
- UI-only parametre (theme) jasno odvojiti od audio parametara.

## PR checklist

- [ ] Vizualno prati referencu
- [ ] Nema direct DSP/audio thread callova iz UI
- [ ] Svi control IDs odgovaraju param specu
- [ ] `Gužvanje` i `LFO` disable/enable logika ispravna
- [ ] Resize i HiDPI smoke test prosao
