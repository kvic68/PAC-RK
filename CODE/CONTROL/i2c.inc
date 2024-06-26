/*
 * i2c_lib.asm
 *
 * ���������� �������� ��� ���� i2c (Atmega48)
 * ���������� ������������� ��� ������������� � �������� ������������� ������ 
 * � ������ ��������. 
 *
 * ���������� �������� � ����������� TWI � avr �����������������
 *
 * ���������� �������� � ��������� r16
 *
 *  Created: 15.07.2013 23:51:32
 *   Author: ������� ��������
 */ 

 ;======= ��������� ������� �� ���� i2c =================================================
i2c_start: 
	push    r16					
	ldi     r16,(1<<TWINT)|(1<<TWSTA)|(1<<TWEN)  ; ��������� ������� ��������� ����������
	sts     TWCR,r16      ; �������� ���������� ���� � TWCR
	rcall   i2c_wait      ; �������� ������������ start � ����� TWI	
	pop     r16           ; ���������� ������ � r16 �� �����
	ret
 ;======= �������� ������� �� ���� i2c ==================================================
i2c_stop:	
    push    r16					
    ldi     r16,(1<<TWINT)|(1<<TWSTO)|(1<<TWEN)  ; ���������� �������� �������
    sts     TWCR,r16      ; �������� ���������� ���� � TWCR
    pop     r16           ; ���������� ������ � r16 �� �����
    ret
 ;======= ������� ����� ���������� �� ���� i2c ==========================================
i2c_send:	
    push    r16					
    sts     TWDR,r16      ; ���������� ������������ ���� � ������� TWDR
    ldi     r16,(1<<TWINT)|(1<<TWEN)  ; ��������� ����, ���������� 
                                      ; �� ��������� ��������������� �����
    sts     TWCR,r16      ; �������� ���������� ���� � TWCR
    rcall   i2c_wait      ; �������� ��������� ��������� �����
	//brne i2c_send_ex
	//sbi portb,5
i2c_send_ex:
    pop     r16           ; ���������� ������ � r16 �� �����
    ret
 ;======= ���� ��������������� ����� �� ���� i2c =======================================
i2c_receive:	
 ; �������� ���� ���������� � ������� r16, ������� �������������	
 ; ��������� ��������� ���, ����� � ���� ������ � ��� �� ���� 
 ; ������ ����������, ���� �� ����������� � ����� � ���� ������ 
 ; ���������
    ldi     r16,(1<<TWINT)|(1<<TWEN)|(1<<TWEA)  ; ��������� ����, ���������� �� ����� 
    sts     TWCR,r16      ; �������� ���������� ���� � TWCR
    rcall   i2c_wait      ; �������� ��������� ����� �����
    lds     r16,TWDR      ; ��������� ���������� ���������� �� TWDR
    ret
 ;======= ���� ���������� ����� (NACK) =================================================
i2c_receive_last:		
 ; �������� ���� ���������� � ������� r16, ������� �������������	
 ; ��������� ��������� ���, ����� � ���� ������ � ��� �� ���� 
 ; ������ ����������, ���� �� ����������� � ����� � ���� ������ 
 ; ���������
    ldi     r16,(1<<TWINT)|(1<<TWEN) ; ��������� ����, ���������� �� ����� ��������������� �����
    sts	    TWCR,r16      ; �������� ���������� ���� � TWCR
    rcall   i2c_wait      ; �������� ��������� ����� �����
    lds     r16,TWDR      ; ��������� ���������� ���������� �� TWDR
    ret
 ;======= �������� ���������� TWI =======================================================
i2c_wait:
				push ZL
				push ZH
				ldi ZH,high(1000)
				ldi ZL,low (1000)
				clz
				ldi r16,$00
i2c_wait_cycle:
				sbiw Z,1				;	2
				adiw Z,1 sbiw Z,1		;	2+2
				adiw Z,1 sbiw Z,1		;	2+2
				adiw Z,1 sbiw Z,1		;	2+2
				breq i2c_wait_error		;	1/2 Zero=1

    			lds     r16,TWCR      ; ��������� �������� �� TWCR � r16							;	2
    			sbrs    r16,TWINT     ; ������� �������� ����������� �� ��� ���, ���� ������ ���� 	;	1/2/3
                			          ; ���������� � 1
    			rjmp    i2c_wait_cycle	
				
				lds r16,(TWSR)
				rjmp i2c_wait_exit

