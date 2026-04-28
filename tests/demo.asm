addi $t0, $zero, 8
addi $t1, $zero, 4
nop
nop
add $t2, $t0, $t1
nop
nop
sub $t3, $t0, $t1
nop
nop
mul $t4, $t0, $t1
nop
nop
and $t5, $t0, $t1
nop
nop
or $t6, $t0, $t1
nop
nop
sll $t7, $t0, 1
nop
nop
srl $s0, $t0, 1
nop
nop
sw $t2, 0($zero)
nop
nop
lw $s1, 0($zero)
nop
nop
beq $t0, $t0, done
addi $s2, $zero, 99
done:
j end
addi $s3, $zero, 99
end:
nop
