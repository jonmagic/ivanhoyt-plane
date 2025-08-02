# Copilot Instructions

This project contains all of the code and instructions for my dad's plane and avionics. The repository is structured to facilitate easy navigation and understanding of the various components involved in the project.

```
├─ README.md               # Overview of all components and high‑level wiring
├─ docs/                   # Architecture diagrams, wiring diagrams, CHANGELOG
│   ├─ overview.md
│   └─ …other docs…
├─ devices/
│   ├─ f405-wing/
│   │   └─ README.md       # How to flash and configure INAV/BetaFlight/ArduPilot
│   ├─ feather-m4-can/
│   │   ├─ README.md       # Build & deployment instructions for Arduino code
│   │   └─ mav-to-can.cpp  # Custom Arduino/CircuitPython code
│   ├─ raspberry-pi/
│   │   ├─ README.md       # Step‑by‑step setup from bare SD card to running EFIS
│   │   ├─ build-image.sh  # Script to build or refresh a custom Pi image
│   │   ├─ setup.sh        # Idempotent script to install deps and configure services
│   │   └─ config/
│   │       ├─ fixgw/      # Your customised FIX‑Gateway configs (preferences.yaml.custom etc.)
│   │       └─ pyefis/     # EFIS screen and instrument definitions
│   └─ sensors/
│       ├─ README.md       # Description and wiring of each new sensor added
│       └─ <sensor>/       # Code and configs if sensors require microcontroller code
│
├─ scripts/
│   └─ backup-pi.sh        # Creates compressed image of the Pi SD card using `dd + gzip`
│
└─ .github/
    ├─ copilot‑instructions.md
    └─ workflows/…
```

When you start working on this project, ensure you:

1. Create small commits with clear semantic commmit messages.
2. Use the provided scripts and documentation to set up your development environment.
3. Follow the directory structure to maintain organization and clarity.
4. Update the documentation as you add new features or make changes to existing components.
