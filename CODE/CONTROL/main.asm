	.include ("m328Pdef.inc")
	.include ("equ.inc")
	.include ("macro.inc")
// ----------------------------------------------------------------
.dseg
	.include ("ramData.inc")
// ----------------------------------------------------------------
.cseg
reset:
// --  Таблица векторов прерываний --------------------------------
	.include ("intvectors.inc")
start:		
// -- Чистим память и регистры ------------------------------------
//
			ldi		ZL,low(SRAM_START)
			ldi		ZH,high(SRAM_START)
			clr		r16
RamFlushCycle:
	    	st		Z+,r16
			cpi		ZH,high(RAMEND)
			brne	RamFlushCycle
			cpi		ZL,low(RAMEND)
			brne	RamFlushCycle
			clr		ZL
			clr		ZH
ClearReg:	st		Z,ZH
        	inc		ZL
        	cpi		ZL,30
        	brne	ClearReg
        	clr		ZL
// -- Настраиваем стек --------------------------------------------
			ldi R16,low(RAMEND)
    		out SPL,r16
    		ldi R16,high(RAMEND)
    		out SPH,r16
// -- переключаем векторы прерываний в нормальный вид
		in r16,(MCUCR)
		SETB r16,IVCE
		mov r17,r16
		CLRB r16,IVSEL
		out (MCUCR),r17
		out (MCUCR),r16
; ----------------------------------------------------------------------------------------
// -- Настраиваем железо ------------------------------------------
		// Порты ввода/вывода ---------
			ldi r16,$FF
			out DDRB,r16
			ldi r16,0b11011111
			out PORTB,r16

			ldi r16,0b00000000
			out DDRC,r16
			ldi r16,0b00110000
			out PORTC,r16

			ldi r16,0b00011110
			out DDRD,r16
			ldi r16,0b11111111
			out PORTD,r16
		// ----------------------------
		// Отключаем тактирование таймеров
			ldi r16,(1<<TSM) | (1<<PSRASY) | (1<<PSRSYNC)
			out GTCCR,r16
		//-----------------------------
		// Таймер 0 -------------------
		// задаёт временные интервалы
			ldi r16,(1<<WGM01)|(0<<WGM00)			; CTC mode
			out TCCR0A,r16	
			ldi r16,(0<<CS02)|(1<<CS01)|(1<<CS00)	; clk/64
			out TCCR0B,r16
			ldi r16,249								; 1000 Hz
			out OCR0A,r16							; 
			ldi r16,(1<<OCIE0A)						; прерывание по совпадению А разрешаем
			sts TIMSK0,r16
		//-----------------------------
		// Таймер 1 -------------------
		// тактирует драйвер ШД
.equ		T1Stop	= 0b00001000	; CTC,	stopped
.equ		T1Run	= 0b00001010	; CTC,	00001yyy: 000 - no clock, 001 - Fclk, 010 - Fclk/8, 011 - Fclk/64, 
									; 100 - Fclk/256, 101 - Fclk/1024, 110 - ext falling, 111 - ext rising
			; ----------
			ldi r16,0b00000000		; 01000000-Toggle OC1A, 00000000-Normal Port op
			sts TCCR1A,r16
				ldi r16,T1Stop
				sts TCCR1B,r16
			ldi r17,high(6250)		; 50 ms
			ldi r16, low(6250)
			sts OCR1AH,r17
			sts OCR1AL,r16
			ldi r16,(1<<OCIE1A)		; прерывание по совпадению
			sts TIMSK1,r16
		//-----------------------------
		// Таймер 2 -------------------
		// используется для генерации звуков
.equ		SoundOn  = 0b00010010
.equ		SoundOff = 0b00000010
			; CTC, 00000010 - normal port op 0100xx10 - toggle OC2A, 0001xx10 - toggle OC2B
			ldi r16,SoundOff		
			sts TCCR2A,r16
			ldi r16,0b00000011		; clk/32;	00000yyy: 000 - no clock, 001 - Fclk, 010 - Fclk/8, 011 - Fclk/32, 
									; 100 - Fclk/64, 101 - Fclk/128, 110 - Fclk/256, 111 - Fclk/1024
			sts TCCR2B,r16
			ldi r16,124	; 2 kHz
			sts OCR2A,r16
			ldi r16,(0<<OCIE2B)|(1<<OCIE2A)|(0<<TOIE2) 
			sts TIMSK2,r16
		//-----------------------------
		// Последовательный порт ------
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
		//-----------------------------
		// I2C интерфейс - 100 кГц (72), 400 кГц (12)
			ldi r16,12
			sts TWBR,r16
			call i2c_stop
		//-----------------------------
		//	call init23017
		; -- настраиваем экран --------
			call initlcd
		; -- очищаем экран ------------
			call clearlcd
		; -- включаем экран ------------
			call LED_ON
				LDZ HELLOSTRING
				call printstring
		//-----------------------------
		// определяем наличие температурных датчиков
initDS18:	call GetPresence
			; -- принудительно ставим 12-бит
			ldi r16,SkipROM
			call WriteByte

			ldi r16,WriteScratchPad
			call WriteByte

			ldi r16,$00
			call WriteByte

			ldi r16,$00
			call WriteByte

			ldi r16,0b01111111
			call WriteByte
			; -- и записываем в EEPROM DS
;				call GetPresence
;				ldi r16,SkipROM
;				call WriteByte
;				ldi r16,CopyScratchPad
;				call WriteByte
;				M_DELAY_US 15000		; задержка 15 мс
;				ldi r16,RecallE2
;				call WriteByte
			; -- запускаем на измерение
			call ds18NewConvert
		//-----------------------------
		// посмотрим наличие BMP180 и, если есть, считаем коэффициенты
			; ---------------------------
			; --- Настройка BMP 180 -----
			call BMPCheck
			lds r16,(BMPPresence)
			tst r16
			brne loadEEValues
			call BMP180_READ_K
			; ---------------------------
			call BMP180_READ_D
			call BMP180_DECODE_NEW
			; --- первоначальное заполнение буфера ---
			ldi r16,BUFLEN	;16
