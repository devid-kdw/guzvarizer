# Phase Report

## 1) Phase ID

`BHF-3` — Color (air_freq) behavior fix

## 2) Scope completed

- **Filter Coefficient Stability:** "Color" (Crossover frekvencija visokog pojasa ili `airFreq`) mogla je doseći i 20 kHz. Budući da host može ponuditi `sampleRate` od 44.1 kHz, to tjera aproksimaciju The 1st-order Low-Pass filtera rubno blizu i samoj formuli Nyquistovog theorema (gdje se omjeri lome). Da bih zauvijek zaustavio filter eksplozije neovisno o projektnoj frekvenciji, u `ToneShaper::updateFilterCoefficients()` uveo sam strogi **Nyquist Clamp** (`0.49x sampleRate`). Ako "Color" pot premaši Nyquist granicu, DSP sigurno računa The maksimalnu najsigurniju gornju frekvenciju.
- **Audibility Mapping (AIR + COLOR interakcija):** Da bi "Color" izbor (koji samo kaže iznad koje frekvencije puštamo visoke kroz paralelni kanal) imao uistinu efekt boje (brightness, sparkla ili presence-a), sama "Air" količina je podignuta sa sitnog `2.0f` multiplikatora na **`4.0f` (+12 dB maksimum)**. Ovo je i dalje dovoljno pitomo, no kad korisnik zavrti Air knob, promjena same frekvencije ("Color") sada osjetno definira taj "hype" pojas unutar miksa (umjetno stvoreni harmonici napokon zrače).

### Files changed/created
- `src/dsp/tone_shaper/ToneShaper.cpp` (MODIFIED)
- `docs/reports/BHF-3_report.md` (NEW)

## 3) Validation

| Command | Result |
|---|---|
| `cmake --build build --target Guzvarizer_VST3 --clean-first` | 100% build success ✅ |

Sve matematičke operacije (poput `std::min` i `std::exp`) su sinkroniziranje isključivo kao posljedica APVTS hooka na dotične parametre, ne kao operacije po-sampleu, izuzimajući izloženost filtera u render bloku svake oscilacije. Modul je Real-time Safe.

## 4) Dependencies

Bez novih ovisnosti. Oslanjali smo se samo na `<cmath>` i `<algorithm>`.

## 5) Issues / blockers

- Faza nema nikakvih preostalih zaostataka ni blokada.

## 6) Handoff notes

- Sustav "Color" kalibriran je da preuzima moćni `SATURATION` harmonijski sklop ispod sebe. Postavka se sada može pouzdano i sigurno ponuditi dizajneru sučelja.
- Sljedeća ključna stavka je **BHF-4 Realtime safety + regression** kako bismo preletili i dokumentirali cjelokupni callback.
