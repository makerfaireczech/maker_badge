"""
This example show how to use multiple bitmaps, combine them to one screen, read and show battery capacity. At the end, board goes to deep sleep. You can wake it up with Boot button.

You can learn more about deep sleep here: https://learn.adafruit.com/deep-sleep-with-circuitpython

MIT License
Copyright (c) 2023 Czech maker

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

"""
import alarm
import time
import terminalio
import board
import neopixel
import touchio
import displayio
import adafruit_ssd1680
import analogio
from adafruit_display_text import label
from digitalio import DigitalInOut, Direction
from adafruit_simplemath import map_range

# Function for append text to the display data
def _addText(text, scale, color, x_cord, y_cord):
    text_group = displayio.Group(scale=scale, x=x_cord, y=y_cord)
    text_label = label.Label(terminalio.FONT, text=text, color=color)
    text_group.append(text_label)
    display_data.append(text_group)

# Function which reads battery voltage
def get_voltage(pin):
    enable_battery_reading.value = False
    bat_value = (pin.value * 3.3) / 65536 * 2
    enable_battery_reading.value = True
    return bat_value


# Define board pinout
board_spi = board.SPI()  # Uses SCK and MOSI
board_epd_cs = board.D41
board_epd_dc = board.D40
board_epd_reset = board.D39
board_epd_busy = board.D42

# Define pin for battery reading
vbat_voltage = analogio.AnalogIn(board.D6)

# Define pin to enable power for the display and set it as output
enable_display = DigitalInOut(board.D16)
enable_display.direction = Direction.OUTPUT

# Define pin to enable battery reading and set it as output
enable_battery_reading = DigitalInOut(board.D14)
enable_battery_reading.direction = Direction.OUTPUT
battery_reading_interval = 180 #300 # 5*60 seconds

# Read battery voltage and convert it to the percentage
battery_voltage = get_voltage(vbat_voltage)
battery_percentage = map_range(battery_voltage, 3.7, 4.2, 0, 100)
print("VBat voltage: {:.2f}".format(battery_voltage), "/", battery_percentage, "%")

# Define ePaper display colors value
display_black = 0x000000
display_white = 0xFFFFFF

# Define ePaper display resolution
display_width = 250
display_height = 122

# Prepare ePaper display
displayio.release_displays()

# Setup bus for the display
display_bus = displayio.FourWire(
    board_spi,
    command=board_epd_dc,
    chip_select=board_epd_cs,
    reset=board_epd_reset,
    baudrate=1000000,
)

# Let it rest for second ;)
time.sleep(1)

# Create display object
display = adafruit_ssd1680.SSD1680(
    display_bus,
    width=display_width,
    height=display_height,
    rotation=270,
    busy_pin=board_epd_busy,
)

# Create main group/buffer which will be displayed
display_data = displayio.Group()

# Path to logo bitmap
logo = displayio.OnDiskBitmap("/logo.bmp")

# Buffer for logo
logo_tilegrid = displayio.TileGrid(
    logo, pixel_shader=logo.pixel_shader
)

# Add logo buffer to the main one
display_data.append(logo_tilegrid)

# Path to battery icon bitmap
bat_icon = displayio.OnDiskBitmap("/battery_icon.bmp")

# Buffer for battery icon. X & Y are coordinates where logo will be displayed
bat_tilegrid = displayio.TileGrid(
    bat_icon, pixel_shader=bat_icon.pixel_shader, x=215, y=0
)

# Add icon buffer to the main one
display_data.append(bat_tilegrid)

# Print battery capacity to the screen
_addText("{:.0f}%".format(battery_percentage), 1, display_black, 223, 8)
_addText("{:.2f}V".format(battery_voltage), 1, display_black, 220, 28)

# Enable power to the display
enable_display.value = False

# Write buffer to the display
display.show(display_data)

# Refresh display to show data
display.refresh()

# Create alarm so you can wake up your board
pin_alarm = alarm.pin.PinAlarm(pin=board.D0, value=False, pull=True)

# Time for display to finish refreshng
time.sleep(10)

# Exit the program, and then deep sleep until the alarm wakes us.
alarm.exit_and_deep_sleep_until_alarms(pin_alarm)
