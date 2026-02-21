# Parameter Specification (Source of Truth)

## Audio parameters (host automation)

| ID | Label | Range | Default | Mapping | Notes |
|---|---|---|---|---|---|
| threshold_db | THRESHOLD | -60.0 .. 0.0 dB | -18.0 dB | linear | Compressor threshold |
| attack_ms | ATTACK | 0.1 .. 200.0 ms | 10.0 ms | log | Attack time |
| release_ms | RELEASE | 10.0 .. 2000.0 ms | 120.0 ms | log | Release time |
| ratio | RATIO | 1.0 .. 20.0 | 4.0 | log or stepped | Compression ratio |
| output_db | OUTPUT | -24.0 .. +24.0 dB | 0.0 dB | linear | Output makeup |
| bypass | BYPASS | 0/1 | 0 | boolean | Host-sync bypass |
| saturation_drive | SATURATION | 0 .. 100 % | 25 % | linear | Tone shaper drive |
| harmonics | HARMONICS | 0 .. 100 % | 40 % | linear | Harmonic amount/mix |
| grit | GRIT | 0 .. 100 % | 20 % | linear | Grit amount |
| air | AIR | 0 .. 100 % | 30 % | linear | Exciter/high tilt amount |
| air_freq | AIR FREQ / COLOR | 1000 .. 20000 Hz | 8000 Hz | log | Air focus frequency |
| vibe_mode | VIBE | 0..2 | 0 | enum | 0=SMOOTH,1=PUNCH,2=DIRTY |
| guzvanje_enabled | GUŽVANJE | 0/1 | 0 | boolean | OFF=normal comp, ON=jaca comp + LFO |
| lfo_rate_hz | LFO RATE | 0.05 .. 20.0 Hz | 2.0 Hz | log | koristi se kad je `lfo_sync_enabled=0` |
| lfo_sync_enabled | LFO SYNC | 0/1 | 0 | boolean | 0=Hz mode, 1=tempo sync mode |
| lfo_sync_division | LFO DIV | 0..7 | 4 | enum | 0=1/1,1=1/2,2=1/4,3=1/8,4=1/16,5=1/8T,6=1/16T,7=1/4D |

## Mode behavior contract

- `guzvanje_enabled = 0`:
  - kompresor radi standardno
  - LFO modulacija nije aktivna
  - LFO UI kontrola je disabled
- `guzvanje_enabled = 1`:
  - kompresija je jaca od standardnog moda
  - LFO modulacija je aktivna
  - LFO UI kontrola je enabled

## UI-only theme parameters (not in audio path)

| ID | Label | Range | Default | Mapping | Notes |
|---|---|---|---|---|---|
| accent_hue | Accent Hue | 0 .. 360 | 190 | linear | Global neon hue |
| glow_intensity | Glow | 0 .. 100 % | 65 % | linear | UI bloom intensity |
| panel_brightness | Panel | 0 .. 100 % | 35 % | linear | Background brightness |
| neon_saturation | Neon Sat | 0 .. 100 % | 80 % | linear | Saturation level |
| scanlines_amount | Scanlines | 0 .. 100 % | 20 % | linear | CRT style overlay |
| grain_amount | Grain | 0 .. 100 % | 15 % | linear | Film grain intensity |

## Host transport inputs (read-only backend)

| Signal | Range | Notes |
|---|---|---|
| host_bpm | 20 .. 300 | koristi se za LFO sync izracun |
| host_is_playing | 0/1 | opcionalno reset/freeze LFO phase logike |

## Meter channels (read-only from DSP to UI)

| Channel | Range | Refresh target | Notes |
|---|---|---|---|
| gain_reduction_db | 0 .. -30 dB | 30-60 FPS | clamp + smoothing in UI |
| input_level_db | -90 .. +6 dB | 30-60 FPS | for top visualizer |
| output_level_db | -90 .. +6 dB | 30-60 FPS | for bars/strip reaction |
| rms_normalized | 0.0 .. 1.0 | 30-60 FPS | optional neon strip driver |
