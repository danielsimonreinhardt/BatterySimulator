# Hardware

Hier sollen die KiCad-Quellen des Batterie-Simulators liegen. **Noch nicht übernommen:** die Quellen liegen außerhalb dieses
Repositorys (`C:\DATA\Hobby-Projekte\Longboard\03_TOOLS\Batterie-Simulator\HARDWARE`) und werden dort vom Nutzer gepflegt.

Vorgesehene Unterordner (je ein KiCad-Projekt):

| Ordner | Inhalt |
|---|---|
| `BattSimMaster/` | Master mit S32K146 (Blätter `BattSimMaster` und `MCU`) |
| `BattSim_V1/` | Zellkanal |
| `BattSim_BusPcb/` | Rückwand |
| `BattSim_Supply/` | Versorgung (230 V → 48 V → 5 V) |

Beschreibung der Hardware und Sicherheitsbefunde: [`../Doku/hardware-system.html`](../Doku/hardware-system.html) und
[`../Doku/hardware-zellkanal.html`](../Doku/hardware-zellkanal.html).
