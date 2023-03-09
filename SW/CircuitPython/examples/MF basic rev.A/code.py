import time
import terminalio
import board
import neopixel
import touchio
import displayio
import adafruit_uc8151d
from adafruit_display_text import label

# Function for append text to the display data
def _addText(text, scale, color, x_cord, y_cord):
    group = displayio.Group(scale = scale, x = x_cord, y = y_cord)
    text_label = label.Label(terminalio.FONT, text = text, color = color)
    group.append(text_label)
    display_data.append(group)

# Define board pinout
board_spi = board.SPI()  # Uses SCK and MOSI
board_epd_cs = board.D41
board_epd_dc = board.D40
board_epd_reset = board.D39
board_epd_busy = board.D42

# Define touch buttons
touch_threshold = 20000
touch_1 = touchio.TouchIn(board.D5)
touch_1.threshold = touch_threshold
touch_2 = touchio.TouchIn(board.D4)
touch_2.threshold = touch_threshold
touch_3 = touchio.TouchIn(board.D3)
touch_3.threshold = touch_threshold
touch_4 = touchio.TouchIn(board.D2)
touch_4.threshold = touch_threshold
touch_5 = touchio.TouchIn(board.D1)
touch_5.threshold = touch_threshold

# Define LED
led_pin = board.D18
led_matrix = neopixel.NeoPixel(led_pin, 4, brightness = 0.1, auto_write = False)

# Define LED colors value
led_off = (0, 0, 0)
led_red = (255, 0, 0)
led_green = (0, 255, 0)
led_blue = (0, 0, 255)

# Define ePaper display colors value
display_black = 0x000000
display_white = 0xFFFFFF

# Define ePaper display resolution
display_width = 212
display_height = 104

# Prepare ePaper display
displayio.release_displays()
display_bus = displayio.FourWire(
    board_spi, command = board_epd_dc, chip_select = board_epd_cs, reset = board_epd_reset, baudrate = 1000000
)
time.sleep(1)
display = adafruit_uc8151d.UC8151D(
    display_bus, width = display_width, height = display_height, rotation = 270, busy_pin = board_epd_busy
)
display_data = displayio.Group()
display_background = displayio.Bitmap(display_width, display_height, 1)
display_color_palette = displayio.Palette(1)
display_color_palette[0] = display_white

# Append tilegrid with the background to the display data
display_data.append(displayio.TileGrid(display_background, pixel_shader = display_color_palette))

# Render namecard to display
_addText("Name", 3, display_black, 70, 20)
_addText("Surname", 3, display_black, 45, 55)
_addText("Company/Project", 2, display_black, 20, 90)
display.show(display_data)
display.refresh()

# MAIN LOOP
while True:
    if touch_1.value:
        # Turn off the LED
        led_matrix.fill(led_off)
        led_matrix.show()
    if touch_2.value:
        # Set LED to red
        led_matrix.fill(led_red)
        led_matrix.show()
    if touch_3.value:
        # Set LED to green
        led_matrix.fill(led_green)
        led_matrix.show()
    if touch_4.value:
        # Set LED to blue
        led_matrix.fill(led_blue)
        led_matrix.show()
    if touch_5.value:
        # Turn off the LED
        led_matrix.fill(led_off)
        led_matrix.show()
