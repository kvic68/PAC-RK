;****************************************************************
; ¬ывод URMS на индикатор, базирующийс€ на TM1637
;*****************************************
;
;	TM1637 
;
;*******************************************
;****** –≈√»—“–џ
//.def	r16	=r16
//.def	r17	=r17

;***************************************************
tm1637hex:	PUSHF
			push r16
			PUSHZ

			rcall	start1637		;маркер начала посылки
			ldi	r16,0xC0			;”—“јЌќ¬ ј Ќј„јЋ№Ќќ√ќ јƒ–≈—ј (0хC0 - крайнее левое знакоместо)
			rcall	outcom1637		;вывожу адрес

			ldi ZL, low(DIGIT_7*2)
			ldi ZH,high(DIGIT_7*2)
		movw r12,Z

			lds r16,(TM_BUFFER+1)
			andi r16,$F0
			swap r16
			add ZL,r16
			ldi r16,0
			adc ZH,r16
			lpm r16,Z
			call outcom1637
		movw Z,r12

			lds r16,(TM_BUFFER+1)
			andi r16,$0F
			add ZL,r16
			ldi r16,0
			adc ZH,r16
			lpm r16,Z
			call outcom1637
		movw Z,r12

			lds r16,(TM_BUFFER+0)
			andi r16,$F0
			swap r16
			add ZL,r16
			ldi r16,0
			adc ZH,r16
			lpm r16,Z
			call outcom1637
		movw Z,r12

			lds r16,(TM_BUFFER+0)
			andi r16,$0F
			add ZL,r16
			ldi r16,0
			adc ZH,r16
			lpm r16,Z
			call outcom1637

			rcall	end1637		;маркер конца посылки
			POPZ
			pop r16
			POPF
			ret
;***************************************************
tm1637:	PUSHF
		push r16
		push r17
		push ZL
		push ZH
		rcall	start1637		;маркер начала посылки
		ldi	r16,0xC0			;”—“јЌќ¬ ј Ќј„јЋ№Ќќ√ќ јƒ–≈—ј (0хC0 - крайнее левое знакоместо)
		rcall	outcom1637		;вывожу адрес

		lds r16,(TM_BUFFER+0)
		lds r17,(TM_BUFFER+1)
		call bin2bcd16

		; цифири в R11,R10
		ldi ZL, low(DIGIT_7*2)
		ldi ZH,high(DIGIT_7*2)
		movw r12,Z
		clr r17
		mov r16,r11
		swap r16
		andi r16,$0F
		tst r16
		brne zz0
		ldi r16,$0F
zz0:	add ZL,r16
		adc ZH,r17
		lpm r16,Z
		call outcom1637

		movw Z,r12
		clr r17
		mov r16,r11
		tst r16
		brne zz1
		ldi r16,$FF
zz1:	andi r16,$0F
		add ZL,r16
		adc ZH,r17
		lpm r16,Z
		call outcom1637

		movw Z,r12
		clr r17
		mov r16,r10
		swap r16
		andi r16,$0F
		add ZL,r16
		adc ZH,r17
		lpm r16,Z
		ori r16,$80
		call outcom1637

		movw Z,r12
		clr r17
		mov r16,r10
		andi r16,$0F
		add ZL,r16
		adc ZH,r17
		lpm r16,Z
		call outcom1637

		rcall	end1637		;маркер конца посылки
		pop ZH
		pop ZL
		pop r17
		pop r16
		POPF
	ret
DIGIT_7:		.db	$3F,$06,$5B,$4F,$66,$6D,$7D,$07,$7F,$6F,$00,$00,$5C,$79,$71,$00
;инициализаци€	
TM1637_INIT:

		rcall	start1637		;маркер начала посылки
		ldi		r16,0x8A		;включение диспле€. яркость 88-8F
		rcall	outcom1637		;вывожу команду
		rcall	end1637		;маркер конца посылки

		rcall	pause1637		;пауза 100 мкс
;включение режима передачи данных с автоинкрементом адреса
		rcall	start1637		;маркер начала посылки
		ldi		r16,0x40		;режим передачи данных с автоинкрементом адреса
		rcall	outcom1637		;вывожу команду
		rcall	end1637		;маркер конца посылки

		rcall	pause1637		;пауза 100 мкс
	
	ret
;***********************************
start1637:	;маркер начала посылки
		rcall	pause1637		;пауза 100 мкс
		sbi		TM_PORT,TM_CLK	;
		sbi		TM_PORT,TM_DIO	;
		rcall	pause1637		;пауза 100 мкс
		cbi		TM_PORT,TM_DIO	;	
		rcall	pause1637		;пауза 100 мкс	
	ret
;***********************************
end1637:	;маркер конца посылки
		sbi		TM_PORT,TM_CLK	;
		rcall	pause1637		;пауза 100 мкс
		sbi		TM_PORT,TM_DIO	;
		rcall	pause1637		;пауза 100 мкс
	ret
;***********************************
outcom1637:	;последовательный вывод. выводимый байт должен находитьс€ в r16
		push	r17
	
; вывод восьми битов регистра r16
		ldi		r17,0x08	;
outc10:	cbi	TM_PORT,TM_CLK	;опускаю CLK
		rcall	pause1637		;пауза 100 мкс

		lsr	r16		; \
		brcc	outc20		;  |
		sbi	TM_PORT,TM_DIO		;   > младший бит r16 выставл€ю на DIO
		rjmp	outc30		;  |
outc20:	cbi	TM_PORT,TM_DIO		; /

outc30:	rcall	pause1637		;пауза 100 мкс
		sbi	TM_PORT,TM_CLK		;поднимаю CLK
		rcall	pause1637		;пауза 100 мкс
		dec 	r17		;
		brne	outc10		;последовательно вывожу весь байт

; стоп-бит
		cbi	TM_DDR,TM_DIO		;переключаю DIO как вход, чтобы исключить коллизию с ACK
		cbi	TM_PORT,TM_CLK		;опускаю CLK
		//***
		rcall	pause1637		;пауза 100 мкс
		in r17,TM_PIN
		clt
		sbrc r17,TM_DIO
		set
//		BST r17,TM_DIO
		//***
		sbi	TM_PORT,TM_CLK		;поднимаю clk
		rcall	pause1637		;пауза 100 мкс
		cbi	TM_PORT,TM_CLK		;опускаю CLK
		cbi	TM_PORT,TM_DIO		;	
		sbi	TM_DDR,TM_DIO		;окончание стоп-бита, переключаю DIO как выход
				; (выхожу с нул€ми на обеих шинах)
		rcall	pause1637		;пауза 100 мкс
		pop	r17
	
	ret			;
		
pause1637:	;пауза 100 мкс
		push	r16		;
		ldi	r16,0x0f
de10:	;dec	r16
		subi r16,1
		brne	de10
		pop	r16
	ret
	ret
;*****************************************************************
