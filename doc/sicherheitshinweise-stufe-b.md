# Sicherheitshinweise Stufe B (battsim_master)

Stand: 28.10.2026, Zweig `stufe-b-ersatz`. Gilt fuer die Firmware-Stufe B (14 Zellkanaele mit Einzel-PWM, Betriebsarten,
Abschaltung bei Ausfall von 0x130). Der Geraeteteil der Stufe B ist noch nicht gelaufen.

## 1. DFA-4-Warnungen von `nxconfig check` (hingenommen)

`nxconfig check projects/battsim_master/project.yaml` meldet vier Warnungen, je eine pro Einzel-PWM-Gruppe
(`pwm.SET_FTM5`, `pwm.SET_FTM4`, `pwm.SET_FTM3`, `pwm.SET_FTM0`, project.yaml Zeilen 45 bis 48):

> the outputs of single PWM group ... have no safe_off output on another peripheral: a switched-off clock gate of FTM<n>
> freezes their pins at the level they had (DFA-4, doc/safety/dfa.md) ... (AoU-07 of the safety manual)

**Entscheidung (Review Stufe B, B-1): fuer Stufe B hingenommen.** Die Warnungen bleiben stehen und werden nicht
unterdrueckt.

Begruendung:

- Die 14 Sollwertausgaenge sind Einzel-PWM-Kanaele (IO-116, Modus `single`, 1 kHz, aktiv low, sicherer Zustand Pin
  high). Eine Gruppe liegt auf je einem FTM (FTM5, FTM4, FTM3, FTM0). Fehlt dessen Taktfreigabe, frieren die Pins auf dem
  letzten Pegel ein, das kann der aktive Pegel sein.
- Ein `safe_off`-Ausgang auf einer anderen Peripherie ist fuer die Einzel-PWM-Gruppen mit dem gepinnten neXtBSW nicht
  deklarierbar. Die Freigabepins EN_01..EN_14 sind DIO-Pins; sie koennen derzeit nicht als `safe_off` einer PWM-Gruppe
  eingetragen werden.
- Zweiter Abschaltweg ist deshalb das Abschalten ueber EN_01..EN_14 (Zellfreigabe). Die Modellkomponente CellChannels
  nimmt beim Abschalten (`OutputEnable` aus) den Tastgrad auf 0 und gleichzeitig alle EN_xx aus.
- Als deklarierter zweiter Abschaltweg im Konfigurationsmodell wirkt das erst mit **neXtBSW IO-119**. Bis dahin steht
  der Weg nur in dieser Beschreibung und im Modell.
- Wirksam ist der zweite Weg nur, wenn EN_xx im Reset „aus“ ist. Das haengt an den Pull-Widerstaenden der Hardware und an
  der Pin-Initialisierung beim Reset (IO-116, Pull-up Variante c / S2); es ist bisher nicht nachgewiesen.
- **Der Pegel von EN_xx im Reset wird im Geraeteteil der Stufe B gemessen** (separate Boardfreigabe durch den Nutzer).
  Das Ergebnis wird hier nachgetragen. Bis dahin gilt der zweite Abschaltweg nicht als belegt.

Folge fuer AoU-07 des Safety-Manuals (was die Ausgaenge treiben, muss bei eingefrorenen Pins einen definierten sicheren
Zustand haben): fuer die Zellkanaele des Batterie-Simulators ist das ueber EN_xx (Zelle aus) gedacht, s. o.; die Bewertung
gehoert in die Safety-Bewertung der Stufe C.

## 2. Offener Punkt: ungueltige Strommessung (B-2)

SupplyMonitor (Ueberlast der 48-V-Versorgung) verwirft Abtastwerte, deren Qualifier der Strommessung (`ISupplyQlf`) nicht
`QLF_OK` ist: der zuletzt gefilterte Wert bleibt stehen, es gibt keine eigene Reaktion. Faellt die Strommessung dauerhaft
aus, bleibt die Ueberlasterkennung (FAIL Grund 3) damit unwirksam und der Ausfall wird nicht gemeldet.

**Entscheidung (Review Stufe B, B-2): bleibt in Stufe B unbehandelt.** Die Reaktion auf eine ungueltige Strommessung
(Fail-safe Abschalten, Meldung, Entprellzeit) wird in der **Safety-Bewertung von Stufe C** entschieden. Bis dahin ist das
ein offener Punkt, der nicht durch eine Zwischenloesung im Kundenprojekt umgangen wird.

**Entschieden am 30.10.2026 (Review Stufe C, C-1 a):** In RUN laenger als 200 ms ungueltig fuehrt zu FAIL Grund 5, die Ausgaenge sind im
selben Schritt aus. Umsetzung und Grenzen: `sicherheitshinweise-stufe-c.md`, Abschnitt 2.

## 3. Alive-Zaehler der Steuerbotschaften (bs-alive-rx b)

