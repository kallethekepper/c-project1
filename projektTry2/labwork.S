  # labwork.S
  # Written 2015 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1 2015-2016.
  # The macros PUSH and POP are in the public domain.
  
  

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

  # You can add your own code here
  #
  
  
	.globl hexasc
	.globl delay
	.globl time2string
	
#just testing from here:	
	.global led_test
led_test:
 lui $t0,0xbf88
 ori $t0,$t0,0x6110
 sw $a0,0($t0)
 jr $ra
#to here	

 hexasc:
	andi	$a0,$a0,0xf 	#masking to get only 0 - f
	addi	$t0,$0,9	# describes where separates between 0-9 and a - f
	bgt	$a0,$t0,change	#branch if a0 is bigger than t0
	nop
	j	nummber		#branch if a0 is less or equal too t0
	nop
change:		
	addi	$v0, $a0, 0x37
	j 	num	
nummber:	
	addi	$v0, $a0, 0x30
num:
	jr	$ra
	nop

	
	
delay:
	addi 	$t3,$0,0
	add	$t5,$0,$a0
here:	
	
	ble 	$t5,$0, while
	addi	$t5,$t5,-1
	nop
	
for:	
	addi	$t4,$0,20711
	addi	$t3,$t3,1
	bne 	$t3, $t4,for
	nop
	addi 	$t3,$0,0	# sets $t3 0 so the nestled loop can run again
	
	j	here
	nop
	
while:	
	jr 	$ra
	nop
	

time2string:
	# input: $a0 - write string to this addr
	# input: $a1 - input, ignore last 16 bits, the rest contains NBCD coded digits
	
	PUSH $ra
	PUSH $s4
	PUSH $s3
	add $s3, $a0, $zero	 # copy addres to $t3
	add $s4, $a1, $zero 	 # copy natrual binary code decimal to $t4
	
	sra  $a0, $s4, 12 	 # argument to hexasc, NBCD shifted to the right 12 bits
	jal  hexasc
	nop
	andi $v0, $v0, 0x7F      # mask everything except 7 first bits
	sb   $v0, 0($s3)	 # write byte
	
	sra  $a0, $s4, 8	 # argument to hexasc, NBCD shifted to the right 8 bits
	jal  hexasc
	nop
	andi $v0, $v0, 0x7F      # mask everything except 7 first bits
	sb   $v0, 1($s3)	 # write byte
	
	addi $v0, $zero, 0x3A     # put colon char into register
	sb   $v0, 2($s3)         # write colon char
	
	sra  $a0, $s4, 4	 # argument to hexasc, NBCD shifted to the right 4 bits
	jal  hexasc
	nop
	andi $v0, $v0, 0x7F      # mask everything except 7 first bits
	sb   $v0, 3($s3)	 # write byte
	
	sra  $a0, $s4, 0	 # argument to hexasc, NBCD shifted to the right 0 bits
	jal  hexasc
	nop
	addi $t1,$0,0x39
	#beq  $v0,$t1, lazy
	nop
	andi $v0, $v0, 0x7F      # mask everything except 7 first bits
	sb   $v0, 4($s3)	 # write byte	
	
	
	
	andi $v0, $v0, 0x7F     # mask everything except 7 first bits
	sw    $v0, 4($s3)
	nop
	j	end
	nop
lazy:	
	
	addi $v0, $zero, 0x4E     
	sb   $v0, 4($s3)
	addi $v0, $zero, 0x49     
	sb   $v0, 5($s3)
	addi $v0, $zero, 0x4E     
	sb   $v0, 6($s3)
	addi $v0, $zero, 0x45     
	sb   $v0, 7($s3)
	andi $v0, $v0, 0x83     # mask everything except 7 first bits
	sw    $v0, 8($s3)
	nop
	
	
end:	
	POP $s3
	POP $s4
	POP $ra
	
	jr   $ra
	nop

