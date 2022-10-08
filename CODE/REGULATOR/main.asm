.list
.include "macro.asm"
//.include "m328Pdef.inc"
.include "m168Pdef.inc"
.include "equ.inc"
; ----------------------------------------------------------------------------------------
.cseg 
; ***** INTERRUPT VECTORS ************************************************
.org 	0x00		jmp Start			; RESTART
.org	INT0addr 	jmp ZEROCROSS;	External Interrupt Rorgest 0		; Для отлова перехода сетевого напряжения через 0
.org	INT1addr 	RETI;		; External Interrupt Rorgest 1		
.org	PCI0addr 	RETI;		; Pin Change Interrupt Rorgest 0
.org	PCI1addr 	RETI;		; Pin Change Interrupt Rorgest 1
.org	PCI2addr 	RETI;		; Pin Change Interrupt Rorgest 2
.org	WDTaddr 	RETI;		; Watchdog Time-out Interrupt
.org	OC2Aaddr 	RETI;		; Timer/Counter2 Compare Match A
.org	OC2Baddr 	RETI;		; Timer/Counter2 Compare Match B
.org	OVF2addr 	RETI;		; Timer/Counter2 Overflow
.org	ICP1addr 	RETI;		; Timer/Counter1 Capture Event
.org	OC1Aaddr 	jmp OPTO_ON;		; Timer/Counter1 Compare Match A	; включить  оптрон управления симистором
.org	OC1Baddr 	jmp OPTO_OFF;		; Timer/Counter1 Compare Match B	; выключить оптрон управления симистором
.org	OVF1addr 	jmp OPTO_OFF_E;		; Timer/Counter1 Overflow			; выключить оптрон управления симистором аварийная ситуация
.org	OC0Aaddr 	jmp T0CompareA;			; TimerCounter0 Compare Match A
.org	OC0Baddr 	RETI;		; TimerCounter0 Compare Match B
.org	OVF0addr 	RETI;		; Timer/Couner0 Overflow
.org	SPIaddr 	RETI;		; SPI Serial Transfer Complete
.org	URXCaddr 	jmp RX_OK	;reti;			; USART Rx Complete
.org	UDREaddr 	jmp UD_OK	;reti;			; USART, Data Register Empty
.org	UTXCaddr 	jmp TX_OK	;reti;			; USART Tx Complete
.org	ADCCaddr 	jmp ADC_COMPLETE	; ADC Conversion Complete						; 
.org	ERDYaddr 	RETI;		; EEPROM Ready
.org	ACIaddr 	RETI;		; Analog Comparator
.org	TWIaddr 	jmp TWI;RETI;		; Two-wire Serial Interface
.org	SPMRaddr 	RETI;		; Store Program Memory Read

; ************************************************************************
START:
; ----------------------------------------------------------------------------------------
; переключаем векторы прерываний в нормальный вид
		in r16,(MCUCR)
		SETB r16,IVCE
		mov r17,r16
		CLRB r16,IVSEL
		out (MCUCR),r17
		out (MCUCR),r16
; ----------------------------------------------------------------------------------------

			CLEAR_ALL
; ----------------------------------------------------------------------------------------
			; порт С
			; 0 - вход аварийного стопа, 1 - вход включения разгона
			; 2 - , 3 - 
			; 4 - SDA, 5 - SCL
			ldi r16,0b00000000
			out DDRC,r16
			ldi r16,0b00111111
			out PORTC,r16
			; ----------------------------------------------------------------------------------------
			; порт D
			; 0 - RX, 1 - TX; переназначены 0 - индикатор захвата, 1 - выход разгона
			; 2 - детектор нуля; 3,4 - сюда подключен энкодер; 5 - кнопка энкодера;
			; 6 - MOC3052; 7 - индикатор захвата стабилизации
			ldi r16,0b11000010
			out DDRD,r16
			ldi r16,0b11111010
			out PORTD,r16
			; ----------------------------------------------------------------------------------------
			; порт B
			; 0 - строб,1 - данные, это подключение индикатора на TM1637
			; 3 - пищалка OC2
			; 5 - встроенный светодиод и выход на разгонный блок
			ldi r16,0b00101011
			out DDRB,r16
			ldi r16,0b11111100
			out PORTB,r16
