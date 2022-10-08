.equ		F_CPU 					= 16000000 ; Частота процессора в Гц
.equ		MCP23017ADDR			= $40
.equ		LED_ADDR				= $78
.equ 		BMP180ADDR				= $EE
.equ		IIC_REGULATOR			= $32
.equ		BUFLEN 					= 16
.equ 		baudrate 				= 9600  
.equ 		bauddivider 			= (F_CPU/(16*baudrate))-1
.equ		defaultStepsFor100ml	= 185000	;250000	;	
.equ		minStepsFor100ml		= 100000	;150000	;		
.equ		maxStepsFor100ml		= 1500000	;400000	;
.equ		minRateValue			= 10;
.equ		maxRateValue			= 3600;
.equ		maxTargetVolt			= 2500;
.equ		maxDoseValue			= 5000;
	
; ----------------------------------------------------------------------------------------
; Делители для Timer1
.equ	T1Disable	= 0b0001000
.equ	T1Div1 		= 0b0001001
.equ	T1Div8 		= 0b0001010
.equ	T1Div64 	= 0b0001011
.equ	T1Div256  	= 0b0001100
.equ	T1Div1024	= 0b0001101
; ----------------------------------------------------------------------------------------
; ----------------------------------------------------------------------------------------
; MCP23017 expander http://www.gammon.com.au/forum/?id=10940

.equ	LCD_CS0		= 4	;(0) 15	23(GPA2)    CS1   	Chip select for IC1 (1 = active)  (see LCD_CS1)
.equ	LCD_CS1		= 3	;(1) 16	24(GPA3)    CS2	    Chip select for IC2 (1 = active)  (see LCD_CS2)
.equ	LCD_RES		= 2 ;( ) 17	25(GPA4)    ~RST  	1 = not reset, 0 = reset
.equ	LCD_DI		= 7	;(3) 4	26(GPA5)    D/~I   	1 = data, 0 = instruction    (see LCD_DATA)
.equ	LCD_RW		= 6	;( ) 5	27(GPA6)    R/~W	1 = read, 0 = write (to LCD) (see LCD_READ)
.equ	LCD_STROBE	= 5	;(2) 6	28(GPA7)	E		Enable data transfer on 1 -> 0 transition  (see LCD_ENABLE)
.equ	LCD_LED		= 1

// для моёго индикатора
/*
.equ	LCD_CS0		= 2	;(0) 15	23(GPA2)    CS1   	Chip select for IC1 (1 = active)  (see LCD_CS1)
.equ	LCD_CS1		= 3	;(1) 16	24(GPA3)    CS2	    Chip select for IC2 (1 = active)  (see LCD_CS2)
.equ	LCD_RES		= 4 ;( ) 17	25(GPA4)    ~RST  	1 = not reset, 0 = reset
.equ	LCD_DI		= 5	;(3) 4	26(GPA5)    D/~I   	1 = data, 0 = instruction    (see LCD_DATA)
.equ	LCD_RW		= 6	;( ) 5	27(GPA6)    R/~W	1 = read, 0 = write (to LCD) (see LCD_READ)
.equ	LCD_STROBE	= 7	;(2) 6	28(GPA7)	E		Enable data transfer on 1 -> 0 transition  (see LCD_ENABLE)
.equ	LCD_LED		= 1
*/
; ----------------------------------------------------------------------------------------
; --- Настройка MCP 23017 ---
; -- регистры MCP23017
.equ	IOCON	=	$0A
.equ	IODIRA	=	$00
.equ	IODIRB	=	$01
.equ	GPPUA	=	$0C
.equ	GPPUB	=	$0D
.equ	GPIOA	=	$12
.equ	GPIOB	=	$13
.equ	OLATA	=	$14
.equ	OLATB	=	$15
; -- биты регистра IOCON
.equ	BANK	=	7
.equ	MIRROR	=	6
.equ	SEQOP	=	5	; 1-disabled, 0-enabled
.equ	DISSLW	=	4
.equ	HAEN	=	3
.equ	ODR		=	2
.equ	INTPOL	=	1
; ----------------------------------------------------------------------------------------
; -- Команды для DS18B20 --
.equ	SearchROM		= $F0
.equ	ReadROM			= $33
.equ	MatchROM		= $55
.equ	SkipROM			= $CC
.equ	AlarmSearch		= $EC

.equ	ConvertT		= $44
.equ	WriteScratchpad	= $4E
.equ	ReadScratchpad	= $BE
.equ	CopyScratchpad	= $48
.equ	RecallE2		= $B8
.equ	ReadPowerSupply	= $B4

; ----------------------------------------------------------------------------------------
.equ  BUTTON_HOLDING_TIME  	= 0  ; Пять битов   DButtonStatus[4:0] = счётчик количества полусекунд, в течение которых Кнопка удержива(лась/ется) "нажатой".  (фиксирует время до 16сек!)
.equ  BUTTON_STATUS_CODE  	= 5  ; В трёх битах DButtonStatus[7:5] = кодируется итоговый "статус-код кнопки" (см. ниже макроопределения констант).
.equ  BUTTON_HOLDEN_LONG  	= 5  ; Флаг "времени удержания" кнопки: 0-короткое или 1-длинное.
.equ  BUTTON_IS_PRESSED  	= 6  ; Флаг "зафиксировано полноценное нажатие кнопки": "0" - кнопка не нажималась, "1" - было нажатие.
.equ  BUTTON_IS_HOLDDOWN  	= 7  ; Флаг "кнопка удерживается в нажатом состоянии": "0" - сейчас кнопка "отпущена", "1" - сейчас кнопка "нажата и удерживается".

