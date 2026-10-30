# Sicherheitshinweise Stufe C (battsim_master)

Stand: 30.10.2026, Zweig `stufe-c-kalibrierung`. Gilt fuer die Firmware-Stufe C (Kalibrierung je Kanal, Regler, Diagnose,
FAIL-Gruende 5 und 6). Die Bewertung entstand im Review-Artefakt "Review BattSim Stufe C"; der Nutzer hat am 30.10.2026
(08:44 bis 08:45 UTC) **alle sieben Punkte C-1 bis C-7 mit a entschieden**. Nichts davon ist am Geraet gelaufen.

## 1. Entscheidungen im Ueberblick

| Punkt | Inhalt | Entscheidung | Stand |
|---|---|---|---|
| C-1 | Ungueltige Strommessung (aus B-2) | a: in RUN laenger als 200 ms ungueltig, FAIL Grund 5, Ausgaenge im selben Schritt aus | umgesetzt, SIL-Test gruen |
| C-2 | Ueberspannung je Kanal (FW-17) | a: Istwert laenger als 100 ms mehr als 0,2 V ueber dem Soll, alle Ausgaenge aus, FAIL Grund 6 | umgesetzt, SIL-Test gruen |
| C-3 | Grenzen der Kalibrierwerte | a: Offset +-0,2 V, Steigung 0,9000 bis 1,1000 | umgesetzt (Stufe C, Schreibtisch) |
| C-4 | Schutz beim Schreiben | a: 2E nur Extended-Session mit Security-Access, Uebernahme per Routine, nur Ausgang aus, alles oder nichts | umgesetzt |
| C-5 | Verlust der gespeicherten Kalibrierung | a: Werkswerte, Meldung in 0x0210, DTC je Block, kein FAIL | umgesetzt |
| C-6 | Regler | a: nach dem Start ein, +-0,1 V | umgesetzt |
| C-7 | Zweiter Abschaltweg ueber EN und Hardware-Abhilfe | a: Bewertung erst nach dem Einbau der Pull-ups und einer wiederholten Reset-Messung | **offen**, siehe 4 |

Der offene Punkt B-2 aus Stufe B (`sicherheitshinweise-stufe-b.md`, Abschnitt 2) ist damit durch C-1 entschieden.

## 2. C-1: ungueltige Strommessung (FAIL Grund 5)

* `SupplyMonitor` zaehlt aufeinanderfolgende Abtastwerte (10 ms) mit ungueltigem ADC-Qualifier von `I_SUPPLY` und setzt
  `CurrentInvalid`, sobald `CurrentInvalid_cycles` (20, also 200 ms) erreicht sind. Ein gueltiger Wert setzt den Zaehler zurueck.
  Der gefilterte Strom bleibt wie bisher auf dem letzten Wert.
* `BattSimMode` wertet `CurrentInvalid` in **RUN** aus: FAIL mit Grund 5 (0x121 `BATTSIM_FAIL_REASON`), `OutputEnable` im selben
  10-ms-Schritt aus (`SupplyMonitor` laeuft vor `BattSimMode`).
* In **STOP** gibt es kein FAIL (die Ausgaenge sind aus). Ist die Messung beim Wechsel STOP nach RUN noch ungueltig, geht der Schritt
  **direkt in FAIL**: die Ausgaenge waren keinen Schritt lang ein, weil die Ueberlastabschaltung sonst blind waere.
* Das Zuruecksetzen geht wie bei jedem FAIL mit REMOTE=2 und MODE=1 oder der ALL-ON/OFF-Taste; bei weiterhin ungueltiger Messung
  fuehrt der naechste Wechsel nach RUN wieder in FAIL Grund 5.
* DBC: Wertetabelle von `BATTSIM_FAIL_REASON` um 5 "Strommessung ungueltig" und 6 "Ueberspannung Zelle" ergaenzt
  (`BattSimV2.dbc`; das Feld hat 4 Bit). Die Gegenseite (LabControl-Treiber) muss die neuen Werte kennen.
* Restrisiko: Ein **gueltiger, aber falscher** Strommesswert (zum Beispiel Offset des Messverstaerkers) bleibt unentdeckt. Dagegen
  gibt es nur die Netzteil-Strombegrenzung der 48-V-Versorgung.

## 3. C-2: Ueberspannung eines Kanals (FAIL Grund 6)

* `CellChannels` vergleicht je Kanal, der freigegeben ist (Ausgang ein und `CELLn_ENABLE` = 2), den kalibrierten Istwert mit dem
  begrenzten Sollwert (ohne Reglerkorrektur). Liegt er `OverVoltTime_cycles` (10, also 100 ms) gueltige Abtastwerte in Folge mehr als
  `OverVoltThd_V` (0,2 V) darueber, ist `OverVoltage` gesetzt.
* `BattSimMode` schaltet in RUN auf FAIL mit Grund 6 und alle Ausgaenge aus. Weil `CellChannels` nach `BattSimMode` laeuft, wirkt das
  **einen Schritt (10 ms) spaeter** als bei C-1; gegenueber den 100 ms ist das unerheblich.
* Ein ungueltiger ADC-Wert des Kanals zaehlt weder hoch noch setzt er zurueck. Ein nicht freigegebener Kanal oder ein abgeschalteter
  Ausgang loescht den Zaehler.
* **Verglichen wird der Abtastwert, nicht der tiefpassgefilterte Istwert** (Zeitkonstante etwa 95 ms): Nach einem grossen Sprung des
  Sollwerts nach unten haelt der gefilterte Wert sonst mehrere hundert Millisekunden mehr als 0,2 V ueber dem neuen Soll, auch wenn der
  Ausgang laengst folgt. Dieser Unterschied zum Wortlaut des Reviews ("Istwert") ist eine Umsetzungsentscheidung; die Schwelle und
  die Zeit sind Kalibrierwerte (`OverVoltThd_V`, `OverVoltTime_cycles`).
* Grenzen: Die Erkennung nutzt dieselbe Messkette wie der Regler; ein zu niedrig messender ADC-Kanal verdeckt die Ueberspannung
  (und laesst den Regler bis zu +0,1 V anheben). Der Regler selbst kann die Schwelle nicht ausloesen (+0,1 V gegen 0,2 V).
* **Nachtrag (Geraeteteil, Abschnitt 6.4 und 7):** Die Abwaertsflanke ist schnell genug (47 ms), aber ohne Last hat die Endstufe einen Pegelboden von 0,70 V; C-2 ist
  dafuer mit C-2c angepasst (Abschnitt 7).
