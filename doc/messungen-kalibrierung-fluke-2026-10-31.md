# Prüfung und Kalibrierung mit dem Fluke 175 (31.10.2026, Firmware 0.3.4)

Gerät: Master V1, Chip-UID 000effffffffffff4e535161400e001b. Aufbau: kein Prüfling, keine Last, Fluke 175 (DC, 6-V-Bereich) an den Zellklemmen, J-Link und CANcaseXL
Kanal 2 angeschlossen. Regler aus (`2E 0211 01`), 30 min Aufwärmen, Sollwert aller Zellen 1,0 V und 4,2 V (4,5 V geht nicht: der LabControl-Grenzwert je Zelle ist 4,30 V,
Entscheidung des Nutzers im Board `bs-fluke-42v`). ADC = Mittel aus 30 Proben im Abstand von 0,5 s (Streuung 1 bis 2 mV). Das Fluke wurde vom Nutzer abgelesen (Auflösung 1 mV;
die Herstellerangabe für den Bereich liegt bei etwa ±(0,15 % + 2 Stellen), das sind etwa ±3,5 mV bei 1 V und ±8 mV bei 4,2 V).

## 1. Flash und Erhalt der Kalibrierung (RTE-56)

* 0.3.3 vorher gesichert (`pflash_033_vor_034_0x00000000_1M.bin`, gleich dem 0.3.3-Hex). 0.3.4 (Commit `ed96fef`) per J-Link mit 1000 kHz, nur App und Record. Rückgelesen:
  Bootloader, App und Record ohne Abweichung, Schlüsselsektor und FCF unverändert. Firmware meldet 0.3.4; `check_battsim --hardware` (nur lesen) ohne Befund.
* Erhalt: vor dem Flash Kanal 1 mit `0064 2774 0000 2710` (Zustand 1) beschrieben, nach dem Flash gelesen: **gleiche Werte, Zustand 1**. Danach Werkswerte zurückgeschrieben.
  Damit bleiben gespeicherte Werte aus 0.3.3 nach dem Wechsel auf 0.3.4 gültig (die offene Frage aus Abschnitt 15 der Sicherheitshinweise).

## 2. Messung bei Regler aus, Werkskalibrierung

Fehler = ADC minus Fluke (Messpfad), Ist−Soll = Fluke minus Sollwert (Stellpfad, ohne Regler).

| Zelle | Fluke 1,0 V | ADC 1,0 V | Fehler 1,0 V | Fluke 4,2 V | ADC 4,2 V | Fehler 4,2 V | Ist−Soll 1,0 / 4,2 V |
|---|---|---|---|---|---|---|---|
| 1 | 0,964 | 0,9795 | +15,5 mV | 4,201 | 4,2102 | +9,2 mV | −36 / +1 mV |
| 2 | 0,960 | 0,9570 | −3,0 | 4,183 | 4,1630 | −20,0 | −40 / −17 |
| 3 | 0,963 | 0,9814 | +18,4 | 4,203 | 4,2340 | +31,0 | −37 / +3 |
| 4 | 0,972 | 0,9725 | +0,5 | 4,228 | 4,2203 | −7,7 | −28 / +28 |
| 5 | 0,975 | 0,9976 | +22,6 | 4,249 | 4,3033 | +54,3 | −25 / +49 |
| 6 | 0,966 | 0,9683 | +2,3 | 4,224 | 4,2219 | −2,1 | −34 / +24 |
| 7 | 0,975 | 0,9830 | +8,0 | 4,234 | 4,2277 | −6,3 | −25 / +34 |
| 8 | 0,965 | 0,9646 | −0,4 | 4,203 | 4,1967 | −6,3 | −35 / +3 |
| 9 | 0,967 | 0,9714 | +4,4 | 4,193 | 4,1988 | +5,8 | −33 / −7 |
| 10 | 0,972 | 0,9674 | −4,6 | 4,240 | 4,2638 | +23,8 | −28 / +40 |
| 11 | 0,972 | 0,9917 | +19,7 | 4,234 | 4,2562 | +22,2 | −28 / +34 |
| 12 | 0,967 | 0,9639 | −3,1 | 4,216 | 4,1974 | −18,6 | −33 / +16 |
| 13 | 0,967 | 0,9770 | +10,0 | 4,199 | 4,2058 | +6,8 | −33 / −1 |
| 14 | 0,969 | 0,9731 | +4,1 | 4,218 | 4,2408 | +22,8 | −31 / +18 |

## 3. Kalibrierung des Messpfads

Zwei-Punkt-Gerade `Fluke = MessSteigung * ADC + MessOffset` je Kanal. Kalibriert wurden die Kanäle mit mehr als etwa 10 mV Fehler am ADC bei 1,0 oder 4,2 V:
1, 2, 3, 5, 10, 11, 12, 14. Nicht kalibriert: 4, 6, 7, 8, 9 und 13 (13 grenzwertig, +10,0 und +6,8 mV, innerhalb der Fluke-Unsicherheit).
Geschrieben per UDS (`2E 0201`..`020E`, Stellpfad = Werkswert `0000 2710`), übernommen mit Routine 0x0200 (Ergebnis 0), Reset per `11 01`: alle Werte erhalten, Zustand 1.

