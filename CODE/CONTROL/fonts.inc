FontAdressesTable:	.dw	FONT6x8		; 	00
					.dw FONT12x16	;	01
					
// ----------------------------------------------------------- 

FONT12x16:

.DB $00,$FF,12,2	; ��� ���������� �������, ��� ��������� �������, ������(�������), ������(����� �� 8)

.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00;	$00 � ������
.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00;
.DB	$00,$F0,$0C,$02,$32,$31,$01,$31,$32,$02,$0C,$F0;	$01 � ����
.DB	$00,$03,$0C,$10,$11,$22,$22,$22,$11,$10,$0C,$03;
.DB	$00,$F0,$FC,$FE,$CE,$CF,$FF,$CF,$CE,$FE,$FC,$F0;	$02 - ���� ��������
.DB	$00,$03,$0F,$1F,$1E,$3D,$3D,$3D,$1E,$1F,$0F,$03;
.DB	$00,$00,$F0,$F8,$F8,$F0,$E0,$F0,$F8,$F8,$F0,$00;	$03 � �����
.DB	$00,$00,$00,$01,$03,$07,$0F,$07,$03,$01,$00,$00;
.DB	$00,$00,$80,$C0,$E0,$F0,$F8,$F0,$E0,$C0,$80,$00;	$04 - �����
.DB	$00,$00,$00,$01,$03,$07,$0F,$07,$03,$01,$00,$00;
.DB	$00,$80,$C0,$C0,$B8,$FC,$FC,$FC,$B8,$C0,$C0,$80;	$05 - ������
.DB	$00,$03,$07,$07,$13,$1B,$1F,$1B,$13,$07,$07,$03;
.DB	$00,$00,$80,$C0,$E0,$F0,$FC,$F0,$E0,$C0,$80,$00;	$06 - ����
.DB	$00,$00,$03,$07,$17,$1B,$1F,$1B,$17,$07,$03,$00;
.DB	$00,$00,$00,$80,$C0,$E0,$E0,$C0,$80,$00,$00,$00;	$07 - ����������� ����� �� ������
.DB	$00,$00,$00,$01,$03,$07,$07,$03,$01,$00,$00,$00;
.DB	$FF,$FF,$FF,$FF,$7F,$3F,$3F,$7F,$FF,$FF,$FF,$FF;	$08 - ����������� ��������� ����� �� ������
.DB	$FF,$FF,$FF,$FF,$FE,$FC,$FC,$FE,$FF,$FF,$FF,$FF;
.DB	$00,$F8,$F8,$18,$18,$18,$18,$18,$F8,$F8,$00,$00;	$09 - �������
.DB	$00,$1F,$1F,$18,$18,$18,$18,$18,$1F,$1F,$00,$00;
.DB	$FF,$FF,$FF,$3F,$DF,$EF,$EF,$DF,$3F,$FF,$FF,$FF;	$0A - �� ����������� ��������� ���� �� ������
.DB	$FF,$FF,$FF,$FC,$FB,$F7,$F7,$FB,$FC,$FF,$FF,$FF;
.DB	$00,$C0,$30,$10,$08,$88,$48,$30,$30,$C8,$00,$00;	$0B - �������
.DB	$00,$09,$06,$06,$09,$08,$08,$04,$06,$01,$00,$00;
.DB	$00,$00,$38,$44,$82,$82,$82,$44,$38,$00,$00,$00;	$0C - ������
.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00;
.DB	$00,$00,$00,$00,$00,$00,$FF,$0E,$7C,$38,$00,$00;	$0D - ����
.DB	$00,$00,$00,$0C,$1E,$1E,$0F,$00,$00,$00,$00,$00;
.DB	$00,$F0,$F8,$0C,$06,$06,$06,$06,$06,$0C,$F8,$F0;	$0E - �����
.DB	$00,$33,$33,$3F,$3E,$00,$00,$00,$3E,$3F,$33,$01;
.DB	$00,$80,$C8,$F0,$30,$18,$1E,$18,$30,$F0,$C8,$80;	$0F - ������
.DB	$00,$00,$09,$07,$06,$0C,$3C,$0C,$06,$07,$09,$00;
.DB	$00,$00,$00,$FC,$F8,$F0,$E0,$C0,$80,$00,$00,$00;	$10 - ������� ������� ������
.DB	$00,$00,$00,$1F,$0F,$07,$03,$01,$00,$00,$00,$00;
.DB	$00,$00,$00,$80,$C0,$E0,$F0,$F8,$FC,$00,$00,$00;	$11 - ������� ������� �����
.DB	$00,$00,$00,$00,$01,$03,$07,$0F,$1F,$00,$00,$00;
.DB	$00,$00,$00,$10,$18,$1C,$FE,$1C,$18,$10,$00,$00;	$12 - ����� ������� �����/����
.DB	$00,$00,$00,$04,$0C,$1C,$3F,$1C,$0C,$04,$00,$00;
.DB	$00,$00,$30,$30,$30,$FE,$FE,$30,$30,$30,$00,$00;	$13 - ����/�����
.DB	$00,$00,$18,$18,$18,$19,$19,$18,$18,$18,$00,$00;
.DB	$00,$38,$7C,$C6,$82,$FE,$FE,$02,$FE,$FE,$02,$00;	$14 - "��"
.DB	$00,$00,$00,$00,$00,$3F,$3F,$00,$3F,$3F,$00,$00;
.DB	$00,$00,$DC,$FE,$22,$22,$22,$22,$E6,$C4,$00,$00;	$15 - ��������
.DB	$00,$00,$08,$19,$11,$11,$11,$11,$1F,$0E,$00,$00;
.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00;	$16 - ������ �������������
.DB	$00,$00,$00,$38,$38,$38,$38,$38,$38,$38,$38,$00;
.DB	$00,$00,$62,$51,$51,$49,$46,$00,$00,$00,$00,$00;	$17 - ���� "� ��������"
.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00;
.DB	$00,$00,$00,$10,$18,$1C,$FE,$1C,$18,$10,$00,$00;	$18 - ����� ������� �����
.DB	$00,$00,$00,$00,$00,$00,$3F,$00,$00,$00,$00,$00;
.DB	$00,$00,$00,$00,$00,$00,$FE,$00,$00,$00,$00,$00;	$19 - ����� ������� ����
.DB	$00,$00,$00,$04,$0C,$1C,$3F,$1C,$0C,$04,$00,$00;
.DB	$00,$00,$80,$80,$80,$80,$80,$F0,$E0,$C0,$80,$00;	$1A - ����� ������� ������
.DB	$00,$00,$00,$00,$00,$00,$00,$07,$03,$01,$00,$00;
.DB	$00,$00,$80,$C0,$E0,$F0,$80,$80,$80,$80,$80,$00;	$1B - ����� ������� �����
.DB	$00,$00,$00,$01,$03,$07,$00,$00,$00,$00,$00,$00;
.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00;	$1C - ���� "������"
.DB	$00,$3F,$20,$20,$20,$20,$20,$20,$20,$20,$20,$00;
.DB	$00,$80,$C0,$E0,$F0,$80,$80,$80,$F0,$E0,$C0,$80;	$1D - ����� ������� �����/������
.DB	$00,$00,$01,$03,$07,$00,$00,$00,$07,$03,$01,$00;
.DB	$00,$00,$00,$00,$80,$C0,$E0,$C0,$80,$00,$00,$00;	$1E - ������� ������� �����
.DB	$00,$04,$06,$07,$07,$07,$07,$07,$07,$07,$06,$04;
.DB	$00,$20,$60,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$60,$20;	$1F - ������� ������� ����
.DB	$00,$00,$00,$00,$01,$03,$07,$03,$01,$00,$00,$00;
.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00;	$20 - ������
.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00;
.DB	$00,$00,$00,$00,$7C,$FF,$FF,$7C,$00,$00,$00,$00;	$21 - !
.DB	$00,$00,$00,$00,$00,$33,$33,$00,$00,$00,$00,$00;
.DB	$00,$00,$00,$3C,$3C,$00,$00,$3C,$3C,$00,$00,$00;	$22 - "
.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00;
.DB	$00,$00,$10,$90,$F0,$7E,$1E,$90,$F0,$7E,$1E,$10;	$23 - #
.DB	$00,$02,$1E,$1F,$03,$02,$1E,$1F,$03,$02,$00,$00;
.DB	$00,$00,$78,$FC,$CC,$FF,$FF,$CC,$CC,$88,$00,$00;	$24 - $
.DB	$00,$00,$04,$0C,$0C,$3F,$3F,$0C,$0F,$07,$00,$00;
.DB	$00,$00,$38,$38,$38,$00,$80,$C0,$E0,$70,$38,$1C;	$25 - %
.DB	$00,$30,$38,$1C,$0E,$07,$03,$01,$38,$38,$38,$00;
.DB	$00,$00,$00,$B8,$FC,$C6,$E2,$3E,$1C,$00,$00,$00;	$26 - &
.DB	$00,$00,$1F,$3F,$31,$21,$37,$1E,$1C,$36,$22,$00;
.DB	$00,$00,$00,$00,$27,$3F,$1F,$00,$00,$00,$00,$00;	$27 - '
.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00;
.DB	$00,$00,$00,$F0,$FC,$FE,$07,$01,$01,$00,$00,$00;	$28 - (
.DB	$00,$00,$00,$03,$0F,$1F,$38,$20,$20,$00,$00,$00;
.DB	$00,$00,$00,$01,$01,$07,$FE,$FC,$F0,$00,$00,$00;	$29 - )
.DB	$00,$00,$00,$20,$20,$38,$1F,$0F,$03,$00,$00,$00;
.DB	$00,$00,$98,$B8,$E0,$F8,$F8,$E0,$B8,$98,$00,$00;	$2A - *
.DB	$00,$00,$0C,$0E,$03,$0F,$0F,$03,$0E,$0C,$00,$00;
.DB	$00,$00,$80,$80,$80,$F0,$F0,$80,$80,$80,$00,$00;	$2B - +
.DB	$00,$00,$01,$01,$01,$0F,$0F,$01,$01,$01,$00,$00;
.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00;	$2C - ,
.DB	$00,$00,$00,$00,$B8,$F8,$78,$00,$00,$00,$00,$00;
.DB	$00,$00,$80,$80,$80,$80,$80,$80,$80,$80,$00,$00;	$2D - -
.DB	$00,$00,$01,$01,$01,$01,$01,$01,$01,$01,$00,$00;
	.DB	$00,$00,$00,$00,$00,$00,	$00,$00,$38,$38,$38,$00;	$2E - .
	.DB	$00,$00,$00,$00,$38,$38,	$38,$00,$00,$00,$00,$00;
