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

### Nachtrag 06.11.2026: Ursache gefunden - die I2C-Pins werden nie auf FlexIO gemuxt

Board `bs-anzeige-weiter` a. J-Link EDU 801042658, 1000 kHz, `connect; g; sleep; lesen; g` (`connect` setzt den Prozessor zurück, danach läuft die Firmware 0.5.2 im Standby neu an), Ja des Nutzers je Schritt; kein Prüfling, keine Last. Skripte: `scratchpad/rte57/70_disp_err.jlink` (nur lesen), `71_mux_test.jlink` (Laufzeit-Mux).

**Lesen (nach 3 s Laufzeit, Standby):**

| Größe | Wert | Bedeutung |
|---|---|---|
| RTE `i2c.DISPLAY.write.error` (0x20001656) | 1, dauerhaft | jeder Auftrag scheitert |
| RTE `...write.byte` (0x20001654) | immer 0x76 (Clear) | Wiederholung nach Fehler, nie ein Bild |
| Zähler Request/Done (0x20001657/0x20001655) | laufen etwa alle 0,5 s weiter | Wiederholung nach 50 Takten |
| FLEXIO CTRL / PIN / SHIFTERR | 0x40000001 / 0 / 0 | FlexIO an, Shifter und Timer im Ruhezustand, **FLEXIO_PIN = 0** |
| **PORTE_PCR15, PCR16** | **0x00000000** | **Pins nicht auf FlexIO (ALT6) gemuxt, kein Pull-up** |
| GPIOE PDIR Bit 15/16 | 0 | beide Pins als low gelesen |

`FlexioI2cMaster::transfer()` prüft vor jedem Auftrag SDA und SCL über FLEXIO_PIN und liefert bei low sofort `BUS_BUSY` (Fehler, es wird nichts gesendet). Mit PCR = 0 sieht FlexIO die Pins nie high.

**Gegenprobe zur Laufzeit** (Ja des Nutzers; nichts im Flash, mit dem nächsten Reset weg): PORTE_PCR15 = 0x0600 (ALT6), PCR16 = 0x0603 (ALT6, Pull-up).

| Danach | Wert |
|---|---|
| `write.error` | 0 |
| `write.byte` | läuft durch die Bildfolge (0x01, 0x77, 0x03 ...) |
| FLEXIO_PIN / PDIR Bit 15/16 | 0x0C / beide high (Bus frei) |
| **Anzeige** | **leuchtet, zeigt 0.023** (Standby, Messwert Kanal 1; Dezimalpunkt durch 0x77 0x01), Beobachtung des Nutzers |

**Ursache:** neXtBSW muxt die Pins PTE15/PTE16 nicht. `ioConfiguration.h` ist nur die Tabelle; gemuxt wird erst durch `Io::setDefaultConfiguration(pin)`. Das ruft für die I2C-Pins niemand auf (`FlexioI2cBuses.cpp` nimmt irrtümlich an, staticBsp mux sie). QuadratureDecoder, FlexCAN, AnalogInput, InputCapture und SPI muxen ihre Pins selbst. Dieselbe Lücke wie IO-25 (SPI) und IO-48 (InputCapture PE8). Damit war die Anzeige mit 0.5.0, 0.5.1 und 0.5.2 dunkel, unabhängig von Takt (100 kHz, 31,25 kHz, 9541 Hz), SCL-Pull-up (IO-125, steht im selben PCR-Eintrag und wirkte deshalb nie) und Task. Fix gehört nach neXtBSW, nicht nach BattSimFW (kein Commit im Untermodul).

**Nicht getestet** (Nutzer, 06.11.): Abziehen/Anstecken der Anzeige (Fehlerflag, Wiederanlauf nach 0,5 s), Task-Hängedauer bei `timeout_ms` 15 und Bus-Clear. Die Tests sind nur bei geöffnetem Gerät möglich; der Nutzer hat sie übersprungen. Sie bleiben offen, bis ein Abbild mit gemuxten Pins dauerhaft läuft. Die Oszilloskop-Messung entfiel, weil die Gegenprobe die Ursache belegt hat.

