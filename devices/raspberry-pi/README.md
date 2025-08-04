# Raspberry Pi

This directory contains the setup and configuration files for the Raspberry Pi used in the avionics system. The Raspberry Pi serves as the central processing unit for the EFIS (Electronic Flight Instrument System) and other avionics components.

His Raspberry Pi is connected to a 10.1" HDMI touchscreen, which displays the EFIS interface and other relevant flight data. The Pi runs a custom image that includes all necessary software and configurations to operate the avionics system effectively.

## How to Use

You can switch between GUI and TUI by pressing `Ctrl+Alt+F1` for TUI or `Ctrl+Alt+F7` for GUI.

1. **Follow [these instructions](https://github.com/makerplane/pyEfis/blob/master/INSTALLING.md) to install PyEFIS using the `snap` package manager**. Those instructions also cover a number of other scenarios.
2. Copy the files in the makerplane folder here to the same locations the files starting at `~/makerplane` on the Raspberry Pi. For example copy `devices/raspberry-pi/makerplane/fixgw/config/canfix/map.yaml` from this repository to `~/makerplane/fixgw/config/canfix/map.yaml` on your Raspberry Pi.

## Creating and restoring from backups

Create:

```
sudo dd if=/dev/disk4 bs=64K status=progress | gzip -c > /path/to/ivanhoyt-plane/backups/2025-08-02-1427.img.gz
```

Restore:

```
gunzip -c /path/to/ivanhoyt-plane/backups/2025-08-02-1427.img.gz | sudo dd of=/dev/disk4 bs=64K status=progress
```