; ----------------------------------------------------------------------------------------
			; Настройка прерываний от изменения уровне на линиях порта C (PCINT8..15) PCMSK1, PCIE1
			ldi r16,0b00000011	//0b00000011
			sts PCMSK1,r16
			ldi r16,0b00000000	//0b00000010
			sts PCICR,r16

; ----------------------------------------------------------------------------------------
			; Настройка i2c slave
			ldi r16,12
			sts TWBR,r16
			ldi r16,I2C_MasterAddress
			sts TWAR,r16
			ldi r16,0<<TWSTA|0<<TWSTO|0<<TWINT|1<<TWEA|1<<TWEN|1<<TWIE
			sts TWCR,r16
; ----------------------------------------------------------------------------------------
			; Настройка UART
			ldi r16, low(bauddivider)
			sts UBRR0L,r16
			ldi r16,high(bauddivider)
			sts UBRR0H,r16
			; Прерывания разрешены, прием-передача разрешен.
			ldi r16,(1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<TXCIE0)|(0<<UDRIE0)
			sts UCSR0B,r16
			; Формат кадра - 8 бит, один стоп бит, четность не проверяется
			ldi r16,(1<<UCSZ01)|(1<<UCSZ00)
			sts UCSR0C,r16
; ----------------------------------------------------------------------------------------
			; ----------------------------------------------------------------------------------------
			; Настройка таймера 2
			; для извлечения звуков на OC2A
