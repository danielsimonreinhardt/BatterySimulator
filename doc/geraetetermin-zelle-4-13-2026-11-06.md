# Zellkanäle bleiben nach „alle aus“ mit Last auf Spannung (06.11.2026, Firmware 0.5.5)

Stand: Auswertung aus Mitschnitt und Quellen, Ursache **nicht geklärt**. Board: `bs-zelle-4-13`, `bs-hw-enable-failsafe`, `bs-fehlerzustand`, `bs-pwm-bei-aus`.
Alles hier ist gelesen (LabControl, passiver CAN-Mitschnitt, KiCad-Quellen `BattSim_V1`); am Gerät wurde von dieser Session nichts gesteuert. Datenblätter liegen nicht im
Workspace, Bauteilangaben daraus sind als „aus Erinnerung“ gekennzeichnet.

## 1. Befunde

**Erstfund (Nutzer, 06.11. 15:51, LabControl 0.26.0, kurz nach LabControl-Neustart).** Ausgang freigegeben, alle 14 Kanäle `disabled` (ENABLE-Befehl 0), alle Sollwerte 3,7 V:
Zelle 4 = 3,699 V und Zelle 13 = 3,700 V dauerhaft (25 s Mitschnitt), die übrigen ≈ 0,02 V. Bei der allerersten Lesung (Ausgang noch aus, Stapel 20,6075 V = Summe der Meldungen):
Zelle 1 = 3,6995, Zelle 2 = 4,5564, Zelle 3 = 4,8251, Zelle 4 = 3,6991, Zelle 13 = 3,6997 V; Rohframes dazu wurden nicht mitgeschnitten.

**Nachstellung (Koordinator mit dem Nutzer, 16:06, Mitschnitt `alle_aus_mitschnitt_1.txt`, Beginn 16:06:14).** Last des Nutzers zwischen Zelle 1− und Zelle 14+ (beim ersten Versuch
noch nicht angeschlossen):

| Zeit (lokal) | Aktion | Ergebnis |
|---|---|---|
| 16:06:18 | BS_ALL_OFF ohne Last | alle Zellen ≈ 0,02 V, sauber |
| 16:06:32 (t = 17,7 s) | BS_ALL_ON | alle ≈ 3,7 V |
| 16:06:35 (t = 21,515 s) | BS_ALL_OFF mit Last | sechs Steuerframes 0x100–0x104 mit allen ENABLE = 0 innerhalb 1 ms |
| ab t = 21,54 s | | Zellen 1, 2, 3, 7, 12, 13 bleiben auf 3,71 / 4,54 / 4,84 / 4,51 / 4,49 / 3,83 V, die übrigen acht fallen auf ≈ 0,02 V |
| 16:06:58 (t ≈ 46 s) | BS_OUT_OFF (Duty 0, Ausgang aus) | die sechs Werte ändern sich nicht |
| bis t = 363 s | | unverändert (3,71 / 4,53 / 4,80 / 4,52 / 4,48 / 3,83 V), kein FAIL, kein Overload/Limit |

Der Nutzer misst an der Last **28 V** (echt, Summe der Meldungen 26,07 V).

**Zeitverlauf der Meldung (Firmware-Tiefpass τ ≈ 100 ms):** gute Zellen fallen mit Faktor 0,36 je 100 ms (3,70 → 3,15 → 1,14 → 0,41 → 0,16 → 0,07 → 0,03 V); die sechs betroffenen
Werte laufen mit derselben Zeitkonstante zu ihrem Endwert (Zelle 2: 3,70 → 3,92 → 4,36 → 4,48 → 4,53 → 4,54 V). Der Rohwert springt also innerhalb ≲ 30 ms nach dem Befehl.

**48-V-Versorgungsstrom (0x121, vom Gerät gemessen):** Ruhe 0,242 A, alle Zellen an 0,312 A, nach ALL_OFF 0,296–0,302 A, nach OUT_OFF 0,275 A (dauerhaft über Ruhe).

