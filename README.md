# bindic

Progress bar that takes two parameter to display the level. Can be used when hitting multimedia keys (volume, backlight).

## commands

### makefile

| command | description |
| - | - |
| make all | compiles bindic |
| make compile | same as above |
| make clean | removes bindic and temp files |
| make run | compiles and starts bindic |
| make install | installs bindic.bin to /usr/bin/bindic and css, glade, bindic files to /usr/share/bindic |
| make uninstall | removes /usr/bin/bindic and /usr/share/bindic |

### bindic

| command | description |
| - | - |
| ./bindic \[VALUE\] \[MAX\] | starts bindic and sets progress bar to the given value |