* **Offen war fuer den Geraeteteil:** Wie schnell der echte Ausgang nach einem Sprung des Sollwerts nach unten folgt (Entladung ohne
  Last), ist am Geraet nicht gemessen. Faellt er langsamer als 100 ms um mehr als 0,2 V ab, loest C-2 bei jedem solchen Sprung
  falsch aus. Die Messung gehoert in den Geraeteteil (Sprung 4,8 V nach 0,5 V je Kanal, Ausgang und ADC aufzeichnen); je nach
  Ergebnis wird die Zeit angepasst oder der Sprung vom Vergleich ausgenommen. Dieser Punkt ist vor dem ersten Betrieb mit Pruefling zu klaeren.

## 4. C-7: zweiter Abschaltweg und Hardware-Abhilfe (offen)

Entscheidung a: Die Bewertung des zweiten Abschaltwegs (EN_01 bis EN_14, DFA-4-Warnungen aus B-1) wird **erst nach dem Einbau der
Pull-ups und einer wiederholten Reset-Messung** abgeschlossen. Bis dahin gilt unveraendert die Annahme **AoU S-1 a**
(`sicherheitshinweise-stufe-b.md`, Abschnitt 5.1):

* Ein Pruefling (BCU-Board, MC33771C) wird erst angeschlossen, wenn der Master laeuft (0x120 meldet STANDBY).
* Mit angeschlossenem Pruefling wird nicht geflasht, und es gibt keinen Halt und keinen Reset per J-Link; ebenso keinen Watchdog-,
  Safety- oder Versorgungs-Reset.
* In jedem Reset des Masters sind die Zellausgaenge auf etwa 5,25 V (Zelle 1 gemessen, Stufe B).

Die Stufe C aendert daran nichts: Die neuen Abschaltungen (FAIL 5 und 6) wirken nur bei laufender Firmware und nicht im Reset. Die Hardware-Abhilfe (Pull-ups
an SET und EN, 28 Stueck) ist eine Aufgabe im Workspace BatterieSimulator; die wiederholte Reset-Messung folgt danach. Die Stufe C
gilt bis dahin **nicht als abgeschlossen** (Bewertung C-7 offen).

## 5. C-3 bis C-6 (wie im Review beschrieben umgesetzt)

Beschreibung und Datenfluss: `stufe-c-kalibrierung-regler.md`. Fuer die Safety-Bewertung relevant:

* Kalibrierwerte ausserhalb der Grenzen werden dreifach abgewiesen (Diagnose NRC 0x31, `CellCal`, `CellChannels` nimmt dann Werkswerte).
  Der Tastgrad bleibt am Tabellenende (5 V nominal) gedeckelt; eine Kalibrierung kann nie mehr als den Tastgrad fuer 5,0 V einstellen.
* Die Uebernahme geschieht nur bei Ausgang aus, im Betrieb aendert sich nie etwas.
* Verlust der gespeicherten Kalibrierung gibt kein FAIL: die Genauigkeit sinkt, Grenzen und Schutzfunktionen bleiben.
* Der Regler arbeitet mit +-0,1 V Eingriff und steht bei LIMIT der Versorgung.
* Die FlexNVM-Partition ist nicht ausgefuehrt, die NvM-Bloecke laufen bis dahin im RAM: die Kalibrierung geht bei jedem Reset verloren.

## 6. Geraeteteil Stufe C (30.10.2026, J-Link, CANcaseXL Ch2, PicoScope an Zelle 1, Firmware 5a23104)

Aufbau: Nutzer bestaetigte J-Link, EDU-Lizenz, CANcaseXL Ch2 und PicoScope A an Zelle 1; **kein Pruefling** (AoU S-1 a). Freigaben im
Board: `bs-stufec-flexnvm` a (Partition mit 20 Schluesselplaetzen, kein LOAD_KEY, kein Schreibschutz), `bs-stufec-geraet` a.

### 6.1 Sicherung, Partition, Flash

* Gesichert (`%LOCALAPPDATA%/NeXtBSW/battsim_backup/2026-10-30/`): P-Flash 1 MB (SHA-256 `cb1f88f3...1ab0`, gleich der Werkssicherung vom 27.10.),
  D-Flash 64 KB (komplett 0xFF). FlexRAM liess sich als RAM nicht lesen (Bus-Fehler), nicht gesichert. Der IFR ist auf dem S32K1 weder per Software noch per
  Debugger lesbar (kein Read Resource, `nextbsw/doc/hardware/s32k1xx/flexnvm-partition-state.md`); sichtbar ist nur, was der Reset daraus laedt.
  Vorher: SIM_FCFG1 0xFF32F000 (DEPART 0xF), SIM_FCFG2 0x40080000, FTFC FSTAT 0x80, FCNFG 0x02, FSEC 0xFE, FOPT 0x7F.
* Partition: der Master hat keine Konsole, deshalb per J-Link die Registerfolge von `FlexNvmPartition`/`EepromDriver` (FCCOB0..5 =
  0x80 0x03 0x00 0x00 0x02 0x0B, entspricht `PGMPART-3-0-0-2-B`), vorher ein Trockenlauf ohne Start. FSTAT danach 0x80 (kein ACCERR, FPVIOL, MGSTAT0), kein
  SETRAM. Nach dem Reset: SIM_FCFG1 0xFF32B000 (**DEPART 0xB**, EEERAMSIZE 2), SIM_FCFG2 0x40040000, FCNFG 0x01 (**EEERDY 1**), FSEC 0xFE unveraendert.
  **Nicht umkehrbar**: Mit Schluesselraum verweigert der Chip ein Mass Erase, bis LOAD_KEY und DBG_CHAL/DBG_AUTH gelaufen sind.
* Flash: `bootloader.hex` und `app.ecu.hex` + `app.ecu.record.hex` mit den generierten Skripten (kein Erase/Unlock). Rueckgelesener P-Flash: 0 Abweichungen
  gegen die drei Hex, FCF unveraendert (FSEC 0xFE), ausserhalb nur Reste der Werksfirmware in 0x3000..0x7FFF (wie Stufe B). UDS: Bootloader 0.3.14, App 1.0.0.

### 6.2 Funktion und Abschaltzeit (S-2)

* `check_battsim.py --hardware` lesend und mit `--remote` (3,7 V, Ausgang ein, Zelle 5 aus, Ausgang aus): Alles ok.
* Abschaltzeit, acht Laeufe mit Hardware-Zeitstempeln (Timeout von 0x130, alle Zellen 3,7 V): FAIL-Entscheidung nach der letzten 0x130 zwischen
  **299,8 und 303,7 ms** (Stufe B: 299,7 bis 308,4 ms). Analoger Abfall an Zelle 1 (PicoScope, 10:1): von 3,69 V auf < 0,5 V in 4,2 ms. Summe
  304 bis 308 ms, Grenze 310 ms (S-2) eingehalten.

