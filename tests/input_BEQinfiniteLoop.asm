# i am a comment
test:
addi $t0, $t0, 1
j frog  
addi $t0, $t0, 5
srl $t0, $t0, 1
frog:
beq $t0, $t0, test #  infinite loop