Alle neun Steuerbotschaften (0x130, 0x100 bis 0x105, 0x131, 0x132) werden beim Empfang auf einen Alive-Zaehler **1..14**
geprueft (`counter_min: 1`, `counter_max: 14`, `max_delta_counter: 2` in `projects/battsim_master/rte.yaml`, COM-54 und
COM-56 des gepinnten neXtBSW). Der Umlauf 14 -> 1 zaehlt als Schritt 1 (signal-qualifier.md, „Counter range (COM-56)“),
0 und 15 sind Alive-Fehler und werden nie zur Referenz. Ein Alive-Fehler wird 150 ms (0x130) entprellt und fuehrt in RUN
danach zu FAIL mit Grund 2. Der SIL-Test (`projects/battsim_master/test/battsim_sil_test.c`) prueft die Folge
13, 14, 1, 2 als OK und 0 und 15 als Fehler gegen die Konfiguration aus dem erzeugten Code.

## 4. Geraeteteil Stufe B (28.10.2026, J-Link, Firmware ca8d4ac, LabControl-Treiber 0.22.1)

Aufbau: PicoScope 2204A Kanal A an Zelle 1, Tastkopf 10:1, DC, eine Masse an Zelle-1-Minus; CAN allein ueber vector:59177:1
mit Hardware-Zeitstempeln (TX-Bestaetigungen der Steuerbotschaften und RX der Statusbotschaften). Plots in
`doc/messungen-stufe-b/`. Der Aufbau ist in der Messunsicherheit begrenzt: 8-Bit-Oszilloskop, Rauschen am Tastkopf ca. 0,3 V
Spitze-Spitze, Statusbotschaften 0x120/0x121 nur alle 100 ms.

### 4.1 Abschaltzeit bei Ausfall von 0x130 (Timeout)

Alle Zellen 3,7 V, Ausgang ein, dann sendet der Treiber nichts mehr (keine Shutdown-Frames). Elf Laeufe.

- **Firmware-Reaktion (CAN):** Der FAIL-Entscheidungszeitpunkt nach der letzten gesendeten 0x130 liegt zwischen
  **299,7 ms und 308,4 ms** (Schnittmenge der zehn Laeufe mit gegeneinander verschobener Phase zum 100-ms-Takt von
  0x120; jeder einzelne Lauf kennt die Entscheidung nur auf 100 ms genau). Erste 0x121 mit FAIL-Grund 1 und erste 0x120 mit
  Status FAIL erscheinen im Mittel 1,5 ms auseinander. Das passt zu Timeout 300 ms plus einem 10-ms-Schritt.
- **Analoger Abfall (Oszilloskop, drei Aufnahmen):** Zelle 1 faellt von 3,69 V auf < 0,5 V in **4,4 bis 4,7 ms**
  (Beginn des Abfalls bis 0,5 V; 90 % bis 0,5 V: 3,9 bis 4,3 ms), Plateau 3,69 V.
- **Summe (Grenze 310 ms, letzte 0x130 bis Zelle 1 < 0,5 V):** 304 ms bis 313 ms. Ob der analoge Abfall genau mit der
  FAIL-Entscheidung beginnt oder eine Verzoegerung der Ausgangsstufe dazukommt, ist mit dieser Messung nicht trennbar
  (Oszilloskop und CAN haben keine gemeinsame Zeitbasis). Im ungunstigsten Fall liegt die Summe damit etwa 3 ms ueber 310 ms.
  Die Grenze wird nicht verschoben, das ist ein Befund fuer die Bewertung.
- Der Umlauf des Alive-Zaehlers 14 -> 1 (elf Laeufe mit je 1,5 bis 4,5 s Betrieb im Zustand RUN) loest keinen Alive-Fehler aus.

### 4.2 Alive-Fehler (Zaehler 0)

Im Betrieb mit Ausgang ein sendet der Treiber 0x130 mit Alive-Zaehler 0 weiter (alle anderen Botschaften gueltig). Vier Laeufe:
FAIL mit **Grund 2** nach 148 bis 215 ms ab dem ersten Frame mit Zaehler 0 (Schnittmenge, Entprellzeit 150 ms), Ausgang aus,
Status FAIL; Rueckkehr mit dem Reset-Kommando in STANDBY.

### 4.3 Zelle 1 mit gehaltenem Reset (nRESET per J-Link `r0`/`r1`)

nRESET am SWD-Stecker ist verdrahtet: 0x120 bleibt waehrend `r0` 20,5 s lang aus und kommt nach `r1` mit STANDBY, Alive 1,
alle Zellen aus zurueck.

- **Zelle 1 liegt waehrend des gehaltenen Resets bei etwa 5,25 V** (nach ca. 50 ms Anstieg, ueber die ganzen 20 s konstant),
  also auf dem hoechsten Wert. Der Ausgang ist im Reset **nicht** aus. Das ist der Fall S2 (etwa 5,3 V), nicht der Fall
  „0 V, mindestens ein Weg aus“.
