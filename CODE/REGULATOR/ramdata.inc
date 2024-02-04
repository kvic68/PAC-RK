.dseg

DInputIntegrator:			.BYTE 1
DButtonStatus:				.BYTE 1

OPTO_ON_TICK	:			.BYTE 2				; Значение момента включения оптрона записывается в OCR1A
OPTO_OFF_TICK	:			.BYTE 2
COUNT_NEG		:			.BYTE 2
COUNT_POS		:			.BYTE 2

REG_AB			:			.BYTE 1				; Счетчик энкодера
T0CompareEnabled:			.BYTE 1
BUTTON_STATE	:			.BYTE 1
BUTTON_COUNTER	:			.BYTE 1
ROTATE_COUNTER	:			.BYTE 1
ExtForcePrev	:			.BYTE 1
ExtForceCounter	:			.BYTE 1

STEP_U			:			.BYTE 2				; Шаг изменения напряжения 1 ед = 100 мВ
FL_LOCK			:			.BYTE 1				; Флаг разрешения вывода на индикатор
FL_MAX_U		:			.BYTE 1				; Флаг максимального значения U
FL_RANGE		:			.BYTE 1				; Флаг нахождения напряжения в диапазоне 00 - вне диапазона, FF - в диапазоне
MODE			:			.BYTE 1				; Режим работы 0 - нормальный, 1 - разгон(максимум), 2 - стоп(выключено), 3 - внешний стоп

ADC_CURRENT_SUM_QUAD	:	.BYTE 4						; Сумма квадратов отсчетов АЦП в текущем периоде
ADC_CURRENT_VALUE_COUNT	:	.BYTE 2						; Количество преобразований АЦП в текущем периоде
CURRENT_QUAD_U			:	.BYTE 4						; Квадрат напряжения

AVERAGE_U				:	.BYTE 2						; Текущее значение усредненного URMS
TARGET_U				:	.BYTE 2						; Задаваемое напряжение
OUTPUT_U_POINTER		:	.BYTE 2						; ссылка на отображаемое значение большими цифирями
//OUTPUT_U				:	.BYTE 2						; Отображаемое на индикаторе большими цифрами
CURRENT_U				:	.BYTE 2						; Текущее значение URMS
CURRENT_U_HISTORY		:	.BYTE 2*CU_HISTORY_LENGTH
//AVERAGE_U_OLD			:	.BYTE 2						; Старое значение усредненного URMS

EXP100MS				:	.BYTE 1						; Счетчик 100 мс 
BRIGHTNESS:					.BYTE 1

milliseconds:				.BYTE 1		; 1/1000 сек
santiseconds:				.BYTE 1		; 1/100
deciseconds:				.BYTE 1		; 1/10
seconds:					.BYTE 1		; 1 сек

ACTIONS_FLAGS			:	.BYTE 1		; Флаги для вызова определенных действий
										; 0 - обработка принятого по UART 
										; 1 - обработка принятого по iic
										; 2 -
										; 3 - короткое нажатие кнопки
										; 4 - длительное удержание кнопки
										; 5 - энкодер в одну   сторону +
										; 6 - энкодер в другую сторону -
										; 7 - заброс данных в уарт
DATA_FLAGS				:	.BYTE 1		; 
										; 0 - 1000 мс
										; 1 -  100 мс
										; 2 -   10 мс
										; 3 -    1 мс
										; 4 -
										; 5 -
										; 6 -
										; 7 -
RX_BUFFER				:	.BYTE 32	; буфер приема
TX_BUFFER				:	.BYTE 32	; буфер передачи
RX_PTR					:	.BYTE 1		; указатель буфера приема 
TX_PTR					:	.BYTE 1		; указатель буфера передачи

POSX:						.BYTE 1
POSY:						.BYTE 1
OLDX:						.BYTE 1
OLDY:						.BYTE 1
FontAddr:					.BYTE 2
FontNo:						.BYTE 1
StringCounter:				.BYTE 1
//MASK:						.BYTE 1

LED_ADDRESS				:	.BYTE 1
COUNTER_D				:	.BYTE 1		; счетчик невхождений в диапазон
ExtStopCounter			:	.BYTE 1		; счетчик времени нахождения сигнала внешнего стопа в активном уровне
ExtFullPowerCounter		:	.BYTE 1		;

DIGIT0					:	.BYTE 1
DIGIT1					:	.BYTE 1
DIGIT2					:	.BYTE 1
DIGIT3					:	.BYTE 1
DIGIT4					:	.BYTE 1
DIGIT5					:	.BYTE 1
DIGIT6					:	.BYTE 1
DIGIT7					:	.BYTE 1
DIGIT8					:	.BYTE 1
DIGIT9					:	.BYTE 1
HidedZeroesCount		:	.BYTE 1
//--
ExtrasConfig:		.BYTE 128			; таблица координат вывода значений, закодированных символами
										; byte+0:	координата Х (0...191)
										; byte+1:	биты 0...2 	- координата Y (0...7)
										; 			бит	 4...5	- размер шрифта (0-3)
										; 			бит  6		- разрешение вывода (0-вывод выкл, 1-вывод вкл)
										; 			бит  7		- признак изменившегося значения (0-старое, 1-новое)