### 6.3 Regler und Genauigkeit an Zelle 1

* Teilerfaktor des Tastkopfs 10 (3,0 V Sollwert: 0,30 V am Scope mit Einstellung 1:1), Messung mit 10:1 und +-10 V.
* Istwert im Status (ADC, kalibriert, Regler an) gegen Sollwert: 1,0 V 0,9994, 2,0 V 1,9994, 3,0 V 3,0005, 3,7 V 3,7005, 4,8 V 4,8006 V (Mittel aus je 80 Werten,
  Streuung +-5 mV). Das PicoScope (8 Bit, Quantisierung 0,08 V im Bereich 10 V) zeigte 0,97...1,0 / 1,96 / 3,02 / 3,67 V; feiner laesst sich der echte
  Ausgang mit diesem Aufbau nicht aufloesen.
* Nach einem grossen Sprung nach unten (4,8 V auf 1,0 V) unterschwingt der Ausgang: der gefilterte Istwert (95 ms) haengt nach, der Regler schiebt bis an
  seine Grenze (-0,1 V) und holt es mit Ki 1,0/s erst nach etwa einer Sekunde zurueck (Scope-Mittel 0,86 bis 0,91 V bis 0,8 s, ADC 0,913 V nach 0,5 s).
  Befund fuer die Abstimmung, noch nicht geaendert.

### 6.4 C-2b: Sprung von 4,8 V nach 0,5 V ohne Last, und Pegelboden der Endstufe

* Sprung 4,8 V auf 0,5 V: der Ausgang faellt in 47 ms von 4,0 V auf 0,7 V (5 ms geglaettet), bleibt dann bei **0,70 V**, und C-2 schaltet nach etwa
  100 ms ab (FAIL Grund 6, `BATTSIM_STATUS` 5, alle Ausgaenge aus; Abfall auf 0 V etwa 85 ms nach dem Trigger bei 2,5 V).
* Ursache: die Endstufe hat ohne Last einen **Pegelboden von etwa 0,70 V**. Gemessen im Sprung auf 0,3 V: Plateau 0,70 V; auf 0,5 V: 0,70 V (C-2 loest aus);
  auf 0,6 V: 0,70 V (kein Ausloesen, Abstand 0,1 V); auf 1,0 V: folgt dem Sollwert. Auch das Einschalten mit 0,5 V Sollwert aus dem Stillstand loest C-2 aus.
* Folge: Mit Schwelle 0,2 V sind Sollwerte bis etwa 0,5 V ohne Last nicht betreibbar (FAIL Grund 6 nach 100 ms); ab 0,6 V gibt es keine Ausloesung.
  Dies ist der Befund zum Nachtrags-Review C-2b. Schwelle und Zeit wurden nicht geaendert.
* Zeit bis unter Sollwert + 0,2 V nach dem Sprung (vom Abfallbeginn, 4,0 V): 1,0 V Sollwert 43 ms (unter 1,2 V bei +31,5 ms nach dem Trigger bei 2,5 V, Abfallbeginn
  -11,8 ms); 0,5 V Sollwert: 0,7 V nach 47 ms, bleibt dort an der Schwelle.

### 6.5 Provisionierung und Kalibrierung per UDS (Board `battsim-provisionierung` a, 30.10.2026)

* Master-Key `battsim_master_v1` mit `canflash keygen` im Schluesselspeicher des Nutzers angelegt (nichts ueberschrieben):
  `C:\Users\danie\AppData\Local\neXtBSW\keys\battsim_master_v1.key`. **Der Nutzer muss ihn ausserhalb des PCs sichern**; ohne ihn laesst sich das
  Geraet nicht mehr ueber CAN entsperren und nicht ueber den Bootloader flashen (per J-Link geht es weiter).
* Geraeteschluessel mit `provision.py --uid 000effffffffffff4e535161400e001b` per J-Link (1000 kHz, `loadfile` nur des Key-Sektors 0xFE000, kein Erase-Befehl, kein
  Unlock, `verifybin` erfolgreich); die Datei mit dem Geraeteschluessel wurde danach geloescht. Kein CSEc-Schluessel, kein LOAD_KEY, kein Schreibschutz.
* Schritt 5 an Kanal 1, Ausgang aus, kein Pruefling: `27 01/02` entsperrt. `2E 0201` mit Offset +10 mV, Steigung 1,0100 (`0064 2774 0000 2710`) wirkt erst nach der
  Routine (`22 0201` danach unveraendert). `31 01 0200`, `31 03 0200` ergibt result 0 (angewendet), badChannel 0, `22 0201` zeigt die neuen Werte, `22 0210` = 1.
* Reset per `11 01` (4 s nach der Routine, der NvM schreibt 2 s nach der Aenderung): `22 0201` zeigt weiter `0064 2774 0000 2710`, `22 0210` = 1, `22 0211` = 0 (Regler nach dem
  Start an). Die Kalibrierung bleibt ueber den Reset erhalten, die Partition traegt.
* Wirkung bei 3,0 V, Regler per `2E 0211 01` aus (0x121 `BATTSIM_CONTROLLER_ON` = 0; der Rueckleseversuch im selben 10-ms-Zyklus zeigt noch den alten Wert):
  Testkalibrierung ADC-Istwert 3,0402 V (n = 200), Werkskalibrierung 3,0001 V; Unterschied **+40,1 mV**, wie berechnet (1,01 * 3,0 + 0,01 V Befehl). PicoScope (Mittel 0,1 s,
  Aufloesung 0,04 V): 3,015 V gegen 2,987 V, Unterschied +28 mV, im Rahmen der Aufloesung.
* Danach Werkswerte (`0000 2710 0000 2710`) geschrieben und uebernommen, Regler per `2E 0211 00` wieder an (`22 0211` = 0), Standby, Ausgaenge aus.

## 7. Nachtrag-Review Geraetebefund (30.10.2026, alle a): C-2c, R-1, F-1

Entschieden im Review "Stufe C Geraetebefund" (Board `bs-stufec-boden`): C-2a a, C-2b a (Nachtrag), **C-2c a, R-1 a, F-1 a**. Umgesetzt im Zweig
`stufe-c-kalibrierung`, mit SIL-Test; **am Geraet noch nicht geflasht** (das Flashen dieses Stands ist nicht freigegeben). Auf dem Geraet laeuft bis dahin 5a23104 ohne
diese Aenderung: nur Sollwerte ab 0,6 V verwenden (F-1 a).

