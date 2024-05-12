# Maker badge
The Maker badge board is the official badge board for Maker Faire's in the Czech republic. The main purpose is to show visitors/exhibitors name and/or project, but the secondary goal is to have at each Maker Faire some interactive game so people can have more fun at the time of the visit.

This board supports [Arduino](https://github.com/espressif/arduino-esp32) and [CircuitPython](https://learn.adafruit.com/welcome-to-circuitpython) platform. 

CircuitPython will be **primary** platform for this board because it is very easy to use, and anybody who will buy a board at some Maker Fair should be able very easily change (at least) her/his name on the display, so anybody knows the name of the person when they start chatting about something.  

I will try to add as many examples for both platforms, as I could, but I already saw few very interesting projects.

## How to use this board in 5 steps:  

1) Connect USB-C cable to the board and turn it on. 
Either use a slide switch (top = OFF, down = ON) or move the jumper to the middle pins. This depends on the version which you have. 

2) Locate drive name "**Circuitpy**" and open **code.py** in some text editor. 
We recommend using **Mu editor**, but even Notepad will work. 

3) Find a line where is **"Jmeno"** (Name), **"Prijmeni"** (Surname), **"Firma/Projekt"** (Company/Project) and change it to whatever suit your need. 
There is yet no "auto aligning", so you have to also change X & Y coordinates for particular text. 

4) Save it via **CTRL+S/CMD+S** or you application menu for that. If the screen does not refresh, push the RESET button. 

5) Unplug the USB cable, put the strap around your neck, and wear it with pride. 

## Board version

There are currently following revisions of the board:

- rev. A : this version was distributed by me and it was using **212 x 104 px** e-ink display with UC8151D driver
- rev. B : this board was assembled by me for Maker Faire Prague 2022 and it was using **250 x 122 px** e-ink display with SSD1680 driver from Make More. It contains the same type of components as rev. A .  
- rev. C (with rev. B markings at the back): this board was assembled by PCBWay and it was using **250 x 122 px** e-ink display with SSD1680 driver from Make More. Some of the components are different because the original parts were not available due to chip shortage. These boards were distributed between Maker Faire Brno 2022 (22 - 23/10/2022) and Maker Faire Prague 2023 (10 - 11/6/2023).
- rev. D : this board was assembled by PCBWay and it contains these changes:
	- slide switch for power control instead of jumper over pins
	- transistor at e-ink display power line to save power 
	- transistor at resistor divider for battery measuring
	- CR2032 holder was removed to save the cost of the board

	These boards were distributed at Maker Faire Prague 2023 (10 - 11/6/2023) and onwards.
	

## Where can you buy it? 

[Makermarket](https://www.makermarket.cz/maker-badge)

