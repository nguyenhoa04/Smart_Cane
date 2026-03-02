# SM CANE

ESP32 + Arduino firmware project using PlatformIO.

## Current status

This repository is currently in scaffold stage:

- Platform configured for `esp32dev` in `platformio.ini`
- Entry point is `src/main.cpp`
- One header declaration in `include/MyFunction.h`
- Private library module folders exist under `lib/`, but no source files are committed there yet

## Hardware/Framework

- Board: `esp32dev`
- Platform: `espressif32`
- Framework: `arduino`

## Project structure

```text
.
|-- include/
|   `-- MyFunction.h
|-- lib/
|   |-- app_loop/
|   |-- audio_dfplayer/
|   |-- config/
|   |-- detection_handler/
|   |-- gps/
|   |-- http_uplink/
|   |-- labels/
|   |-- mqtt_link/
|   |-- pins/
|   |-- sim_a7680c/
|   |-- sos_button/
|   |-- topics/
|   |-- ultrasonic/
|   `-- utils/
|-- src/
|   `-- main.cpp
|-- test/
|-- platformio.ini
`-- .gitignore
```

## Build and upload

Install PlatformIO first, then run from project root:

```bash
pio run
```

Upload firmware:

```bash
pio run -t upload
```

Open serial monitor:

```bash
pio device monitor
```

## Development notes

- Add module implementation files in each `lib/<module>/src/`
- Keep shared declarations in `include/`
- Add unit tests under `test/`
- If you change board/build flags, update `platformio.ini`

## Next recommended steps

1. Implement `myFunction(...)` or remove placeholder code in `src/main.cpp`
2. Add at least one real module under `lib/*/src`
3. Add a basic test file in `test/` and run `pio test`
