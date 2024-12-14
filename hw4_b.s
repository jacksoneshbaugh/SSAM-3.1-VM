.pos 0x400

main:
	lodi R0, 0x0 ; short x = 0
loop:
	addi R0, 0x01 ; x++
	mov R0, AC
	subi R0, 0x022 ; if (x < 34) …
	jmpn loop ; loop again
	halt ; otherwise, halt