**Offen:** die Wirkung der Rate und des SCL-Pull-ups ist nach dem Fix noch zu prüfen (9541 Hz und 31,25 kHz sind beide nicht mit gemuxten Pins getestet). SDA hat im MCU keinen Pull-up und auf der Platine keinen; gemessen ist nur, dass SDA im Ruhezustand high liest (PDIR Bit 15 nach dem Mux), woher der High-Pegel kommt (Pull-up im Modul?), ist nicht untersucht.

Endzustand: Firmware 0.5.2, Standby, Ausgang aus, Fernsteuerung frei; die beiden PCR-Werte gelten bis zum nächsten Reset oder Spannungsausfall (Laufzeit-Mux), danach ist die Anzeige wieder dunkel.

### Termin 06.11.2026, Teil 2: Firmware 0.5.3 geflasht, Anzeige leuchtet, friert aber ein (Ursache: Auftragszähler)

Board `bs-anzeige-053` a. Aufbau vom Nutzer bestätigt, Ja zum Flash im Chat. SHA-256 des Hex vorher `8cd0d61b0a623b73a463c279aee2b46b01cca1ad80e91efd844a1d5d3efaea2c` (Stand b02e062). Gerät vorher Standby (0x120 Byte 1 = 1), 0.5.2.

| Schritt | Ergebnis |
|---|---|
| `canflash flash` 0.5.3 | Check OK, **CRC-32 0x4179DE27** (Flashbereich 0x8000..0x3FBC8, 228296 Byte), ECU Reset |
| Version | 0x121 Byte 6/7 = `05 03`, Standby |
| Anzeige ohne Laufzeit-Mux | **leuchtet im Standby und zeigt 0.021** (Beobachtung des Nutzers): IO-129 wirkt, PCR noch nicht gelesen |
| Verhalten | erste Sekunden korrekt (0.019..0.021), dann dauerhaft 0.021; auch im Handbetrieb (dort müssten etwa 0.69 stehen) |

**Ursache (Fehler im Modell `DisplayDriver`, seit 0.5.0):** Der Auftragszähler `req` wurde mit uint8-Sättigung erhöht und blieb bei 255 stehen. `I2cByteWriter` erkennt einen Auftrag an der Änderung von `request` (der Zähler läuft laut Treiber über). Nach 255 Bytes geht nichts mehr raus; `done == request == 255`, das Modell läuft ohne Wirkung weiter. Im Standby wechselt der Messwert, das Bild wird dort etwa alle 0,14 s neu gesendet (14 Bytes), die 255 Bytes sind nach wenigen Sekunden verbraucht. Beleg: die J-Link-Werte vom Laufzeit-Mux-Test (Request/Done = 0xFF, Byte läuft weiter, Nachtrag oben). Der SIL-Test lief nie über 255 Aufträge, deshalb wurde es bei dunkler Anzeige nicht gefunden.

**Version 0.5.4:** `DisplayDriver_create.m`: Zähler läuft bei 255 auf 0 über; `DisplayDriver.slx` und `generated/` mit MATLAB R2024a neu gebaut (nur der Zähler ändert den C-Code). Neuer SIL-Test `theOrderCounterWrapsAndThePicturesKeepComing` (25 Bilder = 350 Aufträge): mit dem alten Modell 43 Fehler, mit dem neuen grün. Workspace-Tests 9/9 grün, 1981 Testfälle. Build `-j 2` grün. Hex-Stand Commit dff313e (sauberer Baum): Abbild 228192 Byte, Build-CRC (romcheck) 0xD3825621, SHA-256 `b26b1860a905cbc2e29bbb481dd1b4cd97aab166769d2e5b28b2b3ba456eae3d`. Nichts davon geflasht; auf dem Gerät läuft 0.5.3 (Standby). Schritte 4-6 (Aus/Ein, PCR lesen, Kalibrierung 0x0201) folgen mit 0.5.4 in einem Termin (Nutzer, 06.11.).


