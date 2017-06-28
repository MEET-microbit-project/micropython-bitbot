Bit:Bot module
==============

This is the MicroPython source code for the BBC micro:bit, modified to
include a module for controlling the [Bit:Bot](4tronix.co.uk/bitbot).
For instructions on how to build, see further down.

The following is an overview of the module.

```python
import bitbot

bb = bitbot.BitBot()
```

Motor controls
--------------

```python
# set the speed of the motor in percent
# for each side separately (left_speed, right_speed):
bb.set_speed(100, 50)  # make a right curve
# or for both sides at once (moving straight).
# negative values mean driving backwards
bb.set_speed(-30.5)
# alternatively it is possible to set only one side:
bb.set_speed(left=40)
```

Sensor input
------------
```python
# get readings from the light sensors
# returns a tuple of percentages - higher is brighter
light_left, light_right = bb.get_light()  # returns e.g. (70.3, 71.0)

# get input from the line sensors
bb.is_line_left()  # returns True / False
bb.is_line_right()
```

Output
------
```python
# control the 12 neopixel (index 0 - 11)
bb.set_rgb(3, 255, 0, 0)  # (index, red, green, blue)
bb.set_rgb(0, 10, 0, 10)
bb.show() # show changes

# make a buzzing sound for an indicated number of milliseconds
bb.buzz(100)
```


MicroPython for the BBC micro:bit
=================================

This is the source code for MicroPython running on the BBC micro:bit!

To get involved with the community subscribe to the microbit@python.org
mailing list (https://mail.python.org/mailman/listinfo/microbit). You need to
be a member to post messages.

Various things are in this repository, including:
- Source code in source/ and inc/ directories.
- Example Python programs in the examples/ directory.
- Tools in the tools/ directory.

The source code is a yotta application and needs yotta to build, along
with an ARM compiler toolchain (eg arm-none-eabi-gcc and friends).

Ubuntu users can install the needed packages using:
```
sudo add-apt-repository -y ppa:team-gcc-arm-embedded
sudo add-apt-repository -y ppa:pmiller-opensource/ppa
sudo apt-get update
sudo apt-get install cmake ninja-build gcc-arm-none-eabi srecord libssl-dev
pip3 install yotta
```

Once all packages are installed, use yotta to build.  You will need an ARM
mbed account to complete the first command, and will be prompted to create one
as a part of the process.

- Use target bbc-microbit-classic-gcc-nosd:

  ```
  yt target bbc-microbit-classic-gcc-nosd
  ```

- Run yotta update to fetch remote assets:

  ```
  yt up
  ```

- Start the build:

  ```
  yt build
  ```

The resulting microbit-micropython.hex file to flash onto the device can be
found in the build/bbc-microbit-classic-gcc-nosd/source from the root of the
repository.

There is a Makefile provided that does some extra preprocessing of the source,
which is needed only if you add new interned strings to qstrdefsport.h.  The
Makefile also puts the resulting firmware at build/firmware.hex, and includes
some convenience targets.

How to use
==========

Upon reset you will have a REPL on the USB CDC serial port, with baudrate
115200 (eg picocom /dev/ttyACM0 -b 115200).

Then try:

    >>> import microbit
    >>> microbit.display.scroll('hello!')
    >>> microbit.button_a.is_pressed()
    >>> dir(microbit)

Tab completion works and is very useful!

Read our documentation here:

https://microbit-micropython.readthedocs.io/en/latest/

You can also use the tools/pyboard.py script to run Python scripts directly
from your PC, eg:

    $ ./tools/pyboard.py /dev/ttyACM0 examples/conway.py

Be brave! Break things! Learn and have fun!
