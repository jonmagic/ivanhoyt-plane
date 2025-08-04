# Copilot Instructions for `ivanhoyt-plane`

This repository manages avionics, wiring, and software for a custom experimental aircraft. It is organized for modularity, reproducibility, and clarity for both hardware and software integration. Use these project-specific guidelines to maximize AI agent productivity:

## 1. Architecture & Major Components

- **Top-level:**
    - `README.md` gives a high-level summary and links to all device- and config-specific docs.
    - `docs/` contains guides (e.g., `add-mavlink-instrumentation.md`), diagrams, and changelogs.
- **Devices:**
    - `devices/` contains subfolders for each hardware/software node:
        - `speedybee-f405-wing/`: Flight controller (INAV/BetaFlight/ArduPilot). See its `README.md` for firmware/config steps.
        - `feather-m4-can/`: Arduino/CircuitPython code for CAN bus bridging. `mav-to-can.cpp` is the main source file.
        - `raspberry-pi/`: Main compute node for EFIS (Electronic Flight Instrument System) and gateway services. See its `README.md` for setup and workflows.

## 2. Developer Workflows

- **Raspberry Pi provisioning:**
    - See `devices/raspberry-pi/README.md` for SD card imaging, file copying, and pyEFIS install steps.
    - Backups are managed in `backups/` as compressed images.
- **Firmware/MCU code:**
    - Each device subfolder has a `README.md` with build/flash instructions. E.g., use Arduino IDE for Feather M4 CAN, follow INAV/BetaFlight/ArduPilot docs for F405.
- **Configuration:**
    - `devices/raspberry-pi/makerplane/fixgw/config/canfix/map.yaml`: CAN-FIX to FIX-Gateway input/output mapping.
    - `devices/raspberry-pi/makerplane/pyefis/config/screens/`: EFIS screen and instrument definitions.

## 3. Project Conventions & Patterns

- **Documentation-first:**
    - All new features or hardware integrations must be documented in the relevant `README.md` or `docs/` file (see `docs/add-mavlink-instrumentation.md` for instrumentation changes).
- **Small, semantic commits:**
    - Use clear, descriptive commit messages (e.g., `feat: add OAT sensor integration`).
- **Idempotent scripts:**
    - All setup scripts are safe to re-run.
- **Directory structure:**
    - Follow the established folder layout for new devices, sensors, or configs. Place all code and docs in the appropriate subfolder.

## 4. Integration & Data Flow

- **CAN bus:**
    - `feather-m4-can/mav-to-can.cpp` bridges MAVLink and CAN for sensor/actuator communication. Only add CAN-FIX inputs that exist in the official parameter list (see `docs/add-mavlink-instrumentation.md`).
- **FIX-Gateway:**
    - Configured via YAML in `devices/raspberry-pi/makerplane/fixgw/config/canfix/map.yaml`.
- **EFIS:**
    - Screen layouts and instruments defined in `devices/raspberry-pi/makerplane/pyefis/config/screens/`.

## 5. External Dependencies & Integration Points

- **Firmware tools:** INAV, BetaFlight, ArduPilot, Arduino IDE, CircuitPython.
- **Raspberry Pi:** Raspbian OS, custom scripts, Python for EFIS.
- **CAN-FIX spec:** Always reference the official parameter list at https://github.com/makerplane/canfix-spec/blob/master/src/parameter_list.rst before adding new CAN-FIX inputs.

---

**Always update documentation and configs when making changes.**
