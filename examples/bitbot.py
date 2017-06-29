from microbit import *
import bitbot

display.scroll("H")

bitbot.buzz(100)

bitbot.neopixel[5] = (10, 0, 0)
bitbot.neopixel.show()

while(True):
    if bitbot.get_brightness()[0] < 30:
        bitbot.buzz(50)
    sleep(100)