Zelle 2 und 3 zeigen in beiden Ereignissen fast denselben Wert (4,54–4,56 / 4,83–4,84 V): kartenspezifisch, nicht zufällig. Die Menge der betroffenen Zellen wechselt zwischen den Läufen
(Erstfund 4 und 13, Nachstellung 1, 2, 3, 7, 12, 13). Einzelne Zellen ausschalten funktioniert, „alle aus“ ohne Last funktioniert (Nutzer).

## 2. Was die Quellen sagen (BattSim_V1, eine Karte = ein Kanal)

* Ein per ENABLE inaktiver Kanal ist **gebrückt**, nicht hochohmig: `ENABLE_ISO` (Rückwand c10) → R47 1k → Q9 (invertiert, Pull-up R48 10k an +5V) → ISO6742 INB → OUTB = ENABLE (Zellseite, TP10)
  → R27 1k → Q5. Aus: Q5 sperrt, `Net-(Q5-C)` liegt über R24 10k auf +5VA; darüber zieht Q4 (R23) den SET-Knoten EN_SW auf ≈ 0 V und Q6 (AO3400A, Gate über R28) schließt
  V_OUT+ über R25 (0 Ω) auf GND (= V_OUT−).
* Sollwert: PWM → ISO6742 OUTA → Filter (R11/C9, R10) → U5A (×1,5, +9V) → R7 1k → EN_SW → R8 100 Ω → SET des LT3080. LT3080: IN = Vregin = +9V − R1 (1 Ω), VCONTROL = +9V, Ausgang folgt SET,
  nur Quelle (aus Erinnerung). Herunterziehen von SET: Q4 (ENABLE) und Q1 (Strombegrenzung).
* Je Karte eigener +5VA: 48 V → L1/C2 → U2 SI06W8-09 (isoliert, 6 W, 9 V) → +9V → U8 NCP1117-5.0 → +5VA. +5VA versorgt VCC2 des ISO6742, VDD1 des AMC1311 und den Pull-up R24 (Gate von Q6 und Basis von Q4).
* Firmware: `CellChannels` lässt den Sollwert-PWM bei `disabled` weiterlaufen (Duty 0 nur bei OutputEnable aus oder Sollwert 0); die Abschaltung hängt hardwareseitig an Q4/Q6. `CELLn_STATUS` folgt dem
  **Befehl**, nicht dem Pin (`disabled` sagt nichts über den Zustand von `EN_nn`). `verify: true` in `board.yaml` ist ein Pin-Bestätigungsvermerk, keine Laufzeit-Rückmessung.
* ADC-Umrechnung: `VoltageSenseMeas_Ax` 0 … 1996 mV ↔ 0 … 5 V (0,399 V/V, über 5 V linear extrapoliert); 4,54 V ↔ 1,81 V, 4,84 V ↔ 1,93 V am ADC-Pin (Referenz 2,048 V): mittlere Pegel, keine Anschläge.

## 3. Bewertung

Ausgeschlossen oder unwahrscheinlich:

* *Abgeschaltete Zelle ist hochohmig und schwimmt:* laut Schaltplan gebrückt; zwölf bzw. acht Karten zeigen im selben Zustand sauber ≈ 0 V.
* *Dauerhafter +5VA-Einbruch:* der AMC1311 hängt an derselben +5VA und meldet plausibel (Summe 26,07 V gegen 28 V am Messgerät, ≈ −7 %); Zelle 4/13 lagen im Erstfund auf 1 mV genau am Sollwert.
* *Fail-safe des AMC1311 (aus Erinnerung negative Differenz bei fehlender VDD1):* würde über U11 auf den unteren Anschlag laufen (≈ 0 V) und auf allen betroffenen Karten gleich aussehen; gemessen sind 3,7 … 4,84 V,
  je Karte verschieden, und die Spannung ist am Gerät echt.
