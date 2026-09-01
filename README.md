# ims.project.smartclock

## Inhaltsverzeichnis

- [Einleitung](#einleitung)
  - [Ausgangslage](#ausgangslage)
  - [Vision](#vision)
  - [Quick Start](#quick-start)
    - [Installation](#installation)
    - [Allgemeine Steuerung](#allgemeine-steuerung)
    - [Timer Steuerung](#timer-steuerung)
- [Weitere](#weitere)
  - [Links](#links)
  - [Quellen](#quellen)
  - [Kontaktdaten](#kontaktdaten)

## Einleitung

### Ausgangslage

In der IMS-T wurde uns gesagt das wir ein Projekt durchführen sollen. Da wusste ich schon das ich ein Projekt erstellen möchte, welches ein Problem löst und zusätzlich noch weitere Funktionen besitzt. Nach langem Überlegen bin ich dabei auf die Idee gekommen eine Uhr zu machen, welches nicht nur die Uhrzeit anzeigt, sondern auch den CO₂-Gehalt im Zimmer misst und bei schlechten Bedingungen den Nutzer davon warnt. Zusätzlich sollte noch ein einstellbarer Timer vorhanden sein.

### Vision

Der Grund für die Entstehung der SmartClock ist der CO₂-Gehalt. Dieser wird oft in verschiedenen Räumen wie Klassenzimmern oder Büro's missachtet. Ein hoher CO₂-Gehalt ist nicht nur schlecht für die Gesundheit, sondern verschlechtert auch die Konzentration. Die Kombination vom Sensor mit dem Buzzer sind perfekt, um den Nutzer von einem hohen CO₂-Gehalt oder von zu hohen Temperaturen zu warnen.

Die einzelnen Funktionen werden jeweils in einem eigenem Menü dargestellt. Das Menü ändert sich automatisch alle 30 Sekunden und lässt sich auch Manuell mit einem Knopfklick ändern.

Zusätzlich gibt es noch eine genaue Anzeige von der Uhrzeit und dem Datum. Das Einstellen eines Timers ist ebenfalls möglich.

### Quick Start

#### Installation

Lade den Ordner [SmartClock_Project](https://github.com/gbssg/ims.project.smartclock/tree/main/SmartClock_Project) vom Github herunter und öffne diesen mit PlatformIO. Nun kann der Code hochgeladen werden.

#### Allgemeine Steuerung

Um SmartClock zu steuern werden der Knopf und der Joystick verwendet. Der Knopf ermöglicht das Ändern der Anzeige (Menü). Beim Joystick werden das Wischen nach oben, unten, links und rechts erkannt. Diese können verwendet werden, um den Buzzer zu stoppen und auch den Timer einstellen zu können

#### Timer Steuerung

Die Steurung des Timers verläuft mit dem Joystick. Mit dem Hoch- und Runterwischen kann die ausgewählte Ziffer um 1 vergrössert und verkleinert werden. Mit dem Links- und Rechtswischen, kann eine andere Ziffer ausgewählt werden. Der Timer startet, sobald die Position vom Cursor auf dem ersten Feld (ganz links) ist. Mit jeder Bewegung auf dieser Position, stoppt der Timer. Damit der Timer von dieser Position wieder gestartet werden kann, muss nach oben gewischt werden.

## Weitere

### Links

- [Technische Dokumentation](./SmartClock_TechnischeDokumentation.md)
- [Betriebshandbuch](./SmartClock_Betriebshandbuch.md)

### Quellen

Dr. Christian M. (2012) [Arbeitsleistung und Raumluftqualität –
über den Nutzen guter Raumluft](https://www.seco.admin.ch/dam/seco/de/dokumente/Arbeit/Arbeitsbedingungen/Gesundheitsschutz%20am%20Arbeitsplatz/Arbeitsr%C3%A4ume%20und%20Umgebungsfaktoren/VW_Arbeitsleistung-und-Raumluft.pdf.download.pdf/VW_Arbeitsleistung-und-Raumluft_de.pdf)

### Kontaktdaten

E-Mail: sai.ragavan412@gmail.com  
Github: [sai-412](https://github.com/sai-412)
