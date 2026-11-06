# Hardware

KiCad-Quellen des Batterie-Simulators (je Ordner ein KiCad-Projekt) und die Stückliste. Lizenz: CERN-OHL-P-2.0, siehe [`LICENSE`](LICENSE).

| Ordner | Inhalt |
|---|---|
| `BattSimMaster/` | Master mit S32K146 (Blätter `BattSimMaster` und `MCU`) |
| `BattSim_V1/` | Zellkanal |
| `BattSim_BusPcb/` | Rückwand |
| `BattSim_Supply/` | Versorgung (230 V → 48 V → 5 V) |
| `BOM_gesamt.xlsx` | Stückliste aller Baugruppen |

Übernommen sind nur die Quellen (`.kicad_pro`, `.kicad_sch`, `.kicad_pcb`). Nicht dabei sind die 3D-Modelle (`.step`), die Fertigungsdaten (Gerber),
die KiCad-Sicherungen, die Simulationsdateien und Zwischenspeicher; die 3D-Modelle der Bauteile kommen aus den KiCad-Standardbibliotheken.
Die Quellen werden vom Nutzer außerhalb dieses Repositorys gepflegt und hier bei Bedarf nachgezogen.

Beschreibung der Hardware und Sicherheitsbefunde: [`../Doku/hardware-system.html`](../Doku/hardware-system.html) und
[`../Doku/hardware-zellkanal.html`](../Doku/hardware-zellkanal.html).