## Termin 06.11.2026, Teil 3: Firmware 0.5.4 geflasht und am Gerät geprüft

Aufbau vom Nutzer bestätigt (kein Prüfling/keine Last an den Zellklemmen, BCU aus, Anzeige angesteckt), Standby, ausdrückliches Ja je Schritt. Hex-SHA-256 vor dem Flashen geprüft: `b26b1860a905cbc2e29bbb481dd1b4cd97aab166769d2e5b28b2b3ba456eae3d` (Commit dff313e).

| Schritt | Ergebnis |
|---|---|
| 1. Flashen (canflash, Bootloader 0.3.14) | „Check OK (CRC-32 0xDC006115)", ECU-Reset |
| 2. Version 0x121 | 0.5.4 (Bytes 6/7 = 05 04), Standby |
| 3. Anzeige Standby und Handbetrieb | Nutzer: leuchtet, kein Einfrieren (je mindestens 1 min), Handbetrieb folgt dem Kanalwert und dem Spannungsdrehgeber |
| 4. Power-Cycle (Versorgung aus/ein, ohne J-Link-Reset) | Anzeige geht von selbst an und lebt; 0x121 weiter 0.5.4. Handbetrieb danach vom Nutzer selbst eingeschaltet |
| 5. PORTE_PCR15/16 per J-Link, nur lesend, ohne Reset/Halt (device Cortex-M4) | PCR15 = 0x00000600, PCR16 = 0x00000603 (wie erwartet); GPIOE PDIR = 0x0201D238 (Momentaufnahme); Gerät lief danach weiter, kein Lizenzdialog |
| 6. Kalibrierung 0x0201 (22 0201, nur lesen) | 112 Byte, gleich dem erwarteten Satz vom 31.10. |
| 7. Endzustand | Standby, Ausgang aus |

Nicht getestet (Gerät müsste geöffnet werden): Abziehen/Anstecken der Anzeige, Bus-Clear, Task-Hängedauer. Fehler 5/6/3 im Handbetrieb nicht am Gerät geprüft.


## Termin 06.11.2026, Teil 4: Firmware 0.5.5 geflasht und am Gerät geprüft

Aufbau vom Nutzer bestätigt (kein Prüfling/keine Last an den Zellklemmen, BCU aus, Anzeige angesteckt), Standby, ausdrückliches Ja je Schritt (Board bs-test-055 a). Hex-SHA-256 vor dem Flashen geprüft: `0e3903beb39e0a90f634f02dbd767de5f8a5f231f784d9652c5973981fe33fa1` (Commit 7a5e27f, Build-CRC romcheck 0xBD97E5A7).

| Schritt | Ergebnis |
|---|---|
| 1. Flashen (canflash, Bootloader 0.3.14) | „Check OK (CRC-32 0xDD84932D)", ECU-Reset |
| 2. Version 0x121 | 0.5.5 (Bytes 6/7 = 05 05), Standby |
| 3. Drehgeber im Handbetrieb | Nutzer: „sieht erstmal alles gut aus, wie erwartet" (Sprung 0 ↔ 0,7 V). Kanalstatus bei 0,7 V (OL oder nicht) vom Nutzer nicht gemeldet, nicht geprüft |
| 4. Anzeige ruhig (Standby und Handbetrieb je mindestens 1 min) | vom Nutzer bestätigt |
| 5. Power-Cycle (Versorgung aus/ein, ohne J-Link-Reset) | 0x121 weiter 0.5.5, Standby; Anzeige geht von selbst an und ist ruhig (Nutzer bestätigt) |
| 6. Endzustand | Standby, Ausgang aus (Nutzer bestätigt) |

Der Nutzer sieht weitere Anpassungen für später. Nicht getestet: PCR-Werte (bei 0.5.4 gelesen, Code dazu unverändert), Kalibrierung 0x0201 nicht erneut gelesen, Fein-Modus-Sprung und Anzeige bei kleinen Schritten nicht einzeln gemeldet.
