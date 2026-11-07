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

Der Nutzer misst an der Last **28 V** (echt, Summe der Meldungen 26,07 V). Die Last war ein **Gerät mit Eingangskondensatoren** (Angabe des Nutzers, Kapazität unbekannt); sie war beim Erstfund
nicht, bei der Nachstellung beim zweiten Versuch angeschlossen. Das stützt den **Entladestoß beim gleichzeitigen Brücken aller Zellen** (Q6 + R25 ohne Strombegrenzung, siehe Abschnitt 3 und 4) als Auslöser; die Kapazität
ist bisher weder bekannt noch gemessen, und ob R25/Q6 beschädigt sind, ist nicht geprüft.

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

1. Q9 hat keinen Pull-**up** an der Basis (07.11. korrigiert, vorher fälschlich „Pull-down“; EN_nn ist active low) und R48 zieht INB auf +5V: ein offener oder nicht getriebener `ENABLE_ISO` sperrt Q9 und schaltet den Kanal **ein** (fail-unsafe). Abhilfe: Pull-up von der Basis (R47.2) nach +5V, 47 kΩ (22–100 kΩ; Basis bei Low des Masters ≈ V_OL + 0,1 V, bei offen I_B ≈ 90 µA). Sicherer Zustand hängt am aktiv getriebenen High des Masters
   (ISO6742F, Ruhepegel low, gilt nur bei unversorgter Eingangsseite; passt zu den Resetpulsen vom 22.10.).
2. Q6 + R25 (0 Ω) brücken ohne Strombegrenzung (schon B2 der Zellkanal-Analyse): R25 als belastbarer Widerstand (1–2 Ω) oder Reihenfolge Q4 vor Q6; Impulsfestigkeit von Q6 prüfen.
3. Es gibt keine Rückmeldung des tatsächlichen ENABLE-/Brückenzustands der Karte an den Master; ein zweiter, vom ENABLE-Pfad unabhängiger Abschaltweg fehlt (Firmware-Duty 0 hilft nicht, wenn OUTA nicht treibt).

## 5. Offene Messungen (am geöffneten Gerät, Nutzer entscheidet im Board `bs-fehlerzustand`)

* Fehlerzustand halten: V_OUT+ (TP1) jeder betroffenen Karte mit Fluke, Strom und Spannung der Last, SET-Knoten (U1 Pin 2), +5VA (TP9), +9V (TP8), TP10, Q6-Gate (`LINK_SW`), `ENABLE_ISO`/R47.1 gegen GNDPWR,
  OUTA/OUTB (U6 Pin 14/13) gegen Karten-GND.
* Danach ohne Last, Simulator aus, Karten gezogen: R25 (soll 0 Ω), Q6 Drain-Source, Q4/Q5 Kollektor-Emitter der sechs betroffenen Karten (1, 2, 3, 7, 12, 13) gegen acht gute; Kartentausch (folgt der Fehler der Karte oder dem Steckplatz?).
* Tests mit Last erst nach Prüfung/Austausch von R25 und Q6 und nur strombegrenzt wiederholen.
* Bis zur Klärung kein BCU-Board/MC33771C und keine speichernde Last am Simulator.

## 6. Nachtrag: ohne Last, nach Aus/Ein (Nutzer, über den Koordinator)

Der Nutzer hat die Last abgeklemmt und den Simulator netzseitig aus- und wieder eingeschaltet. Ohne Last misst er am Stapelausgang (Zellen selbst über LabControl geschaltet): **alle ein 52 V, alle aus 0,3 V**
(≈ 21 mV je Karte, wie die Meldungen der guten Karten). `bs-fehlerzustand` ist damit erledigt (Fehlerzustand nicht gehalten).

Bewertung: spricht für A (Steuerseite der Karten verklemmt, durch Aus/Ein gelöst). Es beweist **nicht**, dass R25 und Q6 intakt sind: ohne Last bringt Q4 allein (SET → 0) die Zelle auf ≈ 0 V.

Vorschläge, Q6/R25 je Karte zu prüfen, ohne das Gerät zu öffnen (noch nicht ausgeführt, Freigabe über das Board):

