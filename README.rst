:orphan:

#####################################
sw_usb_media_player: USB Media Player
#####################################

:vendor: xross
:description: USB mass-storage video and audio player for xcore.ai
:scope: Demo
:category: General purpose
:keywords: USB, mass storage, MPEG-1, MJPEG, MP2, LCD, I2S
:hardware: XK-EVK-XU316

*******
Summary
*******

Experimental USB mass-storage media player for xcore.ai. It plays MPEG-1 or
MJPEG video on an SPI LCD and supports 48-kHz MP2 audio through an I2S DAC.

The application targets XK-EVK-XU316, with the AIV carrier, LCD and optional
DAC wiring described in the `player guide <doc/rst/sw_usb_media_player.rst>`_.
The reference clips are in ``media/``; copy them to the root of the USB stick.

********
Features
********

* Read-only playback from FAT-formatted USB mass storage
* MPEG-1 elementary and program-stream video, including I/P/B pictures
* Baseline MJPEG video
* SPI LCD output using ILI9341 or ST7789V controllers
* Optional 48-kHz stereo MP2 audio output through an I2S DAC
* Audio-clock-paced playback and full-speed decode/discard benchmarking
* On-screen FPS, stereo level and audio/video timing diagnostics
* Native tests, simulator tests and bounded hardware test runners

************
Known issues
************

* Playback synchronisation is experimental.
* Seeking and arbitrary mid-stream resolution changes are not supported.
* DAC output supports 48-kHz audio only; there is no sample-rate conversion.
* USB transport and disconnect recovery retain the limitations of
  `lib_xuh <https://github.com/xross/lib_xuh>`_.

****************
Development repo
****************

* `sw_usb_media_player <https://github.com/xross/sw_usb_media_player>`_

*****************
Required hardware
*****************

* XK-EVK-XU316 board with the AIV carrier, an XTAG debug adapter and the
  appropriate USB data and board-power cables.
* High-speed USB memory stick, FAT-formatted, with the reference files from
  ``media/`` copied to its root directory.
* USB OTG/data adapter with an auxiliary 5-V power input, such as this
  `BRITECO USB power/data splitter <https://www.amazon.co.uk/dp/B0D9LZQCQP>`_.
  **The demonstrated board setup does not supply VBUS to power the memory
  stick**, so external USB power is required; a data-only adapter is not enough.
* 240 x 320 SPI LCD using an **ILI9341** or **ST7789V** controller. The tested
  display is the `Waveshare 2.4-inch LCD module (ILI9341)
  <https://thepihut.com/products/2-4-lcd-display-module-240x320>`_.
* Pin headers soldered to the board's expansion footprints as needed, plus
  jumper wires for the LCD. The DAC SHIM uses J3.
* For audible output: `Pimoroni Audio DAC SHIM (Line-Out)
  <https://thepihut.com/products/audio-dac-shim-line-out>`_, a 3.5-mm stereo cable
  and powered speakers or an amplifier. The DAC is optional for video-only or
  timed-stand-in playback.
* Optional `GPIO Edge Extension
  <https://thepihut.com/products/gpio-edge-extension-for-raspberry-pi>`_ to make
  the header pins easier to access alongside the DAC SHIM.

See the player guide's `hardware requirements
<doc/rst/sw_usb_media_player.rst#hardware-requirements>`_ for power precautions,
LCD controller selection and the wiring tables. A Raspberry Pi is not required.

**************
Required tools
**************

* XMOS XTC Tools: 15.3.1
* CMake: 3.21 or later
* Python 3 and FFmpeg for tests and media preparation

Build
=====

Activate XTC Tools and use a sandbox containing this repository alongside its
library dependencies. Configure from the repository root::

    cmake -G "Unix Makefiles" -S app_usb_media_player -B build
    xmake -C build -j 8

The ``jpeg``, ``mpeg1`` and ``mpeg1_audio`` configurations are described in
the guide, together with input files and hardware execution. Player configuration
options retain their ``APP_XUH_MSC_PLAYER_*`` names.

For a fresh checkout, follow the guide's `clean sandbox build
<doc/rst/sw_usb_media_player.rst#clean-sandbox-build>`_. XCommon fetches missing
dependencies but does not change existing sibling checkouts.

Tests
=====

Run Python tooling in a virtual environment. Hardware and simulator test
dependencies are in ``tests/requirements.txt``; hardware runners also require
a reserved board. Native and target test commands are in the player guide.
Generated builds and profiling artefacts are not part of the source repository.

*********************************
Required libraries (dependencies)
*********************************

Dependencies and pinned revisions are declared in ``deps.cmake``:

* ``lib_xuh`` for USB host and mass-storage access, with its ``lib_xud`` dependency
* ``lib_fatfs`` for filesystem access
* ``lib_logging`` for diagnostic output
* ``lib_jpeg`` or ``lib_mpeg`` for the selected video configuration
* ``lib_spi`` for the LCD interface
* ``lib_i2s`` and ``lib_sw_pll`` when DAC output is enabled

*************************
Related application notes
*************************

* None

*******
Support
*******

Report bugs and feature requests through the repository's
`GitHub issues <https://github.com/xross/sw_usb_media_player/issues>`_.
