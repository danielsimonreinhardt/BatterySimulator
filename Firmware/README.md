# BattSimFW

Steuergeraete-Repository mit neXtBSW als Untermodul in fester Version: **1.0.0** (Entwicklungsstand ohne Release-Tag).
Welche, steht in [`nextbsw.yaml`](nextbsw.yaml); der Aufbau ist `nextbsw/doc/architecture/customer-project.md`.

## Klonen

```sh
git clone --recurse-submodules <url dieses repositorys> BattSimFW
cd BattSimFW
python nextbsw/tools/setup venv .          # eigene .venv aus den Anforderungen des gepinnten Baums
```

## Bauen

```sh
cmake --preset battsim-master
cmake --build --preset battsim-master
```

Gebaut wird nach `build/battsim-master/`. Auf Windows zaehlt die Pfadlaenge: der laengste Objektpfad
eines Ziel-Baus misst 247 von 260 Zeichen, deshalb liegt dieses Repository moeglichst weit oben
im Dateisystem.

## Pruefen und erzeugen

```sh
.venv/Scripts/python nextbsw/tools/nxconfig check projects/battsim_master/project.yaml
.venv/Scripts/python nextbsw/tools/test --workspace .
.venv/Scripts/python nextbsw/tools/ci run --workspace .
```

## neXtBSW aktualisieren

Nie automatisch. Ein Umzug des Pins ist ein eigener Zweig mit Review: neuen Tag im Untermodul
auschecken, `nextbsw.version` anpassen, `nxconfig migrate .` laufen lassen, Bericht nach
`doc/nextbsw-updates/` schreiben (customer-project.md 5.5).
