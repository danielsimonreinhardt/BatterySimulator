# libs/

Eigene Module dieses Repositorys (`<modul>/module.yaml`): Geraetetreiber und Dienste, die nur diese ECU hat.

Vor `libs/bsw` und `libs/bsp` des Baums durchsucht; ein Modulname, den es zweimal gibt, ist ein Fehler mit beiden Pfaden.

Gefunden wird der Ordner ueber `search.modules` in `nextbsw.yaml`; deshalb muss es ihn geben, auch wenn er noch leer ist. Diese Datei haelt ihn in git.
