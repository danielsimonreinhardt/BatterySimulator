# Gerätetermin 04.11.2026: Firmware 0.5.0 (Handbetrieb), Drehgeber-Pull-up, Anzeige

Board `termin-vorort-1104` c, Aufbau vom Nutzer bestätigt: CANcaseXL Kanal 2, J-Link am Master (nur einmal zum Lesen benutzt), **kein Prüfling, keine Last** an den Zellklemmen (AoU S-1), BCU aus. LabControl lief und
sendete nichts auf CAN. Bootloader 0.3.14 (FCF/FOPT 0x7F unverändert, `nmi: disabled` kommt erst mit Bootloader 0.3.15). Quelle: BattSimFW `main` 16b9430 (erster Flash), danach Zweig `fix-enc-pullup`
(zweiter Flash). Mensch am Gerät: Tasten und Drehgeber bedient der Nutzer, ich lese über LabControl (Statusbotschaften) und, einmal, über den J-Link.

## Ablauf und Ergebnis

| Schritt | Ergebnis |
|---|---|
| Vorabprüfung | Gerät Standby, Ausgang aus, 0.4.0; CPU-Last etwa 41 %; `canflash info`: Anwendung aktiv, Bootloader 0.3.14. |
| Flash 1: 0.5.0 (main 16b9430) per CAN | 227912 Byte, Check OK (CRC-32 0x33BD946C), ECU Reset. 0x121 meldet **0.5.0**. Kalibrierung 0x0201 (112 Byte) **unverändert** (gleich dem Stand vom 31.10.). |
| ALL ON/OFF in Standby | **STANDALONE** („Handbetrieb“), alle 14 Kanäle freigegeben, SELECT-LED nur Kanal 1, REMOTE-LED aus. Zellen bei etwa 0,69 V (Boden der Endstufe, kein Fehler). |
| Drehgeber (erster Stand, Board ohne `pull`) | 60 Rasten am Spannungs-Drehgeber **ohne Wirkung**, Kanal-Drehgeber ohne Wirkung; Karten-Taste wirkte (Kanal ein/aus). |
| J-Link, nur lesen (`60_enc.jlink`, Reset+Halt beim `connect`, danach `g`) | FTM2/FTM1 Quadraturdecoder aktiv (QDCTRL 0xC1, FILTER 0xAA), Zähler 10/5 **konstant**, RTE-Puffer gleich; **Pinpegel PTE4, PTE5, PTC6, PTC7 dauerhaft Low** (GPIO PDIR), PCR ohne Pull. Das Gerät stand danach wieder in Standby. |
| Auswertung der Werksfirmware (`BATTSIM_CONTROL.slx`, Sicherung vom 27.10.) | Die Drehgeber-Pins PTE4, PTE5, PTC6 (und PTC7) sind mit `PORT_INTERNAL_PULL_UP_ENABLED` konfiguriert, Abfrage per Flankeninterrupt (A fallend, B lesen). **Der Schaltplan zeigt zwar 10-kΩ-Pull-ups (R34..R37), die gebaute Platine liefert sie offenbar nicht.** Ursache des Fehlers: `pull: up` fehlte im Board. |
| Flash 2: Zweig `fix-enc-pullup` (`board.yaml`: `pull: up` an beiden Decodern) | 227928 Byte, Check OK (CRC-32 0xFB5BA852). Kalibrierung unverändert. |
| Drehgeber nach der Korrektur | **60 Rasten grob: Kanal 1 0,9165 V** (erwartet 0,90 V); **50 Rasten fein: +46 mV** (erwartet +50 mV). Richtung stimmt, 3 Rasten am Kanal-Drehgeber setzen die SELECT-LED von Kanal 1 auf Kanal 4. |
| Taste Spannungs-Drehgeber | Grob/Fein wirkt (15 mV/1 mV je Raste). Der Nutzer behält die Abstufung. |
| Taste Kanal-Drehgeber (Alle-Modus) | Alle SELECT-LEDs an; **alle 14 Kanäle gleich** (2,030 V ± 3 mV). |
| ALL ON/OFF im Handbetrieb | Standby, Ausgang aus, alle Zellen aus. Erneut: STANDALONE mit **0 V Sollwert auf allen Kanälen** (Zellen wieder bei 0,69 V) - **Befund F8 am Gerät bestätigt**. |
| Taste REMOTE im Handbetrieb | „Fernsteuerung“ (STOP), **Ausgang aus**, alle Zellen aus. Zurück nach Standby über LabControl `BS_REMOTE_ON` / `BS_REMOTE_OFF` (ohne Ausgänge). |
| Anzeige (I²C 0x71) | **Dunkel** (auch beim Einschalten des Handbetriebs). Siehe Befunde. |
| FAIL 5/6, Überlast im Handbetrieb | Am Gerät nicht prüfbar (braucht Fehlerinjektion oder Last); nur SIL (1042 + 299 Prüfungen). |

