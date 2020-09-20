;This is a test for Maman 14 which represents the possible errors in the program

Begin:  dec A1
        add #7
        inc
		.extern A1
X:   cmp  #5,#6
        inc  #3	
        and  r1,r2	
        cmp  r3,,r5	
		
        prn  r1 hello		
X:   clr  r6
        lea  #3,r2
        stop Begin
Y:     .data   200 4		
Z:  .data   100,    	20 ,  -5,300
W:     .data   4,
add:    .data   20
r1:     .data   25
T:  .data   1, +200, -30
U:  .data   300
X:      .entry  Z
        .extern A1
		.entry  STOP
B   .data   4
C1:     .string hello
C2:     .string "ag" blabla
STR1:   .string "a, ,dbc"
ababababababababababababababbabbabababab: .data 1
C3:     .data
C4:    .unable 85, 19
C5:		.data 100
        lea  &C5,r1
		jsr &32
		cmp r1,#-1048576
		cmp r1,#-1048577
STOP:   stop