| Zelle | MessOffset (0,1 mV) | MessSteigung (1e-4) | Draht (`MessOffset MessSteigung`) |
|---|---|---|---|
| 1 | −174 | 10019 | ff52 2723 |
| 2 | −21 | 10053 | ffeb 2745 |
| 3 | −146 | 9961 | ff6e 26e9 |
| 5 | −130 | 9904 | ff7e 26b0 |
| 10 | +129 | 9914 | 0081 26ba |
| 11 | −189 | 9992 | ff43 2708 |
| 12 | −15 | 10048 | fff1 2740 |
| 14 | +15 | 9943 | 000f 26d7 |

## 4. Gegenprobe (nach dem Reset, unabhängige Ablesung des Fluke an Zelle 5 und Zelle 10)

| Bedingung | Zelle 5 | Zelle 10 |
|---|---|---|
| Regler aus, 4,2 V (vorher ADC-Fehler +54,3 / +23,8 mV) | Fluke 4,249, ADC 4,2481 (−0,9 mV) | Fluke 4,240, ADC 4,2396 (−0,4 mV) |
| Regler aus, 1,0 V (vorher +22,6 / −4,6 mV) | Fluke 0,975, ADC 0,9750 (0,0 mV) | Fluke 0,972, ADC 0,9728 (+0,8 mV) |
| Regler an, 4,2 V (ADC aller 14 Zellen 4,1998..4,2005 V) | Fluke 4,200 | Fluke 4,200 |

Mit eingeschaltetem Regler (Standard nach dem Start) liegt die Ausgangsspannung damit bei beiden geprüften Zellen innerhalb der Ablesung (1 mV) auf dem Sollwert.

## 5. Offen (Stand nach Abschnitt 6)

* Der Stellpfad ist in Abschnitt 6 kalibriert (Board `bs-stellpfad-kalibrierung` a).
* Nur zwei Punkte (1,0 und 4,2 V) und nur zwei Kanäle in der Gegenprobe; keine Messung bei Last und keine Temperaturabhängigkeit.
* Beobachtung während der Aufwärmphase: Die Ausgänge fielen nach einem kurzen Ausbleiben von 0x130 (Status 14:29:38 für etwa 1,3 s weg) in den sicheren Zustand (FAIL-Grund 1, Timeout 0x130).
  Das Gerät reagierte wie vorgesehen; Wiederanlauf mit `BS_RESET_FAIL` und danach `BS_OUT_ON`. Auslöser wahrscheinlich Rechnerlast (ein Build einer anderen Sitzung), nicht nachgewiesen.

## 6. Kalibrierung des Stellpfads (Board `bs-stellpfad-kalibrierung` a, Abend des 31.10.)

Aus denselben Fluke-Messungen (Regler aus, Werks-Stellpfad): `Fluke = a * Soll + b` je Kanal, daraus `SetSteigung = 1/a`, `SetOffset = −b/a`. Das ist unabhängig vom Messpfad. Geschrieben für alle 14 Kanäle
(zusammen mit den Messpfad-Werten aus Abschnitt 3, die sechs unkalibrierten Kanäle mit Werks-Messwerten), Routine 0x0200 Ergebnis 0, Reset `11 01`, danach alle Werte erhalten (Zustand 1).

| Zelle | SetOffset (0,1 mV) | SetSteigung (1e-4) | Zelle | SetOffset (0,1 mV) | SetSteigung (1e-4) |
|---|---|---|---|---|---|
| 1 | +470 | 9886 | 8 | +463 | 9883 |
| 2 | +469 | 9929 | 9 | +408 | 9919 |
| 3 | +489 | 9877 | 10 | +482 | 9792 |
| 4 | +447 | 9828 | 11 | +465 | 9810 |
| 5 | +470 | 9774 | 12 | +476 | 9849 |
| 6 | +512 | 9822 | 13 | +426 | 9901 |
| 7 | +427 | 9819 | 14 | +456 | 9849 |

Gegenprobe (Regler aus, Fluke, Zelle 5 und Zelle 6, vorher Ist−Soll 1,0 V: −25 / −34 mV, 4,2 V: +49 / +24 mV):

| Sollwert | Zelle 5 | Zelle 6 |
|---|---|---|
| 1,0 V | Fluke 0,999 (ADC 0,9976) | Fluke 0,999 (ADC 1,0011) |
| 4,2 V | Fluke 4,196 (ADC 4,1962) | Fluke 4,197 (ADC 4,1990) |

Der Ausgang ohne Regler liegt damit an den beiden geprüften Zellen innerhalb von 1 mV (1,0 V) und 4 mV (4,2 V) auf dem Sollwert, im Bereich der Ablesegenauigkeit des Fluke. Die ADC-Werte aller 14 Zellen ohne Regler
bei 1,0 V lagen zwischen 0,9971 und 1,0091 V, bei 4,2 V zwischen 4,1906 und 4,2037 V. Zum Ausgang an den übrigen zwölf Zellen gibt es keine Fluke-Ablesung.

Während der ersten Gegenprobe fiel der Ausgang wieder in FAIL "Timeout 0x130" (Rechnerlast: zwei vergessene `find`-Läufer, einer davon aus dieser Sitzung); nach deren Ende blieb er stabil.
