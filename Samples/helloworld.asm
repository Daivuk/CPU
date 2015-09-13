// Setup compiler flags first
.compiler output "helloworld.elf"
.compiler section start
.compiler section code 0x80
.compiler section data
.compiler section video 0x3FC0
.compiler section end

.section start
jmp main

.section data
// Global variables
g_currentScanLine:      .int 0
g_offsetX:              .int 0

// Data
PATTERN_MAIN:           .incbin "main.chr"
NAM_LVL1_1:             .incbin "lvl1_1.nam"
ATR_LVL1_1:             .incbin "lvl1_1.atr"

.section video
// Video labels
VIDEO_ENABLED:          .int 0 // 0
VIDEO_SCAN_LINE_IRQ:    .int 0 // 4
VIDEO_VSYNC_IRQ:        .int 0 // 8
VIDEO_BG_PATTERN:       .int 0 // 12
VIDEO_SP_PATTERN:       .int 0 // 16
VIDEO_BG_NAM:           .int 0 // 20
VIDEO_BG_OFFSET_X:      .int 0 // 24
VIDEO_BG_OFFSET_Y:      .int 0 // 28
VIDEO_BG_PAL:           .incbin "tileset.pal" // 32
VIDEO_SP_PAL:           .incbin "sprites.pal" // 48

.section code
main:
    mov sp 0x80

    // Setup video IRQs
    ldr r0 onScanLine
    str r0 VIDEO_SCAN_LINE_IRQ
    ldr r0 onVsync
    str r0 VIDEO_VSYNC_IRQ

    // Setup patterns
    ldr r0 PATTERN_MAIN
    str r0 VIDEO_SP_PATTERN
    add r0 0x1000
    str r0 VIDEO_BG_PATTERN

    // Setup nametable
    ldr r0 NAM_LVL1_1
    str r0 VIDEO_BG_NAM

    // Enable video
    mov r0 1
    str r0 VIDEO_ENABLED

    brk

onScanLine:
    ldr r0 g_currentScanLine
    ldr r0 [r0]

    cmp r0 32
    ==ldr r1 g_offsetX
    ==ldr r1 [r1]
    ==str r1 VIDEO_BG_OFFSET_X

    add r0 1
    str r0 g_currentScanLine
    
    brk

onVsync:
    // Reset some stuff to 9
    mov r0 0
    str r0 VIDEO_BG_OFFSET_X
    str r0 g_currentScanLine

    // Move the screen horizontally
    ldr r0 g_offsetX
    ldr r0 [r0]
    add r0 1
    str r0 g_offsetX
    
    brk
    
.section end
end:
