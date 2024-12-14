.pos 0x0400

main:
  lodi R0 0x00 ; short x
  ; would save R0 here if necessary
  lodi R1, 0x02 ; load the value 2
  stor (SP), R1 ; and save it on the
  addi SP, 0x02 ; stack
  mov SP, AC
  call fun
  halt
fun:
  lodrd R1, (BP + -4) ; load param cnt
  lodi R2, 0x00 ; short x
  subi R1, 0x00 ; if (cnt == 0) …
  jmpz end      ; … goto end
  subi R1, 0x01 ; Otherwise, prepare to call fun(cnt - 1)
  stoa (SP), AC
  addi SP, 0x02
  mov SP, AC
  call fun
end:
  ret