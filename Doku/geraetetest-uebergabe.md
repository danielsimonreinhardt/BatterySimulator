# Übergabe Gerätetest BS-1.8 (Stand 22.10.2026)

Für die Session, die den Gerätetest am Batterie-Simulator durchführt. Prüfumfang A–G steht im
Entscheidungsboard https://claude.ai/artifact/RNwfdpYBfNxwNVDEL7XjS9, db-Sammlung `items`, Punkt `bs-geraetetest`
(Board-Antwort „später, 28.10.“ ist überholt: Termin vorgezogen auf 22.10.).

## Stand

| Was | Wo |
|---|---|
| Treiber | LabControl v0.22.0, `device-driver/battsim/` (Commits `b958d3c`, `9448e0a`, nicht gepusht) |
| Prüfskript | `LabControl/tools/check_battsim.py` (Nachbildung: 44 Prüfungen grün) |
| Erwartetes CAN-Verhalten | `doc/can-verhalten.md` (aus dem Simulink-Modell, nicht am Gerät bestätigt) |
| Analysen | Zellkanal `doc/hardware-zellkanal.html` (Artefakt archiviert 2026-11-05), System `doc/hardware-system.html` (Artefakt archiviert 2026-11-05), Firmware `doc/firmware-analyse.html` (Artefakt archiviert 2026-11-05) |

## Vor dem Start klären

1. VN1610-Kanal (Ch1/Ch2) und die Kanalbezeichnung für `--channel` (Beispiel im README: `75816:0`).
2. Was am Zellabgriff J19 hängt: nichts oder ein Prüfling, der 0–3,7 V je Zelle verträgt. **Kein BCU-Board** (S2).
3. Laufende LabControl-App: Version prüfen (war 0.20.2). Für Teil D Neustart aus dem Quellstand nötig. Das Prüfskript
   läuft ohne App; hält die App das CAN-Interface, vorher dort trennen.
4. Oszilloskop für Teil F: Tastkopf und erwarteten Pegel erfragen (MCP-Regel).

## Sicherheit während des Tests

- **S1**: Netzspannung läuft über die Rückwand (DIN 41612, 1–2 mm Abstand). Nicht in das offene Gerät greifen,
  solange es am Netz hängt; Messungen an der Rückwand nur, wenn der Nutzer das ausdrücklich so aufbaut.
- **S2**: Bei Reset oder Ausfall des Masters gehen alle Kanäle auf ≈5,26 V und ein. Teil F (Reset-Messung) nur ohne
  Prüfling am Zellabgriff.
- **F5**: Nach einer Überlast hilft nur die Taste ALL ON/OFF am Gerät, nicht „FAIL zurücksetzen“.
- Ausgänge nur auf Ja des Nutzers einschalten (Teil B). Bei Auffälligkeiten: Ausgang aus (OUTPUT=1), Nutzer informieren.

## Ablauf in Kürze

| Teil | Inhalt | Befehl / Ort |
|---|---|---|
| A | nur lesen, es wird nichts gesendet | `python tools/check_battsim.py --hardware --interface vector --channel <ch>` |
| B | Fernsteuerung, 3,7 V, Zelle 5 aus, Multimeter an einer Zelle | zusätzlich `--remote --volts 3.7` (erst nach Ja) |
| C | Senden hart abbrechen, Zeit bis Abschalten messen (Modell 4,5–6 s), FAIL zurücksetzen | |
| D | App: Spalte „Gerät am Bus“, Kachel, Bedienung, MCP `read_device`/`control_device` | |
| E | Zellkanal: Strom abgeschalteter Kanal + Temperatur U1 (B2), TP6 bei Laststrom (B1), Soll/Ist je Kanal (B4/B5) | |
| F | ohne Prüfling: Kanalausgang bei Reset/Einschalten (S2, Oszi), +3,3 V an TP3 (S3), LIMIT an PTD10 (R1), 48V_CURRENT im Standby | |
| G | welche Taste ALL ON/OFF bzw. REMOTE (F2), LED3 leuchtet nie (F1) | |

Python: venv-Python von LabControl verwenden. `check_battsim.py` stubbt die Hardware-Suche des DeviceWorkers ab;
eigene Skripte mit `DeviceWorker.start()` würden echte COM-Geräte öffnen (Netzteil wird dabei auf 0 A gesetzt).

## Nachher

- Abweichungen vom Modell in `doc/can-verhalten.md` und in Treiber/README einarbeiten; Hardware-Befunde in die
  Analyse-Artefakte (gleiche URL neu veröffentlichen).
- LabControl: CHANGELOG-Eintrag „am Gerät geprüft“ mit Befund, Patch-Version hoch, falls der Treiber geändert wird.
- Board-Punkt `bs-geraetetest` auf erledigt mit `resolution`; Folgefragen als neue offene Punkte.
- Plan `doc/plan.html` (BS-1.8) und Memory `project_batteriesimulator.md` nachziehen.