* **Test 1, Stapelwiderstand bei „alle aus“ (kein Strom aus dem Simulator):** Simulator eingeschaltet, Ausgang ein, alle Zellen aus (Karten versorgt, Q6 leitet), Multimeter (Widerstand) an den Stapelklemmen
  Zelle 1− / Zelle 14+, Plus an Zelle 14+. Intakte Brücken: 14 × (Q6 ≈ 30 mΩ + R25) plus Zuleitungen, also einige 100 mΩ (Relativmessung/Vierleiter, Zuleitung abziehen). Ist bei einer Karte R25 oder Q6 offen, sperrt dort
  die Body-Diode von Q6 (Strom Drain → Source) und es bleibt der Messteiler (25 kΩ) parallel: Anzeige ≥ 10 kΩ. Findet nur, DASS mindestens eine Karte nicht brückt, nicht welche.
* **Test 2, welche Karte (lastarm, gestuft):** Stapel mit allen Zellen ein bei niedriger Spannung (z. B. je 1,0 V), rein ohmsche oder CC-Last mit kleinem Strom (0,1–0,2 A, **ohne Kondensatorbank am Eingang**, nicht die Last,
  die den Stoß ausgelöst hat). Danach EINE Karte k nach der anderen mit `BS_CELL_OFF` ausschalten (Ausgang bleibt ein, nie „alle aus“) und den 48-V-Versorgungsstrom (0x121) sowie die Meldung der Karte k lesen.
  Begründung: Eine eingeschaltete Karte nimmt den Laststrom I über ihren LT3080 aus der 9-V-Schiene (P = 9 V · I, unabhängig von ihrer Ausgangsspannung). Bei intaktem Q6/R25 übernimmt die Brücke den Strom: der 48-V-Strom
  fällt um ≈ 9 V · I / (48 V · η) (η ≈ 0,8; bei 0,2 A ≈ −47 mA), die Meldung von Karte k ≈ 0,02 V. Ist die Brücke offen, muss der Strom weiter durch den LT3080 (SET ≈ 0 V durch Q4): der 48-V-Strom ändert sich
  kaum (≈ 0), die Meldung bleibt ≈ 0,02 V, und die Karte setzt die Leistung 9 V · I in Wärme um (LT3080 ohne Kühlkörper, Befund B6; bei 0,2 A ≈ 1,8 W). Die acht unauffälligen Karten dienen als Vergleich.
* Beide Tests: keine Kondensatorlast, kein „alle aus“ unter Strom, Strom klein halten (der DC/DC liefert nur etwa 0,6 A), erst nach Freigabe im Board.

## 7. Kartenprüfung mit 100 Ω (16:25–16:31, Board `bs-karten-pruefung` a): die Brücke trägt den Strom bei KEINER Karte

Aufbau: 100 Ω ohmsch am Stapelausgang, ohne Kondensatoren, alle Zellen 1,0 V, Ausgang ein; Karte k = 1 … 14 einzeln mit `BS_CELL_OFF` aus (≈ 8 s), wieder ein (≈ 7 s), nie „alle aus“ unter Strom.
Mitschnitt `karten_pruefung_mitschnitt.txt` (Beginn 16:25:22), Auswertung `segmente_stromauswertung.py` (Mittelwerte nach 1,5 s Einschwingen, ≥ 3 s je Schritt).

| Zustand | 48-V-Strom (0x121) | Stapelspannung / Meldungen |
|---|---|---|
| alle 14 ein | 0,721 A | 14,0 V (je 1,00 V), Laststrom ≈ 0,14 A |
| Karte k aus (k = 1 … 14) | 0,690–0,692 A, **ΔI = −28 … −31 mA bei jeder der 14 Karten** | Karte k 0,02–0,04 V, übrige 1,00 V |
| Karte k wieder ein | 0,719–0,720 A, ΔI = +28 … +30 mA | alle 1,00 V |
| alle 14 aus (Ende, ohmsch) | 0,258 A | 0,28 V |

**Modell:** Eine eingeschaltete Karte nimmt den Laststrom I über den LT3080 aus der 9-V-Schiene (P = 9 V · I). Aus „alle ein“ gegen „alle aus“ folgt η(48 V → 9 V) ≈ 0,79. Schaltet man eine Karte aus,
sinkt der Strom im Stapel von 0,140 auf 0,130 A (13 V an 100 Ω); die 13 übrigen Karten sparen dadurch 13 · 9 V · 10 mA.

