// ----------------------------------------------------------------
;	������� ��������� ��������
// ----------------------------------------------------------------
; http://forum.avr.ru/showthread.php?goto=nextoldest&t=36050
; ������ �������� �� ��������� ���������� ������
; @0 -- ���������� ������, @0 = 0...770
; @1 -- ������������ ������� ��� ���������� ��������
.MACRO M_DELAY_CLK_A
.if (@0) > 0                  ; ���� @0 <= 0 -- ��� ��������� ��������
.if (@0) > 770                ; ������ 770 ������ ������, ������� ������
.error "@0 must be less or equal 770"
.endif
.if (@0)/3 > 0                ; ����� �� 3, �.�. 1 �������� ����� -- 3 �����
    ldi     @1, (@0-3)/3
    subi    @1, 1             ; 1 ����
    brcc    (PC-1)            ; 2 �����
    ; �� ��������� ���� brcc ���������� �� 1 ����, �� � ������ ldi �����-����
    ; ��������� 2 �����
.endif
.if (@0)%3 > 0
    nop
.endif
.if (@0)%3 > 1
    nop
.endif
.endif
.ENDMACRO
; ----------
; �������� � ������������ �� ��������� ���������� ������
; @0 -- �������� � ������, @0 = 0...262145
; @1, @2 -- ��������, ������������ ��� ���������� ��������
; �������� ����������� (���������� ������ � ������� �������):
;   @0 = 1...3   -- ��� �����������;
;   @0 = 4...768 -- �� ���� ������;
;   @0 > 768     -- �� ���� ������
.MACRO M_DELAY_CLK
.if (@0) <= 0
    ; ���� @0 <=0 -- ��� ��������� ��������
.elif (@0) == 1                 ; ����� �������� ��������
    nop
.elif (@0) == 2
    nop
    nop
.elif (@0) <= 768               ; �������� ��������
    ldi     @1, (@0-1)/3        ; ���������� �� ��������� 3 � ������� �������
    subi    @1, 1               ; 1 ����
    brcc    (PC-1)              ; 2 �����, ����� 1 �������� ����� -- 3 �����
.elif (@0) <= 262145            ; ������� ��������
    ldi     @1, Low((@0-2)/4)   ; ���������� �� ��������� 4 � ������� �������
    ldi     @2, High((@0-2)/4)  ; "����� 2" -- ����������� ������ ldi
    subi    @1, 1               ; 1 ����
    sbci    @2, 0               ; 1 ����
    brcc    (PC-2)              ; 2 �����, ����� 1 �������� ����� -- 4 �����
.else
.error "@0 must be less or equal 262145"
.endif
.ENDMACRO
; ----------
#define M_US2CLK(t) ((F_CPU-1)*(t)/1000000+1)
; t -- ����� �����������
; ������ "����� 1" � "���� 1"? "���� 1" ������, ��� ����� ������ ����������� �
; ������� �������, �� ���� ���� ��� ������� 1 �� ��� t=1 ����������� �����
; 1 ����. "����� 1" ����� ��� ����, ����� ������� "���� 1" �� ���� ��� "������"
; ������, ���� 1000000, 2000000, � �.�.
; ----------
; �������� �� ��������� ����� ���
; ����� ���� ��������� ��������
; @0 -- �������� � ���;
; @1, @2 -- ��������, ������������ ��� ���������� ��������
.MACRO M_DELAY_US_R
    M_DELAY_CLK     M_US2CLK(@0), @1, @2
.ENDMACRO

; �������� �� ��������� ����� ���
; ������������ �������� �� ���������, ���������� ������� ���
; @0 -- �������� � ���
.MACRO M_DELAY_US
    M_DELAY_CLK     M_US2CLK(@0), R16, R17
.ENDMACRO

; ������ �������� �� ��������� ����� ���
; @0 -- �������� � ���;
; @1 -- ������������ �������
.MACRO M_DELAY_US_RA
    M_DELAY_CLK_A   M_US2CLK(@0), @1
.ENDMACRO

; ������ �������� �� ��������� ����� ���
; ������������ ������� �� ���������, ���������� ������� ���
; @0 -- �������� � ���
.MACRO M_DELAY_US_A
    M_DELAY_CLK_A   M_US2CLK(@0), R16
.ENDMACRO
; ----------
; �������� � ������������ �� ��������� ����� ��� � ������������ ������
; @0 -- �������� � ��� ����� @1 ������
; @2, @3 -- ������������ �������� ��� ���������� ��������
.MACRO M_DELAY_US_RC
    M_DELAY_CLK     M_US2CLK(@0)-@1, @2, @3
.ENDMACRO

; �������� � ������������ �� ��������� ����� ��� � ������������ ������
; ������������ �������� �� ���������, ���������� ������� ���
; @0 -- �������� � ��� ����� @1 ������
.MACRO M_DELAY_US_C
    M_DELAY_CLK     M_US2CLK(@0)-@1, R16, R17
