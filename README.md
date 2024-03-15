# evdev-keepalive
Keepalive workaround for some 8bitdo (and possibly other) gamepads. 

Originally developed here: https://github.com/tobiasjakobi/ayaneo_kun/

## Firmware reset behaviour
Some 8BitDo firwares seem to implement a very strict watchdog mechanism. This could be related to an incomplete autodetect mechanism for determining xinput, dinput, android, or switch mode. If the firmware doesn't see a USB transfer from the host within about a second (exact value is unknown), it resets the USB device. The device drops of the bus, the kernel unloads the driver. Once the device pops up again on the bus, it is then again probed by the kernel, which then loads the driver. This cycle of USB disconnect followed by almost immediate reconnect is repeated ad infinitum.

The workaround consists of immediately opening the event device of controller and to keep it open. On the kernel driver level this results in periodic USB transfers to query button and other states. Due to these transfer happening several times each second the firmware watchdog is happy and keeps the device alive.

A issue report is open on the xpad issue tracker: https://github.com/paroj/xpad/issues/256

## Workaround implementation
A draft implementation of the workaround can be found here, cloned from the original source and made to work with addtional 8BitDo hardware. The purpose of this repository is to provide a stable target for the AUR.

It consists of a UDev rule, a systemd unit template and a C application. The rule automatically starts the systemd unit once the event device of the controller is available. The unit is just a thin wrapper around the application that keeps the event device open until the application is terminated by the unit again.

### Prerequisites
- systemd
- clang

## Installation
- clone this repository
- cd evdev-keepalive
- `$ clang evdev_keepalive.c -o evdev_keepalive`
- `# cp evdev_keepalive /usr/bin/evdev_keepalive`
- `# cp evdev-keepalive@.service /usr/lib/systemd/system/evdev-keepalive@.service`
- `# cp 10-evdev-keepalive.rules /usr/lib/udev/rules.d`
> NOTE: You can substitute /etc/ for /usr/lib for the systemd unit and udev rules, and /bin for the compiled binary if you prefer, but you'll need to updat the paths in each file.


