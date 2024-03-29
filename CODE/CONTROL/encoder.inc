; ------------------------------------------------------------------------
; ������������ ������������ ��������
; http://we.easyelectronics.ru/Soft/esche-odin-sposob-oprosa-enkodera.html
.equ		phaseA		=	7
.equ		phaseB		=	6
.equ		eButton		=	5
.equ		rotateL		=	0
.equ		rotateR		=	1
.equ		clickBtn	=	2
.equ		longHold	=	3

encoder:	
				in		r16,PinD			;��������� �������� ����� D � ������� 
				andi r16,(1<<phaseA)|(1<<phaseB)	;�������� ��� ���� R16
				lds 	r17,(REG_AB)
				cpi		r17,0				;������� RegAB ����� 0?
				brne	encoderScan			;���� ��� , ������ �� ����� ScanEncoder
				cpi		r16,0				;�� ����� ������ 0?
				brne	encoderExit			;���� ��� , ������� ���
				ldi		r17,128				;����� "�������" ��� �������� �� ������� ���������
				rjmp	encoderExit			;�.�. � ��������� ��� ������ �� ������ ����� ���������
encoderScan:
				cpi	r16,(1<<phaseA)|(1<<phaseB)	;�� ����� ������ ������� ������� ?
				breq	encoderGoWork		;���� �� , �� ����� �� ����������
				cpi	r16,(1<<phaseA)		;���? ����� ����� �� ������ ����� ���.�������
				brne	encoderM1			;���� ��� �� ����� ��������� ������ �����
				inc		r17					;����� ����������� �������� RegA �� �������
				cpi		r17,250				;����������������� �� ������������ � ���� ���������
				brsh	encoderWorkA		;�� 250,������ �������� ���� �������� � ������� �
				rjmp	encoderExit			;� �������
encoderM1:		cpi r16,(1<<phaseB)		;����� �� ������ ����� ���. �������?
				brne	encoderM2			;���� ����� ���,������� ���
				dec		r17					;����� ����������� �������� RegB �� �������
				cpi		r17,5				;����������������� �� ������������ � ���� ���������
				brlo	encoderWorkB		;�� 5,������ �������� ���� �������� � ������� �
encoderM2:		rjmp	encoderExit			;�������
encoderGoWork:	
				cpi		r17,128				;����������, ��� � ��� ��� ���������� � ��������
				breq	encoderExit			;���� ������ �� ���������� - ������� (���� ������ ��� ������� �� ������ ���������)
				brlo	encoderWorkA		;���� RegAB ������ 128 - ������ �������� �
encoderWorkB:							;����� ������ �������� �
				;�������� � increase ������� �� �������
				lds r16,(KEY_FLAGS)
				SETB r16,rotateR
				sts (KEY_FLAGS),r16
				clr		r17					;����� ���������� �������� ����������� ������� RegAB
				rjmp	encoderExit			;�������
encoderWorkA:
				;�������� � decrease ������� ������ �������
				lds r16,(KEY_FLAGS)
				SETB r16,rotateL
				sts (KEY_FLAGS),r16
				clr		r17				;����� ���������� �������� ����������� ������� RegAB
encoderExit:	sts (REG_AB),r17		;�������
				ret
; ------------------------------------------------------------------------
;
;
pressButton:	in r16,PinD
				andi r16,(1<<eButton)
				ldi r17,$FF
				eor r16,r17
				bst r16,eButton

				LDX DInputIntegrator
				LDY	DButtonStatus

				call KEY_PROCESS_INTEGRATOR
				call KEY_UPDATE_BUTTON_STATUS	
			
				lds r17,(KEY_FLAGS)
				lds r16,(DButtonStatus)
				andi r16,0b11100000
				cpi r16,(BSC_LongHold<<BUTTON_STATUS_CODE)
				brne chkShortPress
				SETB r17,longHold
				rjmp pressButtonEnd
chkShortPress:	cpi r16,(BSC_ShortPress<<BUTTON_STATUS_CODE)
				brne pressButtonEnd
				SETB r17,clickBtn
pressButtonEnd:	sts (KEY_FLAGS),r17
				ret
; ------------------------------------------------------------------------

.def	temp1	= r16
.def	temp2	= r17
.def	temp3	= r18

KEY_PROCESS_INTEGRATOR:
;		LDX DInputIntegrator
		LD	temp1,	X					; ��������� ������� �����������
