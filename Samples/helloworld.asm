// Setup compiler flags first
.compiler output "helloworld.elf"
.compiler section start
.compiler section code 0x80
.compiler section data
.compiler section video 0x3FBC
.compiler section end

// Defines

.section start
jmp main

.section data
PATTERN_MAIN: .incbin "main.chr"
NAM_LVL1_1: .incbin "lvl1_1.nam"
ATR_LVL1_1: .incbin "lvl1_1.atr"

.section video
VIDEO_ENABLED: .int 0
VIDEO_SCAN_LINE_IRQ: .int 0
VIDEO_VSYNC_IRQ: .int 0
VIDEO_BG_PATTERN: .int 0
VIDEO_SP_PATTERN: .int 0
VIDEO_BG_NAM: .int 0
VIDEO_BG_ATR: .int 0
VIDEO_BG_OFFSET_X: .int 0
VIDEO_BG_OFFSET_Y: .int 0
VIDEO_BG_PAL: .byte 0x0f,0x37,0x27,0x17,0x0f,0x18,0x08,0x0f,0x0f,0x1c,0x2c,0x3c,0x0f,0x2c,0x07,0x16
VIDEO_SP_PAL: .byte 0x0f,0x37,0x27,0x17,0x0f,0x18,0x08,0x0f,0x0f,0x1c,0x2c,0x3c,0x0f,0x2c,0x07,0x16

.section code
main:
	mov sp 0x80

	// Setup video IRQs
	ldr r0 VIDEO_SCAN_LINE_IRQ
	ldr r1 onScanLine
	str r1 [r0]
	ldr r0 VIDEO_VSYNC_IRQ
	ldr r1 onVsync
	str r1 [r0]

	// Setup patterns
	ldr r0 VIDEO_BG_PATTERN
	ldr r1 PATTERN_MAIN
	add r1 4096
	str r1 [r0]

	// Setup nametable
	ldr r0 VIDEO_BG_NAM
	ldr r1 NAM_LVL1_1
	str r1 [r0]

	// Setup nametable attribs
	ldr r0 VIDEO_BG_ATR
	ldr r1 ATR_LVL1_1
	str r1 [r0]

	// Enable video
	ldr r0 VIDEO_ENABLED
	mov r1 1
	str r1 [r0]

	main_loop: 
		jmp main_loop

onScanLine:
	jmp onScanLine

onVsync:
	jmp onVsync
	
.section end
end:
