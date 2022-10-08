; ------------------------------------------------------------------------
; подпрограмма обслуживания энкодера
; http://we.easyelectronics.ru/Soft/esche-odin-sposob-oprosa-enkodera.html
.equ		phaseA		=	7
.equ		phaseB		=	6
.equ		eButton		=	5
.equ		rotateL		=	0
.equ		rotateR		=	1
.equ		clickBtn	=	2
.equ		longHold	=	3

encoder:	
				in		r16,PinD			;загружаем значение порта D в регистр 
				andi r16,(1<<phaseA)|(1<<phaseB)	;выделить два бита R16
				lds 	r17,(REG_AB)
				cpi		r17,0				;регистр RegAB равен 0?
				brne	encoderScan			;если нет , уходим на метку ScanEncoder
				cpi		r16,0				;на обоих портах 0?
				brne	encoderExit			;если нет , выходим нах
				ldi		r17,128				;иначе "взводим" наш алгоритм на подсчет импульсов
				rjmp	encoderExit			;т.е. в следующий раз пойдем по другой ветви алгоритма
encoderScan:
				cpi	r16,(1<<phaseA)|(1<<phaseB)	;на обоих линиях высокий уровень ?
				breq	encoderGoWork		;если да , то пошли их сравнивать
				cpi	r16,(1<<phaseA)		;нет? тогда может на первой линии лог.единица
				brne	encoderM1			;если нет то пошли проверять вторую линию
				inc		r17					;иначе увеличиваем значение RegA на единицу
				cpi		r17,250				;подстраховываемся от переполнения и если досчитали
				brsh	encoderWorkA		;до 250,значит полюбому было вращение в сторону А
				rjmp	encoderExit			;и выходим
encoderM1:		cpi r16,(1<<phaseB)		;может на второй линии лог. единица?
				brne	encoderM2			;если опять нет,выходим нах
				dec		r17					;иначе увеличиваем значение RegB на единицу
				cpi		r17,5				;подстраховываемся от переполнения и если досчитали
				brlo	encoderWorkB		;до 5,значит полюбому было вращение в сторону В
encoderM2:		rjmp	encoderExit			;выходим
encoderGoWork:	
				cpi		r17,128				;сравниваем, что у нас там накопилось в регистре
				breq	encoderExit			;если ничего не изменилось - выходим (хотя такого мне кажется не должно случиться)
				brlo	encoderWorkA		;если RegAB меньше 128 - делаем действие А
encoderWorkB:							;иначе делаем действие В
				;ДЕЙСТВИЕ В increase поворот по часовой
				lds r16,(KEY_FLAGS)
				SETB r16,rotateR
				sts (KEY_FLAGS),r16
				clr		r17					;после выполнения действия ОБЯЗАТЕЛЬНО очищаем RegAB
				rjmp	encoderExit			;выходим
encoderWorkA:
				;ДЕЙСТВИЕ А decrease поворот против часовой
				lds r16,(KEY_FLAGS)
				SETB r16,rotateL
				sts (KEY_FLAGS),r16
				clr		r17				;после выполнения действия ОБЯЗАТЕЛЬНО очищаем RegAB
encoderExit:	sts (REG_AB),r17		;выходим
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
		LD	temp1,	X					; Загружаем регистр Интегратора
;		MOV	temp2,	IntegratorLatchDepth			; это значение мы будем менять походу - поэтому перекинем его во временный регистр, во избежание side effects...
		ldi temp2,15
		BRTS	PressedDecrement__KEY_PROCESS_INTEGRATOR	; Выбираем режим модификации
	
	ReleasedIncrement__KEY_PROCESS_INTEGRATOR:
		DEC	temp2						; =(LatchDepth-1)	перед сравнением, предекрементируем значение "защёлки": потому что верхняя граница счётчика должна быть <= (LatchDepth-1)
		CP	temp1,	temp2
		BREQ	End__KEY_PROCESS_INTEGRATOR			; temp1 == (LatchDepth-1), защёлка уже на пределе - модификация запрещена...
		INC	temp1						; инкрементируем счётчик интегратора
		RJMP	End__KEY_PROCESS_INTEGRATOR

	PressedDecrement__KEY_PROCESS_INTEGRATOR:
		NEG	temp2						; =(-LatchDepth)	перед сравнением, переводим значение границы в дополнительный код
		CP	temp1,	temp2
		BREQ	End__KEY_PROCESS_INTEGRATOR			; temp1 == (-LatchDepth), защёлка уже на пределе - модификация запрещена...
		DEC	temp1						; декрементируем счётчик интегратора
		;RJMP	End__KEY_PROCESS_INTEGRATOR

	End__KEY_PROCESS_INTEGRATOR:
		ST	X,	temp1					; Сохраняем регистр Интегратора
		RET




;---------------------------------------------------------------------------
KEY_UPDATE_BUTTON_STATUS:
;LDX DInputIntegrator
;LDY	DButtonStatus
		LD	temp2,	Y					; Загружаем статус-регистр Кнопки
		BST	temp2,	BUTTON_IS_HOLDDOWN			; Флаг "кнопка удерживается в нажатом состоянии" -> T
		LD	temp1,	X					; Загружаем регистр Интегратора
		TST	temp1						; Текущее состояние канала кнопки -> N
		BRMI	ButtonHold__KEY_UPDATE_BUTTON_STATUS	
	
	ButtonReleased__KEY_UPDATE_BUTTON_STATUS:			
		; (состояние: сейчас кнопка "отпущена")
		BRTC	Exit__KEY_UPDATE_BUTTON_STATUS			; Кнопка УЖЕ считалась "отпущенной" - поэтому ничего сейчас делать не нужно...
		; (состояние: но в статус-регистре ещё хранится старый статус "удерживается")
		SBRC	temp2,	BUTTON_IS_PRESSED			; состояние: кнопка "удерживается и отпущена одновременно"?
		RJMP	ResetStatus__KEY_UPDATE_BUTTON_STATUS

		ANDI	temp2,	~(1<<BUTTON_IS_HOLDDOWN)		; установить статус = кнопка "отпущена",
		ORI	temp2,	(1<<BUTTON_IS_PRESSED)			; ... но фиксируем, что "было нажатие".
									; при этом, флаг "времени удержания" (короткое или длинное) уже правильно установлен в регистре - мы его не трогаем.
		RJMP	Save__KEY_UPDATE_BUTTON_STATUS

	ResetStatus__KEY_UPDATE_BUTTON_STATUS:
		; (установлено служебное и исключительное состояние: "фиксация, до ожидания следующего отпускания" кнопки - ОТЛОЖЕННЫЙ СБРОС...)
		; (и при этом, физическую кнопку только что отпустили!)
		CLR	temp2						; статус-регистр "сбросить в ноль", в исходное положение.
		RJMP	Save__KEY_UPDATE_BUTTON_STATUS
	
	ButtonHold__KEY_UPDATE_BUTTON_STATUS:			
		; (состояние: сейчас кнопка "удерживается")
		BRTS	Exit__KEY_UPDATE_BUTTON_STATUS			; Кнопка УЖЕ считалась "удерживаемой" - поэтому ничего сейчас делать не нужно...
		; (состояние: но в статус-регистре ещё хранится старый статус "отпущена")
		LDI	temp2,	(1<<BUTTON_IS_HOLDDOWN)			; установить статус = "кнопка удерживается в нажатом состоянии"; 
									; при этом, счётчик "времени предыдущего удержания" принудительно обнуляется; 
									; и т.к. кнопку только-только нажали, то также сбрасывается её "статус-код" (до прояснения ситуации)...
		;RJMP	Save__KEY_UPDATE_BUTTON_STATUS
	
	Save__KEY_UPDATE_BUTTON_STATUS:
		ST	Y,	temp2					; Сохраняем статус-регистр Кнопки
	Exit__KEY_UPDATE_BUTTON_STATUS:
		RET

;---------------------------------------------------------------------------
KEY_ENHANCE_TIME_FOR_ALL_BUTTONS:

;		LDI	StatusAddressLow,	Low(DButtonStatus)	; (примечание: здесь загружаем в регистр адрес, а не значение)
;		LDI	StatusAddressHigh,	High(DButtonStatus)	; где регистр StatusAddress = Y(R29:R28)
		LDY DButtonStatus
		LDI	temp3,	CButtonInputChannelCount		; temp3 = счётчик цикла
		
	Loop__KEY_ENHANCE_TIME_FOR_ALL_BUTTONS:
		LD	temp2,	Y+					; temp2 = Загружаем текущий статус-регистр Кнопки
		
		BST	temp2,	BUTTON_IS_HOLDDOWN			; Флаг "кнопка удерживается в нажатом состоянии" -> T
		BRTC	Skip__KEY_ENHANCE_TIME_FOR_ALL_BUTTONS		; ещё "не нажатые" кнопки пропускаем...
		BST	temp2,	BUTTON_IS_PRESSED			; Флаг "зафиксировано полноценное нажатие кнопки" -> T
		BRTS	Skip__KEY_ENHANCE_TIME_FOR_ALL_BUTTONS		; уже "отпущенные" кнопки пропускаем...
		
		; (состояние: текущая кнопка нажата и удерживается)
		MOV	temp1,	temp2
		ANDI	temp1,	(0b11111<<BUTTON_HOLDING_TIME)		; temp1 = выделить значение "счётчика времени удержания" кнопки
		
		CPI	temp1,	0b11111					; сравнить значение "счётчика времени удержания" с его максимумом
		BREQ	Skip__KEY_ENHANCE_TIME_FOR_ALL_BUTTONS		; если значение счётчика уже на максимуме - то статус этой кнопки больше не меняется...
		
		; (состояние: требуется увеличить значение "счётчика времени удержания")
		INC	temp1
		ANDI	temp2,	~(0b11111<<BUTTON_HOLDING_TIME)		; обнулить биты счётчика в статус-регистре
		OR	temp2,	temp1					; побитово скопировать в статус-регистр новое значение счётчика
		
		; (состояние: требуется обновить статус-код)
		CPI	temp1,	CShortButtonTouchDuration		; сравнить значение "счётчика времени удержания" с эталонным граничным значением, различающим статусы "Short/Long"
									; если temp1< CShortButtonTouchDuration ("Short" time), то C = 1	-> Флаг "времени удержания" кнопки: BUTTON_HOLDEN_LONG = 0-короткое
									; если temp1>=CShortButtonTouchDuration ("Long" time),  то C = 0	-> Флаг "времени удержания" кнопки: BUTTON_HOLDEN_LONG = 1-длинное
		; Реализация 1: (универсальная и избыточная)
		;STOREB	SREG,	SREG_C					; C (Carry Flag) -> T (Transfer bit, Bit Copy Storage)
		;BLD	temp2,	BUTTON_HOLDEN_LONG			; T -> копируем в статус-регистр кнопки, в бит Флага "времени удержания"
		;LDI	temp1,	1<<BUTTON_HOLDEN_LONG
		;EOR	temp2,	temp1					; инвертируем бит Флага "времени удержания" = теперь он равен требуемому значению
		; Реализация 2: (упрощённая и оптимальная: т.к. у нас счётчик растёт только вверх, а изначально он всегда обнулён, следовательно, здесь бит BUTTON_HOLDEN_LONG может только установиться из 0 в 1)
		BRLO	Save__KEY_ENHANCE_TIME_FOR_ALL_BUTTONS		; если temp1< CShortButtonTouchDuration ("Short" time), то пропускаем установку Флага "времени удержания"...
		ORI	temp2,	(1<<BUTTON_HOLDEN_LONG)			; Флаг "времени удержания" = 1

	Save__KEY_ENHANCE_TIME_FOR_ALL_BUTTONS:
		ST	-Y,	temp2					; Сохраняем текущий статус-регистр Кнопки
		LD	temp2,	Y+					; (коррекция: адрес в регистре Y++)
	Skip__KEY_ENHANCE_TIME_FOR_ALL_BUTTONS:
		DEC	temp3						; счётчик цикла--
		BRNE	Loop__KEY_ENHANCE_TIME_FOR_ALL_BUTTONS

		RET



;---------------------------------------------------------------------------
KEY_RESET_STATUS_FOR_ALL_BUTTONS:
		push temp1
		push temp2
		PUSH	YL						; запомнить значения
		PUSH	YH

		; Статус-регистры всех Кнопок СБРАСЫВАЮТСЯ В "ОТЛОЖЕННЫЙ СБРОС":
		LDI	YL,	Low(DButtonStatus)			; (примечание: здесь загружаем в регистр адрес, а не значение)
		LDI	YH,	High(DButtonStatus)
		LDI	temp2,	CButtonInputChannelCount		; количество Кнопок в системе
		LDI	temp1,	0b11111111				; в статус-регистры загрузить код = "ОТЛОЖЕННЫЙ СБРОС"
	ButtonLoop__KEY_RESET_STATUS_FOR_ALL_BUTTONS:
		ST	Y+,	temp1					; адрес в регистре Y++ (минуем: статус-регистр)
		DEC	temp2						; счётчик цикла--
		BRNE	ButtonLoop__KEY_RESET_STATUS_FOR_ALL_BUTTONS
		

		POP	YH						; восстановить значения
		POP	YL
		pop temp2
		pop temp1
		RET


.undef	temp1
.undef	temp2
.undef	temp3