### 7.1 C-2c: Pegelboden

* Der Vergleich der Ueberspannungserkennung (Abschnitt 3) geht gegen **max(Sollwert, `OutputFloor_V`) + `OverVoltThd_V`**. `OutputFloor_V` ist ein Parameter, Vorgabe **0,7 V** (gemessen
  **ohne Last**, Abschnitt 6.4). Der Boden tritt nur ohne aeussere Last auf (Abschnitt 10.3, Review T-1: bleibt 0,7 V); mit dem Pruefling ist er nachzumessen.
* Ein freigegebener Kanal mit Sollwert groesser 0 V und kleiner als der Boden meldet im Kanalstatus **4 "setpoint below level floor"** (`CELLn_STATUS`, DBC: Wert 4 neu, das Feld
  hat 3 Bit und war bis 3 begrenzt, jetzt 0..7); es gibt **kein FAIL**. Sein Regler arbeitet nicht (der Sollwert ist nicht erreichbar, Korrektur 0).
* Restrisiko: Eine echte Ueberspannung unter max(Sollwert, Boden) + 0,2 V, also bis 0,9 V bei kleinem Sollwert, bleibt unentdeckt. Das ist weit unter der Grenze von 5,0 V; ein Pruefling
  sieht dann hoechstens 0,9 V statt des kleinen Sollwerts. Mit Last kann der Boden niedriger liegen; dann ist der Vergleich bei zu hohem Boden unempfindlich (der Boden ist der Parameter, der nachzumessen ist).
* Der LabControl-Treiber (Gegenseite) muss den Status 4 kennen.

### 7.2 R-1: Regler nach Sollwertsprung anhalten

* Aendert sich der begrenzte Sollwert eines Kanals um mehr als `StepThreshold_V` (0,2 V, in beiden Richtungen), **steht sein Regler `StepHold_cycles` (50, also 500 ms; in 0.3.1 30, also 300 ms)**:
  Korrektur und Integralanteil bleiben wie sie waren (wie bei LIMIT). Danach arbeitet er normal.
* SIL (Strecke mit 20 ms, Istwert gefiltert 95 ms): Sprung 4,8 V auf 1,0 V gibt **ohne Haltezeit ein Minimum von 0,900 V (-0,10 V, wie am Geraet)**, mit 300 ms 0,970 V (-0,03 V):
  nach 300 ms haengt der gefilterte Istwert noch 0,16 V nach. Die Haltezeit ist ein Parameter; Nachmessung beim naechsten Termin am Geraet, dort ist gegebenenfalls laenger (bis etwa 600 ms) zu waehlen.
* Die Schutzabschaltungen C-1 und C-2 haengen nicht vom Regler ab und sind von R-1 nicht beruehrt.

### 7.3 F-1 und Version

Firmware-Stand fuer 0x121: **0.3.1** (0.3.0 war 5a23104) ist am 30.10.2026 geflasht und nachgemessen (Abschnitt 8). **0.3.2** (Haltezeit 500 ms, Abschnitt 9) ist im Zweig, aber
noch nicht geflasht: das geschieht beim naechsten Geraetetermin mit eigener Freigabe.

## 8. Geraeteteil 0.3.1 (30.10.2026, Board `bs-stufec-flash-031` a, Zweig-Stand 419eb99)

Aufbau unveraendert (Nutzer: "Der Aufbau steht noch so"): J-Link, CANcaseXL Ch2, PicoScope A an Zelle 1 (10:1), kein Pruefling.

* **Flash:** Backup des laufenden P-Flash (0.3.0, `battsim_backup/2026-10-30/pflash_stufec_030_0x00000000_1M.bin`; gegenueber 5a23104 nur der Key-Record bei 0xFE000
  anders). Nur die App und der Record geflasht (`flash.jlink`, kein Erase, kein Unlock; Bootloader nicht angefasst). Rueckgelesen: Bootloader, App und Record 0 Abweichungen
  gegen die Hex, Key-Sektor 0xFE000..0xFEFFF und FCF unveraendert (FSEC 0xFE). Partition und Provisionierung bleiben, Kalibrierung im NvM unberuehrt.
* **0x121:** Firmware 0.3.1, FAIL-Grund 0, Regler ein, 48 V 0,259 A bei 14 Zellen mit 3,7 V. `check_battsim.py --hardware` lesend und mit `--remote` ohne Fehler (Istwerte 3,696 bis 3,721 V
  bei 3,7 V Soll).
* **Abschaltzeit (S-2):** 16 Laeufe mit Hardware-Zeitstempeln: FAIL-Entscheidung nach der letzten 0x130 zwischen **299,7 und 304,5 ms** (0.3.0: 299,8 bis 303,7 ms). Mit dem analogen Abfall (4,2 ms,
  in 6.2 gemessen, Endstufe unveraendert) etwa 304 bis 309 ms; 310 ms eingehalten.
* **C-2c (Pegelboden), Kanal 1 ohne Last:** Sollwert **0,5 V und 0,3 V aus dem Stillstand** (Ausgang ein): Kanalstatus **4**, 12 s lang **kein FAIL** (FAIL-Grund 0, BATTSIM_STATUS nur 1 und 3), ADC-Istwert
  gefiltert 0,707 V, Scope 0,697 bis 0,700 V. **Sprung 4,8 V auf 0,5 V und auf 0,3 V aus dem Lauf** (je 12 s gehalten): Status 4, kein FAIL, Ausgang bleibt bei 0,70 V (Scope 1,8 s lang, Minimum 0,35 V
  = Rauschboden des Scopes, kein Abfall auf 0 V); bei 0.3.0 loeste derselbe Sprung nach etwa 100 ms FAIL Grund 6 aus (Abschnitt 6.4). Der Boden von 0,70 V wurde nicht mit Last gemessen.
* **R-1 (Sprung 4,8 V auf 1,0 V, Haltezeit 300 ms):** Der Ausgang unterschwingt weniger. ADC (gefiltert): Minimum **0,965 V (-0,035 V)**, zuvor ohne Haltezeit 0,913 V (-0,087 V); zurueck auf 0,982 V nach 1,5 s. Scope (Blockmittel,
  8 Bit): Minimum 0,918 V gegen 0,857 V, nach 1,2 bis 1,8 s 0,968 V. Der Rest kommt daher, dass der gefilterte Istwert nach 300 ms noch 0,2 V ueber dem neuen Sollwert liegt (ADC 1,235 V bei +291 ms).
  **Vorschlag (nicht umgesetzt):** `StepHold_cycles` 50 (500 ms); SIL-Strecke: Minimum 0,970 V (30), **0,997 V (50)**, 0,999 V (60). Aenderung ohne weitere Logik nur ueber den Parameter, braucht eine neue Freigabe zum Flashen.