Endzustand: Standby, Ausgang aus, Fernsteuerung frei, Firmware 0.5.0 (Zweig `fix-enc-pullup`), Kalibrierung erhalten.

## Befunde

1. **Drehgeber ohne Pull-up (behoben im Zweig `fix-enc-pullup`, noch nicht in `main`).** Der Schaltplan zeigt Pull-ups, die gebaute Platine nicht; die Werksfirmware setzt sie im MCU. Board: `pull: up`. Die Annahme „Pull-up im Schaltplan,
   also kein `pull`“ in `doc/nextbsw-updates/v1.0.0-b14845b8-v1.0.0-4334fb2d.md` ist damit **überholt**: für die gebaute Platine gilt der MCU-Pull-up.
2. **Skalierung bestätigt:** grob 15 mV, fein 1 mV je Raste (im Rahmen des Zählens des Nutzers, Abweichung bis etwa 8 %, kein systematischer Fehler erkennbar). Keine Änderung.
3. **Anzeige dunkel, Ursache wahrscheinlich der Bus:** Die Werksfirmware setzt **nur an SCL (PTE16) einen internen Pull-up** (SDA ohne) und fährt den Bus mit **9600 Baud**. Der neue FlexIO-I²C-Treiber schaltet an den Bus-Pins keinen Pull-up ein und kann
   bei 16 MHz FlexIO-Takt nicht langsamer als 31,25 kHz (`nxconfig` lehnt 9600 Hz ab: untere Grenze 31250 Hz). Auf der Platine gibt es an PTE15/PTE16 keine Pull-ups (Schaltplan); ob das Modul welche hat, ist offen (der Nutzer kann im
   eingebauten Gerät nicht messen). **Lücke in neXtBSW** (IO-117-Nachtrag): Pull-up-Option für I²C-Pins und eine Rate unterhalb von 31,25 kHz (FlexIO-Takt oder anderer Teiler). Nicht in BattSimFW zu lösen.
4. Die Aussage „Anzeige per UART“ in `BatterieSimulator/doc/hardware-system.html` stammt aus den Netznamen `DISP_TX/DISP_RX`; das Modell nutzt `FlexIO_I2C_Master_Transmit` (I²C, Adresse 0x71 laut Analyse). Die Hardware-Doku sollte entsprechend korrigiert werden (Quellen sind nur zu lesen).
5. Nicht geprüft: Verhalten der Anzeige beim Abziehen und Wiederanstecken (error, nach 0,5 s neues Bild) und Bus-Clear nach Reset mitten im Bild (braucht eine funktionierende Anzeige); FAIL 5/6/3 im Handbetrieb am Gerät.

## Offen

* Zweig `fix-enc-pullup` nach `main` (Go des Koordinators).
* neXtBSW: Pull-up-Option für I²C-Pins und niedrigere SCL-Rate (Anzeige), danach Anzeige erneut am Gerät.
* Bootloader 0.3.15 mit FOPT 0x7B (`nmi: disabled`) im eigenen Termin.