- Nach `r1` faellt Zelle 1 binnen etwa 10 ms auf 0 V (Start der Firmware setzt die PWM-Pins auf den sicheren Pegel),
  ohne sichtbaren Zwischenwert; die Aufloesung war dort 5 ms.
- Folge fuer 1: Der zweite Abschaltweg ueber EN_01..14 ist **im Reset nicht wirksam**; mit dem Reset-Pegel von EN ist
  Zelle 1 auf dem Maximum. Es wurde nur Zelle 1 gemessen und nur der Ausgangspegel, nicht der Pin-Pegel von EN_01 und
  PWM-Pin getrennt: die Pin-Messung bleibt offen. Das Einschalten des Geraets (Spannungsversorgung) wurde nicht gemessen.

## 5. Entscheidungen aus der Safety-Eskalation (Board bs-reset-eskalation, 28.10.2026)

### 5.1 Annahme (AoU) zum Reset (S-1 a)

Zelle 1 liegt im gehaltenen Reset bei etwa 5,25 V (Abschnitt 4.3). Ursache nach dem Schaltplan der Zellkarte
(BatterieSimulator `doc/hardware-system.html`, S2): Die Master-Pins fuer SET und EN sind im Reset hochohmig, Q8/Q9 sperren, die
Pull-ups R45/R48 legen die Isolatoreingaenge auf high, und der ISO6742F gibt bei vorhandener 5-V-Versorgung der
Eingangsseite diesen Pegel weiter: SET voll, EN ein, **beide Abschaltwege sind im Reset „ein“**. Der Ruhepegel low der
Variante F gilt nur bei unversorgter Eingangsseite und hilft hier nicht. Die Pin-Pegel von EN und SET werden getrennt
gemessen, sobald das Geraet offen ist (Messung offen); die Erklaerung beruht bis dahin auf Schaltplan und Ausgangsmessung.

**Annahme fuer den Betrieb des Batterie-Simulators (gilt, bis die Hardware geaendert ist):**

- Ein Pruefling (BCU-Board, MC33771C) wird erst angeschlossen, wenn der Master laeuft, also wenn 0x120 den Zustand
  STANDBY meldet.
- Mit angeschlossenem Pruefling wird nicht geflasht und kein Halt und kein Reset per J-Link ausgeloest.
- Die Annahme gilt ebenso fuer jeden anderen Reset des Masters mit angeschlossenem Pruefling: Watchdog-Reset und
  Safety-Reset (kurz) und Reset an der Versorgung (Ein- und Ausschalten, Einbruch der Versorgung). In jedem dieser Faelle sind
  die Zellausgaenge fuer die Dauer des Resets auf etwa 5,25 V.
- Die Hardware-Abhilfe (Ruhepegel „aus“ fuer SET/EN im Reset) steht als offener Punkt im Repo BatterieSimulator.

### 5.2 Abschaltzeit (S-2 a)

Die Grenze von 310 ms gilt fuer die **Reaktion der Firmware** auf den Ausfall von 0x130: gemessen 299,7 bis 308,4 ms
(Abschnitt 4.1). Der Abfall am Ausgang (Zelle 1 von 3,69 V auf < 0,5 V, 4,4 bis 4,7 ms) ist getrennt dokumentiert. Als
Kennwert fuer das Geraet gilt: von der letzten 0x130 bis Zelle < 0,5 V hoechstens **315 ms**.

### 5.3 Rueckweg (S-3 b)

Auf dem Geraet bleibt die Werksfirmware (Sicherung vom 27.10.2026, P-Flash 1 MB ab 0x0). Die Messung 4.3 (Zelle 1 im
Reset) laesst sich mit der Werksfirmware nicht wiederholen: die alte Firmware reagiert beim Reset genauso, weil der Pegel
von der Hardware kommt und nicht von der Firmware. Das Ergebnis des Rueckwegs steht unten.

**Ergebnis des Rueckwegs (28.10.2026):** Die Sicherung (SHA-256 `cb1f88f3…1ab0`, geprueft vor dem Schreiben) wurde mit
`loadbin` ab 0x0 per J-Link (1000 kHz, ohne Erase- und Unlock-Befehl, `verifybin` erfolgreich) auf das Geraet geschrieben; J-Link
hat zwei Bereiche mit zusammen 237568 Bytes neu programmiert. Der danach gelesene P-Flash (1 MB) hat dieselbe SHA-256 wie die
Sicherung, FCF bei 0x400 unveraendert (FF×12, FSEC 0xFE, FOPT 0x7F), SIM_FCFG1 0xFF32F000, FTFC 80 02 FE 7F. Die
Werksfirmware laeuft: 0x120 wird empfangen (Standby), alle 14 Zellen aus (0,016 bis 0,047 V), `check_battsim.py --hardware`
ohne `--remote` meldet „Alles ok“. Auf dem Geraet bleibt die Werksfirmware.
