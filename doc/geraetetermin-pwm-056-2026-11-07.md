# Gerätetermin 07.11.2026: Firmware 0.5.6 (Sollwert-PWM aus bei nicht freigegebener Zelle)

Board-Punkt bs-test-056, Antwort des Nutzers 07.11. 08:19: „Jetzt flashen und Schritte 1–6 testen“. Jeder Schritt mit eigenem Ja
des Nutzers im Chat. Abbild: Zweig `duty-null-055` 15902af, Hex-SHA-256 `955fee69c06ca2d2c1baf877d93aa359dca40506e6b11665124858003ec46926`,
ROM-CRC 0x688738F3. Beschreibung der Änderung: `pwm-bei-abgeschalteter-zelle-0-5-6.md`.

## Ergebnis in Kürze

| Schritt | Inhalt | Ergebnis |
|---|---|---|
| 1 | Flashen, Version über 0x121 | bestanden: canflash Check OK, Bootloader-CRC-32 0xC397B02A, Firmware 0.5.6, Standby, FAIL „keiner“ |
| 2 | ohne Last ein/aus | bestanden: 14 × 1,00 V (0,997–1,004), Stapel 14,00 V, 0,273 A; Alle aus < 0,3 V; zweites Ein sauber; Ende Standby |
| 3 | 100 Ω, Karte einzeln aus/ein, Versorgungsstrom, Einschaltverlauf | Strom bestanden (−31…−33 mA je Karte, wie 0.5.5 mit 28–31 mA); Einschaltverlauf aufgenommen, Überschwinger +0,09 V |
| 4 | Handbetrieb, Kartentaste | bestanden (Mitschnitt unten) |
| 5 | Aus/Ein (Power-Cycle) | bestanden: Standby, 0.5.6, FAIL „keiner“, 0x0201 unverändert, Anzeige leuchtet (Nutzer) |
| 6 | Ende Standby | erreicht (Standby nach Schritt 5, Ausgang aus, Zellen aus) |

Offener Punkt: der Überschwinger beim Freigeben einer Zelle (Abschnitt 3.2), Vorschlag Reglerpause nach der Freigabe.

## Schritt 1: Flashen

Vorher: Firmware 0.5.5, Zustand FAIL „Timeout 0x130“ (Rest des vorigen Termins), Ausgang aus. BS_REMOTE_OFF, `canflash info` (Anwendung aktiv,
Bootloader 0.3.14), `canflash flash` über Vector-Kanal 1: Löschen 0x8000 bis 0x3FE78, 228984 Byte, 19,0 KB/s, Check OK (CRC-32 0xC397B02A),
ECU-Reset. Danach Firmware 0.5.6, Standby, FAIL-Grund „keiner“, alle 14 Zellen aus (0,01–0,06 V).

## Schritt 2: ohne Last

Fernsteuerung ein, alle Zellen 1,0 V, alle ein, Ausgang ein: 14 Zellen 0,997–1,004 V, Stapel 14,00 V, Versorgungsstrom 0,273 A, Status „ein“.
Alle Zellen aus: alle 14 unter 0,3 V, Status „disabled“. Alle wieder ein: alle 14 bei 1,00 V. Danach Alle aus, Ausgang aus, Fernsteuerung aus (Standby).

## Schritt 3: 100 Ω

### 3.1 Versorgungsstrom (100 Ω ohmsch am Stapel, alle Zellen 1,0 V)

Karte k = 1…14 einzeln mit `BS_CELL_OFF` aus (etwa 6 s), wieder ein (etwa 6 s), passiver CAN-Mitschnitt (`scratchpad/test056_karten.txt`),
Stufenauswertung des 48-V-Stroms (0x121): alle Karten an 0,720–0,721 A; Karte aus 0,687–0,689 A, Sprung **−31…−33 mA** bei jeder der 14 Karten, beim
Einschalten +31…+33 mA. Die ausgeschaltete Karte steht auf 0,01–0,04 V, die anderen bei 1,00 V. 0.5.5 (06.11.): 0,721 A und −28…−31 mA. Die Kartenheizung
ist nach dem Modell unverändert (Q6-Brücke schwach, der Strom läuft durch den LT3080 weiter, siehe `geraetetermin-zelle-4-13-2026-11-06.md`, Abschnitt 7);
die 2 mA Mehrsprung können Streuung zwischen Terminen sein oder die kleine Entlastung des Sollwertpfads bei Duty 0 (gerechnet etwa 1,5 mA je Karte bei 1 V an der
Karte, Größenordnung 0,4 mA am 48-V-Eingang); nicht getrennt belegt.

### 3.2 Einschaltverlauf Karte 1 (PicoScope 2204A, Kanal A, DC, 100 Ω am Stapel, Sollwert 1,0 V)