initavgbuf:	call calcAvgPressure
			dec r16
			brne initavgbuf	
		//-----------------------------
		// загружаем настройки из EEPROM
		// или загружаем значения по умолчанию
		//-----------------------------
loadEEValues:	call chekForEEValues

			call initValues

			LDZ BACK_TIMER_T0
			ldi r16,5
			st Z+,r16
			st Z+,r16
			st Z+,r16
			st Z+,r16

		//-----------------------------
		// Включаем тактирование таймеров
			ldi r16,(0<<TSM) | (0<<PSRASY) | (0<<PSRSYNC)
			out GTCCR,r16
			sei
		//-----------------------------
		// выжидаем окончания первого цикла измерений (1 сек)
initwait:	lds r16,(TIMEFLAGS)
			sbrs r16,3
			rjmp initwait
			CLRB r16,3
			sts (TIMEFLAGS),r16
			call ds18ReadData
			call ds18NewConvert
		//-----------------------------
		// выводим экран основного режима
			call clearlcd
				LDZ MAINSCREEN
				call printstring
		//-----------------------------
				ldi r16,T1Run
				sts TCCR1B,r16
				sts (T1Prescaler),r16

lds r16,(A_FLAGS)
SETB r16,0
sts (A_FLAGS),r16
		
	rjmp main
// ----------------------------------------------------------------
// ----------------------------------------------------------------
main:		lds r16,(TIMEFLAGS)
			// ----------
main_T1000:	sbrs r16,3
			rjmp main_T100
				CLRB r16,3
				sts (TIMEFLAGS),r16
				push r16
					call eachSecondSub	; действия, производящиеся каждые 1000 мс
				pop  r16
			// ----------
main_T100:	sbrs r16,2
			rjmp main_T10
				CLRB r16,2
				sts (TIMEFLAGS),r16
				push r16
					call eachTenthSub	; действия, производящиеся каждые 100 мс
				pop  r16
			// ----------
main_T10:	sbrs r16,1
			rjmp main_KR
				CLRB r16,1
				sts (TIMEFLAGS),r16
				push r16
					call eachHundreedthSub	; действия, производящиеся каждые 10 мс
				pop  r16
			// ------------------------------
main_KR:	lds r16,(KEY_FLAGS)
			// ----------
			sbrs r16,rotateR
			rjmp main_KL
				push r16
					call rotateEnc
				pop  r16
				CLRB r16,rotateR
				sts (KEY_FLAGS),r16
			// ----------
main_KL:	sbrs r16,rotateL
			rjmp main_KB
				push r16
					call rotateEnc
					call resetBackCounter0
				pop  r16
				CLRB r16,rotateL
				sts (KEY_FLAGS),r16
			// ----------
main_KB:	sbrs r16,clickBtn
			rjmp main_KH
				push r16
					call KEY_RESET_STATUS_FOR_ALL_BUTTONS
					call shortKeyPress
					call resetBackCounter0
				pop  r16
				CLRB r16,clickBtn
				sts (KEY_FLAGS),r16
			// ----------
main_KH:	sbrs r16,longHold
			rjmp main_A3
				push r16
					call KEY_RESET_STATUS_FOR_ALL_BUTTONS
					call longKeyHold
					call resetBackCounter0
				pop  r16
				CLRB r16,longHold
				sts (KEY_FLAGS),r16
			// ---------- прием по uart
main_A3:	lds r16,(A_FLAGS)
			sbrs r16,3
			rjmp main_A7
				call RX_DECODE
main_A7:	lds r16,(TX_BUSY)
			tst r16
			breq main_A7A
			rjmp main_end
main_A7A:		call TXmanager
main_end:	rjmp main
// ----------------------------------------------------------------
resetBackCounter0:	push r16
					ldi r16,0
					sts (BACKCOUNTER0),r16
					pop r16
					ret
// ----------------------------------------------------------------
defaultValues:	// Значения по умолчанию. Грузятся, если в EEPROM не установлены.
marginsD:		.dw (7725)*16/100, (7850)*16/100, (9850)*16/100, (6300)*16/100
delaysupD:		.db 3,3,3,3
delaysdnD:		.db 5,5,5,5
actionsD:		.db 8+2+1,8+2+1,8,8+1
heatControlD:	.db 1,$FF,$C0,$03
stepsFor100mlD:	.db byte1(defaultStepsFor100ml), byte2(defaultStepsFor100ml), byte3(defaultStepsFor100ml), byte4(defaultStepsFor100ml)
// ----------------------------------------------------------------
.include ("actions.inc")
.include ("bmp180subs.inc")
.include ("fonts.inc")
.include ("eachtimesubs.inc")
.include ("eepromsubs.inc")
.include ("encoder.inc")
.include ("i2c.inc")
.include ("intsubs.inc")
.include ("lcdcontrolsubs.inc")
.include ("math.inc")
.include ("onewiresubs.inc")
.include ("printsubs.inc")
.include ("reactions.inc")
.include ("screens.inc")
.include ("serial.inc")
// ----------------------------------------------------------------
.eseg
.org 100
emargins:				.dw 0,0,0,0
edelaysup:				.db 0,0,0,0
edelaysdn:				.db 0,0,0,0
eactions:				.db 0,0,0,0
EEnStepsFor100ml:		.db 0,0,0,0
HeatSensor:			.db 1
HeatStatus:			.db $FF
HeatMargin:			.dw $03C0	;	порог 60 С

