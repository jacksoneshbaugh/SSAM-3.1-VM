.pos 0x0400

main:
	lodi R0, 0x00 ; short x = 0
loop:
	subi R0, 0x0a ; if (x < 10) …
	jmpn end_loop ; … end loop
cond:
	subi R0, 0x05
	jmpn true
false:
	addi R0, 0x02
	mov R0, AC
	jmp loop
true:
	addi R0, 0x01
	mov R0, AC
	jmp loop
end_loop:
	halt