i2c_wait_error:	call i2c_stop
				ldi r16,$00
i2c_wait_exit:	sts(IIC_Status),r16
				
				pop ZH
				pop ZL
    		ret
;=======================================================================================
; ----------------------------------------------------------------------------------------
; �������� ������ � ��������� ����������
SendRegData:		//ret
					PUSHF
//					cli
					push r16
//sbi portb,5
					call i2c_start
					ldi r16,IIC_REGULATOR
					call i2c_send
					lds r16,(IIC_Status)
					cpi r16,$18				;	ACK ��������� ��������� �� ������ �� ������
					breq SendRegDataA
					rjmp SendRegDataStop		;	

SendRegDataA:		lds r16,(DataToStab+0)
					call i2c_send
					lds r16,(DataToStab+1)
					call i2c_send
						lds r16,(DataToStab+2)
						call i2c_send
						//lds r16,(DataToStab+3)
						//call i2c_send
SendRegDataStop:	call i2c_stop

					pop r16
					POPF
					ret
; ----------------------------------------------------------------------------------------
; ������ ������ �� ���������� ����������
LoadRegData:		PUSHF
//					cli
					push r16
					push r17
					push r18

					clr r18
					call i2c_start
					ldi r16,IIC_REGULATOR
					ori r16,$01
					call i2c_send

					lds r16,(IIC_Status)
					cpi r16,$40				;	ACK ��������� ��������� �� ������ �� ������
					breq LoadRegDataCont
					rjmp LoadRegDataFail 
					
LoadRegDataFail:	call i2c_stop
					ldi r16,$FF
					sts (STABMODE),r16		;	��� ������ �������������, ����� = 255
//											;	��������� ����� ������
					ldi r16,StabModeV
					call setNewValFlag
					rjmp LoadRegDataExit

LoadRegDataCont:	call i2c_receive
						lds r17,(VOLTS+0)
					sts (VOLTS+0),r16
						cpse r17,r16
						SETB r18,0
LRD1:				call i2c_receive
						lds r17,(VOLTS+1)
					sts (VOLTS+1),r16
						cpse r17,r16
						SETB r18,0

LRD2:				call i2c_receive
						lds r17,(VOLTS+2)
					sts (VOLTS+2),r16
						cpse r17,r16
						SETB r18,1

LRD3:				call i2c_receive
						lds r17,(VOLTS+3)
					sts (VOLTS+3),r16
						cpse r17,r16
						SETB r18,1

LRD4:				call i2c_receive_last

					push r16
						andi r16,$0F
						lds r17,(STABMODE)
						sts (STABMODE),r16
						cpse r17,r16
						SETB r18,2
					pop r16
					push r16
						ldi r17,$00
						sbrc r16,4		;	RANGE
						ldi r17,$FF
						mov r16,r17
						lds r17,(STAB_RANGE)
						sts (STAB_RANGE),r16
						cpse r17,r16
						SETB r18,3
					pop r16
						ldi r17,$00
						sbrc r16,5		;	MAX_U
						ldi r17,$FF
						mov r16,r17
						lds r17,(STAB_MAX_U)
						sts (STAB_MAX_U),r16
						cpse r17,r16
						SETB r18,4
//---------------------------------------------
LRD7:
LRD8:
LRD9:				
					call i2c_stop
					sts (DataFromStab),r18
			//------------------------------------
					sbrs r18,0
					rjmp LRD10
					ldi r16,VoltageT
					call setNewValFlag

LRD10:				sbrs r18,1
					rjmp LRD11
					ldi r16,VoltageA
					call setNewValFlag

LRD11:				sbrs r18,2
					rjmp LRD12
					ldi r16,StabModeV
					call setNewValFlag

LRD12:				sbrs r18,3
					rjmp LRD13
					ldi r16,StabInd
					call setNewValFlag

LRD13:

LoadRegDataExit:	pop r18
					pop r17
					pop r16
					POPF
					ret