* Trägt die **Brücke** den Strom (Q6 leitet, LT3080 stromlos): erwartet ΔI48 ≈ **−64 mA**.
* Fließt der Strom **weiter durch den LT3080** der ausgeschalteten Karte (SET ≈ 0 V durch Q4, Brücke wirkungslos): erwartet ≈ **−33 mA**.
* Gemessen: −28 … −31 mA, bei **allen 14 Karten gleich**, auch bei nie auffälligen (5, 6). Das entspricht „Strom läuft weiter durch den LT3080“, nicht „Brücke trägt“. Die Gleichheit schließt 14 zufällig offene
  R25/Q6 praktisch aus; es handelt sich um eine Eigenschaft der Schaltung.

**Ursache aus dem Schaltplan (Netzliste, Produktions-BOM bestätigt R24 10k, R23/R28 1k, Q4/Q5 BC817, Q6 AO3400A, R25 0 Ω):** Das Gate von Q6 hängt über R28 am Knoten `Net-(Q5-C)`. Bei gesperrtem Q5 (Kanal aus) speist R24
(10k an +5VA) diesen Knoten, und R23 (1k) führt dessen Strom in die **Basis von Q4**. Die Basis-Emitter-Strecke von Q4 klemmt den Knoten auf V ≈ (5 V · 1k + V_BE · 10k)/11k = **1,0 … 1,1 V** (V_BE 0,60–0,70 V,
Basisstrom ≈ 0,4 mA). Q6 bekommt also im Aus-Zustand nur **V_GS ≈ 1,0 … 1,1 V**; der AO3400A hat nach Erinnerung (Datenblatt nicht im Workspace) V_GS(th) typ. ≈ 1,05 V (0,65 … 1,45 V) und seine garantierten
≈ 30 mΩ erst ab V_GS ≈ 2,5 V. Die Brücke ist damit schwach bis kaum leitend; die Annahme „Q6 schließt den Ausgang mit ≈ 30 mΩ“ (Doku BS-2, B2) gilt nicht. Die ausgeschaltete Karte arbeitet als Durchlassglied: der LT3080 hält den
Ausgang auf ≈ 0 V (Meldung 0,02–0,04 V, entspricht dem Nullpunkt-Offset der Messkette) und setzt 9 V · I in Wärme um, ohne Kühlkörper und aus dem 6-W-Wandler (≈ 0,6 A); bei 0,14 A sind das ≈ 1,2 W je ausgeschaltete Karte.

Folgen (zum Teil Vermutung):

* Mit Strom im Stapel (ohmsche Last, Prüfling) bleibt eine ausgeschaltete Karte aktiv in der Strombahn. Bei Strömen über ≈ 0,5 A heizt jede ausgeschaltete Karte mit ≈ 4,5 W bzw. begrenzt der Wandler.
* **Rückspeisung bei Kondensatorlast (Vermutung, nicht gemessen):** Entlädt sich eine Last mit Eingangskondensatoren beim Ausschalten in Rückwärtsrichtung durch die Stapelkarten, hat die Brücke keinen niederohmigen Weg; der Strom
  nimmt den Weg über D1 (V_OUT+ → Vregin) in die 9-V-Schiene und lädt sie über ihre normalen Werte (Grenzen von NCP1117, U3/U5, U2 im Workspace nicht belegt). Das würde zum Zustand „Steuerseite wirkungslos, Spannungen
  festgeklemmt“ nach dem Stoß passen und ersetzt die Annahme geöffneter R25/Q6 (diese bleibt ungeprüft).

Messung zur Bestätigung (Nutzer, geöffnet): Gate von Q6 (`LINK_SW`, R28.2 gegen Karten-GND) bei ausgeschalteter Karte, erwartet ≈ 1,0 … 1,1 V statt ≈ 5 V; danach Q6-Spannung über der ausgeschalteten Karte bei kleinem Strom (Millivolt-Bereich).