Das Geraet ist im Standby, die Ausgaenge sind aus, das PicoScope ist freigegeben.

## 9. Haltezeit 500 ms (Version 0.3.2, Board `bs-haltezeit-500` a, 30.10.2026)

`StepHold_cycles` von 30 auf **50** (500 ms); Firmware-Version in 0x121 **0.3.2**. Es ist nur der Parameter; die Logik von R-1 (Abschnitt 7.2) ist unveraendert, die Schutzabschaltungen C-1 und C-2 haengen
nicht vom Regler ab. Waehrend der Haltezeit regelt der Kanal nicht, er folgt nur der Kennlinie und der Kalibrierung (mit der Korrektur, die er vor dem Sprung hatte).

* Begruendung: Am Geraet blieb mit 300 ms ein Unterschwingen von -0,035 V (Abschnitt 8); nach 300 ms liegt der gefilterte Istwert noch etwa 0,2 V ueber dem neuen Sollwert.
* SIL (Strecke 20 ms, Istwert gefiltert 95 ms, Sprung 4,8 V auf 1,0 V): Minimum 0,900 V ohne Haltezeit, 0,970 V mit 300 ms, **0,997 V mit 500 ms**, 0,999 V mit 600 ms. Der SIL-Test verlangt jetzt
  mehr als 0,99 V (vorher mehr als 0,95 V) und prueft das Stillstehen des Tastgrads ueber alle 50 Zyklen.
* **Nicht geflasht.** Das passiert beim naechsten Geraetetermin mit eigener Freigabe, zusammen mit den Messungen mit Last. Offen bleiben dort: Pegelboden und Schwellen mit Last, C-1 und C-2 am Geraet
  ausloesen (bisher nur im SIL-Test), analoge Abschaltzeit mit der neuen Firmware, Nachmessung R-1 mit 500 ms, C-7 (Pull-ups an SET/EN und wiederholte Reset-Messung).

## 10. Termin 0.3.2 mit Last (30.10.2026, Board `bs-termin-032-last` a, main 9223da8)

Aufbau: J-Link, CANcaseXL Ch2, PicoScope A an Zelle 1 (10:1), kein Pruefling. Die Widerstaende 100 Ohm und 1 kOhm wurden vom Nutzer jeweils nur bei laufendem Master im Standby an Zelle 1
angesteckt und abgezogen (AoU S-1 a); waehrend einer Last gab es keinen J-Link-Lauf, keinen Reset und kein Aus/Ein der Versorgung. Dem Nutzer ist ein Tastkopf waehrend der ersten 100-Ohm-Messung
verrutscht (Scope zeigte 0 V); die Messung wurde nach dem Anschliessen wiederholt.

### 10.1 Flash und Abschaltzeit (ohne Last)

* Backup des laufenden Stands (0.3.1) `pflash_stufec_031_0x00000000_1M.bin`. Nur App und Record geflasht (2 Sektoren), Rueckgelesen: Bootloader, App, Record 0 Abweichungen, Key-Sektor und FCF unveraendert.
  0x121 meldet Firmware **0.3.2**.
* Abschaltzeit: 28 Laeufe: FAIL-Entscheidung nach der letzten 0x130 zwischen **299,8 und 307,4 ms** (die Obergrenze ist das kleinste "erste FAIL" aller Laeufe; 0.3.1 mit gleicher Logik: 299,7 bis 304,5 ms).
  Analoger Abfall an Zelle 1 (3,69 V auf unter 0,5 V): 4,06 und 4,17 ms. Die 310 ms (S-2) sind mit diesen Daten nicht strikt belegt, weil die Statusbotschaft nur alle 100 ms kommt; erwartet sind 300 bis 310 ms
  plus 4 ms.

### 10.2 Versorgungsstrom (0x121, 48 V), alle 14 Kanaele, Ausgang ein

* Standby und Fernsteuerung ohne Ausgang **0,216 A**; Ausgang ein, kein Kanal freigegeben 0,216 A (nach den Stufen 0,244 A, waermebedingt).
* Alle Kanaele freigegeben, ohne Last: 0,6 V 0,266 A, 1,0 V 0,273 A, 2,0 V 0,272 A, 3,7 V 0,272 A, 4,8 V 0,271 A. Mit 100 Ohm an Zelle 1: 0,267 / 0,275 / 0,276 / 0,281 / 0,282 A.
  Nur Zelle 1 freigegeben: 0,220 A (0,3 V) bis 0,231 A (4,8 V) mit 100 Ohm, 0,220 bis 0,222 A mit 1 kOhm.
* Grundlage fuer eine Mindeststrom-Schwelle der Plausibilitaetspruefung der Strommessung: Der Ruhestrom liegt immer ueber 0,2 A, eine tote Messung (0 A) ist daher mit einer Schwelle von etwa 0,1 A sicher erkennbar.

### 10.3 Pegelboden und Spannungsabfall mit Last (Zelle 1)

| Sollwert | ohne Last (ADC) | 100 Ohm: ADC / Scope | 1 kOhm: ADC / Scope |
|---|---|---|---|
| 0,3 V | 0,707 (Boden) | 0,297 / 0,268 | 0,330 / - |
| 0,5 V | 0,707 (Boden) | 0,491 / 0,445 | 0,491 / 0,466 |
| 0,7 V | - | 0,700 / 0,642 | 0,700 / 0,678 |
| 1,0 V | 0,999 | 0,999 / 0,88 bis 0,89 | 1,000 / - |
| 2,0 V | 2,000 | 2,000 / 1,863 | 2,001 / - |
| 3,7 V | 3,701 | 3,701 / 3,455 | 3,701 / 3,626 |
| 4,8 V | 4,801 | 4,801 / 4,510 | 4,801 / 4,732 |

* **Der Pegelboden von 0,70 V tritt nur ohne Last auf.** Schon mit 1 kOhm (0,3 bis 5 mA) folgt der Ausgang dem Sollwert bis 0,3 V. Der Boden ist ein Befund der unbelasteten Endstufe; mit Last kann er nur sinken.
  C-2 loeste mit keiner Last bei keinem Sollwert aus, Status 4 meldet unter 0,7 V weiter "Sollwert unter Pegelboden", auch wenn die Spannung stimmt (Hinweis, kein FAIL).