//.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00;	$2E - .
//.DB	$00,$00,$00,$00,$38,$38,$38,$00,$00,$00,$00,$00;
.DB	$00,$00,$00,$00,$00,$80,$C0,$E0,$70,$38,$1C,$0E;	$2F - /
.DB	$00,$18,$1C,$0E,$07,$03,$01,$00,$00,$00,$00,$00;

.DB	$00,$F8,$FE,$06,$03,$83,$C3,$63,$33,$1E,$FE,$F8;	$30 - 0
.DB	$00,$07,$1F,$1E,$33,$31,$30,$30,$30,$18,$1F,$07;

.DB	$00,$00,$00,$0C,$0C,$0E,$FF,$FF,$00,$00,$00,$00;	$31 - 1
.DB	$00,$00,$00,$30,$30,$30,$3F,$3F,$30,$30,$30,$00;
.DB	$00,$1C,$1E,$07,$03,$03,$83,$C3,$E3,$77,$3E,$1C;	$32 - 2
.DB	$00,$30,$38,$3C,$3E,$37,$33,$31,$30,$30,$30,$30;
.DB	$00,$0C,$0E,$07,$03,$03,$C3,$C3,$C3,$E7,$7E,$3C;	$33 - 3
.DB	$00,$0C,$1C,$38,$30,$30,$30,$30,$30,$39,$1F,$0E;
.DB	$00,$C0,$E0,$70,$38,$1C,$0E,$07,$FF,$FF,$00,$00;	$34 - 4
.DB	$00,$03,$03,$03,$03,$03,$03,$03,$3F,$3F,$03,$03;
.DB	$00,$3F,$7F,$63,$63,$63,$63,$63,$63,$E3,$C3,$83;	$35 - 5
.DB	$00,$0C,$1C,$38,$30,$30,$30,$30,$30,$38,$1F,$0F;
.DB	$00,$C0,$F0,$F8,$DC,$CE,$C7,$C3,$C3,$C3,$80,$00;	$36 - 6
.DB	$00,$0F,$1F,$39,$30,$30,$30,$30,$30,$39,$1F,$0F;
.DB	$00,$03,$03,$03,$03,$03,$03,$C3,$F3,$3F,$0F,$03;	$37 - 7
.DB	$00,$00,$00,$00,$30,$3C,$0F,$03,$00,$00,$00,$00;
.DB	$00,$00,$BC,$FE,$E7,$C3,$C3,$C3,$E7,$FE,$BC,$00;	$38 - 8
.DB	$00,$0F,$1F,$39,$30,$30,$30,$30,$30,$39,$1F,$0F;
.DB	$00,$3C,$7E,$E7,$C3,$C3,$C3,$C3,$C3,$E7,$FE,$FC;	$39 - 9
.DB	$00,$00,$00,$30,$30,$30,$38,$1C,$0E,$07,$03,$00;
.DB	$00,$00,$00,$00,$70,$70,$70,$00,$00,$00,$00,$00;	$3A - :
.DB	$00,$00,$00,$00,$1C,$1C,$1C,$00,$00,$00,$00,$00;
.DB	$00,$00,$00,$00,$70,$70,$70,$00,$00,$00,$00,$00;	$3B - ;
.DB	$00,$00,$00,$00,$9C,$FC,$7C,$00,$00,$00,$00,$00;
.DB	$00,$00,$C0,$E0,$F0,$38,$1C,$0E,$07,$03,$00,$00;	$3C - <
.DB	$00,$00,$00,$01,$03,$07,$0E,$1C,$38,$30,$00,$00;
.DB	$00,$00,$60,$60,$60,$60,$60,$60,$60,$60,$60,$00;	$3D - =
.DB	$00,$00,$06,$06,$06,$06,$06,$06,$06,$06,$06,$00;
.DB	$00,$00,$03,$07,$0E,$1C,$38,$F0,$E0,$C0,$00,$00;	$3E - >
.DB	$00,$00,$30,$38,$1C,$0E,$07,$03,$01,$00,$00,$00;
.DB	$00,$1C,$1E,$07,$03,$83,$C3,$E3,$77,$3E,$1C,$00;	$3F - ?
.DB	$00,$00,$00,$00,$00,$37,$37,$00,$00,$00,$00,$00;
.DB	$00,$F8,$FE,$07,$F3,$FB,$1B,$FB,$FB,$07,$FE,$F8;	$40 - @
.DB	$00,$0F,$1F,$18,$33,$37,$36,$37,$37,$36,$03,$01;
.DB	$00,$00,$00,$E0,$FC,$1F,$1F,$FC,$E0,$00,$00,$00;	$41 - A
.DB	$00,$38,$3F,$07,$06,$06,$06,$06,$07,$3F,$38,$00;
.DB	$00,$FF,$FF,$C3,$C3,$C3,$C3,$E7,$FE,$BC,$00,$00;	$42 - B
.DB	$00,$3F,$3F,$30,$30,$30,$30,$30,$39,$1F,$0F,$00;
.DB	$00,$F0,$FC,$0E,$07,$03,$03,$03,$07,$0E,$0C,$00;	$43 - C
.DB	$00,$03,$0F,$1C,$38,$30,$30,$30,$38,$1C,$0C,$00;
.DB	$00,$FF,$FF,$03,$03,$03,$03,$07,$0E,$FC,$F0,$00;	$44 - D
.DB	$00,$3F,$3F,$30,$30,$30,$30,$38,$1C,$0F,$03,$00;
.DB	$00,$FF,$FF,$C3,$C3,$C3,$C3,$C3,$C3,$03,$03,$00;	$45 - E
.DB	$00,$3F,$3F,$30,$30,$30,$30,$30,$30,$30,$30,$00;
.DB	$00,$FF,$FF,$C3,$C3,$C3,$C3,$C3,$C3,$03,$03,$00;	$46 - F
.DB	$00,$3F,$3F,$00,$00,$00,$00,$00,$00,$00,$00,$00;
.DB	$00,$F0,$FC,$0E,$07,$03,$C3,$C3,$C3,$C7,$C6,$00;	$47 - G
.DB	$00,$03,$0F,$1C,$38,$30,$30,$30,$30,$3F,$3F,$00;
.DB	$00,$FF,$FF,$C0,$C0,$C0,$C0,$C0,$C0,$FF,$FF,$00;	$48 - H
.DB	$00,$3F,$3F,$00,$00,$00,$00,$00,$00,$3F,$3F,$00;
.DB	$00,$00,$00,$03,$03,$FF,$FF,$03,$03,$00,$00,$00;	$49 - I
.DB	$00,$00,$00,$30,$30,$3F,$3F,$30,$30,$00,$00,$00;
.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$FF,$FF,$00;	$4A - J
.DB	$00,$0E,$1E,$38,$30,$30,$30,$30,$38,$1F,$07,$00;
.DB	$00,$FF,$FF,$C0,$E0,$F0,$38,$1C,$0E,$07,$03,$00;	$4B - K
.DB	$00,$3F,$3F,$00,$01,$03,$07,$0E,$1C,$38,$30,$00;
.DB	$00,$FF,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00;	$4C - L
.DB	$00,$3F,$3F,$30,$30,$30,$30,$30,$30,$30,$30,$00;
.DB	$00,$FF,$FF,$1E,$78,$E0,$E0,$78,$1E,$FF,$FF,$00;	$4D - M
.DB	$00,$3F,$3F,$00,$00,$01,$01,$00,$00,$3F,$3F,$00;
.DB	$00,$FF,$FF,$0E,$38,$F0,$C0,$00,$00,$FF,$FF,$00;	$4E - N
.DB	$00,$3F,$3F,$00,$00,$00,$03,$07,$1C,$3F,$3F,$00;
.DB	$00,$F0,$FC,$0E,$07,$03,$03,$07,$0E,$FC,$F0,$00;	$4F - O
.DB	$00,$03,$0F,$1C,$38,$30,$30,$38,$1C,$0F,$03,$00;
.DB	$00,$FF,$FF,$83,$83,$83,$83,$83,$C7,$FE,$7C,$00;	$50 - P
.DB	$00,$3F,$3F,$01,$01,$01,$01,$01,$01,$00,$00,$00;
.DB	$00,$F0,$FC,$0E,$07,$03,$03,$07,$0E,$FC,$F0,$00;	$51 - Q
.DB	$00,$03,$0F,$1C,$38,$30,$36,$3E,$1C,$3F,$33,$00;
.DB	$00,$FF,$FF,$83,$83,$83,$83,$83,$C7,$FE,$7C,$00;	$52 - R
.DB	$00,$3F,$3F,$01,$01,$03,$07,$0F,$1D,$38,$30,$00;
.DB	$00,$3C,$7E,$E7,$C3,$C3,$C3,$C3,$C7,$8E,$0C,$00;	$53 - S
.DB	$00,$0C,$1C,$38,$30,$30,$30,$30,$39,$1F,$0F,$00;
.DB	$00,$00,$03,$03,$03,$FF,$FF,$03,$03,$03,$00,$00;	$54 - T
.DB	$00,$00,$00,$00,$00,$3F,$3F,$00,$00,$00,$00,$00;
.DB	$00,$FF,$FF,$00,$00,$00,$00,$00,$00,$FF,$FF,$00;	$55 - U
.DB	$00,$07,$1F,$38,$30,$30,$30,$30,$38,$1F,$07,$00;
.DB	$00,$07,$3F,$F8,$C0,$00,$00,$C0,$F8,$3F,$07,$00;	$56 - V
.DB	$00,$00,$00,$01,$0F,$3E,$3E,$0F,$01,$00,$00,$00;
.DB	$00,$FF,$FF,$00,$00,$80,$80,$00,$00,$FF,$FF,$00;	$57 - W
.DB	$00,$3F,$3F,$1C,$06,$03,$03,$06,$1C,$3F,$3F,$00;
.DB	$00,$03,$0F,$1C,$30,$E0,$E0,$30,$1C,$0F,$03,$00;	$58 - X
.DB	$00,$30,$3C,$0E,$03,$01,$01,$03,$0E,$3C,$30,$00;
.DB	$00,$03,$0F,$3C,$F0,$C0,$C0,$F0,$3C,$0F,$03,$00;	$59 - Y
.DB	$00,$00,$00,$00,$00,$3F,$3F,$00,$00,$00,$00,$00;
.DB	$00,$03,$03,$03,$03,$C3,$E3,$33,$1F,$0F,$03,$00;	$5A - Z
.DB	$00,$30,$3C,$3E,$33,$31,$30,$30,$30,$30,$30,$00;
.DB	$00,$00,$00,$FF,$FF,$03,$03,$03,$03,$00,$00,$00;	$5B - [
.DB	$00,$00,$00,$3F,$3F,$30,$30,$30,$30,$00,$00,$00;
.DB	$00,$0E,$1C,$38,$70,$E0,$C0,$80,$00,$00,$00,$00;	$5C - "\"
.DB	$00,$00,$00,$00,$00,$00,$01,$03,$07,$0E,$1C,$18;
.DB	$00,$00,$00,$03,$03,$03,$03,$FF,$FF,$00,$00,$00;	$5D - ]
.DB	$00,$00,$00,$30,$30,$30,$30,$3F,$3F,$00,$00,$00;
.DB	$00,$60,$70,$38,$1C,$0E,$07,$0E,$1C,$38,$70,$60;	$5E - ^
.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00;
.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00;	$5F - _
.DB	$00,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0;
.DB	$00,$00,$00,$3E,$3F,$39,$00,$00,$00,$00,$00,$00;	$60 - ������� ����� ������
.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00;
.DB	$00,$00,$40,$60,$60,$60,$60,$60,$60,$E0,$C0,$00;	$61 - a
.DB	$00,$1C,$3E,$33,$33,$33,$33,$33,$33,$3F,$3F,$00;
.DB	$00,$FF,$FF,$C0,$60,$60,$60,$60,$E0,$C0,$80,$00;	$62 - b
.DB	$00,$3F,$3F,$30,$30,$30,$30,$30,$38,$1F,$0F,$00;
.DB	$00,$80,$C0,$E0,$60,$60,$60,$60,$60,$C0,$80,$00;	$63 - c
.DB	$00,$0F,$1F,$38,$30,$30,$30,$30,$30,$18,$08,$00;
.DB	$00,$80,$C0,$E0,$60,$60,$60,$E0,$C0,$FF,$FF,$00;	$64 - d
.DB	$00,$0F,$1F,$38,$30,$30,$30,$30,$30,$3F,$3F,$00;
.DB	$00,$80,$C0,$E0,$60,$60,$60,$60,$60,$C0,$80,$00;	$65 - e
.DB	$00,$0F,$1F,$3B,$33,$33,$33,$33,$33,$13,$01,$00;
.DB	$00,$C0,$C0,$FC,$FE,$C7,$C3,$C3,$03,$00,$00,$00;	$66 - f
.DB	$00,$00,$00,$3F,$3F,$00,$00,$00,$00,$00,$00,$00;
.DB	$00,$80,$C0,$E0,$60,$60,$60,$60,$60,$E0,$E0,$00;	$67 - g
.DB	$00,$03,$C7,$CE,$CC,$CC,$CC,$CC,$E6,$7F,$3F,$00;
.DB	$00,$FF,$FF,$C0,$60,$60,$60,$E0,$C0,$80,$00,$00;	$68 - h
.DB	$00,$3F,$3F,$00,$00,$00,$00,$00,$3F,$3F,$00,$00;
.DB	$00,$00,$00,$00,$60,$EC,$EC,$00,$00,$00,$00,$00;	$69 - i
.DB	$00,$00,$00,$30,$30,$3F,$3F,$30,$30,$00,$00,$00;
.DB	$00,$00,$00,$00,$00,$00,$60,$EC,$EC,$00,$00,$00;	$6A - j
.DB	$00,$00,$00,$60,$E0,$C0,$C0,$FF,$7F,$00,$00,$00;
.DB	$00,$00,$FF,$FF,$00,$80,$C0,$E0,$60,$00,$00,$00;	$6B - k
.DB	$00,$00,$3F,$3F,$03,$07,$0F,$1C,$38,$30,$00,$00;
.DB	$00,$00,$00,$00,$03,$FF,$FF,$00,$00,$00,$00,$00;	$6C - l
.DB	$00,$00,$00,$30,$30,$3F,$3F,$30,$30,$00,$00,$00;
.DB	$00,$E0,$C0,$E0,$E0,$C0,$C0,$E0,$E0,$C0,$80,$00;	$6D - m
.DB	$00,$3F,$3F,$00,$00,$3F,$3F,$00,$00,$3F,$3F,$00;
.DB	$00,$00,$E0,$E0,$60,$60,$60,$60,$E0,$C0,$80,$00;	$6E - n
.DB	$00,$00,$3F,$3F,$00,$00,$00,$00,$00,$3F,$3F,$00;
.DB	$00,$80,$C0,$E0,$60,$60,$60,$60,$E0,$C0,$80,$00;	$6F - o
.DB	$00,$0F,$1F,$38,$30,$30,$30,$30,$38,$1F,$0F,$00;
.DB	$00,$E0,$E0,$60,$60,$60,$60,$60,$E0,$C0,$80,$00;	$70 - p
.DB	$00,$FF,$FF,$0C,$18,$18,$18,$18,$1C,$0F,$07,$00;
.DB	$00,$80,$C0,$E0,$60,$60,$60,$60,$60,$E0,$E0,$00;	$71 - q
.DB	$00,$07,$0F,$1C,$18,$18,$18,$18,$0C,$FF,$FF,$00;
.DB	$00,$00,$E0,$E0,$C0,$60,$60,$60,$60,$E0,$C0,$00;	$72 - r
.DB	$00,$00,$3F,$3F,$00,$00,$00,$00,$00,$00,$00,$00;
.DB	$00,$C0,$E0,$60,$60,$60,$60,$60,$40,$00,$00,$00;	$73 - s
.DB	$00,$11,$33,$33,$33,$33,$33,$3F,$1E,$00,$00,$00;
.DB	$00,$60,$60,$FE,$FE,$60,$60,$60,$00,$00,$00,$00;	$74 - t
.DB	$00,$00,$00,$1F,$3F,$30,$30,$30,$30,$00,$00,$00;
.DB	$00,$E0,$E0,$00,$00,$00,$00,$00,$00,$E0,$E0,$00;	$75 - u
.DB	$00,$0F,$1F,$38,$30,$30,$30,$30,$18,$3F,$3F,$00;
.DB	$00,$60,$E0,$80,$00,$00,$00,$00,$80,$E0,$60,$00;	$76 - v
.DB	$00,$00,$01,$07,$1E,$38,$38,$1E,$07,$01,$00,$00;
.DB	$00,$E0,$E0,$00,$00,$E0,$E0,$00,$00,$E0,$E0,$00;	$77 - w
.DB	$00,$07,$1F,$38,$1C,$0F,$0F,$1C,$38,$1F,$07,$00;
.DB	$00,$60,$E0,$C0,$80,$00,$80,$C0,$E0,$60,$00,$00;	$78 - x
.DB	$00,$30,$38,$1D,$0F,$07,$0F,$1D,$38,$30,$00,$00;
.DB	$00,$00,$60,$E0,$80,$00,$00,$80,$E0,$60,$00,$00;	$79 - y
.DB	$00,$00,$00,$81,$E7,$7E,$1E,$07,$01,$00,$00,$00;
.DB	$00,$60,$60,$60,$60,$60,$E0,$E0,$60,$20,$00,$00;	$7A - z
.DB	$00,$30,$38,$3C,$36,$33,$31,$30,$30,$30,$00,$00;
.DB	$00,$00,$80,$C0,$FC,$7E,$07,$03,$03,$03,$00,$00;	$7B - {
.DB	$00,$00,$00,$01,$1F,$3F,$70,$60,$60,$60,$00,$00;
.DB	$00,$00,$00,$00,$00,$FF,$FF,$00,$00,$00,$00,$00;	$7C - |
.DB	$00,$00,$00,$00,$00,$3F,$3F,$00,$00,$00,$00,$00;
.DB	$00,$00,$03,$03,$03,$07,$7E,$FC,$C0,$80,$00,$00;	$7D - }
.DB	$00,$00,$60,$60,$60,$70,$3F,$1F,$01,$00,$00,$00;
.DB	$00,$10,$18,$0C,$04,$0C,$18,$10,$18,$0C,$04,$00;	$7E - ~
.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00;
.DB	$00,$00,$80,$C0,$60,$30,$30,$60,$C0,$80,$00,$00;	$7F - �����
.DB	$00,$0F,$0F,$0C,$0C,$0C,$0C,$0C,$0C,$0F,$0F,$00;


