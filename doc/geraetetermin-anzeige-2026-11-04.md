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

## Gerätetermin 06.11.2026: Firmware 0.5.2 geflasht, Anzeige weiterhin dunkel

Board `bs-anzeige-052` a (06.11., 06:10). Aufbau vom Nutzer bestätigt (kein Prüfling, keine Last, BCU aus, Anzeige angesteckt, J-Link unbenutzt), Ja zum Flash im Chat. Vorher: SHA-256 des Hex `c9aa75014db2a3d7a3e734559a012d6d639559c489f2ce4b409c335030f77fbf` (gleich dem Build vom 04.11.), Gerät Standby, Ausgang aus, Fernsteuerung frei, Firmware 0.5.1, kein FAIL; 60-V-Netzteil und 150-W-Last offline.

| Schritt | Ergebnis |
|---|---|
| `canflash info` | Anwendung aktiv, Bootloader 0.3.14 |
| `canflash flash` (Zweig `pin-io125` 75f960d) | 228296 Byte (0x00008000..0x0003FBC8), **Check OK (CRC-32 0xDB7D6274)**, ECU Reset. Die Prüfsumme von `canflash` gilt für den Flashbereich; sie ist nicht dieselbe Rechnung wie die Build-CRC 0xB295CA01 im Board. Belegt ist der Flash durch SHA-256 des Hex, Check OK und die Versionsrückmeldung. |
| Version | Statusbotschaft 0x121: Byte 6/7 = `05 02`, also **0.5.2**; UDS 0x22 F181 antwortet (`nextbsw-app 1.0.0`) |
| ALL ON/OFF (Standby → Handbetrieb) | 0x120 Byte 1 = 2 (STANDALONE), Zellwerte auf 0x110 ändern sich; **Anzeige dunkel** (Beobachtung des Nutzers) |
| ALL ON/OFF zurück | 0x120 Byte 1 = 1 (Standby), Ausgang aus |

Abbruch nach der Vorgabe: bei dunkler Anzeige kein Abziehen/Anstecken, keine Task-Hängemessung, keine Bus-Clear-Prüfung.

Beobachtung am Rand: Die Kachel „Batterie-Simulator 1“ in LabControl zeigte nach dem Flash „offline“ (age wuchs weiter), obwohl der Bus 0x110..0x121 im 10-ms-Raster lieferte. Ursache nicht untersucht (LabControl war erst kurz vorher gestartet, Uptime etwa 3 min beim Termin).

### Bewertung

Die Rate (9600 Baud gegen 31,25 kHz) war der einzige belegte Unterschied zur Werksfirmware (Tabelle oben). Mit 9541 Hz, SCL-Pull-up und eigenem Task bleibt die Anzeige dunkel; **die Rate allein ist damit nicht die Ursache.** Offen und ohne Messung nicht zu trennen:

1. Pegel/Flanken an PTE15/PTE16 (SDA hat keinen Pull-up, das Modul vielleicht auch nicht; SCL nur den internen, etwa 20..50 kΩ).
2. Ob das Modul den Adressbyte-ACK gibt (`i2c.DISPLAY.write.error` wird bei NACK gesetzt; der Zähler ist ohne J-Link/XCP nicht lesbar, und der J-Link läuft nur mit Ansage der Lizenzabfrage).
3. Ob der FlexIO-Treiber das Byte-Ende (STOP) und die Adressphase wie der SDK-Treiber der Werksfirmware erzeugt.
4. Pinzuordnung SDA/SCL an J1 Pin 2/3 und Verdrahtung zum Modul (nie gemessen).

Der nächste Schritt, der eine Antwort bringt, ist eine Messung am Bus (PicoScope 2204A oder UNI-T über LabControl an PTE15/PTE16 bzw. J1 Pin 2/3; Tastkopf und Pegel vorher mit dem Nutzer klären) oder das Lesen des Fehlerflags per J-Link.

Endzustand: Firmware 0.5.2, Standby, Ausgang aus, Fernsteuerung frei, Kalibrierung nicht angefasst (0x0201 nach dem Flash nicht gelesen).
