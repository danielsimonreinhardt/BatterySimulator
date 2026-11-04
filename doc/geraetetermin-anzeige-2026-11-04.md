# Geraetetermin 04.11.2026 (abends): Anzeige mit SCL-Pull-up, Firmware 0.5.1

Board `bs-anzeige-geraetetest` a, Quelle: Zweig `pin-io125` f29027e (Pin neXtBSW 23642a8d, IO-125). Aufbau und Ja zum Flash vom Nutzer im Chat. Kein Prueflinge/Last an den Zellklemmen (AoU S-1), CANcaseXL Kanal 2 (`vector:59177:1`), LabControl lief und sendete nichts.
Eval Board, Motor und 60V-Netzteil nicht angefasst.

| Schritt | Ergebnis |
|---|---|
| Vorabpruefung | Standby, Ausgang aus, 0.5.0, Rechnerlast 15..45 %; Abbild SHA-256 250f62ee... und CRC 0x47B4794E gegengeprueft |
| `canflash info` | Anwendung aktiv, Bootloader 0.3.14 |
| Flash 0.5.1 per CAN | 227928 Byte, Check OK (CRC-32 0x3AF3035C), ECU Reset; 0x121 meldet **0.5.1**, Standby, Ausgang aus, kein FAIL |
| ALL ON/OFF (Handbetrieb) | STANDALONE ("Handbetrieb"), Ausgang ein, 14 Zellen bei 0,69..0,71 V (Boden der Endstufe), Sollwert 0 V, kein FAIL, Kanal-LEDs an |
| Anzeige | **dunkel** (Beobachtung des Nutzers, Handbetrieb bestaetigt) |

Abbruch nach der Vorgabe des Koordinators: bei dunkler Anzeige kein Abziehen/Anstecken, keine Bus-Clear-Pruefung.

## Befund

Die Anzeige bleibt auch mit internem SCL-Pull-up (PE16 ALT6|PULLUP, wie die Werksfirmware) bei 31,25 kHz dunkel. Moegliche Ursachen (nicht unterschieden, ohne Messung am Bus/J-Link):
1. Rate: die Werksfirmware faehrt 9600 Baud; das Modul akzeptiert 31,25 kHz eventuell nicht (Variante 2, 9600 Hz, `scratchpad/variante2-9600hz.patch`, braucht einen langsameren Task und damit `rte.yaml`).
2. Adresse/Protokoll des Moduls (0x71 aus der Analyse der Werksfirmware, `verify: true` am Geraet nie bestaetigt).
3. SDA ohne Pull-up (das Modul hat womoeglich keine) oder Pinzuordnung SDA/SCL an J1.
Der I2C-Fehlerzaehler (`i2c.DISPLAY.write.error`) ist ohne J-Link nicht lesbar und wurde nicht gelesen.

## Vergleich mit der Werksfirmware (BATTSIM_CONTROL.slx, Sicherung vom 27.10.; Auftrag des Koordinators nach Board `bs-anzeige-dunkel` c)

Quellen: Modelldump (`DISPLAY_TASK`, `FlexIO_I2C_Master_Config`, `FlexIO_I2C_Master_Transmit`) und das Werksabbild `orig.bin` (nur gelesen). **Beleg** = steht so im Modell, **Schluss** = aus Modell/Abbild abgeleitet, **offen** = nicht belegt.

