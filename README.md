# BatterySimulator

Zellspannungs-Simulator für Tests von Batteriemanagement-Steuergeräten: 14 isolierte Zellkanäle (je 0–5 V, bis 0,5 A, zu einem
Stapel kaskadiert), ein Master mit S32K146, Bedienung über Drehgeber, Anzeige und Taster oder ferngesteuert per CAN (500 kbit/s).

| Ordner | Inhalt |
|---|---|
| [`Hardware/`](Hardware/) | Schaltpläne und Platinen (KiCad): Master, Zellkanal, Rückwand, Versorgung. Noch zu füllen, siehe dortige README. |
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

## Stand

Firmware und Doku sind der Arbeitsstand der Entwicklung; ein Release gibt es nicht. Ein Gerät darf nur nach den
Sicherheitshinweisen in `Firmware/doc/` und `Doku/` angeschlossen und geflasht werden (kein Prüfling und keine Last an den
Zellklemmen beim Flashen).