* **Spannungsabfall zwischen ADC-Abgriff und Klemmen:** Die Klemmenspannung liegt unter Last unter dem ADC-Wert (und damit unter dem Sollwert, auf den der Regler den ADC-Wert ausregelt): 0,03 bis 0,06 V bei 3 bis 7 mA, 0,11 bis 0,14 V bei
  10 bis 20 mA, 0,25 V bei 37 mA, 0,29 V bei 48 mA, entsprechend etwa 5 bis 7 Ohm (Scope-Fehler von etwa 0,03 V eingerechnet). Mit 1 kOhm: 0,02 bis 0,03 V (0,5 bis 0,7 mA), 0,07 V (4 bis 5 mA). Der Regler sieht diesen Abfall nicht.
  Fuer einen Pruefling mit Eingangsstrom im uA- bis mA-Bereich bleibt die Abweichung unter etwa 0,05 V.

### 10.4 R-1 mit Last (Sprung 4,8 V auf 1,0 V)

* 0.3.2 mit 500 ms Haltezeit und 100 Ohm: ADC (gefiltert) Minimum **0,971 V (-0,029 V)**; 0.3.1 mit 300 ms ohne Last: 0,9645 V. Die Verlaengerung der Haltezeit bringt kaum etwas, obwohl der SIL 0,997 V voraussagt.
  Das spricht dagegen, dass allein das Nachhaengen des Filters schuld ist. Vermutete Ursache: Waehrend der Haltezeit bleibt die Korrektur des **alten Arbeitspunktes** (4,8 V) stehen und wird auf 1,0 V angewendet (am Geraet rund -0,03 V,
  im SIL ist die Strecke ideal und die Korrektur 0).
  Am Scope: die Spannung faellt unter 1,2 V nach 32 ms, unter 1,0 V nach 60 ms, Minimum 0,874 V und Endwert 0,88 bis 0,89 V (der Abstand zum Sollwert kommt vom Spannungsabfall an der Last, 10.3).
* **Vorschlag (nicht umgesetzt):** Bei einem Sollwertsprung Korrektur und Integralanteil des Kanals auf 0 setzen und fuer die Haltezeit auf 0 lassen (statt stehen zu lassen); dazu eine SIL-Strecke mit Verstaerkungs- und Offsetfehler.
  Das aendert die Logik von R-1 und braucht eine eigene Entscheidung.

### 10.5 C-2 am Geraet ausgeloest (ohne Last, ohne Eingriff)

* Kanal 1 per UDS auf Messen 1,1000 (`11000`) und +0,2 V (`2000`) gesetzt, Routine 0x0200 (Ergebnis 0). Sollwert 3,0 V, Ausgang ein: Die Firmware rechnet 1,1 * 3,0 + 0,2 = 3,5 V.
* Scope (Kanal 1, Einzeltrigger 1,5 V steigend): Die echte Spannung steigt auf etwa 3,05 V (kein echter Ueberspannungsfall) und wird **etwa 135 ms nach dem Durchgang durch 1,5 V, etwa 155 ms nach dem Einschalten**, abgeschaltet.
  CAN: 0x120 FAIL (5) mit Ausgang 1 und 0x121 **FAIL-Grund 6** beim ersten Status nach 203 ms, alle Ausgaenge aus. Erwartet waren etwa 110 ms plus das Anlaufen der Spannung (die Bedingung ist erst ab etwa 2,7 V wahr); passt.
* Danach Werkswerte (`0000 2710 0000 2710`) zurueckgeschrieben und uebernommen, Standby, Ausgaenge aus.

### 10.6 C-1 und Grenze der Strommessung

* C-1 laesst sich ohne Eingriff nicht ausloesen: `I_SUPPLY` ist in `boards/battsim_master_v1/board.yaml` ein normaler skalierter ADC-Kanal (Faktor 1, keine Sensorgrenzen). `adc.qlf.I_SUPPLY` wird nur ungueltig, wenn die ADC-Wandlung selbst
  scheitert; Rohwert 0 oder Vollausschlag (offener oder kurzgeschlossener Shunt) geben nur bei Sensorkanaelen QLF_ERROR_RANGE (nextbsw `doc/architecture/signal-qualifier.md`, IO-18). Ein Draht- oder Shuntfehler
  macht die Strommessung also **gueltig, aber falsch**; das faengt C-1 nicht (Review `bs-c1-strommessung`).

## 11. Plausibilitaet der Strommessung (Review `bs-c1-strommessung` S-1 a und S-2 a, Version 0.3.3)

Entschieden am 30.10.2026 (Review "C-1 Strommessung"): **S-1 a** Plausibilitaetspruefung der Strommessung ergaenzen, **S-2 a** C-1 am Geraet ohne Hardware-Eingriff nachweisen (Verfahren unten als Vorschlag, nicht umgesetzt).
Auf dem Geraet laeuft 0.3.2; **0.3.3 ist im Zweig `stufe-c-strommessung-plausibel` (von main 9223da8), nicht geflasht.**

### 11.1 Was die Pruefung tut

* Befund: Ein offener Shunt oder eine unterbrochene Sense-Leitung ergibt eine **gueltige** Messung von etwa 0 A (`I_SUPPLY` ist ein normaler skalierter ADC-Kanal ohne Sensorgrenzen, Abschnitt 10.6). C-1 aus Abschnitt 2 erkennt nur einen
  ungueltigen Qualifier; die Ueberlastabschaltung (FAIL Grund 3) waere in diesem Fall blind.
* Neu: Bei **Ausgang ein und mindestens einem freigegebenen Kanal** nimmt der Master immer einen Mindeststrom auf (Ruhestrom 216 mA, mit 14 Kanaelen 270 mA, mit Last bis 282 mA, Abschnitt 10.2). Ein **gueltiger** Abtastwert unter
  `MinCurrent_mA` (Vorgabe **100 mA**) gilt als unplausibel. Sind es `MinCurrentTime_cycles` (**20**, also 200 ms) Abtastwerte in Folge, setzt `SupplyMonitor` `CurrentInvalid` wie bei einem ungueltigen Qualifier:
  `BattSimMode` geht in RUN auf **FAIL Grund 5** (Strommessung ungueltig), die Ausgaenge sind im selben 10-ms-Schritt aus.
* Verglichen wird der Abtastwert (`ISupply_mV` mal `SupplyCurrentGain`), nicht der Tiefpass (Zeitkonstante 1,3 s, er brauchte etwa 1 s, um unter die Schwelle zu fallen). Ein ungueltiger Abtastwert zaehlt weder mit noch loescht er;
  ein plausibler Wert, Ausgang aus oder kein freigegebener Kanal loeschen den Zaehler. Der Zaehler sattelt bei 255.
