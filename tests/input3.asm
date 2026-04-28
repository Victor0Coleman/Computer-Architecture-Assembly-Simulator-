addi $t1, $t1, 4
sw $t1, 4($t1)
addi $t2, $t2, 4
lw $t3, 0($t2)
mul $t1, $t2, $t3
sw $t1, 4($t3)
