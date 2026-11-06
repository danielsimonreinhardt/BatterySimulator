# Sollwert-PWM bei abgeschalteter Zelle (Firmware 0.5.6)

Board-Punkt bs-pwm-bei-aus, Entscheidung des Nutzers 06.11.2026: Variante a. Stand: umgesetzt und im SIL getestet, **nicht
auf dem Geraet** (Flashen und Geraetetest erst nach eigenem Board-Punkt und Ja des Nutzers je Schritt).

## Aenderung

Bis 0.5.5 lief die Sollwert-PWM (Tastgrad) einer Zelle weiter, solange das Ausgangsfreigabe-Signal des Geraets an war und der
Sollwert > 0 V, auch wenn die Zelle selbst nicht freigegeben war (CELLn_ENABLE ungleich 2, im Handbetrieb: Kartentaste
gesperrt). Abgeschaltet wurde nur ueber den ENABLE-Pfad.

Ab 0.5.6 gilt in CellChannels: Tastgrad ungleich 0 nur, wenn Ausgang frei **und** Zelle freigegeben **und** Sollwert > 0 V
(`commanded` im Modell). Eine nicht freigegebene Zelle hat sofort, im selben 10-ms-Schritt, Tastgrad 0.

```
Ausgang frei + CELLn_ENABLE = 2 + Sollwert > 0 V  ->  Tastgrad aus Kennlinie (+ Regler)
sonst                                              ->  Tastgrad 0
```

## Was es bringt und was nicht

- Zweite Abschaltung der Karte, **unabhaengig vom ENABLE-Pfad**: faellt der ENABLE-Pfad aus oder haengt er, liegt am
  Sollwert-Eingang der Karte trotzdem 0 V.
- Es hilft **nicht**, wenn die Steuerseite der Karte verklemmt ist (Befund bs-zelle-4-13, bs-hw-enable-failsafe): dann ist
  der Fehler hinter dem Isolator, den PWM-Tastgrad 0 nicht erreicht.
- Karte aus unter Last (Kartenpruefung 06.11., Doku `geraetetermin-zelle-4-13-2026-11-06.md`, Abschnitt 7): Bei ausgeschalteter
  Karte klemmt Q4 den Sollwert (EN_SW/SET) schon ueber den ENABLE-Pfad auf etwa 0 V, der LT3080 haelt die Zelle auf etwa 0 V
  und setzt 9 V * I in Waerme um, weil Q6 (Bruecke) nur etwa 1 V Gate-Spannung bekommt. Der Sollwert-Pfad aendert daran nach dem
  Modell nichts. **Erwartung am Geraet: Kartenheizung unveraendert**, d. h. der 48-V-Strom faellt beim Ausschalten einer Karte
  weiter um nur 28 bis 31 mA (Messung 0.5.5). Das ist ein Pruefkriterium fuer den Geraetetest, keine Verbesserung. Aenderung
  nur durch die Hardware (Gate-Treiber von Q6 trennen).
- Eine kleine Entlastung bleibt: bei Tastgrad 0 fliesst im Sollwert-Pfad der abgeschalteten Karte (U5A -> R7 1 k -> Q4) kein
  Strom mehr (etwa 1,5 mA je Karte bei 1 V und 7 mA bei 5 V Sollwert; Rechnung, nicht gemessen).

## Einschalten danach

Beim Freigeben springt der Tastgrad im ersten Schritt auf den Kennlinienwert (der Regler kommt von null, Korrektur hoechstens
+0,1 V). Auf der Karte kommt der Sollwert dann ueber den RC-Filter (R11/C9: 10 kOhm/1 uF, 10 ms) und den Verstaerker statt schon
anzuliegen: der Anstieg ist um einige 10 ms langsamer. Der Regler und der Ueberspannungsschutz sind davon nicht betroffen
(Schwelle 0,2 V ueber Sollwert, Istwert-Tiefpass 95 ms).

SIL (Strecke mit 30 ms PWM-Filter, 3,7 V, Verstaerkungsfehler 0,99, Offset -0,02 V, Regler an):
Wiedereinschalten nach Aus, Spitze **3,731 V** (Ueberspannungsschwelle 3,9 V), eingeschwungen 3,701 V, kein FAIL.

## Test und Mutationsbeweis

- `projects/battsim_master/test/battsim_sil_test.c`: Erwartungen fuer gesperrte Kanaele auf Tastgrad 0 (CAN: Zelle 2 mit
  Sollwert, CELL02_ENABLE = 0; CELL01_ENABLE = 1 als "disable"; Handbetrieb: Kartentaste sperrt Kanal 2); neu
  `aChannelWithoutReleaseHasNoPwm` (Freigabe weg: Tastgrad 0 im selben Schritt, Spannung faellt auf 0 V; Freigabe wieder da:
  Tastgrad im ersten Schritt zwischen Kennlinie und +0,1 V, Spitze unter Schwelle, eingeschwungen auf 4 mV; Regler aus: genau der
  Kennlinienwert; Ausgang aus: alle Tastgrade 0). Version 0.5.6 in den Pruefungen der Statusbotschaft.
- Ergebnis: battsim-sil 1056 Pruefungen, 0 Fehler; battsim-sil-hand 1582, 0 Fehler; gesamt `python -m test --workspace .`: 9 gruen,
  2862 Testfaelle.
- Mutationsbeweis: im erzeugten Code die Bedingung zurueck auf `v > 0` (Stand 0.5.5): 34 Fehler, u. a. Zelle 2 mit Tastgrad
  ungleich 0 trotz fehlender Freigabe (`battsim_sil_test.c:252`, `:281`), danach `outputsOff()` an vielen Stellen, im
  Handbetrieb `:1674`. Wiederhergestellt, 0 Fehler.
- Firmware gebaut (`cmake --build --preset battsim-master`): ROM-CRC 0x688738F3, SHA-256 der Hex `955fee69...6926`. Nicht geflasht.

## Vorschlag Geraetetermin (noch kein Board-Punkt)

1. Aufbau wie bei der Kartenpruefung (100 Ohm ohmsch, alle Zellen 1,0 V), Flashen 0.5.6 per CAN, Aus/Ein.
2. Karte einzeln aus/ein (alle 14 wie am 06.11.): 48-V-Stromsprung, erwartet unveraendert 28 bis 31 mA.
3. Einschalten einer Zelle mit LabControl-Oszilloskop am Ausgang: Anstieg, Spitze, Einschwingen; Vergleich mit 0.5.5-Aufnahme,
   falls vorhanden.
4. Handbetrieb: Kartentaste sperren/freigeben, Anzeige und Spannung.