.equ		SoundOn  = 0b01000010
.equ		SoundOff = 0b00000010
			; CTC, 00000010 - normal port op 0100xx10 - toggle OC2A, 0001xx10 - toggle OC2B
			ldi r16,SoundOff		
			sts TCCR2A,r16
			ldi r16,0b00000011		; clk/32;	00000yyy: 000 - no clock, 001 - Fclk, 010 - Fclk/8, 011 - Fclk/32, 
									; 100 - Fclk/64, 101 - Fclk/128, 110 - Fclk/256, 111 - Fclk/1024
			sts TCCR2B,r16
			ldi r16,124	; 2 kHz
			sts OCR2A,r16
			ldi r16,(0<<OCIE2B)|(0<<OCIE2A)|(0<<TOIE2) 
			sts TIMSK2,r16
			; ----------------------------------------------------------------------------------------
			; Настройка таймера 1 
			; режим Normal Mode WGM[13:10] = 0000
			; прескалер FCLK/8, 0,5 мкс/тик, 32,768 мс/цикл CS12..10 = 010
			ldi XH,HIGH(DEFAULT_ON_VALUE)
			ldi XL,LOW (DEFAULT_ON_VALUE)
			sts (OPTO_ON_TICK+0),XL
			sts (OPTO_ON_TICK+1),XH
			sts OCR1AH,XH
			sts OCR1AL,XL

			ldi XH,HIGH(DEFAULT_OFF_VALUE)
			ldi XL,LOW (DEFAULT_OFF_VALUE)
			sts (OPTO_OFF_TICK+0),XL
			sts (OPTO_OFF_TICK+1),XH
			sts OCR1BH,XH
			sts OCR1BL,XL

			ldi r16,(0<<COM1A1)|(0<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(0<<WGM11)|(0<<WGM10)
			sts TCCR1A,r16

			ldi r16,(0<<ICIE1)|(1<<OCIE1A)|(1<<OCIE1B)|(1<<TOIE1)
			sts TIMSK1,r16

			ldi r16,(0<<WGM13)|(0<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10)	
			sts TCCR1B,r16
			; ----------------------------------------------------------------------------------------
			; Настройка таймера 0
			; прерывание раз в 1 мс
			; CTC Mode, N=64, OCR0A=249, T=(N*(OCR0A+1))/XTAL
			ldi r16,249
			out OCR0A,r16
			out OCR0B,r16
			ldi r16,(0<<COM0A1)|(0<<COM0A0)|(0<<COM0B1)|(0<<COM0B0)|(1<<WGM01)|(0<<WGM00)
			out TCCR0A,r16

			ldi r16,(0<<OCIE0B)|(1<<OCIE0A)|(1<<TOIE0)
			sts TIMSK0,r16

			ldi r16,(0<<WGM02)|(0<<CS02)|(1<<CS01)|(1<<CS00)	; (FCLK:64, CTC-mode)
			out TCCR0B,r16
				ldi r16,9							; 10 мсек
				sts (T0EXTENDER+0),r16
				ldi r16,9
				sts (T0EXTENDER+1),r16				; 10*10 мсек
				ldi r16,9
				sts (T0EXTENDER+2),r16				; 10*100 мсек
			; ----------------------------------------------------------------------------------------
			; Настройка внешнего прерывания 0 на отлов изменения уровней это детектор нуля сетевого напряжения
			ldi r16,0b00001010		; INT1 - спадающий фронт, INT0 - спадающий фронт
			sts EICRA,r16
			ldi r16,0b00000001		; разрешаем INT0
			out EIMSK,r16
			; ----------------------------------------------------------------------------------------
			; Настройка АЦП 
			; ----------
			; ADMUX – ADC Multiplexer Selection Register
			; REFS1:0 = 00 - внешний ИОН 2.5V 					ADMUX[7:6]
			; ADLAR = 0 - по правому краю						ADMUX[5]
			; MUX2:0 = 0111 - ADC7 								ADMUX[3:0]
			; ----------
			; ADCSRA – ADC Control and Status Register A
			; ----------
			; ADEN: ADC Enable 									[7]			0 - стоп, 1 - работа
			; ADSC: ADC Start Conversion 						[6]			1
			; ADATE: ADC Auto Trigger Enable					[5]			1
			; ADIF: ADC Interrupt Flag							[4]			0
			; ADIE: ADC Interrupt Enable						[3]			1	
			; ADPS[2:0]: ADC Prescaler Select Bits				[2:0]		101 - Fcpu/32 = 500 кГц	
			.equ		ADC_ON				= 0b11101101	; Заклинание включения  АЦП
			.equ		ADC_OFF				= 0b01101101	; Заклинание выключения АЦП
			; ----------
			ldi r16,$00
			sts ADCSRB,r16
			ldi r16,(0<<REFS1 | 0<<REFS0 | 0<<ADLAR | 0<<MUX3 | 1<<MUX2 | 1<< MUX1 | 1<< MUX0)
			sts ADMUX,r16
			ldi r16,ADC_ON
			sts ADCSRA,r16
			; ----------------------------------------------------------------------------------------
			; ----------------------------------------------------------------------------------------
			; выбор типа дисплея
					PUSHZ
					ldi ZH,high(50000)
					ldi ZL,low (50000)
initledwait:		sbiw Z,1				;	2
					adiw Z,1 sbiw Z,1		;	2+2
					adiw Z,1 sbiw Z,1		;	2+2
					adiw Z,1 sbiw Z,1		;	2+2
					brne initledwait		;	1/2 Zero=1
					POPZ

			//call TM1637_INIT
			//brts checkOLEDaddr7A

			//	ldi r16,$FF
			//	sts LED_ADDRESS,r16
			//call tm1637
			//rjmp endAddrA
			; ----------------------------------------------------------------------------------------
			; выбор адреса OLED дисплея 7A или 78 или 00, если отсутствует
checkOLEDaddr7A:	call softI2CStart
					ldi r16,$7A
					call softI2CSendByte
					call softI2CStop
					tst r16
				brts checkOLEDaddr78 //brne
					ldi r16,$7A
					sts LED_ADDRESS,r16
				rjmp endAddr

checkOLEDaddr78:	call softI2CStart
					ldi r16,$78
					call softI2CSendByte
					call softI2CStop
					tst r16
				brts noOledAddr //brne
					ldi r16,$78
					sts LED_ADDRESS,r16
				rjmp endAddr
noOledAddr:			ldi r16,$00
					sts LED_ADDRESS,r16
				rjmp endAddrB
endAddr:
			; ----------------------------------------------------------------------------------------
			call INIT_LED
			call LED_CLEAR
			call LED_ON

			LDY TARGET_U
			STSY OUTPUT_U_POINTER

			; ----------------------------------------------------------------------------------------
endAddrA:	ldi r16,$FF
			sts (FL_NEW_M),r16
			; ----------------------------------------------------------------------------------------
endAddrB:	in r16,btnPin
			sbrc r16,btnPinNo 				; Если кнопка энкодера не нажата
			jmp START_U_0					; то стартуем с выходного напряжения = 0.
			ldi XH,HIGH(DEFAULT_TARGET_U)	; Иначе
			ldi XL,LOW (DEFAULT_TARGET_U)	; с напряжения по умолчанию.
			jmp SET_START_U
START_U_0:	clr XL
			clr XH
SET_START_U:
			sts (TARGET_U+0),XL
			sts (TARGET_U+1),XH

			ldi r16,TargetV
			call setNewValFlag
			; 
			ldi XH,HIGH(DEFAULT_STEP_U)
			ldi XL,LOW (DEFAULT_STEP_U)
			sts (STEP_U+0),XL
			sts (STEP_U+1),XH

			lds r16,LED_ADDRESS
			cpi r16,$FF
			breq no_hello_screen

;			LDZ HelloScreen
;			call printstring
;				call wait_one_sec
;			call LED_CLEAR

			LDZ Mode0Screen
			call printString

no_hello_screen:		sei

			ldi r16,$FF
			sts (FL_NEW_U),r16
			call KEY_RESET_STATUS_FOR_ALL_BUTTONS
			jmp MAIN
; ----------------------------------------------------------------------------------------
wait_one_sec:	
				LDY 1000
wos_B:			LDZ 8000
wos_A:			sbiw Z,1
				brne wos_A
				sbiw Y,1
				brne wos_B
				ret
; ----------------------------------------------------------------------------------------
MAIN:			lds r16,(ACTIONS_FLAGS)
brunch_A0:	sbrs r16,0
				rjmp brunch_A1
				CLRB r16,0
				sts (ACTIONS_FLAGS),r16
				push r16
				; -- bit 0 set Action -- обработать принятую UART строку
				//	call stringReceived
				pop r16
brunch_A1:	sbrs r16,1
				rjmp brunch_A2
				CLRB r16,1
				sts (ACTIONS_FLAGS),r16
				push r16
				; -- bit 1 set Action -- обработать принятую TWI строку 
					call TWIReceived	
				pop r16
brunch_A2:	/*sbrs r16,2
				rjmp brunch_A3
				CLRB r16,2
				sts (ACTIONS_FLAGS),r16
				push r16
				; -- bit 2 set Action -- 
					
				pop r16*/
brunch_A3:	sbrs r16,3
				rjmp brunch_A4
				CLRB r16,3
				sts (ACTIONS_FLAGS),r16
				push r16
				; -- bit 3 set Action -- короткое нажатие
				call KEY_RESET_STATUS_FOR_ALL_BUTTONS
				call shortKeyPress
				pop r16
brunch_A4:	sbrs r16,4
				rjmp brunch_A5
				CLRB r16,4
				sts (ACTIONS_FLAGS),r16
				push r16
				; -- bit 4 set Action -- длинное удержание
				call KEY_RESET_STATUS_FOR_ALL_BUTTONS
				call longKeyHold
				pop r16
brunch_A5:	sbrs r16,5
				rjmp brunch_A6
				CLRB r16,5
				sts (ACTIONS_FLAGS),r16
				push r16
				; -- bit 5 set Action -- вращение вправо +
			//rjmp brunch_A5slow	
					lds r16,(ROTATE_COUNTER)
					cpi r16,0
					breq brunch_A5slow
						PUSHZ
							LDY 50
							STSY STEP_U
						POPZ
					rjmp brunch_A5fast
brunch_A5slow:		PUSHZ
						LDY DEFAULT_STEP_U
						STSY STEP_U
					POPZ
brunch_A5fast:		ldi r16,2
					sts (ROTATE_COUNTER),r16
					call INCREASE_U
				pop r16
brunch_A6:	sbrs r16,6
				rjmp brunch_A7
				CLRB r16,6
				sts (ACTIONS_FLAGS),r16
				push r16
				; -- bit 6 set Action -- вращение влево -
			//rjmp brunch_A6slow
					lds r16,(ROTATE_COUNTER)
					cpi r16,0
					breq brunch_A6slow
						PUSHZ
							LDY 50
							STSY STEP_U
						POPZ
					rjmp brunch_A6fast
brunch_A6slow:		PUSHZ
						LDY DEFAULT_STEP_U
						STSY STEP_U
					POPZ
brunch_A6fast:		ldi r16,2
					sts (ROTATE_COUNTER),r16
					call DECREASE_U
//					call DECREASE_U
				pop r16
brunch_A7:	/*sbrs r16,7
				rjmp brunch_D0
				CLRB r16,7
				sts (ACTIONS_FLAGS),r16
				push r16
				; -- bit 7 set Action -- 
				pop r16*/
//--------------------------------------------------------
brunch_D0:	lds r16,(DATA_FLAGS)
			sbrs r16,0
				rjmp brunch_D1
				CLRB r16,0
				sts (DATA_FLAGS),r16
				push r16
				; -- bit 0 set Action -- 1000 ms
				// закомментировать если порт не нужен
				//call PrepareDataToSend		; подготовим данные для передачи и запустим её
				pop r16
				rjmp brunch_ex
brunch_D1:	sbrs r16,1
				rjmp brunch_D2
				CLRB r16,1
				sts (DATA_FLAGS),r16
				push r16
				; -- bit 1 set Action -- 100 ms
					call sub100mSec
					call PCI_ONE
				pop r16
				rjmp brunch_ex
brunch_D2:	sbrs r16,2
				rjmp brunch_D3
				CLRB r16,2
				sts (DATA_FLAGS),r16
				push r16
				; -- bit 2 set Action -- 10 ms
					lds r16,(ROTATE_COUNTER)
					cpi r16,0
					breq brunch_D2A
					dec r16
brunch_D2A:			sts (ROTATE_COUNTER),r16
				pop r16
				rjmp brunch_ex
brunch_D3:	sbrs r16,3
				rjmp brunch_D4
				CLRB r16,3
				sts (DATA_FLAGS),r16
				push r16
				; -- bit 3 set Action --  1 ms
					call encoder
					call pressButton
				pop r16
				rjmp brunch_ex
brunch_D4:
brunch_ex:	
jmp MAIN
; ----------------------------------------------------------------------------------------
; ----------------------------------------------------------------------------------------
shortKeyPress:	PUSHF
				push r16	
				lds r16,(MODE)
skp0:			cpi r16,0
				brne skp1
					ldi r16,2		; короткое нажатие в рабочем режиме включаем режим "стоп" на выходе 0
					call setMode
					rjmp skpEx
skp1:			cpi r16,1
				brne skp2
					ldi r16,0		; короткое нажатие в режиме "стоп" включаем основной режим стабилизации, выключаем разгон
					call setMode
					rjmp skpEx
skp2:			cpi r16,2
				brne skp3
					ldi r16,0		; короткое нажатие в режиме "разгон" включаем основной режим стабилизации
					call setMode
					rjmp skpEx
skp3:			cpi r16,3
				brne skp4
					//				; короткое нажатие в режиме "внешний стоп"
						ldi r16,0
						sts (SOUNDNUMBER),r16
					rjmp skpEx
skp4:			//
skpEx:			pop r16
				POPF
				ret
; --------------------------------------------------------------------------------
longKeyHold:	PUSHF
				push r16
				lds r16,(MODE)

lkh0:			cpi r16,0
				brne lkh3
					ldi r16,1		; длинное нажатие в рабочем режиме включаем режим разгона
					call setMode
				rjmp lkhEx

lkh3:			cpi r16,3
				brne lkhEx
					ldi r16,0		; длинное нажатие в режиме "внешний стоп" включаем основной режим стабилизации
					call setMode
				rjmp lkhEx

lkhEx:			pop r16
				POPF	
				ret
; --------------------------------------------------------------------------------
sub100mSec:		PUSHF
				PUSHY
				PUSHZ
				push r16
				push r17
				push r18

				; ----------

				call KEY_ENHANCE_TIME_FOR_ALL_BUTTONS
				call indication		; выбираем что на что и как выводить				

				lds r16,(EXP100MS)
				cpi r16,2
				breq thisMoment

				rjmp next100ms

thisMoment:		clr r16
				push r16
				//----------------------
				// обновляем данные для iic
					lds r16,(TARGET_U+0)
					sts (i2c_OutBuff+0),r16
					lds r16,(TARGET_U+1)
					sts (i2c_OutBuff+1),r16
					lds r16,(AVERAGE_U+0)
					sts (i2c_OutBuff+2),r16
					lds r16,(AVERAGE_U+1)
					sts (i2c_OutBuff+3),r16
				// Пакет в один байт три значения
				// 0..3 - MODE, 4 - FL_RANGE, 5 - FL_MAX_U
					lds r16,(MODE)
					andi r16,$0F
					lds r17,(FL_RANGE)
					andi r17,$10
					or r16,r17
					lds r17,(FL_MAX_U)
					andi r17,$20
					or r16,r17
					sts (i2c_OutBuff+4),r16
				//----------------------
					ldi r16,OutputV
					call setNewValFlag
				//----------------------
					lds r16,(MODE)
					tst r16
					brne thisMomentSkip
					lds r16,(FL_MAX_U)
					tst r16
					breq enableTarget
				//----------------------
enableMeasured:		ldi r16,TargetV
					call disableOutput
					ldi r16,OutputV
					call enableOutput
					rjmp thisMomentSkip
				//----------------------
enableTarget:		ldi r16,TargetV
					call enableOutput
					ldi r16,OutputV
					call disableOutput
				//----------------------
thisMomentSkip:	pop  r16
next100ms:		inc r16
				sts (EXP100MS),r16
				; ----------
				//call top_rail	// вопилка при невозможности поддерживать выходное напряжение при падении входного
				pop r18
				pop r17
				pop r16
				POPZ
				POPY
				POPF
				ret		
; --------------------------------------------------------------------------------
; ----------------------------------------------------------------------------------------
; ----------------------------------------------------------------------------------------
; ----------------------------------------------------------------------------------------
; ----------------------------------------------------------------------------------------
; ----------------------------------------------------------------------------------------
; ----------------------------------------------------------------------------------------
; Включение оптрона управления симистором 
; Прерывание по совпадению Т1A
OPTO_ON:	cbi MOC_PORT,MOC_PIN
			reti
; ----------------------------------------------------------------------------------------
; Выключение оптрона управления симистором 
; Прерывание по совпадению Т1B
OPTO_OFF:	sbi MOC_PORT,MOC_PIN
			; ----------
			; разрешаем прерывание INT0 (ожидание перехода через 0)
			push r16
			ldi r16,$01
			sts (EIMSK),r16
			pop r16
			reti
; ----------------------------------------------------------------------------------------
OPTO_OFF_E:	sbi MOC_PORT,MOC_PIN
			reti
; ----------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
// -- прерывание Т0А каждые 1 мс -- расставляет флаги по времени
T0CompareA:			PUSHF
					push r19
					push r18
					push r17
					push r16

						lds r17,(DATA_FLAGS)						

						lds r16,(milliSeconds)
						cpi r16,9
						breq millisecondsFull
						inc r16
						sts (milliSeconds),r16
							SETB r17,3
							//
						rjmp T0CompareEnd

millisecondsFull:		clr r16
						sts (milliSeconds),r16
T0CompareSantiSec:		lds r16,(santiSeconds)
						cpi r16,9
						breq santisecondsFull
						inc r16
						sts (santiSeconds),r16
							SETB r17,2
							//
						rjmp T0CompareEnd

santisecondsFull:		clr r16
						sts (santiSeconds),r16
T0CompareDeciSec:		lds r16,(deciSeconds)
						cpi r16,9
						breq decisecondsFull
						inc r16
						sts (deciSeconds),r16
							SETB r17,1
						rjmp T0CompareEnd

decisecondsFull:		clr r16
						sts (deciSeconds),r16
T0CompareSec:			lds r16,(Seconds)
						//
						//
						inc r16
						sts (Seconds),r16
							SETB r17,0


T0CompareEnd:		sts (DATA_FLAGS),r17
						
					pop r16
					pop r17
					pop r18
					pop r19
					POPF
					reti
//---------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------
toneIndex:	.dw notone,tone0,tone1
// ----------------------------------------------------------------
notone:
tone0:	/*.db	 94, 94,118,118, 94, 94,118,118, 88, 88,\
			 94, 94,105,105,158,158,158,158,158,158,\
			158,158,141,141,126,126,118,118,118,118,\
			118,118,$00,$00,$00,$00,$00,$00,$00,$00,\ 
		.db	238,212,189,178,158,141,126,118,105, 94,\
			 88, 79, 70, 62, 59, 59, 62, 70, 79, 88,\
			 94,105,118,126,141,158,178,189,212,238,\
			238,  0,$00,$00,$00,$00,$00,$00,$00,$00,\*/
		.db	238,238,212,212,189,189,178,178,158,158,\
			141,141,126,126,118,118,105,105, 94, 94,\
			 88, 88, 79, 79, 70, 70, 62, 62, 59, 59,\
			  0,  0,$00,$00,$00,$00,$00,$00,$00,$00,\
			$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,\
			$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,\
			$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,\
			$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,\
			$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,\
			$00,$00,$00,$00,$00,$00
// ----------------------------------------------------------------
tone1:	.db  70, 70, 70, 70, 70, 70, 70, 70, 70, 70,\
			$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,\
			 70, 70, 70, 70, 70, 70, 70, 70, 70, 70,\
			$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,\
			 59, 59, 59, 59, 59, 59, 59, 59, 59, 59,\
			$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,\
			$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,\
			$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,\
			$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,\
			$00,$00,$00,$00,$00,$00
// ----------------------------------------------------------------
// -- Извлекалка звуков -------------------------------------------
sound:				PUSHZ
					push r16
					push r17
					lds r16,(SOUNDNUMBER)
					tst r16
					brne soundChoice
					rjmp soundOffAndDisable

soundChoice:		LDZ toneIndex*2
					clr r17
					andi r16,$03
					add ZL,r16	adc ZH,r17
					add ZL,r16	adc ZH,r17
					lpm r16,Z+
					lpm r17,Z
					movw ZH:ZL,r17:r16
					add ZL,ZL
					adc ZH,ZH
					;LDZ tone0*2
					lds r16,(TONECOUNTER)
					ldi r17,$00
					add ZL,r16	adc ZH,r17
					inc r16
					;andi r16,$3F
					cpi r16,$60
					brcs skipZeroCounter
					clr r16
skipZeroCounter:	sts (TONECOUNTER),r16
					lpm r16,Z
					tst r16
					breq soundOffOnly

					sts OCR2A,r16
					ldi r16,soundOn
					sts TCCR2A,r16
					rjmp soundExit

soundOffOnly:		ldi r16,soundOff
					sts TCCR2A,r16
					rjmp soundExit

soundOffAndDisable:	ldi r16,soundOff
					sts TCCR2A,r16
					clr r16
					sts (TONECOUNTER),r16
					rjmp soundExit

soundExit:			pop r17
					pop r16
					POPZ
					ret
// ----------------------------------------------------------------
setMode:			push r16
					clr r16
					sts (FL_MAX_U),r16
					pop r16
					cpi r16,0
					brne setMode1check
					rjmp setMode0
setMode1check:		cpi r16,1
					brne setMode2check
					rjmp setMode1
setMode2check:		cpi r16,2
					brne setMode3check
					rjmp setMode2
setMode3check:		cpi r16,3
					brne setMode4check
					rjmp setMode3
setMode4check:	rjmp setModeExit
// ---------------------------------
setMode0:			sts (MODE),r16		; работа
						ldi r16,$FF
						sts (FL_NEW_U),r16

						LDY TARGET_U
					STSY OUTPUT_U_POINTER
					sbi PORTB,5				; дополнительный разгон выкл

					lds r16,LED_ADDRESS
					tst r16
					breq setMode0Exit
					cpi r16,$FF
					breq setMode0Exit

					call LED_CLEAR
					LDZ Mode0Screen
					call printString
setMode0Exit:		rjmp setModeExit
// ---------------------------------
setMode1:			sts (MODE),r16		; разгон
						ldi r16,$FF
						sts (FL_NEW_U),r16

						LDY AVERAGE_U
					STSY OUTPUT_U_POINTER
					cbi PORTB,5				; дополнительный разгон ВКЛ

					lds r16,LED_ADDRESS
					tst r16
					breq setMode1Exit
					cpi r16,$FF
					breq setMode1Exit

					call LED_CLEAR
					LDZ Mode1Screen
					call printString
setMode1Exit:		rjmp setModeExit
// ---------------------------------
setMode2:			sts (MODE),r16		; стоп

						ldi r16,$FF
						sts (FL_NEW_U),r16

						LDY AVERAGE_U
					STSY OUTPUT_U_POINTER
					sbi PORTB,5				; дополнительный разгон выкл

					lds r16,LED_ADDRESS
					tst r16
					breq setMode2Exit
					cpi r16,$FF
					breq setMode2Exit

					call LED_CLEAR
					LDZ Mode2Screen
					call printString

setMode2Exit:		rjmp setModeExit
// ---------------------------------
setMode3:			sts (MODE),r16		; аварийный стоп
						ldi r16,$FF
						sts (FL_NEW_U),r16

					LDY AVERAGE_U
					STSY OUTPUT_U_POINTER
					sbi PORTB,5				; дополнительный разгон выкл

					lds r16,LED_ADDRESS
					tst r16
					breq setMode3Exit
					cpi r16,$FF
					breq setMode3Exit

						call LED_CLEAR
						LDZ Mode3Screen
						call printString
setMode3Exit:		rjmp setModeExit
// ---------------------------------
setModeExit:
					ret
// ----------------------------------------------------------------
.include "btn_subs.asm"
.include "comport.inc"
.include "control.inc"
.include "fonts.inc"
.include "indication.asm"
.include "led.inc"
.include "math.inc"
.include "measuring.inc"
.include "print.inc"
.include "printextras.inc"
.include "softi2c.inc"
.include "strings.inc"
.include "twi.inc"
// ----------------------------------------------------------------
.dseg
.include "ramdata.inc"
