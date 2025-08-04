# Ivan Hoyt Plane Avionics & Integration

This repository manages the avionics, wiring, and software for a custom experimental aircraft. It is organized for modularity, reproducibility, and clarity for both hardware and software integration.

## Project Overview

- **Purpose:** Centralize all documentation, configuration, and code for the aircraft's electronic systems.
- **Scope:** Covers flight controller setup, CAN bus bridging, EFIS (Electronic Flight Instrument System), and custom gateway/configuration for sensors and actuators.

## Repository Structure

- `devices/` — Main folder for all hardware and software components:
	- `speedybee-f405-wing/` — Flight controller (INAV/BetaFlight/ArduPilot). See its `README.md` for firmware and configuration steps.
	- `feather-m4-can/` — Custom Arduino/CircuitPython code for CAN bus bridging. See its `README.md` for build and integration details.
	- `raspberry-pi/` — Main compute node for EFIS and gateway services. See its `README.md` for setup and workflows.
- `backups/` — SD card or firmware image backups.

## Getting Started

1. **Read this overview.**
2. **See the `README.md` in each device subfolder** (under `devices/`) for hardware-specific setup, build, and configuration instructions.
    **For custom configuration** (e.g., EFIS screens, gateway YAML), see the relevant subfolders under `devices/raspberry-pi/config/`.
3. **For project conventions, integration patterns, and best practices,** see [`.github/copilot-instructions.md`](.github/copilot-instructions.md).