Messaufbau: Tastkopf **1:1** (Schalter am Tastkopf; mit der Einstellung 10:1 las das PicoScope genau das Zehnfache). Erste Versuche mit lockerem
Adapterkabel (Brummen 50 Hz, später 0 V) sind verworfen; danach Karte 1 mit `BS_CELL_OFF`, Aufnahme scharf (Einzeltrigger 0,5 V steigend, aus einer
Hintergrundaufgabe, weil ein Messaufruf das Werkzeug blockiert), dann `BS_CELL_ON`.

Aufnahme `c-20261107-104424-10` (0,2 s, 41 µs): Start etwa 13 ms vor dem Trigger, 0,9 V nach etwa 22 ms, dann Überschwinger, Mittel 1,085 V (50–100 ms),
1,092 V (100–200 ms), Spitze (2 ms geglättet) 1,110 V. Aufnahme `c-20261107-104540-11` (3 s): 0,882 V (0–50 ms), 1,085 V (50–100 ms), 1,092 V (100–200 ms), 1,087 V
(200–300 ms), 1,065 V (300–500 ms), 1,048 V (0,5–0,75 s), 1,038 V (0,75–1 s), 1,019 V (1–1,5 s), 1,009 V (1,5–2 s), 0,998 V (2–3 s); innerhalb 10 mV ab etwa 1,4 s.
CSV in `C:\TOOLS\LAB_CONTROL\captures\`.

Einordnung: der Regler setzt beim Freigeben mit seiner vollen Befugnis (+0,1 V) ein, weil der gefilterte Istwert (95 ms) dem schnellen Anstieg
hinterherläuft und das Integral danach langsam abbaut. Unter der Überspannungsschwelle (0,2 V), kein FAIL, andere Zellen unberührt. Der SIL (vereinfachte Strecke, 0,25 s)
sagte 0,03 V Spitze voraus. Ob 0.5.5 dasselbe zeigte, ist **nicht gemessen** (Vermutung: ja, die Reglerlogik beim Freigeben ist unverändert; belegbar nur mit
zurückgeflashter 0.5.5). Vorschlag für einen eigenen Board-Punkt: Reglerpause nach der Freigabe (wie nach einem Sollwertsprung, `StepHold_cycles`).

## Schritt 4: Handbetrieb mit Kartentaste

Fernsteuerung aus (Standby), 100 Ω am Stapel, passiver Mitschnitt (`scratchpad/hand056.txt`, jede Änderung von Status und auf 0,05 V gerundeten Zellspannungen). Der Nutzer bedient am Gerät.

| Zeit | Ereignis | Beobachtung |
|---|---|---|
| 176,2 s | ALL ON/OFF | Modus 2 (STANDALONE), alle 14 Kanäle freigegeben (Status 2), alle 0 V |
| 184,5–187,4 s | Kanal 1 hochdrehen | 0,35 → 0,60 → 0,70 (Sprung auf den Mindestwert), dann Rasten bis 1,00 V |
| 197,5 s | Kartentaste Kanal 1 | Status „aus“, Spannung fällt über 0,50 auf unter 0,2 V |
| 204,6–206,3 s | Kartentaste Kanal 1 wieder | Status „ein“, 0,60 → 0,90 → 1,05 → 1,00 V nach 1,7 s |
| 212,8–218,7 s | Kanal 2 hochdrehen | wie Kanal 1, Kanal 1 bleibt 1,00 V |
| 222,1 s | Kartentaste Kanal 2 | Status „aus“, unter 0,2 V, Kanal 1 unverändert bei 1,00 V |
| 226,8–228,5 s | Kanal 2 wieder frei | 0,70 → 0,95 → 1,05 → 1,00 V |
| 230,9 s | ALL ON/OFF | Modus 1 (Standby), alle 14 Status „aus“, Spannungen in 0,2 s auf 0 |

Der Tastgrad selbst ist über CAN nicht sichtbar; dass er bei gesperrtem Kanal 0 ist, belegt der SIL-Test, am Gerät passt die Spannung auf unter 0,2 V.

## Schritt 5 und 6: Aus/Ein und Ende

Power-Cycle durch den Nutzer. Danach über CAN: Standby, Ausgang aus, Fernsteuerung aus, FAIL „keiner“, Firmware 0.5.6, alle Zellen aus (0,00–0,03 V), Stapel 0,17 V,
Versorgungsstrom 0,183 A. 0x0201 (112 Byte) schreibgeschützt per UDS gelesen (22 0201): gleich dem Satz vom 31.10. Anzeige leuchtet (Nutzer).

## Stand nach dem Termin

Gerät in Standby, Ausgang aus, alle Zellen aus, Firmware 0.5.6 geflasht (nicht in `main`; Zweig `duty-null-055`). Merge nach `main` und Nachziehen im Repo BatterySimulator:
jeweils eigener Board-Punkt mit Ja des Nutzers.
