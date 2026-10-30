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
* **Offen fuer den Geraeteteil:** Wie schnell der echte Ausgang nach einem Sprung des Sollwerts nach unten folgt (Entladung ohne
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

### 6.5 Offen

* Schritt 5 (Kalibrierung per UDS schreiben, Routine 0x0200, Erhalt nach Reset) ist nicht gelaufen: Das Geraet ist nicht provisioniert (kein Key-Record im Key-Sektor),
  `27 01` antwortet NRC 0x22. Es gibt keinen Master-Key `battsim_master_v1` im Schluesselspeicher. Entscheidung im Board `battsim-provisionierung`
  (keygen und provision.py). Bis dahin laeuft Stufe C mit Werks-Kalibrierung (22 0210 = 1 STORED, die Werkswerte hat der NvM nach der Partition
  aufgenommen und ueber Resets behalten).
