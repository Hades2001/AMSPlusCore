## Version History


### B2.1.3 - 2025-04-27
**Description:**
  1. Updated the Label Generator website, with the current label version V2.0 (compatible with version 1.1 on the device side). Added more filament types and included a "code" key in the JSON generation section.
  2. Changed the MQTT long message reception mechanism to use packetized reception and merging, which may help resolve the reboot issue on the X1.
  3. Fixed the issue where the Calibration Index was not applied for non-GFL99 filaments.
  4. Fixed the problem where the NTAG tag was read during printing due to the filament spool rotation, causing repeated submission of filament change instructions and errors in Bambu Studio.
  5. Fixed the issue with long filament types on scrolling displays.

---

### 2025-03-25
**Description:**
  1. Fixed an issue where the device would reboot when multiple AMS+ units were running and calibration data was being fetched

---

### 2025-03-24
**Description:**
  1. Major changes and integration of functional pages into a single page including device settings, OTA updates, and traffic calibration file viewing

---

### 2025-03-23
**Description:**
  1. Fixed some bugs

---

### 2025-03-19
**Description:**
  1. Updated the version number and fixed a few minor issues

  1. Added the flow calibration setup and the built-in webpage.

---

### 2025-03-15
**Description:**
  1. Updated the invitation link for Discord

---

### 2025-03-10
**Description:**
  1. Added support for multiple AMS units.

---

### 2025-02-08
**Description:**
  1. Version V1.0.5 Fix: Issue with temperature and humidity not displaying on X1/X1C.
  2. New: The firmware upgrade page now filters out .bin files when selecting firmware.
  3. New: Added upgrade status to the firmware upgrade page.

---

### 2025-01-31
**Description:**
  1. Closed the mqtt topic debug logs

  1. Merged branch 'X1Testing'

  1. Fixed the MQTT buffer overflow issue on X1

---

