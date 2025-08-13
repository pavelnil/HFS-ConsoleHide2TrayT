# [HFS-ConsoleHide2TrayT](../../releases/)

---

## Project Description

**[HFS-ConsoleHide2TrayT](../../releases/)** is a tool for managing the console of **HTTP File Server version 3 (HFS 3)**, enabling its **minimization to the system tray**. It consists of two components:
  - **HFS Plugin** (an agent that modifies the console title).
  - **Native Application** (C++ tray manager).

**No installation required** — operates in portable mode.

![screenshot](../../blob/main/screenshots/screenshot1.jpg)

---

## Key Features

### HFS Console Management
* **Automatically hides the console** when the app starts.
* **Quick visibility toggle** via the tray icon:
  * Double left-click.
  * Context menu: "**Hide/Show HFS Console**".
* **Automatic console detection** via a unique window title (format: `HFS_CONSOLE_<PID>`).
* **Windows 11 ConPTY support (CASCADIA_HOSTING_WINDOW_CLASS)**. Handles HWND changes when dragging console tabs.

---

## Technical Specifications

* **Architecture**:
  - **Agent**: HFS plugin in **JavaScript** (Node.js).
  - **Tray Manager**: Native **C++** application.
* **System Integration**:
  - **WinAPI** for window management (`FindWindowEx`, `ShowWindow`).
  - **Shell API** for tray operations (`Shell_NotifyIcon`).
  - Windows message processing (`WndProc`).
* **Security**:
  - Works exclusively with **local HFS processes**.
  - No network calls.

---

## Plugin Installation
1. Copy the plugin folder to `\plugins` in your **HFS** directory.
2. **Activate** the plugin via the HFS web interface.

<details>
  <summary><i>An alternative method for a plugin to change the title:</i></summary>

```
    const uniqueTitle = `HFS_CONSOLE_${process.pid}`;
    log(`Setting console title: ${uniqueTitle}`);
    
    try {
        if (process.stdout.isTTY) {
            process.stdout.write(`\x1b]0;${uniqueTitle}\x07`);
            log("Used escape sequence to set title");
        } 
        else {
            child_process.execSync(`title ${uniqueTitle}`, {stdio: 'ignore'});
            log("Used title command to set title");
        }
    } catch (e) {
        log(`Error setting console title: ${e.message}`);
    }
```

</details>

---

## Requirements

* **Server**:
  - **HTTP File Server v3**
  - Installed **[ConsoleHide2TrayT-agent](../../releases/)** plugin.
* **Client**:
  - **Windows 7** or newer.

---
Support:
* BTC: `bc1qeuq7s8w0x7ma59mwd4gtj7e9rjl2g9xqvxdsl6`
* TON: `UQAOQXGtTi_aM1u54aQjb8QiXZkQdaL9MDSky5LHN0F5-yF2`