Für die nächste Revision: Gate-Treiber von Q6 vom Basisknoten von Q4 trennen (eigene Pull-up/Treiberstufe, V_GS ≥ 4,5 V, z. B. Q5 schaltet zwei getrennte Knoten oder ein zweiter Transistor), R25 mit Strombegrenzung,
und den bekannten Befund aus Abschnitt 4 (Q9 ohne Pull-up an der Basis) beheben.

Werkzeuge der Auswertung: `capture_alle_aus.py` (passiver Mitschnitt, sendet nichts), `analyse_mitschnitt.py`, Netzliste per `kicad-cli` (Scratchpad der Session); der Rohmitschnitt liegt nicht im Repository (≈ 1,3 MB).

## 8. Rework R23 = 22 kΩ und Folgerungen (07.11.2026, Firmware 0.5.6)

**Rework (Nutzer):** R23 (1 kΩ, Basis von Q4) auf 22 kΩ; Karte 1 zuerst, danach Karten 1 bis 5. Gemessen auf dem Tisch (Nutzer): Gate von Q6 bei ausgeschalteter
Karte ≈ 3,6 V (unverändert: 1,0 V, Drain-Source dort 300 kΩ). Q6 bei von außen getriebenem Gate (Karte unbestromt, mehrere Karten): **0,1 Ω bei 3 V**, Q6 ist also in Ordnung
und hat eine normale Schwelle. Eine Widerstandsmessung an der bestromten Karte (14 Ω) war ungültig (Messartefakt, nicht verwerten).

**Gerätetest (100 Ω, alle Zellen 1,0 V, Mitschnitt `rework_k1.txt`):** Karte 1 (umgebaut) aus: 48-V-Strom 0,722 → 0,689 A, **−33 mA**; Karten 2 und 3 (unverändert): −32 mA, −32 mA; Meldung der
ausgeschalteten Karten 0,01 / 0,02 / 0,01 V. Kein Unterschied durch den Rework.

**Korrektur der Erwartung aus Abschnitt 7:** Die Vorhersage „Brücke trägt den Strom → −64 mA“ war falsch. Der LT3080 hält den Kartenausgang auf SET + Offset (≈ +0,01 … +0,04 V, wie gemeldet)
und kann nur Strom liefern. Der Stapelstrom (Vorwärtsrichtung) läuft deshalb **immer** durch den LT3080, egal wie gut Q6 leitet; Q6 könnte nur Kreisstrom aus dem LT3080 aufnehmen. Der Strom-Test sagt
nichts über Q6 aus; die Wärme der ausgeschalteten Karte (9 V · I) lässt sich mit Q6 nicht beheben, sondern nur durch Abschalten des LT3080 selbst (z. B. Versorgung des LT3080 trennen). Das ist
eine Einordnung aus der Schaltung, nicht gemessen.

**Wofür Q6 gebraucht wird (Vermutung):** Rückwärtsrichtung (Kondensatorlast entlädt in den Stapel zurück): der LT3080 nimmt keinen Strom auf, der Strom geht über Q6 (wenn es wirklich leitet, V_GS ≥ 2,5 V)
oder über D1 in die 9-V-Schiene. Das ist der Fehlerfall vom 06.11.; der Rework (Gate ≈ 3,6 V) wirkt dafür, ist dort aber nicht geprüft.

**Offen / vorgeschlagen (Board über den Koordinator):**
* Revisionsliste: R23 auf 22 kΩ (Gate-Treiber von Q4-Basis entkoppeln, besser getrennte Knoten), Pull-up 47 kΩ an der Basis von Q9 (Abschnitt 4.1), R25 strombegrenzend (1–2 Ω, Belastbarkeit prüfen),
  LT3080 bei ausgeschalteter Karte abschalten (Wärme), Rückmeldung des Brückenzustands.
* Rückspeisetest (Rückwärtsrichtung) mit Strombegrenzung (z. B. 0,1 A), ohne Kondensatoren, Zellen aus: fließt der Strom über Q6 (Zelle zeigt wenige mV) oder über D1 in die Schiene (Zelle zeigt Volt)?
  Nur nach Freigabe im Board; gleicher Fehlermodus wie am 06.11.
* Karten 1 bis 5 sind auf 22 kΩ umgerüstet (Nutzer); Test danach nach Absprache.
