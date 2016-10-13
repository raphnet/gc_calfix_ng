# Gamecube L/R shoulder button calibration fixer for (old) raphnet adapters

This is a tool to fix calibration of Gamecube controller L/R shoulder buttons.

## Supported adapters

* Old raphnet (Before version 3) [Gamecube to USB adapters](http://www.raphnet.net/electronique/gc_n64_usb/index_en.php)

This calibration fixer is not required (and has not effect) for [version 3 adapters](http://www.raphnet.net/electronique/gcn64_usb_adapter_gen3/index_en.php).

## Why was it needed?

As explained [here](http://www.raphnet.net/electronique/gc_n64_usb_manual/index_en.php#3)

> The Gamecube controller sliders are often problematic because they lack a central resting position. Many games assume an axis to be active when the value it reads is different, within a certain margin, from the center position. Thus, when performing button assignments, the L and R sliders will often immediately register as active and hinder (or most likely, prevent) successfully configuring the game for the controller.
>
> The following tool must be run after calibration (see instructions above). When it is run, it looks for raphnet GC/N64 to usb adapters and alters the calibration data the system keeps for each unit found. All axis are left as calibrated, except the L and R sliders which are recentered.

## What else can it do?

Even if you do not use the supported adapters, you run the tool to list the DirectX joysticks. (Can be useful sometimes)

## Supported OS(es)

Windows only.

## Built with

* DirectX 8 SDK or later
* [Mingw](http://www.mingw.org/)

## License

This project is public domain.
