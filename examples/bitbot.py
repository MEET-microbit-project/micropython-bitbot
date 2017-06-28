from bitbot import *
from microbit import *

bb = BitBot()

bb.set_rgb(10, 50, 0, 0)
bb.show()

display.scroll("H")

bb.buzz(100)

while(True):
    sleep(100)