* `CellChannels` liefert dazu den neuen Ausgang `ChannelsReleased` (Ausgang ein **und** mindestens ein Kanal freigegeben, `CELLn_ENABLE` = 2). `CellChannels` laeuft nach `SupplyMonitor`: Der Wert wirkt einen 10-ms-Schritt spaeter.
* Beim Wechsel STOP nach RUN ist der Strom noch nicht ueberwacht (kein Kanal freigegeben, Ausgang aus): kein Einfluss auf das Freigeben von RUN; der bestehende Weg "RUN mit ungueltiger Messung wird direkt FAIL" bleibt.
* Parameter (Kalibrierwerte, kein neuer Code beim Aendern): `MinCurrent_mA` (0 bis 200 mA, 100), `MinCurrentTime_cycles` (20). Firmware-Version in 0x121: **0.3.3**.

### 11.2 Restrisiko

* Gueltig, aber falsch **ueber** der Schwelle (zum Beispiel ein Messwert, der bei 0,5 A haengt) bleibt unentdeckt; ebenso eine Messung, die nur etwas zu niedrig liest (die Ueberlastschwelle liegt bei 1,28 A). Die Pruefung deckt nur den Ausfall "Messung faellt auf etwa 0 A" ab.
* Ein Ruhestrom unter 100 mA (zum Beispiel eine Hardware-Aenderung der Karten) loest die Pruefung dauerhaft aus; die Schwelle ist dann nachzumessen und anzupassen. Der Abstand zum gemessenen Ruhestrom (216 mA) ist gut 2:1.
* Die Plausibilitaet gilt nur bei freigegebenem Kanal. Ohne freigegebene Kanaele ist ein tiefer Strom plausibel und die Ueberlast ist dann ohnehin nicht relevant.

### 11.3 SIL-Test

`implausiblyLowCurrentSwitchesOffInRun`: Ruhestrom (220 mA) bleibt ohne Folgen; Messwert 0 mA gueltig: 19 Abtastwerte keine Reaktion, ein plausibler Wert loescht den Zaehler, der 20. fuehrt in FAIL Grund 5 (auch auf dem Bus), Ausgaenge im selben Schritt aus, Rueckweg ueber STANDBY und RUN
wieder moeglich; Schwelle (101 mA nie, 99 mA nach 20 Abtastwerten); ungueltige Abtastwerte zaehlen nicht und loeschen nicht; ohne freigegebenen Kanal oder bei Ausgang aus keine Reaktion. Die uebrigen SIL-Tests laufen jetzt mit einer Strommessung von 220 mA
statt 0 mA (ein Geraet mit freigegebenem Kanal hat nie 0 A).

### 11.4 S-2 a: Nachweis am Geraet ohne Hardware-Eingriff (Vorschlag, nicht umgesetzt)

Die Messung selbst laesst sich am Geraet nicht verfaelschen (kein Eingriff in den Shunt, die Kalibrierung per UDS betrifft nur die Zellkanaele). Zwei Wege, beide ohne Hardware-Eingriff:

1. **Parameter-Testbau (kein neuer Code, nur zwei Zahlen):** Ein Bau, der sich von 0.3.3 nur in `MinCurrent_mA` (zum Beispiel 400 statt 100, dazu die Obergrenze von 200 auf 500 in `swc.yaml`) unterscheidet, wird per J-Link geflasht
   (nur App und Record, kein Pruefling). Alle 14 Kanaele freigegeben mit 3,7 V und Ausgang ein: Die gemessenen 0,27 A liegen unter 0,4 A, nach 200 ms folgt FAIL Grund 5 (0x121, im Scope die abgeschaltete Zelle 1). Danach der Gegenversuch mit
   Ausgang ein ohne freigegebenen Kanal (keine Reaktion). Zum Schluss wieder 0.3.3 flashen, `check_battsim`. **Zwei zusaetzliche Flashvorgaenge, eigene Freigabe.**
2. **Schwelle ueber Diagnose einstellbar machen (Codeaenderung):** `MinCurrent_mA` als RAM-Wert (nicht gespeichert, wie `controllerOff`) per `2E` mit Security-Access setzbar; dann reicht ein Lauf ohne Flash: Schwelle auf 400 mA setzen, Kanaele an, FAIL Grund 5 abwarten, Schwelle zurueck. Ein Eingriff
   in den Produktionscode, nicht nur eine Testfunktion, und eine neue Diagnosekennung.

Empfehlung: Weg 1, weil er den Produktionscode unveraendert laesst. Beide Wege weisen den Pfad "Abtastwert unter Schwelle -> Zaehler -> FAIL 5 -> Ausgaenge aus" am Geraet nach (Zaehler, ChannelsReleased, Mode); den Qualifier-Pfad und einen echten offenen Shunt weisen sie nicht nach.

## 12. Review Termin 0.3.2 (T-1 bis T-4, 30.10.2026, alle a) und Regler-Aenderung (Version 0.3.3)

Entscheidungen im Review "Termin 0.3.2" (Board `bs-termin-032-review`): **T-1 a, T-2 a, T-3 a, T-4 a.** Umgesetzt im Zweig `stufe-c-strommessung-plausibel` zusammen mit der Plausibilitaet der Strommessung (Abschnitt 11), Version **0.3.3**, nicht geflasht.

* **T-1 (Pegelboden): `OutputFloor_V` bleibt 0,7 V.** Der Boden tritt nur ohne aeussere Last auf (Abschnitt 10.3); Status 4 ("Sollwert unter Pegelboden") ist ein Hinweis, kein Fehler. Eingetragen ist das als "ohne Last" im
  Parameterkommentar (`tools/gen_swc_ports.py`, `swc.yaml`, `CellChannels_data.m`), als Signalkommentar der 16 `CELLnn_STATUS` in `BattSimV2.dbc` und in Abschnitt 7.1. Der LabControl-Treiber hat eine Kopie der DBC (das Kommentar wirkt dort erst
  mit der naechsten Kopie).