| Punkt | Werksfirmware (Anzeige funktioniert) | 0.5.1 (dunkel) | Bewertung |
|---|---|---|---|
| Pins/Mux | SDA PTE15 = FXIO_D2, SCL PTE16 = FXIO_D3 (Beleg, `FlexIO_I2C_Master_Config`) | SDA PTE15 FlexIO-Pin 2, SCL PTE16 FlexIO-Pin 3, ALT6 | gleich |
| Pull-ups | SCL: interner MCU-Pull-up, SDA: keiner (Beleg aus pin_settings, Termin 04.11.) | SCL `pull: up` (PE16 ALT6\|PULLUP), SDA keiner | gleich |
| Adresse | Konstante 0x71 am Eingang 3 des Transmit-Blocks (Beleg); 7-Bit-Adresse (Schluss: SDK-Treiber nimmt 7 Bit, SparkFun S7S: 0x71) | 7 Bit 0x71 | gleich (8-Bit-Lesart 0xE2 nicht belegt, aber auch nicht noetig) |
| SCL-Rate | **9600 Baud** (Beleg `BaudRate = 9600`), Polling | 31,25 kHz | **Unterschied** (Variante 2) |
| Taktquelle FlexIO | im Modell nicht angegeben (LPTMR nutzt SIRC DIV2); aus 9600 Baud und dem 8-Bit-Teiler des SDK-Treibers folgt ein FlexIO-Takt von hoechstens 4,9 MHz (Schluss, z. B. 4 MHz SIRC/2 -> 9615 Hz) | 16 MHz | **offen**; 0.5.2 nutzt SPLL/2/64 = 2,5 MHz -> 9541 Hz (Wert wie nxconfig vorschlaegt), die Taktquelle selbst ist fuer die Anzeige nicht kritisch |
| Init-Folge | **keine**: kein Helligkeits- oder Moduswechsel; jeder Rahmen beginnt mit 0x76 (Clear) (Beleg) | Clear nach dem Start und nach Fehlern, kein Helligkeitsbefehl | kein Unterschied in der Wirkung (Modul zeigt nach dem Einschalten Standardhelligkeit) |
| Wartezeit nach Einschalten | keine (Beleg) | keine, erstes Bild beim Start | gleich |
| Bytefolge je Rahmen | 15 Schritte: 0x76, dann je Stelle 0x79, Position 0..3, Ziffer (Stelle 0 mit 0x79,0x00 bei Schritt 2/3, Ziffer Schritt 4; 5/6/7; 8/9/10; 11/12/13), 0x77, 0x01 (Beleg: Counter 1..15, Switch Case) | 0x76 nur beim Start, dann 14 Bytes ohne Clear | gleich im Inhalt, anderer Takt (Clear) |
| Rate der Bytes | **ein Byte alle 10 ms im 10-ms-Task** (`DISPLAY_TASK` Ts 0,01), Rahmen etwa 150 ms, dauerhaft (Beleg); Blockierzeit je Byte etwa 2 ms bei 9600 Baud (Schluss), `Timeout = 10` ms | ein Byte je Auftrag, neues Bild bei Aenderung oder alle 1 s | gleich je Byte; anderer Wiederholtakt |
| STOP/Repeated-Start | `Constant4 = true` am Eingang 2 des Transmit-Blocks, ein Byte je Auftrag (Schluss: STOP nach jedem Byte, kein Repeated Start) | ein Byte je Auftrag, STOP | gleich (Eingang 2 nicht in der Blockbeschreibung belegt) |
| Zustaende | `Relational Operator`: Status = 0 (Standby) -> Schalter liefert Konstante 1 -> Schritt 1 (Clear) in jedem Takt; sonst Zaehler 1..15 (Beleg; der Wert der `Constant1` fehlt im Dump = Standardwert 1, Schluss). Angezeigt wird der Messwert des gewaehlten Kanals x 1000 in mV | schreibt in **allen** Zustaenden den gemessenen Istwert des gewaehlten Kanals (auch im Standby, dort ~0,03 V) | Unterschied im Standby (alt: Anzeige leer, neu: kleine Zahl); im Handbetrieb beide aktiv |
| Task | Anzeige im 10-ms-Task der Steuerung, Blockieren bis 10 ms moeglich | ebenfalls im Steuer-Task (0.5.1) | 0.5.2: eigener Task `disp` mit niedriger Prioritaet |

Ergebnis: **Der einzige belegte Unterschied mit moeglicher Wirkung ist die Rate (9600 Baud gegen 31,25 kHz).** Pins, Pull-ups, Adresse, Bytefolge stimmen ueberein; die Werksfirmware braucht keine Init-Folge. Ob das Modul bei 31,25 kHz tatsaechlich nicht antwortet (Datenblatt des Moduls nicht geprueft) oder ein anderer Grund vorliegt (Treiber, Pegel am Bus), ist ohne Messung offen; der Test mit 9600 Hz trennt das.
Nicht pruefbar ohne Messung: der FlexIO-Takt der Werksfirmware, Flankensteilheit/Pegel an SCL und SDA, ob der Treiber das Byte-Ende (STOP) wie die Werksfirmware setzt.

## Version 0.5.2 (Variante 2, Zweig `pin-io125`)

* `project.yaml`: `clock.output_dividers: {SPLLDIV2: 64}`, `peripheral_clocks.FlexIO: {select: SPLL, divider: 2}`, `i2c.DISP_I2C: {baudrate_hz: 9600, timeout_ms: 15}`; nxconfig: SCL 9541 Hz (2,5 MHz / ((130+1)*2)), Zeitlimit 15 ms, hoechstens 13 Bytes je Uebertragung; SPLLDIV2 taktet nur FlexIO.
* `rte.yaml` (freigegeben im Board `bs-anzeige-dunkel` c): zweiter Task `disp`, Prioritaet **15** (unter bsp 20, ueber background 10; swc 30, can 40 laufen vorrangig), Periode **10 ms**, nur `DisplayDriver.DispTask10ms`; Task `swc` listet jetzt seine fuenf Runnables. **Abweichung vom Auftrag ("langsamerer Task"):** die Periode bleibt 10 ms (wie die Werksfirmware), der Schutz kommt aus der niedrigen Prioritaet: ein Byte (etwa 2 ms) oder ein haengender Bus (2 x 15 ms) verzoegert den Steuer-Task nicht. Eine Periode von 20 oder 50 ms haette die Modellzeit der DisplayDriver-Komponente geaendert (MATLAB-Neubau) und das Bild auf 0,3 bis 0,7 s verlangsamt. Der Wert ist als Entscheidung des Koordinators offen, wenn eine laengere Periode gewuenscht ist.
* `rte.layout.yaml` unberuehrt. SIL-Tests: Austausch HandControl -> DisplayDriver zwischen den Tasks im Test nachgebildet (der RTE kopiert zwischen swc und disp, die C-Tests rufen die Runnables direkt auf).
* Version 0.5.2 (`VersionPatch` 2). Build `-j 2` gruen (`app.ecu.hex` 228208 Byte, text 226392 Byte), Workspace-Tests 9/9 gruen (1565 Testfaelle). Nichts geflasht.
