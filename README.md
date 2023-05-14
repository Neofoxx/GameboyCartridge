# Gameboy Cartridge with a PIC32

A Gameboy cartridge with a microcontroller on it, for quick tests on actual hardware 

## Overview

This projects implements a simple 32kB ROM + 8kB SRAM cartridge, where both are replaced with SRAM chips, and populated by a PIC32MM microcontroller at either boot, or via a USB interface.

Currently working:
- Uploading to the "ROM" part of the cartridge
- Resetting etc.
- Tested that Tetris is playable, if a write to 0x2000 is patched out

Future plans:
- "SRAM" part of cartridge tested
- Speedup of upload/readout scripts

Not planned:
- Bigger size of "ROM"

## Theory of operation

In a normal gameboy cartrdige without an MBC, there is usually just a ROM chip, that contains the game. An exmaple of this would be Tetris. This is memory mapped into the Gameboys address space, and can be 32kB max.

There is also space for an 8kB RAM chip - usually SRAM, which means a battery is required to save the contents. FRAM chips are available nowadays, and remove the battery requirements, at the expense of increased cost. This is also memory mapped, but needs a few logic for address decoding.

In this implementation, both ROM and RAM are replaced with 32kB SRAM chips. One is used as the ROM, the other (well, part of it) is used as SRAM.

The idea is, that for speeds testing on real HW, code can be uploaded from the PC to the Gameboy via the USB connection, removing the need to unplug the cartridge, insert it into the programmer, flash it, remove it, reinstall it, and then test. Convenience is what we're after.

The other idea is for the microcontroller to have a 40kB section of FLASH dedicated to storing said contents, and automatically populating the chips at power-up.

## TODOs

Fix board files. Forgot to connect some pins to the transistors...
