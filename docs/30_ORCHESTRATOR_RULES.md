# Orchestrator Rules

## Mission

Voditi isporuku kroz jasne interface ugovore, redoslijed integracije i quality gateove.

## Primary responsibilities

1. Definirati i cuvati source-of-truth dokumente (`docs/*`).
2. Razdvojiti ownership da FE i BE mogu raditi paralelno.
3. Zakljucati contracts prije vecih implementacija (param IDs, meter bridge API).
4. Upravljati milestone redoslijedom i acceptance kriterijima.
5. Provoditi integration review prije merge-a.

## Non-negotiable policy

- Nema promjene parameter ID-eva nakon Gate A bez migration plana.
- Nema spajanja UI i DSP worka koji krsi thread granice.
- Nema "temporary" workarounda bez ticketa i roka uklanjanja.
- Stack lock: JUCE + C++20 + CMake ostaje baseline dok ADR ne odobri promjenu.
- Naming lock: user-facing naziv mora ostati `Gužvarizer` i `Gužvanje` (sa `ž`).
- Release lock: format ostaje `VST3 (.vst3)` za `macOS` i `Windows`.

## Coordination protocol

- Svaki milestone ima ownera, dependency listu i exit kriterije.
- FE/BE promjene koje diraju shared contract idu u isti integration checkpoint.
- Konflikti oko mappinga ili performansi se rjesavaju prema audio safety prioritetu.
- `Gužvanje` i LFO contract promjene zahtijevaju zajednicki FE+BE review.
- Release promjene koje diraju platform support zahtijevaju BE+Orchestrator sign-off.

## Review checklist

- Contract integrity (`docs/40_PARAMETER_SPEC.md`)
- Thread safety i realtime ogranicenja
- Visual parity prema referenci
- Host automation/preset lifecycle
- Test coverage za kriticne putanje
- `Gužvanje` OFF/ON behavior i LFO sync validacija
- VST3 artefakt dostupan i validiran za `macOS` i `Windows`