.DB	$00,$F0,$FC,$0E,$07,$03,$03,$03,$03,$FF,$FF,$00;	$80 - �
.DB	$00,$3F,$3F,$03,$03,$03,$03,$03,$03,$3F,$3F,$00;
.DB	$00,$FF,$FF,$C3,$C3,$C3,$C3,$C3,$C3,$83,$03,$00;	$81 - �
.DB	$00,$3F,$3F,$30,$30,$30,$30,$30,$39,$1F,$0F,$00;
.DB	$00,$FF,$FF,$C3,$C3,$C3,$C3,$E7,$FE,$BC,$00,$00;	$82 - �
.DB	$00,$3F,$3F,$30,$30,$30,$30,$30,$39,$1F,$0F,$00;
.DB	$00,$FF,$FF,$03,$03,$03,$03,$03,$03,$03,$03,$00;	$83 - �
.DB	$00,$3F,$3F,$00,$00,$00,$00,$00,$00,$00,$00,$00;
.DB	$00,$F0,$FC,$0E,$07,$03,$03,$03,$03,$FF,$FF,$00;	$84 - �
.DB	$F0,$FF,$3F,$30,$30,$30,$30,$30,$30,$3F,$FF,$F0;
.DB	$00,$FF,$FF,$C3,$C3,$C3,$C3,$C3,$C3,$03,$03,$00;	$85 - �
.DB	$00,$3F,$3F,$30,$30,$30,$30,$30,$30,$30,$30,$00;
.DB	$0F,$3E,$F8,$E0,$C0,$FF,$FF,$C0,$E0,$F8,$1E,$07;	$86 - �
.DB	$3C,$1F,$07,$01,$00,$3F,$3F,$00,$01,$07,$1F,$3C;
.DB	$00,$0C,$0E,$07,$C3,$C3,$C3,$C3,$C3,$E7,$7E,$3C;	$87 - �
.DB	$00,$0C,$1C,$38,$30,$30,$30,$30,$30,$39,$1F,$0E;
.DB	$00,$FF,$FF,$00,$00,$C0,$F0,$38,$0E,$FF,$FF,$00;	$88 - �
.DB	$00,$3F,$3F,$1C,$07,$03,$00,$00,$00,$3F,$3F,$00;
.DB	$00,$FF,$FF,$00,$00,$C6,$F3,$38,$0E,$FF,$FF,$00;	$89 - �
.DB	$00,$3F,$3F,$1C,$07,$03,$00,$00,$00,$3F,$3F,$00;
.DB	$00,$FF,$FF,$C0,$E0,$F0,$38,$1C,$0E,$07,$03,$00;	$8A - �
.DB	$00,$3F,$3F,$00,$01,$03,$07,$0E,$1C,$38,$30,$00;
.DB	$00,$F0,$FC,$0E,$07,$03,$03,$03,$03,$FF,$FF,$00;	$8B - �
.DB	$00,$3F,$3F,$00,$00,$00,$00,$00,$00,$3F,$3F,$00;
.DB	$00,$FF,$FF,$1E,$78,$E0,$E0,$78,$1E,$FF,$FF,$00;	$8C - �
.DB	$00,$3F,$3F,$00,$00,$01,$01,$00,$00,$3F,$3F,$00;
.DB	$00,$FF,$FF,$C0,$C0,$C0,$C0,$C0,$C0,$FF,$FF,$00;	$8D - �
.DB	$00,$3F,$3F,$00,$00,$00,$00,$00,$00,$3F,$3F,$00;
.DB	$00,$F0,$FC,$0E,$07,$03,$03,$07,$0E,$FC,$F0,$00;	$8E - �
.DB	$00,$03,$0F,$1C,$38,$30,$30,$38,$1C,$0F,$03,$00;
.DB	$00,$FF,$FF,$03,$03,$03,$03,$03,$03,$FF,$FF,$00;	$8F - �
.DB	$00,$3F,$3F,$00,$00,$00,$00,$00,$00,$3F,$3F,$00;
.DB	$00,$FF,$FF,$83,$83,$83,$83,$83,$C7,$FE,$7C,$00;	$90 - �
.DB	$00,$3F,$3F,$01,$01,$01,$01,$01,$01,$00,$00,$00;
.DB	$00,$F0,$FC,$0E,$07,$03,$03,$03,$07,$0E,$0C,$00;	$91 - �
.DB	$00,$03,$0F,$1C,$38,$30,$30,$30,$38,$1C,$0C,$00;
.DB	$00,$00,$03,$03,$03,$FF,$FF,$03,$03,$03,$00,$00;	$92 - �
.DB	$00,$00,$00,$00,$00,$3F,$3F,$00,$00,$00,$00,$00;
.DB	$00,$7F,$FF,$C0,$C0,$C0,$C0,$C0,$C0,$FF,$FF,$00;	$93 - �
.DB	$00,$18,$38,$30,$30,$30,$30,$30,$38,$1F,$07,$00;
.DB	$00,$F8,$FC,$0C,$0C,$FF,$FF,$0C,$0C,$FC,$F8,$00;	$94 - �
.DB	$00,$03,$07,$06,$06,$3F,$3F,$06,$06,$07,$03,$00;
.DB	$00,$03,$0F,$1C,$30,$E0,$E0,$30,$1C,$0F,$03,$00;	$95 - �
.DB	$00,$30,$3C,$0E,$03,$01,$01,$03,$0E,$3C,$30,$00;
.DB	$00,$FF,$FF,$00,$00,$00,$00,$00,$FF,$FF,$00,$00;	$96 - �
.DB	$00,$3F,$3F,$30,$30,$30,$30,$30,$3F,$FF,$F0,$00;
.DB	$00,$7F,$FF,$C0,$C0,$C0,$C0,$C0,$C0,$FF,$FF,$00;	$97 - �
.DB	$00,$00,$00,$00,$00,$00,$00,$00,$00,$3F,$3F,$00;
.DB	$00,$FF,$FF,$00,$00,$FF,$FF,$00,$00,$FF,$FF,$00;	$98 - �
.DB	$00,$3F,$3F,$30,$30,$3F,$3F,$30,$30,$3F,$3F,$00;
.DB	$00,$FF,$FF,$00,$00,$FF,$FF,$00,$00,$FF,$FF,$00;	$99 - �
.DB	$00,$3F,$3F,$30,$30,$3F,$3F,$30,$30,$3F,$FF,$F0;
.DB	$0F,$0F,$03,$FF,$FF,$60,$60,$60,$E0,$C0,$80,$00;	$9A - �
.DB	$00,$00,$00,$3F,$3F,$30,$30,$30,$38,$1F,$0F,$00;
.DB	$00,$FF,$FF,$60,$60,$E0,$C0,$80,$00,$FF,$FF,$00;	$9B - �
.DB	$00,$3F,$3F,$30,$30,$38,$1F,$0F,$00,$3F,$3F,$00;
.DB	$00,$FF,$FF,$60,$60,$60,$60,$60,$E0,$C0,$80,$00;	$9C - �
.DB	$00,$3F,$3F,$30,$30,$30,$30,$30,$38,$1F,$0F,$00;
.DB	$00,$0C,$0E,$07,$C3,$C3,$C3,$C7,$CE,$FC,$F0,$00;	$9D - �
.DB	$00,$0C,$1C,$38,$30,$30,$30,$38,$1C,$0F,$03,$00;
.DB	$FF,$FF,$C0,$F0,$FC,$0E,$07,$03,$07,$0E,$FC,$F0;	$9E - �
.DB	$3F,$3F,$00,$03,$0F,$1C,$38,$30,$38,$1C,$0F,$03;
.DB	$00,$7C,$FE,$C7,$83,$83,$83,$83,$83,$FF,$FF,$00;	$9F - �
.DB	$00,$30,$38,$1D,$0F,$07,$03,$01,$01,$3F,$3F,$00;
.DB	$00,$00,$40,$60,$60,$60,$60,$60,$60,$E0,$C0,$00;	$A0 - �
.DB	$00,$1C,$3E,$33,$33,$33,$33,$33,$33,$3F,$3F,$00;
.DB	$00,$F0,$F8,$98,$D8,$D8,$D8,$D8,$D8,$8C,$00,$00;	$A1 - �
.DB	$00,$1F,$3F,$31,$30,$30,$30,$30,$39,$1F,$0F,$00;
.DB	$00,$00,$E0,$E0,$60,$60,$60,$60,$E0,$C0,$00,$00;	$A2 - �
.DB	$00,$00,$3F,$3F,$32,$32,$32,$32,$3F,$1D,$00,$00;
.DB	$00,$00,$E0,$E0,$60,$60,$60,$60,$60,$60,$00,$00;	$A3 - �
.DB	$00,$00,$3F,$3F,$00,$00,$00,$00,$00,$00,$00,$00;
.DB	$00,$00,$80,$C0,$60,$60,$60,$60,$E0,$E0,$00,$00;	$A4 - �
.DB	$00,$F0,$FF,$3F,$30,$30,$30,$30,$3F,$FF,$F0,$00;
.DB	$00,$80,$C0,$E0,$60,$60,$60,$60,$60,$C0,$80,$00;	$A5 - �
.DB	$00,$0F,$1F,$3B,$33,$33,$33,$33,$33,$13,$01,$00;
.DB	$60,$E0,$C0,$80,$00,$E0,$E0,$00,$80,$C0,$E0,$60;	$A6 - �
.DB	$30,$38,$1F,$07,$03,$3F,$3F,$03,$07,$1F,$38,$30;
.DB	$00,$00,$80,$C0,$60,$60,$60,$60,$C0,$80,$00,$00;	$A7 - �
.DB	$00,$00,$08,$18,$30,$32,$32,$32,$1F,$0D,$00,$00;
.DB	$00,$00,$E0,$E0,$00,$00,$00,$00,$80,$E0,$E0,$00;	$A8 - �
.DB	$00,$00,$3F,$3F,$18,$0C,$06,$03,$01,$3F,$3F,$00;
.DB	$00,$00,$E0,$E0,$00,$00,$18,$0C,$80,$E0,$E0,$00;	$A9 - �
.DB	$00,$00,$3F,$3F,$18,$0C,$06,$03,$01,$3F,$3F,$00;
.DB	$00,$00,$E0,$E0,$00,$80,$C0,$E0,$60,$00,$00,$00;	$AA - �
.DB	$00,$00,$3F,$3F,$03,$07,$0F,$1C,$38,$30,$00,$00;
.DB	$00,$00,$80,$C0,$60,$60,$60,$60,$E0,$E0,$00,$00;	$AB - �
.DB	$00,$00,$3F,$3F,$00,$00,$00,$00,$3F,$3F,$00,$00;
.DB	$00,$E0,$E0,$E0,$C0,$00,$00,$C0,$E0,$E0,$E0,$00;	$AC - �
.DB	$00,$3F,$3F,$00,$01,$03,$03,$01,$00,$3F,$3F,$00;
.DB	$00,$00,$E0,$E0,$00,$00,$00,$00,$00,$E0,$E0,$00;	$AD - �
.DB	$00,$00,$3F,$3F,$06,$06,$06,$06,$06,$3F,$3F,$00;
.DB	$00,$80,$C0,$E0,$60,$60,$60,$60,$E0,$C0,$80,$00;	$AE - �
.DB	$00,$0F,$1F,$38,$30,$30,$30,$30,$38,$1F,$0F,$00;
.DB	$00,$00,$E0,$E0,$60,$60,$60,$60,$60,$E0,$E0,$00;	$AF - �
.DB	$00,$00,$3F,$3F,$00,$00,$00,$00,$00,$3F,$3F,$00;
.DB	$00,$E0,$E0,$60,$60,$60,$60,$60,$E0,$C0,$80,$00;	$B0 - �
.DB	$00,$FF,$FF,$0C,$18,$18,$18,$18,$1C,$0F,$07,$00;
.DB	$00,$80,$C0,$E0,$60,$60,$60,$60,$60,$C0,$80,$00;	$B1 - �
.DB	$00,$0F,$1F,$38,$30,$30,$30,$30,$30,$18,$08,$00;
.DB	$00,$00,$60,$60,$60,$E0,$E0,$60,$60,$60,$00,$00;	$B2 - �
.DB	$00,$00,$00,$00,$00,$3F,$3F,$00,$00,$00,$00,$00;
.DB	$00,$00,$60,$E0,$80,$00,$00,$80,$E0,$60,$00,$00;	$B3 - �
.DB	$00,$00,$00,$81,$E7,$7E,$1E,$07,$01,$00,$00,$00;
.DB	$00,$80,$C0,$C0,$C0,$E0,$E0,$C0,$C0,$C0,$80,$00;	$B4 - �
.DB	$00,$1F,$3F,$30,$30,$FF,$FF,$30,$30,$3F,$1F,$00;
.DB	$00,$60,$E0,$C0,$80,$00,$80,$C0,$E0,$60,$00,$00;	$B5 - �
.DB	$00,$30,$38,$1D,$0F,$07,$0F,$1D,$38,$30,$00,$00;
.DB	$00,$00,$E0,$E0,$00,$00,$00,$00,$E0,$E0,$00,$00;	$B6 - �
.DB	$00,$00,$3F,$3F,$30,$30,$30,$30,$3F,$FF,$F0,$00;
.DB	$00,$00,$E0,$E0,$00,$00,$00,$00,$E0,$E0,$00,$00;	$B7 - �
.DB	$00,$00,$03,$07,$06,$06,$06,$06,$3F,$3F,$00,$00;
.DB	$00,$E0,$E0,$00,$00,$E0,$E0,$00,$00,$E0,$E0,$00;	$B8 - �
.DB	$00,$3F,$3F,$30,$30,$3F,$3F,$30,$30,$3F,$3F,$00;
.DB	$00,$E0,$E0,$00,$00,$E0,$E0,$00,$00,$E0,$E0,$00;	$B9 - �
.DB	$00,$3F,$3F,$30,$30,$3F,$3F,$30,$30,$3F,$FF,$F0;
.DB	$00,$E0,$E0,$60,$E0,$E0,$00,$00,$00,$00,$00,$00;	$BA - �
.DB	$00,$00,$00,$00,$3F,$3F,$33,$33,$33,$3F,$1E,$00;
.DB	$00,$E0,$E0,$00,$00,$00,$00,$00,$00,$E0,$E0,$00;	$BB - �
.DB	$00,$3F,$3F,$33,$33,$33,$3F,$1E,$00,$3F,$3F,$00;
.DB	$00,$00,$E0,$E0,$00,$00,$00,$00,$00,$00,$00,$00;	$BC - �
.DB	$00,$00,$3F,$3F,$33,$33,$33,$33,$3F,$1E,$00,$00;
.DB	$00,$80,$C0,$60,$60,$60,$60,$60,$C0,$80,$00,$00;	$BD - �
.DB	$00,$08,$18,$30,$30,$33,$33,$3B,$1F,$0F,$00,$00;
.DB	$00,$E0,$E0,$00,$80,$C0,$E0,$60,$E0,$C0,$80,$00;	$BE - �
.DB	$00,$3F,$3F,$03,$0F,$1F,$38,$30,$38,$1F,$0F,$00;
.DB	$00,$00,$C0,$E0,$60,$60,$60,$60,$E0,$E0,$00,$00;	$BF - �
.DB	$00,$00,$31,$3B,$1E,$0E,$06,$02,$3F,$3F,$00,$00;