;		MOV	temp2,	IntegratorLatchDepth			; ��� �������� �� ����� ������ ������ - ������� ��������� ��� �� ��������� �������, �� ��������� side effects...
		ldi temp2,15
		BRTS	PressedDecrement__KEY_PROCESS_INTEGRATOR	; �������� ����� �����������
	
	ReleasedIncrement__KEY_PROCESS_INTEGRATOR:
		DEC	temp2						; =(LatchDepth-1)	����� ����������, ����������������� �������� "�������": ������ ��� ������� ������� �������� ������ ���� <= (LatchDepth-1)
		CP	temp1,	temp2
		BREQ	End__KEY_PROCESS_INTEGRATOR			; temp1 == (LatchDepth-1), ������� ��� �� ������� - ����������� ���������...
		INC	temp1						; �������������� ������� �����������
		RJMP	End__KEY_PROCESS_INTEGRATOR

	PressedDecrement__KEY_PROCESS_INTEGRATOR:
		NEG	temp2						; =(-LatchDepth)	����� ����������, ��������� �������� ������� � �������������� ���
		CP	temp1,	temp2
		BREQ	End__KEY_PROCESS_INTEGRATOR			; temp1 == (-LatchDepth), ������� ��� �� ������� - ����������� ���������...
		DEC	temp1						; �������������� ������� �����������
		;RJMP	End__KEY_PROCESS_INTEGRATOR

	End__KEY_PROCESS_INTEGRATOR:
		ST	X,	temp1					; ��������� ������� �����������
		RET




;---------------------------------------------------------------------------
KEY_UPDATE_BUTTON_STATUS:
;LDX DInputIntegrator
;LDY	DButtonStatus
		LD	temp2,	Y					; ��������� ������-������� ������
		BST	temp2,	BUTTON_IS_HOLDDOWN			; ���� "������ ������������ � ������� ���������" -> T
		LD	temp1,	X					; ��������� ������� �����������
		TST	temp1						; ������� ��������� ������ ������ -> N
		BRMI	ButtonHold__KEY_UPDATE_BUTTON_STATUS	
	
	ButtonReleased__KEY_UPDATE_BUTTON_STATUS:			
		; (���������: ������ ������ "��������")
		BRTC	Exit__KEY_UPDATE_BUTTON_STATUS			; ������ ��� ��������� "����������" - ������� ������ ������ ������ �� �����...
		; (���������: �� � ������-�������� ��� �������� ������ ������ "������������")
		SBRC	temp2,	BUTTON_IS_PRESSED			; ���������: ������ "������������ � �������� ������������"?
		RJMP	ResetStatus__KEY_UPDATE_BUTTON_STATUS

		ANDI	temp2,	~(1<<BUTTON_IS_HOLDDOWN)		; ���������� ������ = ������ "��������",
		ORI	temp2,	(1<<BUTTON_IS_PRESSED)			; ... �� ���������, ��� "���� �������".
									; ��� ����, ���� "������� ���������" (�������� ��� �������) ��� ��������� ���������� � �������� - �� ��� �� �������.
		RJMP	Save__KEY_UPDATE_BUTTON_STATUS

	ResetStatus__KEY_UPDATE_BUTTON_STATUS:
		; (����������� ��������� � �������������� ���������: "��������, �� �������� ���������� ����������" ������ - ���������� �����...)
		; (� ��� ����, ���������� ������ ������ ��� ���������!)
		CLR	temp2						; ������-������� "�������� � ����", � �������� ���������.
		RJMP	Save__KEY_UPDATE_BUTTON_STATUS
	
	ButtonHold__KEY_UPDATE_BUTTON_STATUS:			
		; (���������: ������ ������ "������������")
		BRTS	Exit__KEY_UPDATE_BUTTON_STATUS			; ������ ��� ��������� "������������" - ������� ������ ������ ������ �� �����...
		; (���������: �� � ������-�������� ��� �������� ������ ������ "��������")
		LDI	temp2,	(1<<BUTTON_IS_HOLDDOWN)			; ���������� ������ = "������ ������������ � ������� ���������"; 
									; ��� ����, ������� "������� ����������� ���������" ������������� ����������; 
									; � �.�. ������ ������-������ ������, �� ����� ������������ � "������-���" (�� ���������� ��������)...
		;RJMP	Save__KEY_UPDATE_BUTTON_STATUS
	
	Save__KEY_UPDATE_BUTTON_STATUS:
		ST	Y,	temp2					; ��������� ������-������� ������
	Exit__KEY_UPDATE_BUTTON_STATUS:
		RET

;---------------------------------------------------------------------------
KEY_ENHANCE_TIME_FOR_ALL_BUTTONS:

