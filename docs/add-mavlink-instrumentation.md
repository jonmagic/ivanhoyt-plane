# Adding New MAVLink Instrumentation (for Ivan Hoyt)

This guide explains how to safely add new CAN-FIX inputs to your aircraft system by updating both `map.yaml` and `mav-to-can.cpp`. **Always follow these steps to avoid breaking the EFIS or data flow.**

## 1. Reference the Official Parameter List

- **Never add an input to `map.yaml` that does not exist in the official CAN-FIX parameter list.**
- The authoritative list is here: [CAN-FIX Parameter List](https://github.com/makerplane/canfix-spec/blob/master/src/parameter_list.rst)
- Adding a non-existent parameter will break pyEFIS and possibly other systems.

## 2. Updating `map.yaml`

- Open `devices/raspberry-pi/makerplane/fixgw/config/canfix/map.yaml`.
- Find the `inputs:` section.
- To add a new input, look up the parameter in the CAN-FIX parameter list and copy its `canid` and `fixid` exactly.
- Example (adding OAT):
  ```yaml
  - { canid: 0x406, index: 0, fixid: "OAT", priority: 0 }
  ```
- **Do not invent new fixids or canids!**

## 3. Updating `mav-to-can.cpp`

- Open `devices/feather-m4-can/mav-to-can.cpp`.
- Add or update the corresponding `FIX_` constant for the new parameter, matching the `canid` from the parameter list.
- Implement the logic to extract the value from the MAVLink message and send it using `sendCanMessage()`.
- Example:
  ```cpp
  const uint16_t FIX_OAT = 0x406; // Outside Air Temp
  // ...
  sendCanMessage(FIX_OAT, oat_value);
  ```
- Make sure the variable you send matches the expected units and scaling from the parameter list.

## 4. Test and Validate

- After making changes, reboot the system and check that the new value appears in pyEFIS and does not cause errors.
- If pyEFIS fails to start or shows missing data, double-check that your new input matches the official parameter list.

## 5. Document Your Change

- Update this guide or the relevant `README.md` with a note about the new parameter and why it was added.
- Use clear commit messages (e.g., `feat: add OAT sensor integration`).

---

**Summary:**
- Only use parameters from the official CAN-FIX list.
- Update both `map.yaml` and `mav-to-can.cpp`.
- Test thoroughly.
- Document your work.

If you have questions, ask Jon or check the [CAN-FIX spec](https://github.com/makerplane/canfix-spec/blob/master/src/parameter_list.rst).
