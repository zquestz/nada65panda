# [Nada 65 Panda](https://www.cerakey.com/products/nada-65-panda-keyboard)

This is the first ceramic keycap mechanical keyboard -- 'Smoothest and Thockiest Typing'

- Keyboard Maintainer: [zquestz](https://github.com/zquestz)
- Hardware Supported: Nada 65 Panda

You will need a working QMK installation to build the new firmware.

Note, if the keyboard appears USB "dead" keyboard (error -71 in dmesg), you might try the following first to get the keyboard back into a responsive state:
- Put the physical connection switch in the center position (wired mode)
- Press and hold FN + Spacebar for 3-5 seconds to force wired/reset mode
This is the OEM PCB's manual override (HyphaRF/MonsGeek-style logic).


See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

Once you are all setup.

1. Copy the `nada65panda` folder to the keyboards directory in your QMK installation.

```
mkdir -p ~/src/qmk_firmware/keyboards/cerakey
cp -r nada65panda ~/src/qmk_firmware/keyboards/cerakey/
```

2. Build the new firmware, with Via support.

```
make cerakey/nada65panda:via
```

3. Start flashing the firmware.

```
qmk flash cerakey_nada65panda_via.bin
```

4. When it prompts you to set your keyboard in bootloader mode. Disconnect your keyboard, then plug it back in while holding the ESC key!

5. Wait till the flashing process is complete.

6. Disconnect your keyboard and reconnect it!

7. Profit!
