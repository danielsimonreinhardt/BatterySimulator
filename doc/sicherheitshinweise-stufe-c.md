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