.ENDMACRO
// ----------------------------------------------------------------
;  ������� �������� � ����������� ���� 16 ���
// ----------------------------------------------------------------
	.MACRO	MOVRAM16
		lds @2,(@1+0)
		sts (@0+0),@2
		lds @2,(@1+1)
		sts (@0+1),@2
	.ENDM
	.MACRO	LDX
		ldi		XL,low(@0)
		ldi		XH,High(@0)
	.ENDM

	.MACRO	LDY
		ldi		YL,low(@0)
		ldi		YH,High(@0)
	.ENDM

	.MACRO	LDZ
		ldi		ZL,low(@0)
		ldi		ZH,High(@0)
	.ENDM
	; 
	.MACRO	LDSX
		lds		XL,(@0+0)
		lds		XH,(@0+1)
	.ENDM

	.MACRO	LDSY
		lds		YL,(@0+0)
		lds		YH,(@0+1)
	.ENDM

	.MACRO	LDSZ
		lds		ZL,(@0+0)
		lds		ZH,(@0+1)
	.ENDM
	; 
	.MACRO	STSX
		sts		(@0+0),XL
		sts		(@0+1),XH
	.ENDM

	.MACRO	STSY
		sts		(@0+0),YL
		sts		(@0+1),YH
	.ENDM

	.MACRO	STSZ
		sts		(@0+0),ZL
		sts		(@0+1),ZH
	.ENDM

	.MACRO LDSA
		lds	r22,@0+0
		lds	r23,@0+1
		lds	r24,@0+2
		lds	r25,@0+3
	.ENDM

	.MACRO LDSB
		lds	r18,@0+0
		lds	r19,@0+1
		lds	r20,@0+2
		lds	r21,@0+3
	.ENDM

	.MACRO	STSA
		sts		(@0+0),r22
		sts		(@0+1),r23
		sts		(@0+2),r24
		sts		(@0+3),r25
	.ENDM

	.MACRO	STSB
		sts		(@0+0),r18
		sts		(@0+1),r19
		sts		(@0+2),r20
		sts		(@0+3),r21
	.ENDM
// ----------------------------------------------------------------
	.MACRO	PUSHF
		push r0
		in r0,SREG
		push r0
	.ENDM

	.MACRO	POPF
		pop r0
		out SREG,r0
		pop r0
	.ENDM

	.MACRO PUSHX
		push XL
		push XH
	.ENDM

	.MACRO POPX
		pop XH
		pop XL
	.ENDM

	.MACRO PUSHY
		push YL
		push YH
	.ENDM

	.MACRO POPY
		pop YH
		pop YL
	.ENDM

	.MACRO PUSHZ
		push ZL
		push ZH
	.ENDM

	.MACRO POPZ
		pop ZH
		pop ZL
	.ENDM

	.MACRO PUSHXYZ
		push r26
		push r27
		push r28
		push r29
		push r30
		push r31
	.ENDM

	.MACRO POPXYZ
		pop r31
		pop r30
		pop r29
		pop r28
		pop r27
		pop r26
	.ENDM

	.MACRO PUSHHIGH
		push r16
		push r17
		push r18
		push r19
		push r20
		push r21
		push r22
		push r23
		push r24
		push r25
	.ENDM

	.MACRO POPHIGH
		pop r25
		pop r24
		pop r23
		pop r22
		pop r21
		pop r20
		pop r19
		pop r18
		pop r17
		pop r16
	.ENDM

	.MACRO PUSHLOW
		push r0
		push r1
		push r2
		push r3
		push r4
		push r5
		push r6
		push r7
		push r8
		push r9
		push r10
		push r11
		push r12
		push r13
		push r14
		push r15
	.ENDM

	.MACRO POPLOW
		pop r15
		pop r14
		pop r13
		pop r12
		pop r11
		pop r10
		pop r9
		pop r8
		pop r7
		pop r6
		pop r5
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
	.ENDM


	.MACRO SETB 
		ori @0,(1<<@1)
	.ENDM

	.MACRO CLRB 
		andi @0,~(1<<@1)
	.ENDM
//-----------------------
	.MACRO PUSHB
		push r21
		push r20
		push r19
		push r18
	.ENDM

	.MACRO POPB
		pop r18
		pop r19
		pop r20
		pop r21
	.ENDM
//-----------------------

; -------------------------------------------------
; �������, ������������ � ��������� ������� ��������
.MACRO LDI_A
				ldi r25,byte4(@0)
				ldi r24,byte3(@0)
				ldi r23,byte2(@0)
				ldi r22,byte1(@0)
.ENDM
.MACRO LDI_B
				ldi r21,byte4(@0)
				ldi r20,byte3(@0)
				ldi r19,byte2(@0)
				ldi r18,byte1(@0)
.ENDM
.MACRO LDS_A
				lds r22,@0+1
				lds r23,@0+0
				sbrc r23,7
				rjmp ma_neg
	ma_pos:		clr r24
				clr r25
				rjmp ma_ex
	ma_neg:		ser r24
				ser r25
	ma_ex:		nop
.ENDM
.MACRO LDS_B
				lds r18,@0+1
				lds r19,@0+0
				sbrc r19,7
				rjmp mb_neg
	mb_pos:		clr r20
				clr r21
				rjmp mb_ex
	mb_neg:		ser r20
				ser r21
	mb_ex:		nop
.ENDM
.MACRO SAVE_A_R4
				movw r4,r22
				movw r6,r24
.ENDM
.MACRO SAVE_A_R8
				movw r8,r22
				movw r10,r24
.ENDM
.MACRO SAVE_A_R12
				movw r12,r22
				movw r14,r24
.ENDM
.MACRO LOAD_A_R4
				movw r22,r4
				movw r24,r6
.ENDM
.MACRO LOAD_A_R8
				movw r22,r8
				movw r24,r10
.ENDM
.MACRO LOAD_A_R12
				movw r22,r12
				movw r24,r14
.ENDM
.MACRO LOAD_B_R4
				movw r18,r4
				movw r20,r6
.ENDM
.MACRO LOAD_B_R8
				movw r18,r8
				movw r20,r10
.ENDM
.MACRO LOAD_B_R12
				movw r18,r12
				movw r20,r14
.ENDM
.MACRO COPY_A_B
				movw r18,r22
				movw r20,r24
.ENDM
