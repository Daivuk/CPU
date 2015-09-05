// Setup compiler flags first
.compiler output "helloworld.elf"
.compiler section start
.compiler section code 0x80
.compiler section data
.compiler section end

.section start
jmp main

.section end
end: .int 0

.section data
VIDEO_ADDR: .int 0x10000

.section code
main:
	mov sp 0x80

	ldr r0 VIDEO_ADDR
	ldr r0 [r0]
	mov r1 0xFFFFFFFF
	str r1 [r0]

	mov r0 1
	mov r1 2
	cmp r0 r1
	==mov r3 3
	sub? r0 1
	==mov r3 3

	main_loop: 
		jmp main_loop