;

FONT6x8:
.DB $00,$FF,6,1	; ��� ���������� �������, ��� ��������� �������, ������(�������), ������(����� �� 8)

.DB $00,$00,$00,$00,$00,$00;        $00 - ������
//.DB $00,$3E,$45,$51,$45,$3E;        $01 - ����
//.DB $00,$3E,$6B,$6F,$6B,$3E;        $02 - ���� ��������
//.DB $00,$1C,$3E,$7C,$3E,$1C;        $03 - �����
//.DB $00,$18,$3C,$7E,$3C,$18;        $04 - �����

//.DB $9F,$41,$41,$9F,$00,$00;        $01 - �������� 0
//.DB $F9,$4A,$4A,$31,$00,$00;        $02 - �������� 1
//.DB $CC,$52,$52,$4C,$00,$00;        $03 - �������� 2
//.DB $7B,$20,$10,$78,$00,$00;        $04 - �������� 3

//.DB $FF,$C3,$BD,$BD,$C3,$FF;        $01 - �������� 0
//.DB $FF,$FF,$FB,$81,$FF,$FF;        $02 - �������� 1
//.DB $FF,$BB,$9D,$AD,$B3,$FF;        $03 - �������� 2
//.DB $FF,$DB,$BD,$B5,$CB,$FF;        $04 - �������� 3

