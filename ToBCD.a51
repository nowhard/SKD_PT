$NOMOD51

NAME	TOBCD




?PR?_ToBCD?TOBCD     SEGMENT CODE 
?DT?_ToBCD?TOBCD     SEGMENT DATA OVERLAYABLE 
	PUBLIC	_ToBCD

	RSEG  ?DT?_ToBCD?TOBCD
?_ToBCD?BYTE:
        num?040:   DS   2
	ORG  2
    	bcd_buf?041:   DS   1
; 	

; void ToBCD(unsigned long int num,unsigned char bcd_buf[])

	RSEG  ?PR?_ToBCD?TOBCD
_ToBCD:
	USING	3
			; SOURCE LINE # 3

	MOV  	num?040+01H,R7
	MOV  	num?040,R6



	mov   bcd_buf?041,R1
	mov A,R1
	add A,#04h
	mov R1,A
; ;//---------------------------------
	 

	  acall STORE

	  LOOP1:

	  //inc bcd_buf?041+04h;

	
	  inc @R1
	 // mov   bcd_buf?041+01h,R1
	  
	  clr C

	  mov A,R7;

	  subb A,#LOW(10000)

	  mov	 R7,A

	  mov A,R6;

	  subb A,#HIGH(10000)

	  mov	 R6,A
	  
	  jc BREAK1
	  
	  acall RESTORE
	  
	  jmp LOOP1	 ;cycle

	  BREAK1:
	 
	 
	  //mov   R1,bcd_buf?041+01h
	  dec @R1
	  dec R1
	 // mov   bcd_buf?041+01h,R1 
	 
; ;//---------------------------------

	  acall STORE

	  LOOP2:

	  //inc bcd_buf?041+03h;
	  
	 // mov   R1,bcd_buf?041+01h
	  inc @R1
	 // mov   bcd_buf?041+01h,R1 
	  
	  clr C

	  mov A,R7;

	  subb A,#LOW(1000)

	  mov	 R7,A

	  mov A,R6;

	  subb A,#HIGH(1000)

	  mov	 R6,A
	  
	  jc BREAK2
	  
	  acall RESTORE
	  
	  jmp LOOP2	 ;cycle

	  BREAK2:
	 // mov   R1,bcd_buf?041+01h
	  dec @R1
	  dec R1
	//  mov   bcd_buf?041+01h,R1 
; ;//---------------------------------

	  acall STORE

	  LOOP3:

	  inc @R1


	  clr C

	  mov A,R7;

	  subb A,#LOW(100)

	  mov R7,A

	  mov A,R6;

	  subb A,#HIGH(100)

	  mov	 R6,A
	  
	  jc BREAK3
	  
	  acall RESTORE
	  
	  jmp LOOP3	 ;cycle

	  BREAK3:
	 // mov   R1,bcd_buf?041+01h
	  dec @R1
	  dec R1
	 // mov   bcd_buf?041+01h,R1 
; ;//---------------------------------

	  acall STORE

	  LOOP4:

	  //inc bcd_buf?041+01h;
	  inc @R1
	  clr C

	  mov A,R7;

	  subb A,#LOW(10)

	  mov  R7,A

	  mov A,R6;

	  subb A,#HIGH(10)

	  mov	 R6,A
	  
	  jc BREAK4
	  
	  acall RESTORE
	  
	  jmp LOOP4	 ;cycle

	  BREAK4:
	  dec @R1
	  dec R1
; ;//---------------------------------

	  mov A,num?040+01h
	  mov  @R1,A ; 
; 
	  
	RET  	
; END OF _ToBCD

STORE:
	  //--------store-----------------
	  mov A,num?040+01h;
	  mov  R7,A
	  mov A,num?040;
	  mov  R6,A

	   mov @R1,#00h
	  //--------end_of_store----------
ret

RESTORE:
	  //--------restore-----------------
	  mov A,R7;
	  mov  num?040+01h,A
	  mov A,R6;
	  mov  num?040,A
	  //--------end_of_restore----------
ret

	END
