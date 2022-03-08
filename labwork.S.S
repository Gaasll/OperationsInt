  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text

.global hexasc
.global delay 
.global time2string
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0,25
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

  # you can write your code for subroutine "hexasc" below this line
  #




hexasc:
	andi	$a2, $a2, 0xF	# remove all bits except the 4 last bits.
	addi	$v0, $0, 0x30
	addi	$t0, $0, 0x9
	ble	$a2, $t0, L1
	
	nop
	
	addi	$v0, $v0, 0x7
	
L1:

	add	$v0, $a2, $v0
	
	jr	$ra
	
	nop
  
delay:
	ble	$a0, $0, stop
	nop
	
	sub	$a0, $a0, 1
	addi	$t6, $0, 4711
	addi	$t7, $0, 0
	
	forloop:
		addi	$t7, $t7, 1
		ble	$t7, $t6, forloop
		nop
	j delay
	nop
stop:
	jr $ra
	nop
	
time2string:
	
	PUSH 	$s1
	PUSH 	$s2
	PUSH 	$s3
	PUSH	$s5
	
	add	$s5, $0, $a1	# Ental i sekunder / alla
	srl	$s1, $a1, 4	# Tiotal i sekunder / allt utom ental i sekunder
	srl	$s2, $a1, 8	# Ental i minuter / både
	srl	$s3, $a1, 12	# Ental i sekunder / alla
	
	move	$a2, $s3
	PUSH	$ra
	jal	hexasc
	nop
	POP	$ra
	sb	$v0, 0($a0)
	
	move	$a2, $s2
	PUSH	$ra
	jal	hexasc
	nop
	POP 	$ra
	sb	$v0, 1($a0)
	
	addi	$s4, $0, 0x3A
	sb	$s4, 2($a0)
		
	move	$a2, $s1
	PUSH	$ra
	jal	hexasc
	nop
	POP 	$ra
	sb	$v0, 3($a0)
	
		
	move	$a2, $s5
	PUSH	$ra
	jal	hexasc
	nop
	POP 	$ra
	
	beq	$v0, 0x32,  TWO
normal:
	sb	$v0, 4($a0)
	addi	$s4, $0, 0x0
	sb	$s4, 5($a0)
	j end
	
	
TWO:
	addi	$s4, $0, 84
	sb	$s4, 4($a0)
	
	addi	$s4, $0, 87
	sb	$s4, 5($a0)
	
	addi	$s4, $0, 79
	sb	$s4, 6($a0)
	
	addi	$s4, $0, 0x0
	sb	$s4, 7($a0)
	j 	end
	
	

	
	end:
	
	POP	$s1
	POP	$s2
	POP	$s3
	POP	$s5
	
	jr	$ra
	nop
