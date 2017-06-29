from microbit import *
import bitbot

display.scroll("H")

bitbot.buzz(100)

bitbot.neopixel[5] = (10, 0, 0)
bitbot.neopixel.show()

while(True):
    sleep(100)
