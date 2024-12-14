.pos 0x0400

main:
	lodi R0, 0x05 ; short x = 5
conditional:
	subi R0, 0x022 ; if(x < 34) …
	jmpn true ; … goto true
	jmp false ; else, goto false
true:
	addi R0, 0x01 ; x++
	mov R0, AC
	jmp end
false:
	subi R0, 0x01 ; x--
	mov R0, AC
end:
	halt