.equ  BSC_NotPressed  		= 0b000  ; "не нажата"    (исходное положение для всех кнопок - бывает только после "сброса")
.equ  BSC_ShortHold  		= 0b100  ; "короткое удержание"  (кнопка нажата, и всё ещё удерживается, пока "короткое" время)
.equ  BSC_LongHold  		= 0b101  ; "длинное удержание"  (кнопка нажата, и всё ещё удерживается, уже "длительное" время)
.equ  BSC_ShortPress  		= 0b010  ; "короткое нажатие"  (кнопка была нажата, и затем отпущена, а время её удержания было "незначительным")
.equ  BSC_LongPress  		= 0b011  ; "длинное нажатие"  (кнопка была нажата, и затем отпущена, а время её удержания было "длительным")

.equ  CShortButtonTouchDuration = 10  	; длительность длинного нажатия в х100 мс
.equ  CButtonInputChannelCount = 1		; количество кнопок

; ----------------------------------------------------------------------------------------
; -- Управляющие коды для вывода ---
.equ	EOS			=	$80		;	Признак конца сообщения
.equ	FONT		=	$81		;	Номер шрифта в следующем байте

.equ	XY			=	$83		;	Установить координаты вывода следующие 2 байта
.equ	dXdY		=	$84		;	Сдвинуть вывод на величину следующих 2 байтов
.equ	Inv			=	$85		;	Включить инверсный вывод
.equ	Disable		=	$86		;	Запретить вывод следующего управляющего символа

; -- Строки для вывода, закодированные одним байтом --------------------------------------
.equ	MessageEx	=	$87		;	Вариант выхода из режима настройки отбора
.equ	Message		=	$88		;	Сообщение
.equ	PressureMM	=	$89		;	Атмосферное давление мм.рт.ст (усредненное)
.equ	StepsCount	=	$8A		;	Счетчик шагов
.equ	StepsFor100	=	$8B		;	Количество шагов на 100 мл
.equ	Rate		=	$8C		;	Скорость отбора мл/час
.equ	Total		=	$8D		;	Общий объём отбора, мл
.equ	Remain		=	$8E		;	Остаток до паузы, мл
.equ	Dose		=	$8F		;	Периодичность остановки в мл, если 0, то без остановки

.equ	T0			=	$90		;	Т датчик 0
.equ	T1			=	$91		;	Т датчик 1
.equ	T2			=	$92		;	Т датчик 2
.equ	T3			=	$93		;	Т датчик 3

.equ	MARGIN0		=	$94		;	порог срабатывания датчик 0
.equ	MARGIN1		=	$95		;	порог срабатывания датчик 1
.equ	MARGIN2		=	$96		;	порог срабатывания датчик 2
.equ	MARGIN3		=	$97		;	порог срабатывания датчик 3

.equ	DEL_UP0		=	$98		;	задержка срабатывания датчик 0
.equ	DEL_UP1		=	$99		;	задержка срабатывания датчик 1
.equ	DEL_UP2		=	$9A		;	задержка срабатывания датчик 2
.equ	DEL_UP3		=	$9B		;	задержка срабатывания датчик 3

.equ	DEL_DN0		=	$9C		;	задержка отключения датчик 0
.equ	DEL_DN1		=	$9D		;	задержка отключения датчик 1
.equ	DEL_DN2		=	$9E		;	задержка отключения датчик 2
.equ	DEL_DN3		=	$9F		;	задержка отключения датчик 3

.equ	REACT_0		=	$A0		;	тип реакции на срабатывание датчика 0
.equ	REACT_1		=	$A1		;	тип реакции на срабатывание датчика 1
.equ	REACT_2		=	$A2		;	тип реакции на срабатывание датчика 2
.equ	REACT_3		=	$A3		;	тип реакции на срабатывание датчика 3

.equ	BACK_T0		=	$A4		;	обратный счетчик датчика 0
.equ	BACK_T1		=	$A5		;	обратный счетчик датчика 1
.equ	BACK_T2		=	$A6		;	обратный счетчик датчика 2
.equ	BACK_T3		=	$A7		;	обратный счетчик датчика 3

.equ	StabModeV	=	$A8		;	режим работы внешнего стабилизатора напряжения
.equ	VoltageT	=	$A9		;	целевое напряжение стабилизатора в десятых долях вольта VOLTS+0,+1
.equ	VoltageA	=	$AA		;	усредненное текущее напряжение стабилизатора в десятых долях вольта VOLTS+2,+3
.equ	StabInd		=	$AB		;	индикатор стабилизации

.equ	R_On0		=	$AC		;	индикатор включенной реакции T0
.equ	R_On1		=	$AD		;	индикатор включенной реакции T0
.equ	R_On2		=	$AE		;	индикатор включенной реакции T0
.equ	R_On3		=	$AF		;	индикатор включенной реакции T0

.equ	HStatus		=	$B0		;	состояние контроля разгона вкл/выкл FF/00
.equ	HSensor		=	$B1		;	номер сенсора, следящего за разгоном
.equ	HMargin		=	$B2		;	порог выключения разгона
.equ	HSensorVal	=	$B3		;	текущее значение выбранного датчика
.equ	HString		=	$B4		;	строка состояния контроля

; ----------------------------------------------------------------------------------------

