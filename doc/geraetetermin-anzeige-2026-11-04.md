# Geraetetermin 04.11.2026 (abends): Anzeige mit SCL-Pull-up, Firmware 0.5.1

Board `bs-anzeige-geraetetest` a, Quelle: Zweig `pin-io125` f29027e (Pin neXtBSW 23642a8d, IO-125). Aufbau und Ja zum Flash vom Nutzer im Chat. Kein Prueflinge/Last an den Zellklemmen (AoU S-1), CANcaseXL Kanal 2 (`vector:59177:1`), LabControl lief und sendete nichts.
Eval Board, Motor und 60V-Netzteil nicht angefasst.

| Schritt | Ergebnis |
|---|---|
| Vorabpruefung | Standby, Ausgang aus, 0.5.0, Rechnerlast 15..45 %; Abbild SHA-256 250f62ee... und CRC 0x47B4794E gegengeprueft |
| `canflash info` | Anwendung aktiv, Bootloader 0.3.14 |
| Flash 0.5.1 per CAN | 227928 Byte, Check OK (CRC-32 0x3AF3035C), ECU Reset; 0x121 meldet **0.5.1**, Standby, Ausgang aus, kein FAIL |
| ALL ON/OFF (Handbetrieb) | STANDALONE ("Handbetrieb"), Ausgang ein, 14 Zellen bei 0,69..0,71 V (Boden der Endstufe), Sollwert 0 V, kein FAIL, Kanal-LEDs an |
| Anzeige | **dunkel** (Beobachtung des Nutzers, Handbetrieb bestaetigt) |

Abbruch nach der Vorgabe des Koordinators: bei dunkler Anzeige kein Abziehen/Anstecken, keine Bus-Clear-Pruefung.

## Befund

Die Anzeige bleibt auch mit internem SCL-Pull-up (PE16 ALT6|PULLUP, wie die Werksfirmware) bei 31,25 kHz dunkel. Moegliche Ursachen (nicht unterschieden, ohne Messung am Bus/J-Link):
1. Rate: die Werksfirmware faehrt 9600 Baud; das Modul akzeptiert 31,25 kHz eventuell nicht (Variante 2, 9600 Hz, `scratchpad/variante2-9600hz.patch`, braucht einen langsameren Task und damit `rte.yaml`).
2. Adresse/Protokoll des Moduls (0x71 aus der Analyse der Werksfirmware, `verify: true` am Geraet nie bestaetigt).
3. SDA ohne Pull-up (das Modul hat womoeglich keine) oder Pinzuordnung SDA/SCL an J1.
Der I2C-Fehlerzaehler (`i2c.DISPLAY.write.error`) ist ohne J-Link nicht lesbar und wurde nicht gelesen.
