addi $t1, $t1, 4    # store 4 into $t1
NOP
NOP
NOP
NOP
sw $t1, 4($t1)  # store $t1 (4) into address 4+4  (8)
NOP
NOP
NOP
NOP
addi $t2, $t2, 4    # store 4 into $t2
NOP
NOP
NOP
NOP
lw $t3, 0($t2)  # load 0 (from address 4) to $t3        # 
NOP
NOP
NOP
NOP
mul $t1, $t2, $t3   # multiply $t2 (4) and $t3 (0), store into $t1
NOP
NOP
NOP
NOP
sw $t1, 4($t3)  # store into address 4   the value in $t1 (4)
NOP
NOP
NOP
NOP
