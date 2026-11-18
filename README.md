 # Illuminated LED Leyland Scroll Bus Emblem (Open-Source Heritage Project)

An open-source, bolt-on **12V/24V LED-illuminated Leyland cursive scroll emblem** (`320.0 × 68.0 × 16.0 mm`) engineered for classic bus restorations, transport museums, and heritage rallies (compatible with **Leyland Olympian, Atlantean, National, Leopard, and Tiger** chassis).

---

## Key Features
* **Zero-Protrusion Flat-Back Architecture (`16.0 mm` Thick):** All power regulation, surge protection, RF reception, and microcontroller electronics are miniaturized (`≤ 2.5 mm` SMD height) and hidden inside `2.8 mm` blind CNC pockets behind the cursive letters.
* **Handwriting Startup Sweep:** Powered by **84× individually addressable `SK6812-2020-WWA` micro-LEDs** (Warm White / Cool White / Amber) routed along the natural cursive stroke path. When side/parking lights switch on, the badge executes a one-time handwriting animation and locks into steady, road-legal illumination.
* **433 MHz Wireless Keyfob + NVS Memory:** Adjust color temperature (`3000K / 4500K / 6000K`), brightness (`25%–100%`), sweep speed (`0.8s / 1.4s / 2.2s`), or replay the startup sweep at rallies. Settings save automatically to non-volatile flash memory.
* **True 12V / 24V Automotive Protection:** Built-in `9V–36V` synchronous buck converter (`MP2451`), `33V` alternator load-dump TVS protection (`SMBJ33CA`), reverse-polarity diode (`SS34`), and opto-isolated lighting sense (`LTV-817S`).
* **IP67 Weatherproof & Dual Mounting:** Designed for clamp-on **front radiator grille** mounting or through-panel **rear engine access door** mounting (`220.0 mm` M6 stud spacing).

---

## Repository Structure

```text
leyland-led-scroll-emblem/
├── README.md                                          <-- Project overview & step-by-step build guide
├── LICENSE.txt                                        <-- MIT Software + CERN-OHL-P v2 Hardware + IP Notice
├── docs/
│   ├── Leyland_LED_Scroll_Emblem_FlatBack_Spec_Rev2.pdf
│   ├── Leyland_LED_Emblem_PCB_Placement_Guide.pdf
│   ├── Leyland_LED_Emblem_JLCPCB_Checklist.pdf
│   └── Leyland_LED_Emblem_Wiring_Commissioning_Guide.pdf
├── cad/
│   ├── PART-01_Aluminum_Rear_Housing.step
│   ├── PART-02_PMMA_Optical_Diffuser.step
│   ├── PART-03_Chrome_Script_Faceplate.step
│   ├── PART-04_Light_Leak_Baffles_1.5mm-v2.dxf
│   └── PART-05_EPDM_Gasket_2mm-v2.dxf
├── pcb-bom/
│   └── Leyland_LED_Emblem_SMD_BOM.csv
└── firmware/
    └── Leyland_Scroll_LED.ino
```

---

## Step-by-Step Build Guide

### Step 1: Mechanical Fabrication (`/cad` & `/docs`)
Refer to **`docs/Leyland_LED_Scroll_Emblem_FlatBack_Spec_Rev2.pdf`** for full tolerances and stack-up drawings:
1. **CNC Milling (3-Axis):**
   * `PART-01_Aluminum_Rear_Housing.step` — 6061-T6 Aluminum (`320 × 68 × 6.0 mm`), Type II matte black anodized, milled on a vacuum fixture to maintain the `0.60 mm` flat outer back wall.
   * `PART-02_PMMA_Optical_Diffuser.step` — Frosted white cast PMMA acrylic (`317 × 65 × 8.0 mm`).
   * `PART-03_Chrome_Script_Faceplate.step` — 6061-T6 Aluminum or Zamak 3 (`314 × 62 × 3.0 mm`), polished and chrome electroplated.
2. **Laser Cutting:**
   * `PART-04_Light_Leak_Baffles_1.5mm-v2.dxf` — `1.5 mm` opaque black cast acrylic inserts (prevents light bleed across overlapping cursive loops during the sweep).
   * `PART-05_EPDM_Gasket_2mm-v2.dxf` — `2.0 mm` closed-cell EPDM foam backing gasket.

### Step 2: PCB Layout & SMT Assembly (`/pcb-bom` & `/docs`)
1. Design the shaped **2-layer, 2 oz copper FR-4 board (`1.6 mm` thick, White Solder Mask, ENIG finish)** following **`docs/Leyland_LED_Emblem_PCB_Placement_Guide.pdf`**.
2. Upload **`pcb-bom/Leyland_LED_Emblem_SMD_BOM.csv`** to your PCB/PCBA vendor (e.g., JLCPCB / LCSC) using **`docs/Leyland_LED_Emblem_JLCPCB_Checklist.pdf`** to verify panelization tabs and component rotation angles.

### Step 3: Flashing the Firmware (`/firmware`)
1. Open **`firmware/Leyland_Scroll_LED.ino`** in the Arduino IDE (`ESP32-C3` board target).
2. Install **Adafruit NeoPixel** and **rc-switch** via the Library Manager.
3. Connect a 6-pin Tag-Connect (`TC2030-IDC-NL`) pogo adapter to the programming pads in `PKT-1`, upload the sketch, and pair your 433 MHz keyfob serial codes via the Serial Monitor (`115200 baud`).

### Step 4: IP67 Waterproofing & Bus Installation (`/docs`)
Follow **`docs/Leyland_LED_Emblem_Wiring_Commissioning_Guide.pdf`** to:
1. Conformal-coat the electronics pockets, pot the LED channel and cavities in UV-stable polyurethane resin (`MG Chemicals 832WC`), and seal the `RF-WIN` antenna slot flush with black epoxy.
2. Terminate the `22 AWG` flat PTFE ribbon cable into a waterproof **Deutsch DT04-3P** plug and tap into your bus's `+12V/24V` ignition/battery feed, side-light circuit, and chassis earth.

---

## Credits & Attribution
* **Project Concept, Heritage Specifications & Community Release:** Initiated for classic bus enthusiasts, restorers, and transport museums.
* **Co-Engineered With:** **Google Gemini (AI Engineering Assistant)** — mechanical stack-up design, `16.0 mm` flat-back CNC pocket architecture, miniature SMD power/RF schematic, ESP32-C3 handwriting sweep firmware, and technical documentation.

---

## Trademark & Heritage Attribution Notice
* **Historic Trademarks:** **"Leyland"**, **"Leyland Bus"**, and the classic cursive **Leyland scroll emblem** are registered trademarks and heritage intellectual property of their respective corporate successors and heritage custodians (including **Volvo Buses**, **British Motor Heritage / British Motor Industry Heritage Trust**, **PACCAR / Leyland Trucks**, and **Ashok Leyland**).
* **Non-Commercial Heritage Scope:** The open-source licenses in this repository apply strictly to the original electronic circuit design, PCB layout, 3D mechanical housing geometry, firmware, and engineering documentation. The cursive script outline is reproduced solely for **non-commercial historic bus restoration, educational display, and museum preservation purposes**. Commercial mass production or sale of trademarked badges requires appropriate licensing from the respective rights holders.

---

## Open-Source License
* **Firmware (`/firmware`):** Licensed under the **MIT License**.
* **Hardware, CAD & Documentation (`/cad`, `/pcb-bom`, `/docs`):** Licensed under the **Permissive Open Hardware License (modeled on CERN-OHL-P v2)**.

See [`LICENSE.txt`](LICENSE.txt) for full terms and automotive safety disclaimers.
