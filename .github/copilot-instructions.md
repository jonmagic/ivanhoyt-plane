
# Copilot Instructions for `ivanhoyt-plane`

This repository manages avionics, wiring, and software for a custom aircraft. It is organized for modularity, reproducibility, and clarity for both hardware and software integration. Follow these project-specific guidelines to maximize productivity as an AI coding agent:

## 1. Architecture & Major Components

- **Top-level overview:**
    - `README.md` provides a high-level summary of all avionics, wiring, and integration points.
    - `docs/` contains architecture diagrams, wiring diagrams, and changelogs. Use these for system context.
- **Devices:**
    - `devices/` contains subfolders for each major hardware component:
        - `speedybee-f405-wing/`: Flight controller (INAV/BetaFlight/ArduPilot). See its `README.md` for firmware/config steps.
        - `feather-m4-can/`: Custom Arduino/CircuitPython code for CAN bus bridging. `mav-to-can.cpp` is the main source file.
        - `raspberry-pi/`: Main compute node for EFIS (Electronic Flight Instrument System) and gateway services. See below for workflows.

## 2. Developer Workflows

- **Raspberry Pi setup:**
    - Use `devices/raspberry-pi/README.md` for step-by-step provisioning (from SD card to running EFIS).
- **Firmware/MCU code:**
    - Each device subfolder contains its own `README.md` with build/flash instructions. Follow these for hardware-specific workflows.
- **Configuration:**
    - `devices/raspberry-pi/config/fixgw/`: Custom FIX-Gateway configs (e.g., `preferences.yaml.custom`).
    - `devices/raspberry-pi/config/pyefis/`: EFIS screen and instrument definitions.

## 3. Project Conventions & Patterns

- **Documentation-first:**
    - All new features or hardware integrations must be documented in the relevant `README.md` or `docs/` file.
- **Small, semantic commits:**
    - Use clear, descriptive commit messages for traceability (e.g., `feat: add OAT sensor integration`).
- **Idempotent scripts:**
    - All setup scripts (`setup.sh`, etc.) are designed to be safe to re-run.
- **Directory structure:**
    - Follow the established folder layout for new devices, sensors, or configs. Place all code and docs in the appropriate subfolder.

## 4. Integration & Data Flow

- **CAN bus:**
    - `feather-m4-can` bridges MAVLink and CAN for sensor/actuator communication.
- **FIX-Gateway:**
    - Configured via YAML in `devices/raspberry-pi/config/fixgw/`.
- **EFIS:**
    - Screen layouts and instruments defined in `devices/raspberry-pi/config/pyefis/config/screens/`.

## 5. External Dependencies

- **Firmware tools:** INAV, BetaFlight, ArduPilot, Arduino IDE, CircuitPython.
- **Raspberry Pi:** Raspbian OS, custom scripts, Python for EFIS.

---

**Always update documentation and configs when making changes.**
