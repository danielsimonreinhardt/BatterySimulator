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