.DB $00,$3C,$42,$42,$3C,$00;        $01 - ��������� 0
.DB $00,$00,$04,$7E,$00,$00;        $02 - ��������� 1
.DB $00,$44,$62,$52,$4C,$00;        $03 - ��������� 2
.DB $00,$24,$42,$4A,$34,$00;        $04 - ��������� 3

.DB $00,$30,$36,$7F,$36,$30;        $05 - ������
.DB $00,$18,$5C,$7E,$5C,$18;        $06 - ����
//.DB $00,$00,$78,$08,$08,$08;        $07 - ����������� ����� �� ������
.DB $08,$18,$30,$18,$0C,$06;        $07 - �����
.DB $00,$18,$3C,$3C,$18,$00;        $08(07) - ����������� ����� �� ������
//.DB $18,$18,$18,$18,$18,$18;        $08 - ����������� ��������� ����� �� ������
;.DB $FF,$E3,$C1,$C1,$E3,$FF;        $08 - ����������� ��������� ����� �� ������
//.DB $3E,$22,$22,$22,$3E,$00;        $09 - �������
.DB $00,$3E,$3E,$00,$3E,$3E;        $09 - �������
.DB $FF,$C3,$DB,$DB,$C3,$FF;        $0A - �� ����������� ��������� ���� �� ������
.DB $00,$2C,$12,$2A,$24,$1A;        $0B - �������
.DB $00,$06,$09,$09,$06,$00;        $0C - ������
.DB $00,$60,$70,$3F,$02,$04;        $0D - ����
.DB $00,$5C,$62,$02,$62,$5C;        $0E - �����
.DB $00,$2A,$1C,$36,$1C,$2A;        $0F - ������
.DB $00,$00,$7F,$3E,$1C,$08;        $10 - ������� ������� ������
.DB $00,$08,$1C,$3E,$7F,$00;        $11 - ������� ������� �����
.DB $00,$14,$36,$7F,$36,$14;        $12 - ����� ������� �����/����
.DB $00,$44,$44,$5F,$44,$44;        $13 - ����/�����
.DB $00,$06,$09,$7F,$01,$7F;        $14 - ��
.DB $00,$22,$4D,$55,$59,$22;        $15 - ��������
.DB $00,$60,$60,$60,$60,$00;        $16 - ������ �������������
;.DB $00,$19,$15,$13,$00,$00;        $17 - ���� "� ��������"
.DB $00,$48,$64,$54,$48,$00;        $17 - ���� "� ��������"
.DB $00,$04,$06,$7F,$06,$04;        $18 - ����� ������� �����
.DB $00,$10,$30,$7F,$30,$10;        $19 - ����� ������� ����
.DB $00,$08,$08,$3E,$1C,$08;        $1A - ����� ������� ������
.DB $00,$08,$1C,$3E,$08,$08;        $1B - ����� ������� �����
.DB $00,$78,$40,$40,$40,$40;        $1C - ���� "������"
.DB $00,$08,$1C,$08,$1C,$08;        $1D - ����� ������� �����/������
.DB $00,$30,$3C,$3F,$3C,$30;        $1E - ������� ������� �����
.DB $00,$03,$0F,$3F,$0F,$03;        $1F - ������� ������� ����
.DB $00,$00,$00,$00,$00,$00;        $20 - ������
.DB $00,$00,$06,$5F,$06,$00;        $21 - !
.DB $00,$07,$03,$00,$07,$03;        $22 - "
.DB $00,$24,$7E,$24,$7E,$24;        $23 - #
.DB $00,$24,$2B,$6A,$12,$00;        $24 - $
.DB $00,$63,$13,$08,$64,$63;        $25 - %
.DB $00,$36,$49,$56,$20,$50;        $26 - &
.DB $00,$00,$07,$00,$00,$00;        $27 - '
.DB $00,$00,$3E,$41,$00,$00;        $28 - (
.DB $00,$00,$41,$3E,$00,$00;        $29 - )
.DB $00,$08,$3E,$1C,$3E,$08;        $2A - *
.DB $00,$08,$08,$3E,$08,$08;        $2B - +
.DB $00,$00,$E0,$60,$00,$00;        $2C - ,
.DB $00,$08,$08,$08,$08,$08;        $2D - -
.DB $00,$60,$60,$00,$00,$00;        $2E - .
.DB $00,$20,$10,$08,$04,$02;        $2F - /

//.DB $00,$3E,$51,$49,$45,$3E;        $30 - 0
.DB $00,$3E,$41,$49,$41,$3E;        $30 - 0

.DB $00,$00,$42,$7F,$40,$00;        $31 - 1
.DB $00,$62,$51,$49,$49,$46;        $32 - 2
;.DB $00,$21,$49,$4D,$4B,$31;        $33 - 3
.DB $00,$22,$41,$49,$49,$36;        $33 - 3
.DB $00,$18,$14,$12,$7F,$10;        $34 - 4
.DB $00,$2F,$49,$49,$49,$31;        $35 - 5
.DB $00,$3C,$4A,$49,$49,$30;        $36 - 6
.DB $00,$01,$71,$09,$05,$03;        $37 - 7
.DB $00,$36,$49,$49,$49,$36;        $38 - 8
.DB $00,$06,$49,$49,$29,$1E;        $39 - 9
.DB $00,$00,$6C,$6C,$00,$00;        $3A - :
.DB $00,$00,$EC,$6C,$00,$00;        $3B - ;
.DB $00,$08,$14,$22,$41,$00;        $3C - <
.DB $00,$24,$24,$24,$24,$24;        $3D - =
.DB $00,$00,$41,$22,$14,$08;        $3E - >
.DB $00,$02,$01,$59,$09,$06;        $3F - ?
.DB $00,$3E,$41,$5D,$55,$1E;        $40 - @
.DB $00,$7E,$11,$11,$11,$7E;        $41 - A
.DB $00,$7F,$49,$49,$49,$36;        $42 - B
.DB $00,$3E,$41,$41,$41,$22;        $43 - C
.DB $00,$7F,$41,$41,$41,$3E;        $44 - D
.DB $00,$7F,$49,$49,$49,$41;        $45 - E
.DB $00,$7F,$09,$09,$09,$01;        $46 - F
.DB $00,$3E,$41,$49,$49,$7A;        $47 - G
.DB $00,$7F,$08,$08,$08,$7F;        $48 - H
.DB $00,$00,$41,$7F,$41,$00;        $49 - I
.DB $00,$30,$40,$40,$40,$3F;        $4A - J
.DB $00,$7F,$08,$14,$22,$41;        $4B - K
.DB $00,$7F,$40,$40,$40,$40;        $4C - L
.DB $00,$7F,$02,$04,$02,$7F;        $4D - M
.DB $00,$7F,$02,$04,$08,$7F;        $4E - N
.DB $00,$3E,$41,$41,$41,$3E;        $4F - O
.DB $00,$7F,$09,$09,$09,$06;        $50 - P
.DB $00,$3E,$41,$51,$21,$5E;        $51 - Q
.DB $00,$7F,$09,$09,$19,$66;        $52 - R
.DB $00,$26,$49,$49,$49,$32;        $53 - S
.DB $00,$01,$01,$7F,$01,$01;        $54 - T
.DB $00,$3F,$40,$40,$40,$3F;        $55 - U
.DB $00,$07,$18,$60,$18,$07;        $56 - V
.DB $00,$3F,$40,$3C,$40,$3F;        $57 - W
.DB $00,$63,$14,$08,$14,$63;        $58 - X
.DB $00,$07,$08,$70,$08,$07;        $59 - Y
.DB $00,$71,$49,$45,$43,$00;        $5A - Z
.DB $00,$00,$7F,$41,$41,$00;        $5B - [
.DB $00,$02,$04,$08,$10,$20;        $5C - "\"
.DB $00,$00,$41,$41,$7F,$00;        $5D - ]
.DB $00,$04,$02,$01,$02,$04;        $5E - ^
.DB $80,$80,$80,$80,$80,$80;        $5F - _
.DB $00,$00,$07,$06,$00,$00;        $60 - ������� ����� ������
.DB $00,$20,$54,$54,$54,$78;        $61 - a
.DB $00,$7F,$44,$44,$44,$38;        $62 - b
.DB $00,$38,$44,$44,$44,$28;        $63 - c
.DB $00,$38,$44,$44,$44,$7F;        $64 - d
.DB $00,$38,$54,$54,$54,$08;        $65 - e
.DB $00,$08,$7E,$09,$09,$00;        $66 - f
.DB $00,$18,$A4,$A4,$A4,$7C;        $67 - g
.DB $00,$7F,$04,$04,$78,$00;        $68 - h
.DB $00,$00,$00,$7D,$40,$00;        $69 - i
.DB $00,$40,$80,$84,$7D,$00;        $6A - j
.DB $00,$7F,$10,$28,$44,$00;        $6B - k
.DB $00,$00,$00,$7F,$40,$00;        $6C - l
.DB $00,$7C,$04,$18,$04,$78;        $6D - m
.DB $00,$7C,$04,$04,$78,$00;        $6E - n
.DB $00,$38,$44,$44,$44,$38;        $6F - o
.DB $00,$FC,$44,$44,$44,$38;        $70 - p
.DB $00,$38,$44,$44,$44,$FC;        $71 - q
.DB $00,$44,$78,$44,$04,$08;        $72 - r
.DB $00,$08,$54,$54,$54,$20;        $73 - s
.DB $00,$04,$3E,$44,$44,$00;        $74 - t
.DB $00,$3C,$40,$20,$7C,$00;        $75 - u
.DB $00,$1C,$20,$40,$20,$1C;        $76 - v
.DB $00,$3C,$60,$30,$60,$3C;        $77 - w
.DB $00,$6C,$10,$10,$6C,$00;        $78 - x
.DB $00,$9C,$A0,$60,$3C,$00;        $79 - y
.DB $00,$64,$54,$54,$4C,$00;        $7A - z
.DB $00,$08,$3E,$41,$41,$00;        $7B - {
.DB $00,$00,$77,$00,$00,$00;        $7C - |
.DB $00,$00,$41,$41,$3E,$08;        $7D - }
.DB $00,$02,$01,$02,$01,$00;        $7E - ~
.DB $00,$3C,$26,$23,$26,$3C;        $7F - �����

.DB $00,$7E,$11,$11,$11,$7E;        $80 - �
.DB $00,$7F,$49,$49,$49,$31;        $81 - �
.DB $00,$7F,$49,$49,$49,$36;        $82 - �
.DB $00,$7F,$01,$01,$01,$01;        $83 - �
.DB $C0,$7E,$41,$41,$7F,$C0;        $84 - �
.DB $00,$7F,$49,$49,$49,$41;        $85 - �
.DB $00,$77,$08,$7F,$08,$77;        $86 - �
.DB $22,$49,$49,$49,$36,$00;        $87 - �
.DB $00,$7F,$20,$10,$08,$7F;        $88 - �
.DB $00,$7E,$21,$11,$09,$7E;        $89 - �
.DB $00,$7F,$08,$14,$22,$41;        $8A - �
.DB $00,$40,$7E,$01,$01,$7F;        $8B - �
.DB $00,$7F,$02,$04,$02,$7F;        $8C - �
.DB $00,$7F,$08,$08,$08,$7F;        $8D - �
.DB $00,$3E,$41,$41,$41,$3E;        $8E - �
.DB $00,$7F,$01,$01,$01,$7F;        $8F - �
.DB $00,$7F,$09,$09,$09,$06;        $90 - �
.DB $00,$3E,$41,$41,$41,$22;        $91 - �
.DB $00,$01,$01,$7F,$01,$01;        $92 - �
.DB $00,$27,$48,$48,$48,$3F;        $93 - �
.DB $00,$0E,$11,$7F,$11,$0E;        $94 - �
.DB $00,$63,$14,$08,$14,$63;        $95 - �
.DB $00,$7F,$40,$40,$7F,$C0;        $96 - �
.DB $00,$07,$08,$08,$08,$7F;        $97 - �
.DB $00,$7F,$40,$7F,$40,$7F;        $98 - �
.DB $00,$7F,$40,$7F,$40,$FF;        $99 - �
.DB $03,$01,$7F,$48,$48,$30;        $9A - �
.DB $00,$7F,$48,$48,$30,$7F;        $9B - �
.DB $00,$7F,$48,$48,$48,$30;        $9C - �
.DB $00,$22,$41,$49,$49,$3E;        $9D - �
.DB $00,$7F,$08,$3E,$41,$3E;        $9E - �
.DB $00,$66,$19,$09,$09,$7F;        $9F - �
.DB $00,$20,$54,$54,$54,$78;        $A0 - �
.DB $00,$3C,$4A,$4A,$4A,$31;        $A1 - �
.DB $00,$7C,$54,$54,$54,$28;        $A2 - �
.DB $00,$7C,$04,$04,$04,$04;        $A3 - �
//.DB $00,$7C,$04,$04,$0C,$00;        $A3 - �
.DB $C0,$78,$44,$44,$7C,$C0;        $A4 - �
.DB $00,$38,$54,$54,$54,$08;        $A5 - �
.DB $00,$6C,$10,$7C,$10,$6C;        $A6 - �
.DB $00,$28,$44,$54,$54,$28;        $A7 - �
.DB $00,$7C,$20,$10,$08,$7C;        $A8 - �
.DB $00,$7C,$20,$12,$0A,$7C;        $A9 - �
.DB $00,$7C,$10,$28,$44,$00;        $AA - �
.DB $40,$38,$04,$04,$7C,$00;        $AB - �
.DB $00,$7C,$08,$10,$08,$7C;        $AC - �
.DB $00,$7C,$10,$10,$10,$7C;        $AD - �
.DB $00,$38,$44,$44,$44,$38;        $AE - �
.DB $00,$7C,$04,$04,$04,$7C;        $AF - �
.DB $00,$FC,$44,$44,$44,$38;        $B0 - �
.DB $00,$38,$44,$44,$44,$28;        $B1 - �
.DB $00,$04,$04,$7C,$04,$04;        $B2 - �
.DB $00,$9C,$A0,$60,$3C,$00;        $B3 - �
.DB $00,$18,$24,$7C,$24,$18;        $B4 - �
.DB $00,$6C,$10,$10,$6C,$00;        $B5 - �
.DB $00,$7C,$40,$40,$7C,$C0;        $B6 - �
.DB $00,$0C,$10,$10,$10,$7C;        $B7 - �
.DB $00,$7C,$40,$7C,$40,$7C;        $B8 - �
.DB $00,$7C,$40,$7C,$40,$FC;        $B9 - �
.DB $0C,$04,$7C,$50,$50,$20;        $BA - �
.DB $00,$7C,$50,$50,$20,$7C;        $BB - �
.DB $00,$7C,$50,$50,$50,$20;        $BC - �
.DB $00,$28,$44,$54,$54,$38;        $BD - �
.DB $00,$7C,$10,$38,$44,$38;        $BE - �
.DB $00,$48,$34,$14,$14,$7C;	    $BF - �

