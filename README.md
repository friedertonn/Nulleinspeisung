# Nulleinspeisung

## Nutzung des überschüssigen Solarstroms zur Warmwassererzeugung
Bei diesem Projekt handelt es sich um ein Hobbyprojekt, dass keine kommerziellen Absichten verfolgt.
Die vorgestellte Schaltung besteht aus mehreren Komponenten und hat das Ziel,
überschüssigen Solarstrom zum Betrieb eines Heizstabes in einem Warmwasserbehälter zu verwenden.
Das ist möglich, wenn folgende Voraussetzungen erfüllt sind:
- eine PV-Anlage ist vorhanden, z. B. ein Balkonkraftwerk,
- ein Stromzähler mit Infrarot-Schnittstelle, der Verbrauchswerte im Sekundentakt liefert,
- ein Warmwasserspeicher, der sich für die Montage eines zusätzlichen Heizstabes eignet.

## Wirkungsprinzip der Steuerung:
Der Stromzähler des Netzbetreibers liefert im Sekundentakt Werte für den aktuellen Verbrauch einer
Hausinstallation mit PV-Anlage. 
Im Fall einer Einspeisung in das öffentliche Netz sind die Verbrauchswerte negativ.
In diesem Fall wird ein Dimmer aktiviert, der einen Heizstab in einem Warmwasserspeicher steuert.
Da die Erzeugung von Solarstrom Schwankungen unterworfen ist und andere Verbraucher im Haus aktiv/inaktiv
werden können, ist die Dimmersteuerung als Regelkreis ausgelegt:
Wenn die PV-Anlage einen Überschuss liefert, fährt der Dimmer in verschiedenen Stufen hoch, bie er im
"Arbeitsbereich" -20 Watt bis 0 Watt Gesamtbezug liegt. Bei einem Abfall des Überschusses wird der
Dimmer entsprechend abgeregelt.
Die Herausforderung besteht darin, durch eine geeignete Dimmersteuerung eine maximale Nutzung
des überschüssigen Stroms bei gleichzeitiger Vermeidung eines zusätzlichen Strombezugs zu realisieren.
Insofern stellt diese Anlage keine echte "Nulleinspeisung" dar. Eine Watt-genaue Steuerung wäre mit
einem erheblich höheren Aufwand verbunden und erfordert Verbrauchswerte im Millisekundenbereich.

## Umsetzung:
Die Steuerung erfolgt über zwei 32-bit Microcontroller ESP8266 der Firma espressif.
Der erste ESP8266 beinhaltet die Tasmota-Software, die den
Infrarot-Lesekopf des Stromzählers liest und die Verbrauchs-/Einspeisedaten erfasst.
https://github.com/arendst/Tasmota
Der zweite ESP8266 enthält die Dimmersteuerung. 
https://github.com/RobotDynOfficial/RBDDimmer
Da dieser Prozessor im Interruptbetrieb arbeitet und die Dimmersteuerung zeitkritisch ist, 
empfiehlt sich das Zusammenlegen der Software auf einem ESP8266 nicht.
Beide Microcontroller sind über drei Steuerleitungen miteinander verbunden.

![](https://www.ftonn.de/GIT-Projekte/Nulleinspeisung/solardimmer.png)

## ESP8266 mit Tasmota-Software:
Die aktuelle Version der Tasmota-Software steht auf der Github-Seite.
https://github.com/arendst/Tasmota
Vor dem Compilieren muss in der Datei tasmota/user_config_override.h die Scriptsteuerung aktiviert werden.
Alternativ findet sich ein compiliertes Firmware-Image der Version 12.2.0 im Unterverzeichnis tasmota.
Es kann z. B. mit dem Programm NodeMCU-PyFlasher.exe auf den ESP8266 übertragen werden.
Anschließend öffent sich auf dem ESP8266 ein lokaler WLAN-Hotspot und erfragt die WLAN-Daten des Heimnetzes.
Zum Auslesen der Verbrauchsdaten wird ein Infrarot-Lesekopf von Hichi eingesetzt. 
Er ist über zwei Steuerleitungen mit RX und TX des ESP8266 verbunden.
Die Konfiguration erfolgt über eine Scriptdatei, die auf den vorhandenen Stromzähler angepasst werden muss.
Für die Anpassung der Scriptdatei wird die Website https://tasmota.github.io/docs/Smart-Meter-Interface/ empfohlen.
Der aktuelle Script für meinen Stromzähler "DD3 BZ10 ETA - ODZ1" befindet sich im Unterverzeichnis tasmota.
Beim Hochladen und Ändern von Scripten auf der Tasmota-Weboberfläche ist es wichtig, 
das Häckchen bei "Script enable" zu setzen.
Wenn alles funktioniert, zeigt die Tasmota-Weboberfläche die Verbrauchsdaten an.
Über GPIO-Pins wird der aktuelle Bezug nach folgendem Schema ausgegeben:

<table style="border-collapse: separate;
  border-spacing: 0;
  padding: 5px;">
    <tbody>
    <tr style="background-color: lightblue">
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;"><b>Bezug/Einspeisung</b></td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2"><b> GPIO 5 (D1)</b> </td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2"><b> GPIO 4 (D2)</b> </td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2"><b> GPIO 14 (D5)</b> </td>
    </tr>
    <tr>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;">Bezug > 500 Watt</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">HIGH</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">HIGH</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">HIGH</td>
    </tr>
    <tr>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;">500 Watt > Bezug > 100 Watt</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">HIGH</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">HIGH</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">LOW</td>
    </tr>
    <tr>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;">100 Watt > Bezug > 20 Watt</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">HIGH</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">LOW</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">HIGH</td>
    </tr>
    <tr>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;">20 Watt > Bezug > 0 Watt</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">HIGH</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">LOW</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">LOW</td>
    </tr>
    <tr>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;">0 Watt < Einspeisung < 20 Watt</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">LOW</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">HIGH</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">HIGH</td>
    </tr>
    <tr>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;">20 Watt < Einspeisung < 40 Watt</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">LOW</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">HIGH</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">LOW</td>
    </tr>
    <tr>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;">40 Watt < Einspeisung < 120 Watt</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">LOW</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">LOW</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">HIGH</td>
    </tr>
    <tr>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;">120 Watt < Einspeisung</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">LOW</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">LOW</td>
        <td style="border: 1px solid #bbb; border-bottom: 1px solid #bbb; padding: 10px 15px 10px 15px;" colspan="2">LOW</td>
    </tr>
    </tbody>
</table>

## ESP8266 mit Dimmer-Steuerung:
Die Signale der drei GPIO-Pins werden verwendet, um den Dimmer zu steuern.
Zum Einsatz kommt ein 1-Phasen-Dimmer der Firma RobotDyn.
An den Dimmer sollten ausschließlich ohmsche Lasten angeschlossen werden.
Es empfiehlt sich der Einbau eines EMI-Filters in die Zuleitung, um die HF-Abstrahlung zu minimieren.
Eine Bibliothek für die Dimmersteuerung ist auf Github vorhanden.
https://github.com/RobotDynOfficial/RBDDimmer
Sie wurde für die Steuerung angepasst und befindet sich im Unterverzeichnis solardimmer.
Die Aufgabe der Dimmersteuerung ist es:
- den Dimmer bei einem Stromüberschuss ein- und nach 10 Minuten Strombezug wieder auszuschalten,
- entsprechend des Stromüberschusses oder -bedarfes die Dimmer-Nachregelung in 3 Stufen vorzunehmen,
- bei hohen Strombedarfen den Dimmer sofort abzuschalten.

Die dazu notwendige Software solardimmer.ino befindet sich ebenfalls im Unterverzeichnis solardimmer
und kann entsprechend eigenen Erfordernissen modifiziert werden.
