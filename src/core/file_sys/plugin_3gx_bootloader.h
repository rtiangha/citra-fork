// Copyright 2022 Citra Emulator Project
// Copyright 2024 Borked3DS Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

// Copyright 2022 The Pixellizer Group
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

// Plugin bootloader payload
// Compiled with https://shell-storm.org/online/Online-Assembler-and-Disassembler/
/*
; Backup registers

    stmfd   sp!, {r0-r12}
    mrs     r0, cpsr
    stmfd   sp!, {r0}

; Check plugin validity and exit if invalid (also set a flag)

    adr     r0, g_plgstartendptr
    ldr     r1, [r0, #4]
    ldr     r0, [r0]
    adr     r2, g_plgloadexeargs
    mov     lr, pc
    adr     pc, g_loadexefunc
    adr     r1, g_loadexechecksum
    ldr     r1, [r1]
    cmp     r0, r1
    adr     r0, g_plgldrlaunchstatus
    ldr     r0, [r0]
    moveq   r1, #1
    movne   r1, #0
    str     r1, [r0]
    svcne   0x3

; Flash top screen light blue

    adr     r0, g_plgnoflash
    ldrb    r0, [r0]
    cmp     r0, #1
    beq     skipflash
    ldr     r4, =0x90202204
    ldr     r5, =0x01FF9933
    mov     r6, #64
    flashloop:
    str     r5, [r4]
    ldr     r0, =0xFF4B40
    mov     r1, #0
    svc     0xA
    subs    r6, r6, #1
    bne     flashloop
    str     r6, [r4]
    skipflash:

; Set all memory regions to RWX

    ldr     r0, =0xFFFF8001
    mov     r1, #1
    svc     0xB3

; Restore instructions at entrypoint

    adr     r0, g_savedGameInstr
    adr     r1, g_gameentrypoint
    ldr     r1, [r1]
    ldr     r2, [r0]
    str     r2, [r1]
    ldr     r2, [r0, #4]
    str     r2, [r1, #4]
    svc     0x94

; Launch the plugin

    adr     r0, g_savedGameInstr
    push    {r0}
    adr     r5, g_plgentrypoint
    ldr     r5, [r5]
    blx     r5
    add     sp, sp, #4

; Restore registers and return to the game

    ldmfd   sp!, {r0}
    msr     cpsr, r0
    ldmfd   sp!, {r0-r12}
    adr     lr, g_gameentrypoint
    ldr     pc, [lr]

.pool

g_savedGameInstr:
    .word 0xDEAD0000, 0xDEAD0001
g_gameentrypoint:
    .word 0xDEAD0002
g_plgloadexeargs:
    .word 0xDEAD0003, 0, 0, 0
g_plgstartendptr:
    .word 0xDEAD0004, 0xDEAD0005
g_loadexechecksum:
    .word 0xDEAD0006
g_plgldrlaunchstatus:
    .word 0xDEAD0007
g_plgentrypoint:
    .word 0xDEAD0008
g_plgnoflash:
    .word 0xDEAD0009
g_loadexefunc:
    .word 0xDEAD000A
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    bx lr
*/

#include <array>
#include "common/common_types.h"

constexpr std::array<u8, 412> g_plugin_loader_bootloader = {
    0xff, 0x1f, 0x2d, 0xe9, 0x00, 0x00, 0x0f, 0xe1, 0x01, 0x00, 0x2d, 0xe9, 0xf0, 0x00, 0x8f, 0xe2,
    0x04, 0x10, 0x90, 0xe5, 0x00, 0x00, 0x90, 0xe5, 0xd4, 0x20, 0x8f, 0xe2, 0x0f, 0xe0, 0xa0, 0xe1,
    0xf4, 0xf0, 0x8f, 0xe2, 0xe0, 0x10, 0x8f, 0xe2, 0x00, 0x10, 0x91, 0xe5, 0x01, 0x00, 0x50, 0xe1,
    0xd8, 0x00, 0x8f, 0xe2, 0x00, 0x00, 0x90, 0xe5, 0x01, 0x10, 0xa0, 0x03, 0x00, 0x10, 0xa0, 0x13,
    0x00, 0x10, 0x80, 0xe5, 0x03, 0x00, 0x00, 0x1f, 0xc8, 0x00, 0x8f, 0xe2, 0x00, 0x00, 0xd0, 0xe5,
    0x01, 0x00, 0x50, 0xe3, 0x09, 0x00, 0x00, 0x0a, 0x78, 0x40, 0x9f, 0xe5, 0x78, 0x50, 0x9f, 0xe5,
    0x40, 0x60, 0xa0, 0xe3, 0x00, 0x50, 0x84, 0xe5, 0x70, 0x00, 0x9f, 0xe5, 0x00, 0x10, 0xa0, 0xe3,
    0x0a, 0x00, 0x00, 0xef, 0x01, 0x60, 0x56, 0xe2, 0xf9, 0xff, 0xff, 0x1a, 0x00, 0x60, 0x84, 0xe5,
    0x5c, 0x00, 0x9f, 0xe5, 0x01, 0x10, 0xa0, 0xe3, 0xb3, 0x00, 0x00, 0xef, 0x54, 0x00, 0x8f, 0xe2,
    0x58, 0x10, 0x8f, 0xe2, 0x00, 0x10, 0x91, 0xe5, 0x00, 0x20, 0x90, 0xe5, 0x00, 0x20, 0x81, 0xe5,
    0x04, 0x20, 0x90, 0xe5, 0x04, 0x20, 0x81, 0xe5, 0x94, 0x00, 0x00, 0xef, 0x34, 0x00, 0x8f, 0xe2,
    0x04, 0x00, 0x2d, 0xe5, 0x58, 0x50, 0x8f, 0xe2, 0x00, 0x50, 0x95, 0xe5, 0x35, 0xff, 0x2f, 0xe1,
    0x04, 0xd0, 0x8d, 0xe2, 0x01, 0x00, 0xbd, 0xe8, 0x00, 0xf0, 0x29, 0xe1, 0xff, 0x1f, 0xbd, 0xe8,
    0x18, 0xe0, 0x8f, 0xe2, 0x00, 0xf0, 0x9e, 0xe5, 0x04, 0x22, 0x20, 0x90, 0x33, 0x99, 0xff, 0x01,
    0x40, 0x4b, 0xff, 0x00, 0x01, 0x80, 0xff, 0xff, 0x00, 0x00, 0xad, 0xde, 0x01, 0x00, 0xad, 0xde,
    0x02, 0x00, 0xad, 0xde, 0x03, 0x00, 0xad, 0xde, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0xad, 0xde, 0x05, 0x00, 0xad, 0xde, 0x06, 0x00, 0xad, 0xde,
    0x07, 0x00, 0xad, 0xde, 0x08, 0x00, 0xad, 0xde, 0x09, 0x00, 0xad, 0xde, 0x0a, 0x00, 0xad, 0xde,
    0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3,
    0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3,
    0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3,
    0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3,
    0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3,
    0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3,
    0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3,
    0x00, 0xf0, 0x20, 0xe3, 0x00, 0xf0, 0x20, 0xe3, 0x1e, 0xff, 0x2f, 0xe1};