* **T-2 (Spannungsabfall): Kennwert der Genauigkeit.** Abweichung an der Klemme **etwa Laststrom mal 6 Ohm**, gemessen 5 bis 7 Ohm (Abschnitt 10.3); aufgenommen in `stufe-c-kalibrierung-regler.md`. Keine Codeaenderung; der Regler regelt den ADC-Wert.
* **T-3 (Regler beim Sprung auf 0):** Aendert sich der begrenzte Sollwert eines Kanals um mehr als `StepThreshold_V` (0,2 V), werden **Korrektur und Integralanteil auf 0 gesetzt und fuer `StepHold_cycles` (50, 500 ms) auf 0 gehalten**
  (0.3.1 und 0.3.2: sie blieben stehen). Das Regelverhalten danach ist unveraendert (PI, +-`CtrlLimit_V`).
  * Begruendung: Am Geraet blieb das Unterschwingen nach 4,8 V auf 1,0 V mit 300 ms und mit 500 ms Haltezeit gleich (-0,035 V und -0,029 V, Abschnitt 10.4): Die Korrektur des alten Arbeitspunktes (rund -0,03 V bei 4,8 V auf Kanal 1) wirkt waehrend der
    Haltezeit auf den neuen Sollwert. Die Strecke ist ohne Regler genau genug (3,0 V Soll, ADC 3,0001 V ohne Regler, Abschnitt 6.3).
  * SIL-Test `stepClearsTheCorrectionOfTheOldOperatingPoint`: Strecke mit Verstaerkungsfehler +0,6 % (Korrektur bei 4,8 V etwa -0,03 V, bei 1,0 V nur -0,006 V), wie der Befund am Geraet. Sprung 4,8 V auf 1,0 V: **mit der Logik von 0.3.2 Minimum 0,977 V
    (-0,023 V, wie am Geraet), mit 0.3.3 Minimum > 0,99 V**; waehrend der Haltezeit entspricht der Tastgrad der Kennlinie (Korrektur 0), nach 2 V Sprung nach oben ebenso. Der bisherige R-1-Test prueft jetzt auch, dass nach einem Sprung nach oben der Tastgrad
    der Kennlinie entspricht.
  * Nachmessung am Geraet (Sprung 4,8 V auf 1,0 V, ADC-Minimum und Scope) beim naechsten Termin; die Erwartung ist ein Minimum nahe 1,0 V.
* **T-4 (Abschaltzeit): Der Kennwert 315 ms (S-2 a) gilt weiter.** Die Messung mit 0.3.2 bestaetigt ihn: FAIL-Entscheidung 299,8 bis 307,4 ms nach der letzten 0x130 (28 Laeufe) plus analoger Abfall 4,1 ms (4,06 und 4,17 ms) gibt eine Summe von hoechstens
  311,5 ms, unter den 315 ms. Die 310 ms der Konzeptgrenze (Timeout 300 ms plus ein 10-ms-Schritt) sind damit nicht strikt belegt, der Kennwert 315 ms ist es.

## 13. Geraeteteil 0.3.3 (30.10.2026, Board `bs-flash-033` a): T-3 und C-1 am Geraet

Aufbau unveraendert (Nutzer: "Ja der Aufbau steht noch"): J-Link, CANcaseXL Ch2, PicoScope A an Zelle 1 (10:1), kein Pruefling, keine Last.

* **Flash:** Backup des laufenden Stands (0.3.2) `pflash_stufec_032_0x00000000_1M.bin`. **0.3.3** (Zweig-Stand 3afeaa6): nur App und Record, Rueckgelesen 0 Abweichungen (Bootloader, App, Record), Key-Sektor und FCF unveraendert.
  `check_battsim --hardware --remote` Alles ok, 0x121: Firmware 0.3.3, FAIL-Grund 0, Regler ein, 48 V 0,260 A. Mit allen 14 Kanaelen bei 3,7 V und Ausgang ein keine Reaktion der neuen Plausibilitaet (0,27 A gegen 0,1 A).
* **T-3 (Sprung 4,8 V auf 1,0 V an Zelle 1, ohne Last):** ADC gefiltert **Minimum 0,985 V (-0,015 V)** gegen 0,9645 V mit 0.3.1 (300 ms, Korrektur stehen) und 0,913 V mit 0.3.0 (ohne Haltezeit); Scope (Blockmittel): 0,965 bis 0,975 V gegen 0,918 bis 0,937 V (0.3.1) und 0,857 bis 0,866 V (0.3.0).
  Der ADC-Wert laeuft danach auf 0,99 und mit dem Integralanteil (Ki 1/s) langsam auf 1,00 V (nach 2,4 s). Das Unterschwingen ist damit weniger als halb so gross wie bei 0.3.1; der Rest ist nicht mehr die Rest-Korrektur, sondern die statische Abweichung des offenen
  Kanals bei 1,0 V (etwa -0,01 V), die der Regler mit 1 V/s abbaut. Das SIL (ideale Strecke) sagt 1,001 V voraus; am Geraet bleibt die Abweichung der Karte.
* **C-1 am Geraet ausgeloest (S-2 a, Weg 1, ohne Hardware-Eingriff):** **Testbau** (nur `MinCurrent_mA` 400 statt 100, Grenze 500, Version 0.3.253; Zweig `testbau-c1-mincurrent-400`, Commit acd8289, **nie mergen, nie auf dem Geraet lassen**), nur App und Record geflasht, Rueckgelesen 0
  Abweichungen. Alle 14 Kanaele mit 3,7 V, Ausgang ein: **0x121 FAIL-Grund 5** beim ersten Status nach der Abschaltung (308 ms nach dem Einschalten, die Statusbotschaft kommt nur alle 100 ms), `BATTSIM_STATUS` FAIL (5), Ausgang 1. Scope an Zelle 1: Die Spannung steigt ab -11 ms (Trigger 1,5 V
  = 0) auf etwa 3,4 V und wird bei **189 ms**, also **etwa 200 ms nach dem Einschalten**, abgeschaltet: genau die Zeit `MinCurrentTime_cycles` (20 Zyklen). Gegenprobe (Ausgang ein, kein Kanal freigegeben, 9 s): 0,217 A, FAIL-Grund 0, keine Reaktion.
  Damit ist der Pfad "Abtastwert unter Schwelle - Zaehler - FAIL 5 - Ausgaenge aus" am Geraet nachgewiesen; nicht der Qualifier-Pfad und kein echter offener Shunt (Abschnitt 11.4).
* **Zurueck auf 0.3.3:** Produktionsstand erneut geflasht (nur App und Record), Rueckgelesen 0 Abweichungen, Key-Sektor und FCF unveraendert, `check_battsim --hardware --remote` Alles ok, 0x121 Firmware 0.3.3, Kalibrierung Werkswerte (`22 0201` = 0000 2710 0000 2710, `22 0210` = 1), Regler an, Standby, Ausgaenge aus.
  Auf dem Geraet laeuft **0.3.3**; es ist noch nicht auf `main` (Zweig `stufe-c-strommessung-plausibel`, 3afeaa6).
