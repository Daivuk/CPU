// Setup compiler flags first
.compiler output "helloworld.elf"
.compiler section start
.compiler section code 0x80
.compiler section data
.compiler section end

.section start
mov r0 1
mov r1 r0
add r2 r0 r1
brk
/*
jmp main

.section end
end: .int 0

.section code
main:
	mov sp 0x80
	main_loop:
		jmp main_loop
		*/