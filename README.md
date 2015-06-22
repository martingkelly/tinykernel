I tested and wrote this on Linux. It would probably run fine in Windows too, if
you setup an Eclipse project for it.

To run on Linux:
- Plugin board
- ./openocd
  Depends on having openocd installed with the right config.  This config should
  come preinstalled with Linux distros. I'm using Debian, and it's all
  available.
- In a new window/tab, run minicom.
  * Should be setup with the following parameters:
    - pu port             /dev/ttyUSB0
      pu baudrate         38400
      pu bits             8
      pu parity           N
      pu stopbits         1
      pu rtscts           No 
    - Note that /dev/ttyUSB0 could vary depending on your Linux distro and
      openocd version. If so, change it.
- In a new window/tab, run make install. This will compile everything, download
  the image to the board, and start gdb. Type "c" or "continue" to start
  running.
- You should see output in your minicom window.
