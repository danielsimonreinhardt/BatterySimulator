# BatterySimulator

Zellspannungs-Simulator für Tests von Batteriemanagement-Steuergeräten: 14 isolierte Zellkanäle (je 0–5 V, bis 0,5 A, zu einem
Stapel kaskadiert), ein Master mit S32K146, Bedienung über Drehgeber, Anzeige und Taster oder ferngesteuert per CAN (500 kbit/s).

| Ordner | Inhalt |
|---|---|
| [`Hardware/`](Hardware/) | Schaltpläne und Platinen (KiCad): Master, Zellkanal, Rückwand, Versorgung, dazu die Stückliste. |
| [`Firmware/`](Firmware/) | Firmware des Masters als neXtBSW-Kundenprojekt (`battsim_master`), mit Tests, Gerätetermin-Protokollen und Sicherheitshinweisen. |
| [`Doku/`](Doku/) | Analyse der alten Hard- und Firmware, Konzept, Reviews und Entscheidungen, CAN-Verhalten. Einstieg: [`Doku/README.md`](Doku/README.md). |

## Firmware klonen und bauen

neXtBSW liegt als Untermodul in `Firmware/nextbsw` (Repository `neXtBSW_development`, **privat**: ohne Zugriff schlägt das
Klonen des Untermoduls fehl). Auf Windows zählt die Pfadlänge (der längste Objektpfad des Baus misst gut 240 von 260 Zeichen),
deshalb dieses Repository möglichst weit oben im Dateisystem klonen, z. B. nach `C:\BatterySimulator`.

```sh
git clone --recurse-submodules https://github.com/danielsimonreinhardt/BatterySimulator.git C:/BatterySimulator
cd C:/BatterySimulator/Firmware
python nextbsw/tools/setup venv .
cmake --preset battsim-master
cmake --build --preset battsim-master
```

Weiter in [`Firmware/README.md`](Firmware/README.md).

## Lizenz

* **Firmware und Dokumentation** (`Firmware/`, `Doku/`): Apache License 2.0, siehe [`LICENSE`](LICENSE). Das Untermodul `Firmware/nextbsw` ist ein eigenes
  Repository und hat seine eigenen Bedingungen.
* **Hardware** (`Hardware/`): CERN Open Hardware Licence Version 2 – Permissive (CERN-OHL-P-2.0), siehe [`Hardware/LICENSE`](Hardware/LICENSE).

Schaltpläne und Platinen enthalten Symbole und Footprints aus den offiziellen KiCad-Bibliotheken (Lizenz: CC-BY-SA 4.0 mit der
KiCad-Ausnahme für Entwürfe, die diese Bibliotheken nur verwenden); sie bleiben dadurch unter deren Bedingungen, der Entwurf selbst unter CERN-OHL-P-2.0.

**Aufdruck auf den Platinen:** Auf den Platinen `BattSimMaster`, `BattSim_V1` und `BattSim_Supply` steht der Text „CREATED 11/2024 – ALL RIGHTS RESERVED“.
Er gilt nicht mehr: Für die Entwürfe in `Hardware/` gilt die CERN-OHL-P-2.0, auch wenn der Aufdruck auf bereits gefertigten Platinen etwas anderes sagt. Der Aufdruck wird mit der
nächsten Platinen-Revision angepasst; die KiCad-Quellen sind bis dahin unverändert.

## Stand

Firmware und Doku sind der Arbeitsstand der Entwicklung; ein Release gibt es nicht. Ein Gerät darf nur nach den
Sicherheitshinweisen in `Firmware/doc/` und `Doku/` angeschlossen und geflasht werden (kein Prüfling und keine Last an den
Zellklemmen beim Flashen).
