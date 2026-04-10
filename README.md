---

          AI generated code (including ths readme.md)
            [ if any error ask any AI to fix it ]

---
## tpSwipe 🖱️


#### Mnimalist 3/4 Finger Swipe program for Thinkpad T14 & DWM

**tpSwipe** is a lightweight gesture program written in C. dwm-style solution: simple, fast, and free of heavy dependencies.
It is a single binary designed to do one thing perfectly.

---

#### 🚀 Why tpSwipe?
*   **Faster than Touchegg:** No XML DOM reading (too slow for a Java dev's taste).
*   **Lighter than libinput-gestures:** No dependency on GTK4 or Python.
*   **Simple:** Just as minimalist as `dwmblocks`.
*   **Native Performance:** Written in C for instant response times.

---

### 🛠️ Prerequisites
Ensure you have the following installed:
*   **Libraries:** `libinput`, `libudev`, and either `elogind` (Artix) or `systemd`.
*   **Tools:** `xdotool` (to simulate keypresses).
*   **Build Tools:** `gcc`, `make`, `pkg-config`.

---

### ⚙️ Installation

#### 1. Permission Setup
To read touchpad events without root, you must be in the **input** group.

```
sudo usermod -aG input $USER

```

Note: You must log out and log back in for this to take effect. Verify by running the groups command.

#### 2. Configuration

Edit config.h to customize your experience:
Map 3 or 4-finger swipes to specific commands (e.g., xdotool key Alt_L+Right).
Fine-tune maxThreshold and minThreshold for swipe sensitivity.
Adjust cooldownMs to prevent accidental double-triggers.

#### 3. Build and Install

```
make clean && make
sudo make install
```

### Usage
Add the following line to your .xinitrc or dwm autostart.sh:

```
/usr/local/bin/tpSwipe &
```

### Restart dwm and you are ready.

The default configuration is optimized for moving between dwm tags without using the keyboard:
Swipe Right: Switches to the Next Tag.
Swipe Left: Switches to the Previous Tag.
Uses Alt_L+Right and Alt_L+Left mappings.

#### 🛠️ Troubleshooting
If the program fails due to permission issues (e.g., not being in the input group),
it will send a single critical desktop notification via notify-send and then exit.
This prevents log flooding while ensuring you know why it isn't working.

Code and this file Generated with the help of AI but refined for minimalist Linux setups.
It starts in milliseconds and uses virtually zero RAM.