* *Reines Messartefakt:* nein, 28 V an der Last.

Verbleibende Erklärungen (nicht unterschieden):

* **A (Karten speisen aktiv):** Auf den sechs Karten wirkt weder ENABLE (OUTB) noch Duty 0 (OUTA): Q4/Q6 bleiben aus, SET bleibt auf einem kartenspezifischen Pegel (Ladung der Filterkondensatoren, wenn OUTA nicht mehr
  treibt, oder verklemmter Isolator nach Masse-/CMTI-Impuls). Passt zur Zusatzleistung am 48-V-Strom (≈ 1,6–2,6 W ≈ 40–60 mA bei 28 V, Lastwiderstand ≈ 500–700 Ω).
* **B (Brücke geöffnet, Spannung von außen):** Der Entladestoß beim gleichzeitigen Brücken aller Zellen mit Last (bei 52 V durch 14 × (Q6 ≈ 30 mΩ + R25 0 Ω): Spitze in der Größe von 100 A, AO3400A nach Erinnerung
  ID ≈ 5,7 A, gepulst ≈ 30 A; R25 ist ein 0-Ω-0805) hat R25 bzw. Q6 auf einigen Karten geöffnet; die Restspannung der Last verteilt sich auf diese sechs Karten. Dagegen spricht der dauerhaft erhöhte 48-V-Strom.

## 4. Hardware-Befunde für die nächste Revision

1. Q9 hat keinen Pull-down an der Basis und R48 zieht INB auf +5V: ein offener oder nicht getriebener `ENABLE_ISO` schaltet den Kanal **ein** (fail-unsafe). Sicherer Zustand hängt am aktiv getriebenen High des Masters
   (ISO6742F, Ruhepegel low, gilt nur bei unversorgter Eingangsseite; passt zu den Resetpulsen vom 22.10.).
2. Q6 + R25 (0 Ω) brücken ohne Strombegrenzung (schon B2 der Zellkanal-Analyse): R25 als belastbarer Widerstand (1–2 Ω) oder Reihenfolge Q4 vor Q6; Impulsfestigkeit von Q6 prüfen.
3. Es gibt keine Rückmeldung des tatsächlichen ENABLE-/Brückenzustands der Karte an den Master; ein zweiter, vom ENABLE-Pfad unabhängiger Abschaltweg fehlt (Firmware-Duty 0 hilft nicht, wenn OUTA nicht treibt).

## 5. Offene Messungen (am geöffneten Gerät, Nutzer entscheidet im Board `bs-fehlerzustand`)

* Fehlerzustand halten: V_OUT+ (TP1) jeder betroffenen Karte mit Fluke, Strom und Spannung der Last, SET-Knoten (U1 Pin 2), +5VA (TP9), +9V (TP8), TP10, Q6-Gate (`LINK_SW`), `ENABLE_ISO`/R47.1 gegen GNDPWR,
  OUTA/OUTB (U6 Pin 14/13) gegen Karten-GND.
* Danach ohne Last, Simulator aus, Karten gezogen: R25 (soll 0 Ω), Q6 Drain-Source, Q4/Q5 Kollektor-Emitter der sechs betroffenen Karten (1, 2, 3, 7, 12, 13) gegen acht gute; Kartentausch (folgt der Fehler der Karte oder dem Steckplatz?).
* Tests mit Last erst nach Prüfung/Austausch von R25 und Q6 und nur strombegrenzt wiederholen.
* Bis zur Klärung kein BCU-Board/MC33771C und keine speichernde Last am Simulator.

Werkzeuge der Auswertung: `capture_alle_aus.py` (passiver Mitschnitt, sendet nichts), `analyse_mitschnitt.py`, Netzliste per `kicad-cli` (Scratchpad der Session); der Rohmitschnitt liegt nicht im Repository (≈ 1,3 MB).
