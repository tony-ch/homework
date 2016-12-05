.data:
glb_cintm: .word 10
glb_cint1: .word 1
glb_cint0: .word 0
glb_cchy: .word 'y'
glb_cchn: .word 'n'
glb_cch0: .word '0'
glb_vara: .word
glb_varm: .word
glb_intarr: .space 40
glb_varn: .word
glb_chara: .word
glb_chararr: .space 40
glb_charb: .word
glb_charc: .word
#str
str_0: .asciiz " input a num: "
str_1: .asciiz " input a char: "
str_2: .asciiz "globol const: "
str_3: .asciiz " cint0 "
str_4: .asciiz " cint1 "
str_5: .asciiz " cchy "
str_6: .asciiz " cchn "
str_7: .asciiz " cch0 "
str_8: .asciiz "read global var, three int three char:"
str_9: .asciiz " var1 "
str_10: .asciiz " var2 "
str_11: .asciiz " var3 "
str_12: .asciiz " char1 "
str_13: .asciiz " char2 "
str_14: .asciiz " char3 "
str_15: .asciiz " sum of var int: "
str_16: .asciiz " some expr: "
str_17: .asciiz " exp1 --10: "
str_18: .asciiz " exp2 -10*(-10--4)/4++1*-9--6/-3: "
str_19: .asciiz " exp3 -fib(fib(+4)+-1)-+1: "
str_20: .asciiz " expr with arr "
str_21: .asciiz " set arr "
str_22: .asciiz " "
str_23: .asciiz " fib(intarr[3]) "
str_24: .asciiz " intarr[fib(2)] "
str_25: .asciiz " intarr[intarr[2]-2] "
str_26: .asciiz "input arg1: "
str_27: .asciiz " input op:(+-*/)(q:end) "
str_28: .asciiz " input arg2"
str_29: .asciiz " input arg2"
str_30: .asciiz " input arg2"
str_31: .asciiz " input arg2"
str_32: .asciiz "result: "
str_33: .asciiz " int arr(i) or char arr(c)? input i or c "
str_34: .asciiz " input int arr content to sort. "
str_35: .asciiz " int after sort: "
str_36: .asciiz " "
str_37: .asciiz " input char arr content to sort. "
str_38: .asciiz " char after sort: "
str_39: .asciiz " "
str_40: .asciiz " invalid input. "
str_41: .asciiz " input cmd: a:arr,c:cal,e:empty,f:fib,s:sum,q:quit "
str_42: .asciiz " invalid cmd. "
.text:
j func_main
nop
func_fib:
sw $ra,20($fp)
lw $t1,0($fp) # load n
lw $t2,glb_cint0
sle $t0,$t1,$t2
sw $t0,56($fp) # sw &0
beq $t0,$0,label_0 #code 17
nop
lw $t0,glb_cint0
add $v0,$0,$t0
j end_func_fib
nop
j label_1#19
nop
label_0:#20
label_1:#21
lw $t1,0($fp) # load n
lw $t2,glb_cint1
seq $t0,$t1,$t2
sw $t0,60($fp) # sw &1
beq $t0,$0,label_2 #code 23
nop
lw $t0,glb_cint1
add $v0,$0,$t0
j end_func_fib
nop
j label_3#25
nop
label_2:#26
label_3:#27
li $t0,1#li &3
lw $t2,0($fp) # load n
sub $t1,$t2,$t0
addi,$sp,$sp,-92
sw $t1,0($sp)
sw $t0,68($fp) # sw &3
sw $t1,72($fp) # sw &4
sw $fp,16($sp)
add $fp,$0,$sp
jal func_fib
nop
add $t0,$0,$v0# ret value from call of func fib
li $t1,2#li &6
lw $t3,0($fp) # load n
sub $t2,$t3,$t1
addi,$sp,$sp,-92
sw $t2,0($sp)
sw $t0,64($fp) # sw &2
sw $t1,80($fp) # sw &6
sw $t2,84($fp) # sw &7
sw $fp,16($sp)
add $fp,$0,$sp
jal func_fib
nop
add $t0,$0,$v0# ret value from call of func fib
lw $t2,64($fp) # load &2
add $t1,$t2,$t0
add $v0,$0,$t1
sw $t0,76($fp) # sw &5
sw $t1,88($fp) # sw &8
j end_func_fib
nop
end_func_fib:
add $sp,$0,$fp
lw $ra,20($fp)
lw $fp,16($fp)
addi,$sp,$sp,92
jr $ra
nop
func_getint:
sw $ra,20($fp)
# var temp code 40
la $a0,str_0
addi $v0,$0,4
syscall
addi $v0,$0,5
syscall#read 42
add $t0,$0,$v0
add $v0,$0,$t0
sw $t0,56($fp) # sw temp
j end_func_getint
nop
end_func_getint:
add $sp,$0,$fp
lw $ra,20($fp)
lw $fp,16($fp)
addi,$sp,$sp,60
jr $ra
nop
func_sum:
sw $ra,24($fp)
lw $t1,0($fp) # load a
lw $t2,4($fp) # load b
add $t0,$t1,$t2
lw $t4,8($fp) # load c
add $t3,$t0,$t4
lw $t6,12($fp) # load d
add $t5,$t3,$t6
sw $t0,56($fp) # sw &9
lw $t0,16($fp) # load e
add $t7,$t5,$t0
add $v0,$0,$t7
sw $t3,60($fp) # sw &10
sw $t5,64($fp) # sw &11
sw $t7,68($fp) # sw &12
j end_func_sum
nop
end_func_sum:
add $sp,$0,$fp
lw $ra,24($fp)
lw $fp,20($fp)
addi,$sp,$sp,72
jr $ra
nop
func_empty:
sw $ra,20($fp)
end_func_empty:
add $sp,$0,$fp
lw $ra,20($fp)
lw $fp,16($fp)
addi,$sp,$sp,56
jr $ra
nop
func_getch:
sw $ra,20($fp)
# var ch code 60
la $a0,str_1
addi $v0,$0,4
syscall
addi $v0,$0,12
syscall#read 62
add $t0,$0,$v0
add $v0,$0,$t0
sw $t0,56($fp) # sw ch
j end_func_getch
nop
end_func_getch:
add $sp,$0,$fp
lw $ra,20($fp)
lw $fp,16($fp)
addi,$sp,$sp,60
jr $ra
nop
func_cal:
sw $ra,20($fp)
# var m code 66
# var n code 67
# var i code 68
# var op code 69
la $a0,str_2
addi $v0,$0,4
syscall
la $a0,str_3
addi $v0,$0,4
syscall
lw $t0,glb_cint0
add $a0,$0,$t0
addi $v0,$0,1
syscall
la $a0,str_4
addi $v0,$0,4
syscall
lw $t1,glb_cint1
add $a0,$0,$t1
addi $v0,$0,1
syscall
la $a0,str_5
addi $v0,$0,4
syscall
lw $t2,glb_cchy
add $a0,$0,$t2
addi $v0,$0,11
syscall
la $a0,str_6
addi $v0,$0,4
syscall
lw $t3,glb_cchn
add $a0,$0,$t3
addi $v0,$0,11
syscall
la $a0,str_7
addi $v0,$0,4
syscall
lw $t4,glb_cch0
add $a0,$0,$t4
addi $v0,$0,11
syscall
la $a0,str_8
addi $v0,$0,4
syscall
addi $v0,$0,5
syscall#read 77
add $t5,$0,$v0
addi $v0,$0,5
syscall#read 78
add $t6,$0,$v0
addi $v0,$0,5
syscall#read 79
add $t7,$0,$v0
addi $v0,$0,12
syscall#read 80
add $t0,$0,$v0
addi $v0,$0,12
syscall#read 81
add $t1,$0,$v0
addi $v0,$0,12
syscall#read 82
add $t2,$0,$v0
la $a0,str_9
addi $v0,$0,4
syscall
add $a0,$0,$t5
addi $v0,$0,1
syscall
la $a0,str_10
addi $v0,$0,4
syscall
add $a0,$0,$t6
addi $v0,$0,1
syscall
la $a0,str_11
addi $v0,$0,4
syscall
add $a0,$0,$t7
addi $v0,$0,1
syscall
la $a0,str_12
addi $v0,$0,4
syscall
add $a0,$0,$t0
addi $v0,$0,11
syscall
la $a0,str_13
addi $v0,$0,4
syscall
add $a0,$0,$t1
addi $v0,$0,11
syscall
la $a0,str_14
addi $v0,$0,4
syscall
add $a0,$0,$t2
addi $v0,$0,11
syscall
li $t3,0#li &14
li $t4,0#li &15
addi,$sp,$sp,-72
sw $t5,0($sp)
sw $t6,4($sp)
sw $t7,8($sp)
sw $t3,12($sp)
sw $t4,16($sp)
sw $t0,glb_chara
sw $t1,glb_charb
sw $t2,glb_charc
sw $t3,76($fp) # sw &14
sw $t4,80($fp) # sw &15
sw $t5,glb_vara
sw $t6,glb_varm
sw $t7,glb_varn
sw $fp,20($sp)
add $fp,$0,$sp
jal func_sum
nop
add $t0,$0,$v0# ret value from call of func sum
la $a0,str_15
addi $v0,$0,4
syscall
add $a0,$0,$t0
addi $v0,$0,1
syscall
la $a0,str_16
addi $v0,$0,4
syscall
li $t1,-10#li &16
addi $t3,$0,0#src1
sub $t2,$t3,$t1
la $a0,str_17
addi $v0,$0,4
syscall
add $a0,$0,$t2
addi $v0,$0,1
syscall
li $t3,10#li &18
li $t4,10#li &19
addi $t6,$0,0#src1
sub $t5,$t6,$t4
li $t7,-4#li &21
sw $t0,72($fp) # sw &13
sub $t0,$t5,$t7
sw $t1,84($fp) # sw &16
mul $t1,$t3,$t0
sw $t2,88($fp) # sw &17
li $t2,4#li &24
sw $t3,92($fp) # sw &18
div $t3,$t1,$t2
sw $t4,96($fp) # sw &19
sw $t5,100($fp) # sw &20
addi $t5,$0,0#src1
sub $t4,$t5,$t3
li $t6,1#li &27
sw $t7,104($fp) # sw &21
li $t7,-9#li &28
sw $t0,108($fp) # sw &22
mul $t0,$t6,$t7
sw $t1,112($fp) # sw &23
add $t1,$t4,$t0
sw $t2,116($fp) # sw &24
li $t2,-6#li &31
sw $t3,120($fp) # sw &25
li $t3,-3#li &32
sw $t4,124($fp) # sw &26
div $t4,$t2,$t3
sub $t5,$t1,$t4
la $a0,str_18
addi $v0,$0,4
syscall
add $a0,$0,$t5
addi $v0,$0,1
syscall
sw $t6,128($fp) # sw &27
li $t6,4#li &37
addi,$sp,$sp,-92
sw $t6,0($sp)
sw $t0,136($fp) # sw &29
sw $t1,140($fp) # sw &30
sw $t2,144($fp) # sw &31
sw $t3,148($fp) # sw &32
sw $t4,152($fp) # sw &33
sw $t5,156($fp) # sw &34
sw $t6,168($fp) # sw &37
sw $t7,132($fp) # sw &28
sw $fp,16($sp)
add $fp,$0,$sp
jal func_fib
nop
add $t0,$0,$v0# ret value from call of func fib
li $t1,-1#li &38
add $t2,$t0,$t1
addi,$sp,$sp,-92
sw $t2,0($sp)
sw $t0,164($fp) # sw &36
sw $t1,172($fp) # sw &38
sw $t2,176($fp) # sw &39
sw $fp,16($sp)
add $fp,$0,$sp
jal func_fib
nop
add $t0,$0,$v0# ret value from call of func fib
addi $t2,$0,0#src1
sub $t1,$t2,$t0
li $t2,1#li &41
sub $t3,$t1,$t2
la $a0,str_19
addi $v0,$0,4
syscall
add $a0,$0,$t3
addi $v0,$0,1
syscall
la $a0,str_20
addi $v0,$0,4
syscall
li $t4,0#li &43
add $t5,$0,$t4 #code 133 i=&43
la $a0,str_21
addi $v0,$0,4
syscall
sw $t0,160($fp) # sw &35
sw $t1,180($fp) # sw &40
sw $t2,184($fp) # sw &41
sw $t3,188($fp) # sw &42
sw $t4,192($fp) # sw &43
sw $t5,64($fp) # sw i
label_4:#135
li $t0,5#li &44
lw $t2,64($fp) # load i
slt $t1,$t2,$t0
sw $t0,196($fp) # sw &44
sw $t1,200($fp) # sw &45
beq $t1,$0,label_5 #code 138
nop
addi,$sp,$sp,-92
lw $v0,64($fp)
sw $v0,0($sp)
sw $fp,16($sp)
add $fp,$0,$sp
jal func_fib
nop
add $t0,$0,$v0# ret value from call of func fib
li $t1,2#li &48
add $t2,$t0,$t1
addi,$sp,$sp,-92
sw $t2,0($sp)
sw $t0,208($fp) # sw &47
sw $t1,212($fp) # sw &48
sw $t2,216($fp) # sw &49
sw $fp,16($sp)
add $fp,$0,$sp
jal func_fib
nop
add $t0,$0,$v0# ret value from call of func fib
li $t1,1#li &50
lw $t3,64($fp) # load i
add $t2,$t3,$t1
mul $t4,$t0,$t2
la $t5,glb_intarr
sll $at,$t3,2 # cal arr offset
add $at,$at,$t5 #cal adr
sw $t4,($at) # set intarr
sll $at,$t3,2
add $at,$at,$t5
lw $t6,0($at)
la $a0,str_22
addi $v0,$0,4
syscall
add $a0,$0,$t6
addi $v0,$0,1
syscall
li $t3,1#li &54
sw $t4,228($fp) # sw &52
lw $t5,64($fp) # load i
add $t4,$t5,$t3
add $t5,$0,$t4 #code 153 i=&55
sw $t0,204($fp) # sw &46
sw $t1,220($fp) # sw &50
sw $t2,224($fp) # sw &51
sw $t3,236($fp) # sw &54
sw $t4,240($fp) # sw &55
sw $t5,64($fp) # sw i
sw $t6,232($fp) # sw &53
j label_4#154
nop
label_5:#155
li $t0,3#li &57
la $t2,glb_intarr
sll $at,$t0,2
add $at,$at,$t2
lw $t1,0($at)
sw $t0,248($fp) # sw &57
addi,$sp,$sp,-92
sw $t1,0($sp)
sw $t1,252($fp) # sw &58
sw $fp,16($sp)
add $fp,$0,$sp
jal func_fib
nop
add $t0,$0,$v0# ret value from call of func fib
la $a0,str_23
addi $v0,$0,4
syscall
add $a0,$0,$t0
addi $v0,$0,1
syscall
li $t1,2#li &60
addi,$sp,$sp,-92
sw $t1,0($sp)
sw $t0,244($fp) # sw &56
sw $t1,260($fp) # sw &60
sw $fp,16($sp)
add $fp,$0,$sp
jal func_fib
nop
add $t0,$0,$v0# ret value from call of func fib
la $t2,glb_intarr
sll $at,$t0,2
add $at,$at,$t2
lw $t1,0($at)
sw $t0,256($fp) # sw &59
la $a0,str_24
addi $v0,$0,4
syscall
add $a0,$0,$t1
addi $v0,$0,1
syscall
li $t0,2#li &62
sll $at,$t0,2
add $at,$at,$t2
lw $t3,0($at)
sw $t0,268($fp) # sw &62
li $t0,2#li &64
sub $t4,$t3,$t0
sll $at,$t4,2
add $at,$at,$t2
lw $t5,0($at)
sw $t4,280($fp) # sw &65
la $a0,str_25
addi $v0,$0,4
syscall
add $a0,$0,$t5
addi $v0,$0,1
syscall
la $a0,str_26
addi $v0,$0,4
syscall
addi $v0,$0,5
syscall#read 173
add $t6,$0,$v0
li $t7,43#li &67
sw $t0,276($fp) # sw &64
add $t0,$0,$t7 #code 175 op=&67
sw $t0,68($fp) # sw op
sw $t1,264($fp) # sw &61
sw $t3,272($fp) # sw &63
sw $t5,284($fp) # sw &66
sw $t6,56($fp) # sw m
sw $t7,288($fp) # sw &67
label_6:#176
li $t0,113#li &68
lw $t2,68($fp) # load op
sne $t1,$t2,$t0
sw $t0,292($fp) # sw &68
sw $t1,296($fp) # sw &69
beq $t1,$0,label_14 #code 179
nop
la $a0,str_27
addi $v0,$0,4
syscall
addi $v0,$0,12
syscall#read 181
add $t0,$0,$v0
sw $t0,68($fp) # sw op
j label_12#182
nop
label_7:#183
la $a0,str_28
addi $v0,$0,4
syscall
addi $v0,$0,5
syscall#read 185
add $t0,$0,$v0
lw $t2,56($fp) # load m
add $t1,$t2,$t0
add $t2,$0,$t1 #code 187 m=&70
sw $t0,60($fp) # sw n
sw $t1,300($fp) # sw &70
sw $t2,56($fp) # sw m
j label_13#188
nop
label_8:#189
la $a0,str_29
addi $v0,$0,4
syscall
addi $v0,$0,5
syscall#read 191
add $t0,$0,$v0
lw $t2,56($fp) # load m
mul $t1,$t2,$t0
add $t2,$0,$t1 #code 193 m=&71
sw $t0,60($fp) # sw n
sw $t1,304($fp) # sw &71
sw $t2,56($fp) # sw m
j label_13#194
nop
label_9:#195
la $a0,str_30
addi $v0,$0,4
syscall
addi $v0,$0,5
syscall#read 197
add $t0,$0,$v0
lw $t2,56($fp) # load m
sub $t1,$t2,$t0
add $t2,$0,$t1 #code 199 m=&72
sw $t0,60($fp) # sw n
sw $t1,308($fp) # sw &72
sw $t2,56($fp) # sw m
j label_13#200
nop
label_10:#201
la $a0,str_31
addi $v0,$0,4
syscall
addi $v0,$0,5
syscall#read 203
add $t0,$0,$v0
lw $t2,56($fp) # load m
div $t1,$t2,$t0
add $t2,$0,$t1 #code 205 m=&73
sw $t0,60($fp) # sw n
sw $t1,312($fp) # sw &73
sw $t2,56($fp) # sw m
j label_13#206
nop
label_11:#207
j end_func_cal
nop
j label_13#209
nop
label_12:#210
addi $t1,$0,43#src1
lw $t2,68($fp) # load op
sne $t0,$t1,$t2
sw $t0,316($fp) # sw &74
beq $t0,$0,label_7 #code 212
nop
addi $t1,$0,42#src1
lw $t2,68($fp) # load op
sne $t0,$t1,$t2
sw $t0,320($fp) # sw &75
beq $t0,$0,label_8 #code 214
nop
addi $t1,$0,45#src1
lw $t2,68($fp) # load op
sne $t0,$t1,$t2
sw $t0,324($fp) # sw &76
beq $t0,$0,label_9 #code 216
nop
addi $t1,$0,47#src1
lw $t2,68($fp) # load op
sne $t0,$t1,$t2
sw $t0,328($fp) # sw &77
beq $t0,$0,label_10 #code 218
nop
addi $t1,$0,113#src1
lw $t2,68($fp) # load op
sne $t0,$t1,$t2
sw $t0,332($fp) # sw &78
beq $t0,$0,label_11 #code 220
nop
label_13:#221
la $a0,str_32
addi $v0,$0,4
syscall
lw $t0,56($fp) # load m
add $a0,$0,$t0
addi $v0,$0,1
syscall
j label_6#223
nop
label_14:#224
end_func_cal:
add $sp,$0,$fp
lw $ra,20($fp)
lw $fp,16($fp)
addi,$sp,$sp,336
jr $ra
nop
func_partition:
sw $ra,20($fp)
# var pivot code 230
# var cond1 code 231
# var cond2 code 232
# var i code 233
# var chpivot code 234
li $t0,0#li &79
add $t1,$0,$t0 #code 236 cond1=&79
li $t2,0#li &80
add $t3,$0,$t2 #code 238 cond2=&80
li $t4,105#li &81
lw $t6,0($fp) # load type
seq $t5,$t6,$t4
sw $t0,76($fp) # sw &79
sw $t1,60($fp) # sw cond1
sw $t2,80($fp) # sw &80
sw $t3,64($fp) # sw cond2
sw $t4,84($fp) # sw &81
sw $t5,88($fp) # sw &82
beq $t5,$0,label_15 #code 241
nop
la $t1,glb_intarr
lw $t2,4($fp) # load low
sll $at,$t2,2
add $at,$at,$t1
lw $t0,0($at)
add $t2,$0,$t0 #code 243 pivot=&83
sw $t0,92($fp) # sw &83
sw $t2,56($fp) # sw pivot
j label_16#244
nop
label_15:#245
la $t1,glb_chararr
lw $t2,4($fp) # load low
sll $at,$t2,2
add $at,$at,$t1
lw $t0,0($at)
add $t2,$0,$t0 #code 247 chpivot=&84
sw $t0,96($fp) # sw &84
sw $t2,72($fp) # sw chpivot
label_16:#248
label_17:#249
lw $t1,4($fp) # load low
lw $t2,8($fp) # load high
slt $t0,$t1,$t2
sw $t0,100($fp) # sw &85
beq $t0,$0,label_58 #code 251
nop
li $t0,0#li &86
add $t1,$0,$t0 #code 253 i=&86
lw $t3,4($fp) # load low
lw $t4,8($fp) # load high
slt $t2,$t3,$t4
sw $t0,104($fp) # sw &86
sw $t1,68($fp) # sw i
sw $t2,108($fp) # sw &87
beq $t2,$0,label_18 #code 255
nop
li $t0,1#li &88
add $t1,$0,$t0 #code 257 cond1=&88
sw $t0,112($fp) # sw &88
sw $t1,60($fp) # sw cond1
j label_19#258
nop
label_18:#259
li $t0,0#li &89
add $t1,$0,$t0 #code 261 cond1=&89
sw $t0,116($fp) # sw &89
sw $t1,60($fp) # sw cond1
label_19:#262
li $t0,105#li &90
lw $t2,0($fp) # load type
seq $t1,$t2,$t0
sw $t0,120($fp) # sw &90
sw $t1,124($fp) # sw &91
beq $t1,$0,label_22 #code 265
nop
la $t1,glb_intarr
lw $t2,8($fp) # load high
sll $at,$t2,2
add $at,$at,$t1
lw $t0,0($at)
lw $t3,56($fp) # load pivot
sge $t2,$t0,$t3
sw $t0,128($fp) # sw &92
sw $t2,132($fp) # sw &93
beq $t2,$0,label_20 #code 268
nop
li $t0,1#li &94
add $t1,$0,$t0 #code 270 cond2=&94
sw $t0,136($fp) # sw &94
sw $t1,64($fp) # sw cond2
j label_21#271
nop
label_20:#272
li $t0,0#li &95
add $t1,$0,$t0 #code 274 cond2=&95
sw $t0,140($fp) # sw &95
sw $t1,64($fp) # sw cond2
label_21:#275
j label_25#276
nop
label_22:#277
la $t1,glb_chararr
lw $t2,8($fp) # load high
sll $at,$t2,2
add $at,$at,$t1
lw $t0,0($at)
lw $t3,72($fp) # load chpivot
sge $t2,$t0,$t3
sw $t0,144($fp) # sw &96
sw $t2,148($fp) # sw &97
beq $t2,$0,label_23 #code 280
nop
li $t0,1#li &98
add $t1,$0,$t0 #code 282 cond2=&98
sw $t0,152($fp) # sw &98
sw $t1,64($fp) # sw cond2
j label_24#283
nop
label_23:#284
li $t0,0#li &99
add $t1,$0,$t0 #code 286 cond2=&99
sw $t0,156($fp) # sw &99
sw $t1,64($fp) # sw cond2
label_24:#287
label_25:#288
label_26:#289
lw $t1,60($fp) # load cond1
lw $t2,64($fp) # load cond2
add $t0,$t1,$t2
li $t3,2#li &101
seq $t4,$t0,$t3
sw $t0,160($fp) # sw &100
sw $t3,164($fp) # sw &101
sw $t4,168($fp) # sw &102
beq $t4,$0,label_35 #code 293
nop
li $t0,1#li &103
lw $t2,8($fp) # load high
sub $t1,$t2,$t0
add $t2,$0,$t1 #code 296 high=&104
lw $t4,4($fp) # load low
slt $t3,$t4,$t2
sw $t0,172($fp) # sw &103
sw $t1,176($fp) # sw &104
sw $t2,8($fp) # sw high
sw $t3,180($fp) # sw &105
beq $t3,$0,label_27 #code 298
nop
li $t0,1#li &106
add $t1,$0,$t0 #code 300 cond1=&106
sw $t0,184($fp) # sw &106
sw $t1,60($fp) # sw cond1
j label_28#301
nop
label_27:#302
li $t0,0#li &107
add $t1,$0,$t0 #code 304 cond1=&107
sw $t0,188($fp) # sw &107
sw $t1,60($fp) # sw cond1
label_28:#305
li $t0,105#li &108
lw $t2,0($fp) # load type
seq $t1,$t2,$t0
sw $t0,192($fp) # sw &108
sw $t1,196($fp) # sw &109
beq $t1,$0,label_31 #code 308
nop
la $t1,glb_intarr
lw $t2,8($fp) # load high
sll $at,$t2,2
add $at,$at,$t1
lw $t0,0($at)
lw $t3,56($fp) # load pivot
sge $t2,$t0,$t3
sw $t0,200($fp) # sw &110
sw $t2,204($fp) # sw &111
beq $t2,$0,label_29 #code 311
nop
li $t0,1#li &112
add $t1,$0,$t0 #code 313 cond2=&112
sw $t0,208($fp) # sw &112
sw $t1,64($fp) # sw cond2
j label_30#314
nop
label_29:#315
li $t0,0#li &113
add $t1,$0,$t0 #code 317 cond2=&113
sw $t0,212($fp) # sw &113
sw $t1,64($fp) # sw cond2
label_30:#318
j label_34#319
nop
label_31:#320
la $t1,glb_chararr
lw $t2,8($fp) # load high
sll $at,$t2,2
add $at,$at,$t1
lw $t0,0($at)
lw $t3,72($fp) # load chpivot
sge $t2,$t0,$t3
sw $t0,216($fp) # sw &114
sw $t2,220($fp) # sw &115
beq $t2,$0,label_32 #code 323
nop
li $t0,1#li &116
add $t1,$0,$t0 #code 325 cond2=&116
sw $t0,224($fp) # sw &116
sw $t1,64($fp) # sw cond2
j label_33#326
nop
label_32:#327
li $t0,0#li &117
add $t1,$0,$t0 #code 329 cond2=&117
sw $t0,228($fp) # sw &117
sw $t1,64($fp) # sw cond2
label_33:#330
label_34:#331
j label_26#332
nop
label_35:#333
li $t0,105#li &118
lw $t2,0($fp) # load type
seq $t1,$t2,$t0
sw $t0,232($fp) # sw &118
sw $t1,236($fp) # sw &119
beq $t1,$0,label_36 #code 336
nop
la $t1,glb_intarr
lw $t2,8($fp) # load high
sll $at,$t2,2
add $at,$at,$t1
lw $t0,0($at)
lw $t2,4($fp) # load low
sll $at,$t2,2 # cal arr offset
add $at,$at,$t1 #cal adr
sw $t0,($at) # set intarr
sw $t0,240($fp) # sw &120
j label_37#339
nop
label_36:#340
la $t1,glb_chararr
lw $t2,8($fp) # load high
sll $at,$t2,2
add $at,$at,$t1
lw $t0,0($at)
lw $t2,4($fp) # load low
sll $at,$t2,2 # cal arr offset
add $at,$at,$t1 #cal adr
sw $t0,($at) # set chararr
sw $t0,244($fp) # sw &121
label_37:#343
li $t0,0#li &122
add $t1,$0,$t0 #code 345 cond1=&122
li $t2,0#li &123
add $t3,$0,$t2 #code 347 cond2=&123
lw $t5,4($fp) # load low
lw $t6,8($fp) # load high
slt $t4,$t5,$t6
sw $t0,248($fp) # sw &122
sw $t1,60($fp) # sw cond1
sw $t2,252($fp) # sw &123
sw $t3,64($fp) # sw cond2
sw $t4,256($fp) # sw &124
beq $t4,$0,label_38 #code 349
nop
li $t0,1#li &125
add $t1,$0,$t0 #code 351 cond1=&125
sw $t0,260($fp) # sw &125
sw $t1,60($fp) # sw cond1
j label_39#352
nop
label_38:#353
li $t0,0#li &126
add $t1,$0,$t0 #code 355 cond1=&126
sw $t0,264($fp) # sw &126
sw $t1,60($fp) # sw cond1
label_39:#356
li $t0,105#li &127
lw $t2,0($fp) # load type
seq $t1,$t2,$t0
sw $t0,268($fp) # sw &127
sw $t1,272($fp) # sw &128
beq $t1,$0,label_42 #code 359
nop
la $t1,glb_intarr
lw $t2,4($fp) # load low
sll $at,$t2,2
add $at,$at,$t1
lw $t0,0($at)
lw $t3,56($fp) # load pivot
sle $t2,$t0,$t3
sw $t0,276($fp) # sw &129
sw $t2,280($fp) # sw &130
beq $t2,$0,label_40 #code 362
nop
li $t0,1#li &131
add $t1,$0,$t0 #code 364 cond2=&131
sw $t0,284($fp) # sw &131
sw $t1,64($fp) # sw cond2
j label_41#365
nop
label_40:#366
li $t0,0#li &132
add $t1,$0,$t0 #code 368 cond2=&132
sw $t0,288($fp) # sw &132
sw $t1,64($fp) # sw cond2
label_41:#369
j label_45#370
nop
label_42:#371
la $t1,glb_chararr
lw $t2,4($fp) # load low
sll $at,$t2,2
add $at,$at,$t1
lw $t0,0($at)
lw $t3,72($fp) # load chpivot
sle $t2,$t0,$t3
sw $t0,292($fp) # sw &133
sw $t2,296($fp) # sw &134
beq $t2,$0,label_43 #code 374
nop
li $t0,1#li &135
add $t1,$0,$t0 #code 376 cond2=&135
sw $t0,300($fp) # sw &135
sw $t1,64($fp) # sw cond2
j label_44#377
nop
label_43:#378
li $t0,0#li &136
add $t1,$0,$t0 #code 380 cond2=&136
sw $t0,304($fp) # sw &136
sw $t1,64($fp) # sw cond2
label_44:#381
label_45:#382
label_46:#383
lw $t1,60($fp) # load cond1
lw $t2,64($fp) # load cond2
add $t0,$t1,$t2
li $t3,2#li &138
seq $t4,$t0,$t3
sw $t0,308($fp) # sw &137
sw $t3,312($fp) # sw &138
sw $t4,316($fp) # sw &139
beq $t4,$0,label_55 #code 387
nop
li $t0,1#li &140
lw $t2,4($fp) # load low
add $t1,$t2,$t0
add $t2,$0,$t1 #code 390 low=&141
lw $t4,8($fp) # load high
slt $t3,$t2,$t4
sw $t0,320($fp) # sw &140
sw $t1,324($fp) # sw &141
sw $t2,4($fp) # sw low
sw $t3,328($fp) # sw &142
beq $t3,$0,label_47 #code 392
nop
li $t0,1#li &143
add $t1,$0,$t0 #code 394 cond1=&143
sw $t0,332($fp) # sw &143
sw $t1,60($fp) # sw cond1
j label_48#395
nop
label_47:#396
li $t0,0#li &144
add $t1,$0,$t0 #code 398 cond1=&144
sw $t0,336($fp) # sw &144
sw $t1,60($fp) # sw cond1
label_48:#399
li $t0,105#li &145
lw $t2,0($fp) # load type
seq $t1,$t2,$t0
sw $t0,340($fp) # sw &145
sw $t1,344($fp) # sw &146
beq $t1,$0,label_51 #code 402
nop
la $t1,glb_intarr
lw $t2,4($fp) # load low
sll $at,$t2,2
add $at,$at,$t1
lw $t0,0($at)
lw $t3,56($fp) # load pivot
sle $t2,$t0,$t3
sw $t0,348($fp) # sw &147
sw $t2,352($fp) # sw &148
beq $t2,$0,label_49 #code 405
nop
li $t0,1#li &149
add $t1,$0,$t0 #code 407 cond2=&149
sw $t0,356($fp) # sw &149
sw $t1,64($fp) # sw cond2
j label_50#408
nop
label_49:#409
li $t0,0#li &150
add $t1,$0,$t0 #code 411 cond2=&150
sw $t0,360($fp) # sw &150
sw $t1,64($fp) # sw cond2
label_50:#412
j label_54#413
nop
label_51:#414
la $t1,glb_chararr
lw $t2,4($fp) # load low
sll $at,$t2,2
add $at,$at,$t1
lw $t0,0($at)
lw $t3,72($fp) # load chpivot
sle $t2,$t0,$t3
sw $t0,364($fp) # sw &151
sw $t2,368($fp) # sw &152
beq $t2,$0,label_52 #code 417
nop
li $t0,1#li &153
add $t1,$0,$t0 #code 419 cond2=&153
sw $t0,372($fp) # sw &153
sw $t1,64($fp) # sw cond2
j label_53#420
nop
label_52:#421
li $t0,0#li &154
add $t1,$0,$t0 #code 423 cond2=&154
sw $t0,376($fp) # sw &154
sw $t1,64($fp) # sw cond2
label_53:#424
label_54:#425
j label_46#426
nop
label_55:#427
li $t0,105#li &155
lw $t2,0($fp) # load type
seq $t1,$t2,$t0
sw $t0,380($fp) # sw &155
sw $t1,384($fp) # sw &156
beq $t1,$0,label_56 #code 430
nop
la $t1,glb_intarr
lw $t2,4($fp) # load low
sll $at,$t2,2
add $at,$at,$t1
lw $t0,0($at)
lw $t2,8($fp) # load high
sll $at,$t2,2 # cal arr offset
add $at,$at,$t1 #cal adr
sw $t0,($at) # set intarr
sw $t0,388($fp) # sw &157
j label_57#433
nop
label_56:#434
la $t1,glb_chararr
lw $t2,4($fp) # load low
sll $at,$t2,2
add $at,$at,$t1
lw $t0,0($at)
lw $t2,8($fp) # load high
sll $at,$t2,2 # cal arr offset
add $at,$at,$t1 #cal adr
sw $t0,($at) # set chararr
sw $t0,392($fp) # sw &158
label_57:#437
j label_17#438
nop
label_58:#439
li $t0,105#li &159
lw $t2,0($fp) # load type
seq $t1,$t2,$t0
sw $t0,396($fp) # sw &159
sw $t1,400($fp) # sw &160
beq $t1,$0,label_59 #code 442
nop
lw $t0,56($fp) # load pivot
la $t1,glb_intarr
lw $t2,4($fp) # load low
sll $at,$t2,2 # cal arr offset
add $at,$at,$t1 #cal adr
sw $t0,($at) # set intarr
j label_60#444
nop
label_59:#445
lw $t0,72($fp) # load chpivot
la $t1,glb_chararr
lw $t2,4($fp) # load low
sll $at,$t2,2 # cal arr offset
add $at,$at,$t1 #cal adr
sw $t0,($at) # set chararr
label_60:#447
lw $t0,4($fp) # load low
add $v0,$0,$t0
j end_func_partition
nop
end_func_partition:
add $sp,$0,$fp
lw $ra,20($fp)
lw $fp,16($fp)
addi,$sp,$sp,404
jr $ra
nop
func_qsort:
sw $ra,20($fp)
# var loc code 454
li $t0,0#li &161
add $t1,$0,$t0 #code 456 loc=&161
lw $t3,4($fp) # load low
lw $t4,8($fp) # load high
slt $t2,$t3,$t4
sw $t0,60($fp) # sw &161
sw $t1,56($fp) # sw loc
sw $t2,64($fp) # sw &162
beq $t2,$0,label_61 #code 458
nop
addi,$sp,$sp,-404
lw $v0,0($fp)
sw $v0,0($sp)
lw $v0,4($fp)
sw $v0,4($sp)
lw $v0,8($fp)
sw $v0,8($sp)
sw $fp,16($sp)
add $fp,$0,$sp
jal func_partition
nop
add $t0,$0,$v0# ret value from call of func partition
add $t1,$0,$t0 #code 463 loc=&163
li $t2,1#li &164
sub $t3,$t1,$t2
addi,$sp,$sp,-88
lw $v0,0($fp)
sw $v0,0($sp)
lw $v0,4($fp)
sw $v0,4($sp)
sw $t3,8($sp)
sw $t0,68($fp) # sw &163
sw $t1,56($fp) # sw loc
sw $t2,72($fp) # sw &164
sw $t3,76($fp) # sw &165
sw $fp,16($sp)
add $fp,$0,$sp
jal func_qsort
nop
add $t0,$0,$v0# ret value from call of func qsort
li $t1,1#li &166
lw $t3,56($fp) # load loc
add $t2,$t3,$t1
addi,$sp,$sp,-88
lw $v0,0($fp)
sw $v0,0($sp)
sw $t2,4($sp)
lw $v0,8($fp)
sw $v0,8($sp)
sw $t0,glb_cintm
sw $t1,80($fp) # sw &166
sw $t2,84($fp) # sw &167
sw $fp,16($sp)
add $fp,$0,$sp
jal func_qsort
nop
add $t0,$0,$v0# ret value from call of func qsort
sw $t0,glb_cintm
j label_62#476
nop
label_61:#477
label_62:#478
j end_func_qsort
nop
end_func_qsort:
add $sp,$0,$fp
lw $ra,20($fp)
lw $fp,16($fp)
addi,$sp,$sp,88
jr $ra
nop
func_arr:
sw $ra,20($fp)
# var i code 482
# var type code 483
la $a0,str_33
addi $v0,$0,4
syscall
addi $v0,$0,12
syscall#read 485
add $t0,$0,$v0
li $t1,105#li &168
seq $t2,$t0,$t1
sw $t0,60($fp) # sw type
sw $t1,64($fp) # sw &168
sw $t2,68($fp) # sw &169
beq $t2,$0,label_67 #code 488
nop
la $a0,str_34
addi $v0,$0,4
syscall
li $t0,0#li &170
add $t1,$0,$t0 #code 491 i=&170
sw $t0,72($fp) # sw &170
sw $t1,56($fp) # sw i
label_63:#492
li $t0,10#li &171
lw $t2,56($fp) # load i
slt $t1,$t2,$t0
sw $t0,76($fp) # sw &171
sw $t1,80($fp) # sw &172
beq $t1,$0,label_64 #code 495
nop
addi,$sp,$sp,-60
sw $fp,16($sp)
add $fp,$0,$sp
jal func_getint
nop
add $t0,$0,$v0# ret value from call of func getint
la $t1,glb_intarr
lw $t2,56($fp) # load i
sll $at,$t2,2 # cal arr offset
add $at,$at,$t1 #cal adr
sw $t0,($at) # set intarr
li $t2,1#li &174
lw $t4,56($fp) # load i
add $t3,$t4,$t2
add $t4,$0,$t3 #code 500 i=&175
sw $t0,84($fp) # sw &173
sw $t2,88($fp) # sw &174
sw $t3,92($fp) # sw &175
sw $t4,56($fp) # sw i
j label_63#501
nop
label_64:#502
li $t0,105#li &176
li $t1,0#li &177
li $t2,9#li &178
addi,$sp,$sp,-88
sw $t0,0($sp)
sw $t1,4($sp)
sw $t2,8($sp)
sw $t0,96($fp) # sw &176
sw $t1,100($fp) # sw &177
sw $t2,104($fp) # sw &178
sw $fp,16($sp)
add $fp,$0,$sp
jal func_qsort
nop
add $t0,$0,$v0# ret value from call of func qsort
la $a0,str_35
addi $v0,$0,4
syscall
li $t1,0#li &179
add $t2,$0,$t1 #code 512 i=&179
sw $t0,glb_cintm
sw $t1,108($fp) # sw &179
sw $t2,56($fp) # sw i
label_65:#513
li $t0,10#li &180
lw $t2,56($fp) # load i
slt $t1,$t2,$t0
sw $t0,112($fp) # sw &180
sw $t1,116($fp) # sw &181
beq $t1,$0,label_66 #code 516
nop
la $t1,glb_intarr
lw $t2,56($fp) # load i
sll $at,$t2,2
add $at,$at,$t1
lw $t0,0($at)
la $a0,str_36
addi $v0,$0,4
syscall
add $a0,$0,$t0
addi $v0,$0,1
syscall
li $t2,1#li &183
lw $t4,56($fp) # load i
add $t3,$t4,$t2
add $t4,$0,$t3 #code 521 i=&184
sw $t0,120($fp) # sw &182
sw $t2,124($fp) # sw &183
sw $t3,128($fp) # sw &184
sw $t4,56($fp) # sw i
j label_65#522
nop
label_66:#523
j label_74#524
nop
label_67:#525
li $t0,99#li &185
lw $t2,60($fp) # load type
seq $t1,$t2,$t0
sw $t0,132($fp) # sw &185
sw $t1,136($fp) # sw &186
beq $t1,$0,label_72 #code 528
nop
la $a0,str_37
addi $v0,$0,4
syscall
li $t0,0#li &187
add $t1,$0,$t0 #code 531 i=&187
sw $t0,140($fp) # sw &187
sw $t1,56($fp) # sw i
label_68:#532
li $t0,10#li &188
lw $t2,56($fp) # load i
slt $t1,$t2,$t0
sw $t0,144($fp) # sw &188
sw $t1,148($fp) # sw &189
beq $t1,$0,label_69 #code 535
nop
addi,$sp,$sp,-60
sw $fp,16($sp)
add $fp,$0,$sp
jal func_getch
nop
add $t0,$0,$v0# ret value from call of func getch
la $t1,glb_chararr
lw $t2,56($fp) # load i
sll $at,$t2,2 # cal arr offset
add $at,$at,$t1 #cal adr
sw $t0,($at) # set chararr
li $t2,1#li &191
lw $t4,56($fp) # load i
add $t3,$t4,$t2
add $t4,$0,$t3 #code 540 i=&192
sw $t0,152($fp) # sw &190
sw $t2,156($fp) # sw &191
sw $t3,160($fp) # sw &192
sw $t4,56($fp) # sw i
j label_68#541
nop
label_69:#542
li $t0,99#li &193
li $t1,0#li &194
li $t2,9#li &195
addi,$sp,$sp,-88
sw $t0,0($sp)
sw $t1,4($sp)
sw $t2,8($sp)
sw $t0,164($fp) # sw &193
sw $t1,168($fp) # sw &194
sw $t2,172($fp) # sw &195
sw $fp,16($sp)
add $fp,$0,$sp
jal func_qsort
nop
add $t0,$0,$v0# ret value from call of func qsort
la $a0,str_38
addi $v0,$0,4
syscall
li $t1,0#li &196
add $t2,$0,$t1 #code 552 i=&196
sw $t0,glb_cintm
sw $t1,176($fp) # sw &196
sw $t2,56($fp) # sw i
label_70:#553
li $t0,10#li &197
lw $t2,56($fp) # load i
slt $t1,$t2,$t0
sw $t0,180($fp) # sw &197
sw $t1,184($fp) # sw &198
beq $t1,$0,label_71 #code 556
nop
la $t1,glb_chararr
lw $t2,56($fp) # load i
sll $at,$t2,2
add $at,$at,$t1
lw $t0,0($at)
la $a0,str_39
addi $v0,$0,4
syscall
add $a0,$0,$t0
addi $v0,$0,11
syscall
li $t2,1#li &200
lw $t4,56($fp) # load i
add $t3,$t4,$t2
add $t4,$0,$t3 #code 561 i=&201
sw $t0,188($fp) # sw &199
sw $t2,192($fp) # sw &200
sw $t3,196($fp) # sw &201
sw $t4,56($fp) # sw i
j label_70#562
nop
label_71:#563
j label_73#564
nop
label_72:#565
la $a0,str_40
addi $v0,$0,4
syscall
label_73:#567
label_74:#568
j end_func_arr
nop
end_func_arr:
add $sp,$0,$fp
lw $ra,20($fp)
lw $fp,16($fp)
addi,$sp,$sp,200
jr $ra
nop
func_main:
addi,$sp,$sp,-80
add $fp,$0,$sp
# var cmd code 572
# var n code 573
li $t0,43#li &202
add $t1,$0,$t0 #code 575 cmd=&202
sw $t0,12($fp) # sw &202
sw $t1,4($fp) # sw cmd
label_75:#576
li $t0,113#li &203
lw $t2,4($fp) # load cmd
sne $t1,$t2,$t0
sw $t0,16($fp) # sw &203
sw $t1,20($fp) # sw &204
beq $t1,$0,label_85 #code 579
nop
la $a0,str_41
addi $v0,$0,4
syscall
addi $v0,$0,12
syscall#read 581
add $t0,$0,$v0
sw $t0,4($fp) # sw cmd
j label_83#582
nop
label_76:#583
addi,$sp,$sp,-200
sw $fp,16($sp)
add $fp,$0,$sp
jal func_arr
nop
add $t0,$0,$v0# ret value from call of func arr
sw $t0,glb_cintm
j label_84#585
nop
label_77:#586
addi,$sp,$sp,-336
sw $fp,16($sp)
add $fp,$0,$sp
jal func_cal
nop
add $t0,$0,$v0# ret value from call of func cal
sw $t0,glb_cintm
j label_84#588
nop
label_78:#589
addi,$sp,$sp,-56
sw $fp,16($sp)
add $fp,$0,$sp
jal func_empty
nop
add $t0,$0,$v0# ret value from call of func empty
sw $t0,glb_cintm
j label_84#591
nop
label_79:#592
addi,$sp,$sp,-60
sw $fp,16($sp)
add $fp,$0,$sp
jal func_getint
nop
add $t0,$0,$v0# ret value from call of func getint
addi,$sp,$sp,-92
sw $t0,0($sp)
sw $t0,28($fp) # sw &206
sw $fp,16($sp)
add $fp,$0,$sp
jal func_fib
nop
add $t0,$0,$v0# ret value from call of func fib
add $a0,$0,$t0
addi $v0,$0,1
syscall
sw $t0,24($fp) # sw &205
j label_84#597
nop
label_80:#598
addi,$sp,$sp,-60
sw $fp,16($sp)
add $fp,$0,$sp
jal func_getint
nop
add $t0,$0,$v0# ret value from call of func getint
addi,$sp,$sp,-60
sw $t0,36($fp) # sw &208
sw $fp,16($sp)
add $fp,$0,$sp
jal func_getint
nop
add $t0,$0,$v0# ret value from call of func getint
addi,$sp,$sp,-60
sw $t0,40($fp) # sw &209
sw $fp,16($sp)
add $fp,$0,$sp
jal func_getint
nop
add $t0,$0,$v0# ret value from call of func getint
addi,$sp,$sp,-60
sw $t0,44($fp) # sw &210
sw $fp,16($sp)
add $fp,$0,$sp
jal func_getint
nop
add $t0,$0,$v0# ret value from call of func getint
addi,$sp,$sp,-60
sw $t0,48($fp) # sw &211
sw $fp,16($sp)
add $fp,$0,$sp
jal func_getint
nop
add $t0,$0,$v0# ret value from call of func getint
addi,$sp,$sp,-72
lw $v0,36($fp)
sw $v0,0($sp)
lw $v0,40($fp)
sw $v0,4($sp)
lw $v0,44($fp)
sw $v0,8($sp)
lw $v0,48($fp)
sw $v0,12($sp)
sw $t0,16($sp)
sw $t0,52($fp) # sw &212
sw $fp,20($sp)
add $fp,$0,$sp
jal func_sum
nop
add $t0,$0,$v0# ret value from call of func sum
add $a0,$0,$t0
addi $v0,$0,1
syscall
sw $t0,32($fp) # sw &207
j label_84#611
nop
label_81:#612
j end
j label_84#614
nop
label_82:#615
la $a0,str_42
addi $v0,$0,4
syscall
j label_84#617
nop
label_83:#618
addi $t1,$0,97#src1
lw $t2,4($fp) # load cmd
sne $t0,$t1,$t2
sw $t0,56($fp) # sw &213
beq $t0,$0,label_76 #code 620
nop
addi $t1,$0,99#src1
lw $t2,4($fp) # load cmd
sne $t0,$t1,$t2
sw $t0,60($fp) # sw &214
beq $t0,$0,label_77 #code 622
nop
addi $t1,$0,101#src1
lw $t2,4($fp) # load cmd
sne $t0,$t1,$t2
sw $t0,64($fp) # sw &215
beq $t0,$0,label_78 #code 624
nop
addi $t1,$0,102#src1
lw $t2,4($fp) # load cmd
sne $t0,$t1,$t2
sw $t0,68($fp) # sw &216
beq $t0,$0,label_79 #code 626
nop
addi $t1,$0,115#src1
lw $t2,4($fp) # load cmd
sne $t0,$t1,$t2
sw $t0,72($fp) # sw &217
beq $t0,$0,label_80 #code 628
nop
addi $t1,$0,113#src1
lw $t2,4($fp) # load cmd
sne $t0,$t1,$t2
sw $t0,76($fp) # sw &218
beq $t0,$0,label_81 #code 630
nop
j label_82#631
nop
label_84:#632
j label_75#633
nop
label_85:#634
addi,$sp,$sp,80
end:
