# Gerätetermin 01.11.2026: Firmware 0.4.0 erstmals über CAN, Kennung 0x0201, Rückweg per J-Link

Board `bs-termin-can-update` a (Safety-Review BL-47/48 BL48-3 a: CAN-Update mit Bootloader 0.3.14 und FCF 0x7F, J-Link bleibt als Rückweg), Aufbau vom Nutzer bestätigt: J-Link am Master, CANcaseXL
Kanal 2, **kein Prüfling, keine Last** an den Zellklemmen (AoU S-1 a, im Bootloader stehen die Zellausgänge auf etwa 5,25 V), Ausgänge aus, BCU-Board aus. LabControl lief, hatte die Fernsteuerung des
BattSim abgegeben (kein Senden von 0x130). Gerät: Chip-UID 000effffffffffff4e535161400e001b. Quelle: BattSimFW `main` f641a0a.

## Ablauf und Ergebnis

| Schritt | Ergebnis |
|---|---|
| 1. J-Link, nur lesen (1000 kHz) | `pflash_vor_can_0x0_1M.bin` ist **byteweise gleich** der Rücklesung von 0.3.4 vom 31.10.: Bootloader, FCF `…FE 7F FF FF`, Schlüsselsektor 0xFE000 unverändert. Das Gerät stand in FAIL "Timeout 0x130" (Rest einer LabControl-Sitzung, Ausgänge aus). |
| 2. `canflash info --bootloader` | Anwendung `nextbsw-app 1.0.0`, im Bootloader `nextbsw-bl 0.3.14`, Hardwarenummer `battsim_master_v1/S32K146`, Security HMAC-SHA256, Schlüssel eingerichtet, installierte Anwendung kompatibel. |
| 3. `canflash flash app.ecu.hex` (0.4.0) über CAN 0x7E1/0x7E9, Kanal 2 | Netz vorbereitet (10 03, 85 02, 28 03), 0x00008000..0x0003CBC0 gelöscht und geschrieben (216000 Byte, 18 KB/s), **Check OK (CRC-32 0x20B47ED9), Done, ECU reset**. 0x121 meldet **0.4.0**, Standby, Regler an, kein FAIL (FAIL durch den Reset gelöscht). |
| 4. J-Link zurücklesen | Bootloader, **App (215960 Byte) und Record ohne Abweichung** zum Hex, Schlüsselsektor und FCF unverändert (`pflash_040_per_can_0x0_1M.bin`). |
| 5. Kennung 0x0201 (`40_calvalues.py`) | `22 0201` liefert 112 Byte, **gleich der Kalibrierung vom 31.10.** (Messpfad 8 Kanäle, Stellpfad 14), Zustand 1. Unverändert zurückgeschrieben (`2E 0201` positiv) und `31 01 0200`: Ergebnis **3 (nichts Neues)**. Eine Steigung 0 in Kanal 7: **NRC 0x31**, benutzte Werte danach unverändert. |
| 6. Rückweg-Probe J-Link | `flash.jlink` (nur App und Record, kein Erase, kein Unlock, 1000 kHz): App "Contents already match", Record neu geschrieben (Fingerprint-Block). Rücklesung: App, Bootloader, FCF, Schlüsselsektor wie zuvor; gegenüber dem CAN-Flash weichen **nur 24 Byte im Fingerprint-Block des Records** (0xFF038..0xFF04F) ab. |

Endzustand: Standby, Ausgang aus, Fernsteuerung aus, Regler an, Firmware 0.4.0, Kalibrierung erhalten.

## Befunde

* Das CAN-Update braucht keinen J-Link und keinen Aus-/Einschaltvorgang (canflash geht selbst in den Bootloader und startet die Anwendung nach dem Flashen per Reset). Der J-Link blieb als Rückweg angeschlossen und wurde auch zum Lesen benutzt.
* Beim CAN-Flash war die Rechnerlast mäßig (21 bis 74 %, im Wesentlichen TGitCache); es gab keinen Abbruch.
* Die NvM-Blöcke der Kalibrierung blieben über 0.3.4 → 0.4.0 erhalten, die Diagnoseschnittstelle (Kennung 0x0201 mit 112 Byte statt 8 Byte für Kanal 1) ändert sich wie in Abschnitt 16 der Sicherheitshinweise beschrieben.
* Nicht geprüft: das Verhalten bei abgebrochener Übertragung (Bootloader bleibt aktiv, `flash … --wait 30` oder J-Link), kein Zellausgang im Bootloader gemessen.
