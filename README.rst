###################
USB media player
###################

Experimental USB mass-storage media player for xcore.ai. It plays MPEG-1 or
MJPEG video on an SPI LCD and supports 48-kHz MP2 audio through an I2S DAC.

The application targets XK-EVK-XU316, with the LCD and optional DAC described in
the `player guide <doc/rst/sw_usb_media_player.rst>`_. Playback synchronisation
is experimental; there is no seeking, sample-rate conversion or support for
arbitrary mid-stream resolution changes. USB transport and disconnect recovery
retain the limitations of `lib_xuh <https://github.com/xross/lib_xuh>`_.

Build
=====

Use XTC Tools 15.3.1 and a sandbox containing this repository alongside its
library dependencies. Configure from the repository root::

    cmake -G "Unix Makefiles" -S app_usb_media_player -B build
    xmake -C build -j 8

The ``jpeg``, ``mpeg1`` and ``mpeg1_audio`` configurations are described in
the guide, together with wiring, input files, native and target tests, and
hardware execution. Dependencies and pinned revisions are in ``deps.cmake``.
Player configuration options retain their ``APP_XUH_MSC_PLAYER_*`` names.

Development
===========

Run Python tooling in a virtual environment. Hardware runners additionally need
``tests/requirements.txt`` and a reserved board. The reference clips are in
``media/``; copy them to the root of the USB stick. Generated builds and
profiling artefacts are not part of the source repository.
