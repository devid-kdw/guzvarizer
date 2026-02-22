# Graphic Asset Specification (Gužvarizer)

## 1) Cilj

Ovaj dokument definira:

- koje UI elemente graficki dizajner mora isporuciti
- kako se elementi imenuju
- gdje se spremaju
- koja je tehnicka forma isporuke da frontend moze precizno poravnati kontrole

Ključna napomena:

- Referentna slika ima 2x `AIR` labelu.
- Finalni UI mora imati `AIR` i `COLOR` (ne 2x AIR).

## 2) Gdje upoznati projekt

Prije dizajna procitati:

1. `README.md`
2. `docs/00_PROJECT_PLAN.md`
3. `docs/10_FRONTEND_AGENT_RULES.md`
4. `docs/40_PARAMETER_SPEC.md`
5. `docs/60_UI_COMPONENT_BLUEPRINT.md`
6. `docs/70_IMPLEMENTATION_STRATEGY.md`
7. `docs/80_RELEASE_TARGETS.md`
8. `docs/101_FRONTEND_FIX_TASKS.md`
9. `assets/images/guzvarizer-ui-reference.png`

## 3) Folder struktura za grafiku

Svi asseti idu iskljucivo u:

`assets/ui`

Struktura:

- `assets/ui/source/figma`
- `assets/ui/source/psd`
- `assets/ui/source/ai`
- `assets/ui/exports/shell`
- `assets/ui/exports/knobs/main`
- `assets/ui/exports/knobs/tone`
- `assets/ui/exports/knobs/lfo`
- `assets/ui/exports/controls`
- `assets/ui/exports/meters`
- `assets/ui/exports/strips`
- `assets/ui/exports/text`
- `assets/ui/exports/overlays`
- `assets/ui/atlases`
- `assets/ui/manifests`
- `assets/ui/qa`

## 4) Pravila imenovanja

Koristi samo ASCII i `snake_case` u nazivima datoteka.

Format:

`gz_<group>_<element>_<variant>_<state>_<size>.png`

Pravila:

- `gz` je obavezan prefix
- `group`: `shell`, `knob`, `control`, `meter`, `strip`, `text`, `overlay`
- `element`: jasno ime (`threshold`, `bypass`, `vibe`, `gr`, `color_strip`...)
- `variant`: npr. `main`, `tone`, `lfo`, `frame`, `cap`, `ring`, `ticks`
- `state`: `base`, `on`, `off`, `hover`, `pressed`, `disabled`, `glow`
- `size`: npr. `128x128`, `256x256`, `1180x760`

Primjeri:

- `gz_knob_threshold_ring_base_256x256.png`
- `gz_knob_threshold_pointer_base_256x256.png`
- `gz_control_bypass_button_on_320x96.png`
- `gz_shell_main_frame_base_1180x760.png`
- `gz_meter_gr_panel_base_220x200.png`

## 5) Export pravila

- format: PNG 32-bit (alpha), sRGB
- bez kompresijskih artefakata
- svaki runtime asset mora imati 2 rezolucije:
  - `@1x` (normal)
  - `@2x` (HiDPI)
- izvorne datoteke obavezno cuvati u `assets/ui/source/*`
- ne flattenati sve u jednu sliku osim ako je eksplicitno trazeno

## 6) Obavezna lista elemenata

### 6.1 Shell i frame

- outer frame (9-slice kompatibilan):
  - 4 kuta
  - 4 brid trake
  - central fill
- header frame
- main section frame
- tone shaper section frame
- footer frame
- side neon deco elementi (L/R)
- background atmosfera (city neon / ambience)

### 6.2 Knob asset set (svaki knob zasebno)

Obavezni knob IDs:

- `threshold`
- `attack`
- `release`
- `ratio`
- `output`
- `saturation`
- `harmonics`
- `grit`
- `air`
- `color`
- `lfo_rate`

Za svaki ID isporuciti:

- `ring_base`
- `ring_active`
- `ticks`
- `cap`
- `pointer`
- `glow`
- opcionalno: `ring_shadow`

Preporucene velicine:

- main knobs: `256x256` + `512x512`
- tone knobs: `224x224` + `448x448`
- lfo knob: `192x192` + `384x384`

Tehnicka pravila za poravnanje:

- centar svih slojeva mora biti isti
- pointer pivot mora biti tocno u centru
- pointer default orijentacija: 12h
- bez bakeane rotacije po frameovima (rotaciju radi kod)

### 6.3 Kontrole i switch elementi

- `BY-PASS` button: `off/on/hover/pressed/disabled`
- `VIBE MODE` segmented shell
- vibe segment item states (`smooth/punch/dirty` on/off)
- `GUŽVANJE` toggle: `off/on/hover/pressed`
- `SYNC` checkbox: `off/on/disabled`
- status LED: `off/on`

### 6.4 Metering i strip

- top visualizer panel frame
- IN bar segment sprite
- OUT bar segment sprite
- GR meter panel frame
- GR meter segment sprite
- scale overlay texture
- color strip base
- color strip segment mask
- color strip hotspot/glow sprite

### 6.5 Tekst i dekoracije

- section badges (ako su graficki, ne font-based)
- optional logo plate `GUŽVARIZER`
- grain overlay
- scanline overlay
- vignette overlay

## 7) Layout mapa (obavezno)

Dizajner mora isporuciti mapu koordinata:

- `assets/ui/manifests/layout_map_1180x760.json`

Minimalna struktura:

```json
{
  "canvas": {"w": 1180, "h": 760},
  "knobs": {
    "threshold": {"cx": 0, "cy": 0, "diameter": 0},
    "attack": {"cx": 0, "cy": 0, "diameter": 0},
    "release": {"cx": 0, "cy": 0, "diameter": 0},
    "ratio": {"cx": 0, "cy": 0, "diameter": 0},
    "output": {"cx": 0, "cy": 0, "diameter": 0},
    "saturation": {"cx": 0, "cy": 0, "diameter": 0},
    "harmonics": {"cx": 0, "cy": 0, "diameter": 0},
    "grit": {"cx": 0, "cy": 0, "diameter": 0},
    "air": {"cx": 0, "cy": 0, "diameter": 0},
    "color": {"cx": 0, "cy": 0, "diameter": 0},
    "lfo_rate": {"cx": 0, "cy": 0, "diameter": 0}
  }
}
```

Bez ove datoteke frontend ne moze tocno poravnati UI s artworkom.

## 8) Dizajn pravila

- plugin ime je `Gužvarizer` (sa `ž`)
- izbjegavati flat/web izgled
- dizajn mora biti modularan (slojevi), ne jedna zakucana slika
- contrast/readability mora ostati dobra u DAW prozoru
- ne uvoditi 2x AIR labelu

## 9) Delivery checklist

- [ ] source fileovi predani (`fig`, `psd`, `ai`)
- [ ] export folderi popunjeni
- [ ] @1x i @2x varijante predane
- [ ] `layout_map_1180x760.json` predan
- [ ] nazivi fileova prate naming pravila
- [ ] QA preview slike stavljene u `assets/ui/qa`