;		LDI	StatusAddressLow,	Low(DButtonStatus)	; (����������: ����� ��������� � ������� �����, � �� ��������)
;		LDI	StatusAddressHigh,	High(DButtonStatus)	; ��� ������� StatusAddress = Y(R29:R28)
		LDY DButtonStatus
		LDI	temp3,	CButtonInputChannelCount		; temp3 = ������� �����
		
	Loop__KEY_ENHANCE_TIME_FOR_ALL_BUTTONS:
		LD	temp2,	Y+					; temp2 = ��������� ������� ������-������� ������
		
		BST	temp2,	BUTTON_IS_HOLDDOWN			; ���� "������ ������������ � ������� ���������" -> T
		BRTC	Skip__KEY_ENHANCE_TIME_FOR_ALL_BUTTONS		; ��� "�� �������" ������ ����������...
		BST	temp2,	BUTTON_IS_PRESSED			; ���� "������������� ����������� ������� ������" -> T
		BRTS	Skip__KEY_ENHANCE_TIME_FOR_ALL_BUTTONS		; ��� "����������" ������ ����������...
		
		; (���������: ������� ������ ������ � ������������)
		MOV	temp1,	temp2
		ANDI	temp1,	(0b11111<<BUTTON_HOLDING_TIME)		; temp1 = �������� �������� "�������� ������� ���������" ������
		
		CPI	temp1,	0b11111					; �������� �������� "�������� ������� ���������" � ��� ����������
		BREQ	Skip__KEY_ENHANCE_TIME_FOR_ALL_BUTTONS		; ���� �������� �������� ��� �� ��������� - �� ������ ���� ������ ������ �� ��������...
		
		; (���������: ��������� ��������� �������� "�������� ������� ���������")
		INC	temp1
		ANDI	temp2,	~(0b11111<<BUTTON_HOLDING_TIME)		; �������� ���� �������� � ������-��������
		OR	temp2,	temp1					; �������� ����������� � ������-������� ����� �������� ��������
		
		; (���������: ��������� �������� ������-���)
		CPI	temp1,	CShortButtonTouchDuration		; �������� �������� "�������� ������� ���������" � ��������� ��������� ���������, ����������� ������� "Short/Long"
									; ���� temp1< CShortButtonTouchDuration ("Short" time), �� C = 1	-> ���� "������� ���������" ������: BUTTON_HOLDEN_LONG = 0-��������
									; ���� temp1>=CShortButtonTouchDuration ("Long" time),  �� C = 0	-> ���� "������� ���������" ������: BUTTON_HOLDEN_LONG = 1-�������
		; ���������� 1: (������������� � ����������)
		;STOREB	SREG,	SREG_C					; C (Carry Flag) -> T (Transfer bit, Bit Copy Storage)
		;BLD	temp2,	BUTTON_HOLDEN_LONG			; T -> �������� � ������-������� ������, � ��� ����� "������� ���������"
		;LDI	temp1,	1<<BUTTON_HOLDEN_LONG
		;EOR	temp2,	temp1					; ����������� ��� ����� "������� ���������" = ������ �� ����� ���������� ��������
		; ���������� 2: (���������� � �����������: �.�. � ��� ������� ����� ������ �����, � ���������� �� ������ ������, �������������, ����� ��� BUTTON_HOLDEN_LONG ����� ������ ������������ �� 0 � 1)
		BRLO	Save__KEY_ENHANCE_TIME_FOR_ALL_BUTTONS		; ���� temp1< CShortButtonTouchDuration ("Short" time), �� ���������� ��������� ����� "������� ���������"...
		ORI	temp2,	(1<<BUTTON_HOLDEN_LONG)			; ���� "������� ���������" = 1

	Save__KEY_ENHANCE_TIME_FOR_ALL_BUTTONS:
		ST	-Y,	temp2					; ��������� ������� ������-������� ������
		LD	temp2,	Y+					; (���������: ����� � �������� Y++)
	Skip__KEY_ENHANCE_TIME_FOR_ALL_BUTTONS:
		DEC	temp3						; ������� �����--
		BRNE	Loop__KEY_ENHANCE_TIME_FOR_ALL_BUTTONS

		RET



;---------------------------------------------------------------------------
KEY_RESET_STATUS_FOR_ALL_BUTTONS:
		push temp1
		push temp2
		PUSH	YL						; ��������� ��������
		PUSH	YH

		; ������-�������� ���� ������ ������������ � "���������� �����":
		LDI	YL,	Low(DButtonStatus)			; (����������: ����� ��������� � ������� �����, � �� ��������)
		LDI	YH,	High(DButtonStatus)
		LDI	temp2,	CButtonInputChannelCount		; ���������� ������ � �������
		LDI	temp1,	0b11111111				; � ������-�������� ��������� ��� = "���������� �����"
	ButtonLoop__KEY_RESET_STATUS_FOR_ALL_BUTTONS:
		ST	Y+,	temp1					; ����� � �������� Y++ (������: ������-�������)
		DEC	temp2						; ������� �����--
		BRNE	ButtonLoop__KEY_RESET_STATUS_FOR_ALL_BUTTONS
		

		POP	YH						; ������������ ��������
		POP	YL
		pop temp2
		pop temp1
		RET


.undef	temp1
.undef	temp2
.undef	temp3
