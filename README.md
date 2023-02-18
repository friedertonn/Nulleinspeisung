# Nulleinspeisung
Nutzung des überschüssigen Solarstroms zur Warmwassererzeugung

Bei diesem Projekt handelt es sich um ein Hobbyprojekt, dass keine kommerziellen Absichten verfolgt.
Die vorgestellte Schaltung besteht aus mehreren Komponenten und verfolgt das Ziel,
überschüssigen Solarstrom zum Betrieb eines Heizstabes in einem Warmwasserbehälter einzusetzen.
Das ist möglich, wenn die folgenden Voraussetzungen gegeben sind:
- eine PV-Anlage, z. B. ein Balkonkraftwerk,
- einen Stromzähler mit Infrarot-Schnittstelle, der Verbrauchswerte im Sekundentakt liefert,
- einen Warmwasserbehälter, der sich für die Montage eines Heizstabes eignet.

### Wirkungsprinzip der Steuerung:
Der Stromzähler des Netzbetreibers liefert im Sekundentakt Werte für den aktuellen Verbrauch einer
Hausinstallation mit PV-Anlage. 
Im Fall einer Einspeisung in das öffentliche Netz sind die Werte negativ.
In diesem Fall wird ein Dimmer aktiviert, der einen Heizstab steuert.
Da die Erzeugung von Solarstrom Schwankungen unterworfen ist und andere Verbraucher im Haus aktiv/inaktiv
werden können, ist die Regelung dynamisch ausgelegt:
Wenn die PV-Anlage einen Überschuss liefert, fährt der Dimmer in verschiedenen Stufen hoch, bie er im
"Arbeitsbereich" -20 Watt bis 0 Watt Gesamtbezug liegt. Bei einem Abfall des Überschusses wird der
Dimmer entsprechend abgeregelt.
Die Herausforderung besteht darin, durch eine geeignete Dimmersteuerung eine maximale Nutzung
des überschüssigen Stroms bei Vermeidung eines zusätzlichen Strombezugs zu realisieren.
Insofern stellt diese Anlage keine echte "Nulleinspeisung" dar. Eine Watt-genaue Steuerung wäre mit
einem erheblich höheren Aufwand verbunden und erfordert Verbrauchswerte im Millisekundenbereich.

### Umsetzung:
Die Steuerung erfolgt über zwei 32-bit Microcontroller ESP8266 der Firma espressif.
Der erste ESP8266 beinhaltet die Tasmota-Software, die den
Infrarot-Lesekopf des Stromzählers liest und die Verbrauchs-/Einspeisedaten erfasst.
https://github.com/arendst/Tasmota
Im Quellcode muss die Scriptsteuerung aktiviert sein.
Der zweite ESP8266 enthält die Dimmersteuerung. 
https://github.com/RobotDynOfficial/RBDDimmer
Da der Prozessor im Interruptbetrieb arbeitet und die Dimmersteuerung zeitkritisch ist, 
empfiehlt sich das Zusammenlegen der Software auf einen ESP8266 nicht.
Beide Microcontroller sind über Steuerleitungen miteinander verbunden.
