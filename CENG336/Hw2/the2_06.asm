; Mert BEKAR, Kübra ALTIN


list P=18F8722

#include <p18f8722.inc>
config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = ON


t1 udata 0x20
t1

t2 udata 0x21
t2

t3 udata 0x22
t3

t4 udata 0x23
t4

p1 udata 0x24
p1

p2 udata 0x25
p2

p3 udata 0x26
p3

p4 udata 0x27
p4

c1 udata 0x28
c1

c2 udata 0x29
c2

c3 udata 0x30
c3

c4 udata 0x31
c4

dir udata 0x50
dir

bit0 udata 0x33
bit0

bit1 udata 0x34
bit1

state udata 0x35
state

counter_time udata 0x46
counter_time

var0      udata 0X36
var0
var1      udata 0X37
var1
var2      udata 0X38
var2
var3      udata 0X39
var3
L1          udata 0X40
L1
L2          udata 0X41
L2

counter    udata 0X42
counter

w_temp  udata 0x43
w_temp

status_temp udata 0x44
status_temp

pclath_temp udata 0x45
pclath_temp

state2 udata 0x47
state2

flagg    udata 0X48
flagg

tmr udata 0x49
tmr

score1 udata 0x51
score1

score2 udata 0x52
score2

gmain udata 0x55
gmain

tmrx udata 0x56
tmrx

L3 udata 0x53
L3

org 0x00
    goto init
org 0x08
    goto isr

init:
    clrf PORTA
    clrf PORTB
    clrf PORTC
    clrf PORTD
    clrf PORTE
    clrf PORTF
    clrf PORTG
    clrf PORTH
    clrf PORTJ

    clrf TRISA
    clrf TRISB
    clrf TRISC
    clrf TRISD
    clrf TRISE
    clrf TRISF
    clrf TRISG
    clrf TRISH
    clrf TRISJ
    movlw d'0'
    movwf score1
    movwf score2
    movwf tmr
    movlw 0Fh
    movwf ADCON1


    movlw b'00000111'
    movwf LATJ
    bsf LATH, 1
    call delay

    movlw b'00111111'
    movwf LATJ
    bsf LATH, 3
    call delay

    goto main

main:
    clrf PORTA
    clrf PORTB
    clrf PORTC
    clrf PORTD
    clrf PORTE
    clrf PORTF
    clrf PORTG

    clrf TRISA
    clrf TRISB
    clrf TRISC
    clrf TRISD
    clrf TRISE
    clrf TRISF
    clrf TRISG

    movlw   b'010000010'
    movwf   T0CON

    clrf    INTCON
    movlw   b'11100000'
    movwf   INTCON


    movlw b'00001111'
    movwf TRISG
    movlw b'00011100'
    movwf LATF
    movwf LATA
    movlw b'00001000'
    movwf LATD
    movlw d'0'
    movwf t1
    movwf t2
    movwf t3
    movwf t4
    movwf p1
    movwf p2
    movwf p3
    movwf p4
    movwf c1
    movwf c2
    movwf c3
    movwf c4
    movwf bit0
    movwf dir
    movwf gmain
    movwf flagg
    movlw d'1'
    movwf t3
    movwf c3
    movwf bit1
    movlw d'21'
    movwf state

    bsf T0CON, 7
    bsf T1CON, 7

    goto loop

loop:
    call lscore0
    incf tmrx
    call lscore0
    movlw d'1'
    btfsc PORTG, 3
    movwf p1
    call lscore0
    btfsc p1, 0
    call lmove
    call lscore0
    movlw d'0'
    movwf p1
    call lscore0
    movlw d'1'
    btfsc PORTG, 2
    movwf p2
    call lscore0
    btfsc p2, 0
    call xlmove
    call lscore0
    movlw d'0'
    movwf p2
    call lscore0
    movlw d'1'
    btfsc PORTG, 1
    movwf p3
    call lscore0
    btfsc p3, 0
    call rmove
    call lscore0
    movlw d'0'
    movwf p3
    call lscore0
    movlw d'1'
    btfsc PORTG, 0
    movwf p4
    call lscore0
    btfsc p4, 0
    call xrmove
    call lscore0
    movlw d'0'
    movwf p4
    call lscore0
    movlw d'5'
    cpfseq score1
    goto loop1
    goto loop3

loop1:
    movlw d'5'
    cpfseq score2
    goto loop2
    goto loop3

loop2:
    btfsc gmain, 0
    goto main
    goto loop

loop3:
    call lscore0
    movlw b'00000000'
    movwf INTCON
    goto loop3

delay:
    movlw 50
    movwf L2

LOOP2:
    movlw 50
    movwf L1

LOOP1:
    decfsz L1,F
    goto LOOP1
    decfsz L2,F
    goto LOOP2
    return

lmove:
    call lscore0
    btfsc t2, 0
    call a0a1a2
    call lscore0
    btfsc t3, 0
    call a1a2a3
    call lscore0
    btfsc t4, 0
    call a2a3a4
    call lscore0
    goto pressed
    return

pressed:
    call lscore0
    btfss PORTG, 3
    return
    goto pressed

a0a1a2:
    clrf LATA
    movlw b'00000111'
    movwf LATA
    movlw d'1'
    movwf t1
    movlw d'0'
    movwf t2
    return

a1a2a3:
    clrf LATA
    movlw b'00001110'
    movwf LATA
    movlw d'1'
    movwf t2
    movlw d'0'
    movwf t3
    return

xa1a2a3:
    clrf LATA
    movlw b'00001110'
    movwf LATA
    movlw d'1'
    movwf t2
    movlw d'0'
    movwf t1
    return

a2a3a4:
    clrf LATA
    movlw b'00011100'
    movwf LATA
    movlw d'1'
    movwf t3
    movlw d'0'
    movwf t4
    return

xa2a3a4:
    clrf LATA
    movlw b'00011100'
    movwf LATA
    movlw d'1'
    movwf t3
    movlw d'0'
    movwf t2
    return

xlmove:
    call lscore0
    btfsc t3, 0
    call xa3a4a5
    call lscore0
    btfsc t2, 0
    call xa2a3a4
    call lscore0
    btfsc t1, 0
    call xa1a2a3
    call lscore0
    call xpressed
    return

xpressed:
    call lscore0
    btfss PORTG, 2
    return
    goto xpressed

xa3a4a5:
    clrf LATA
    movlw b'00111000'
    movwf LATA
    movlw d'1'
    movwf t4
    movlw d'0'
    movwf t3
    return

rmove:
    call lscore0
    btfsc c2, 0
    call f0f1f2
    call lscore0
    btfsc c3, 0
    call f1f2f3
    call lscore0
    btfsc c4, 0
    call f2f3f4
    call lscore0
    goto rpressed
    return

rpressed:
    call lscore0
    btfss PORTG, 1
    return
    goto rpressed

f0f1f2:
    clrf LATF
    movlw b'00000111'
    movwf LATF
    movlw d'1'
    movwf c1
    movlw d'0'
    movwf c2
    return

f1f2f3:
    clrf LATF
    movlw b'00001110'
    movwf LATF
    movlw d'1'
    movwf c2
    movlw d'0'
    movwf c3
    return

xf1f2f3:
    clrf LATF
    movlw b'00001110'
    movwf LATF
    movlw d'1'
    movwf c2
    movlw d'0'
    movwf c1
    return

f2f3f4:
    clrf LATF
    movlw b'00011100'
    movwf LATF
    movlw d'1'
    movwf c3
    movlw d'0'
    movwf c4
    return

xf2f3f4:
    clrf LATF
    movlw b'00011100'
    movwf LATF
    movlw d'1'
    movwf c3
    movlw d'0'
    movwf c2
    return

xrmove:
    btfsc c3, 0
    call xf3f4f5
    call lscore0
    btfsc c2, 0
    call xf2f3f4
    call lscore0
    btfsc c1, 0
    call xf1f2f3
    call lscore0
    call xrpressed
    return

xrpressed:
    call lscore0
    btfss PORTG, 0
    return
    goto xrpressed

xf3f4f5:
    clrf LATF
    movlw b'00111000'
    movwf LATF
    movlw d'1'
    movwf c4
    movlw d'0'
    movwf c3
    return

lscore0:
    movlw d'0'
    cpfseq score1
    goto lscore1
    clrf LATH
    movlw b'00111111'
    movwf LATJ
    bsf LATH, 1
    call delay
    goto rscore0

lscore1:
    movlw d'1'
    cpfseq score1
    goto lscore2
    clrf LATH
    movlw b'00000110'
    movwf LATJ
    bsf LATH, 1
    call delay
    goto rscore0

lscore2:
    movlw d'2'
    cpfseq score1
    goto lscore3
    clrf LATH
    movlw b'01011011'
    movwf LATJ
    bsf LATH, 1
    call delay
    goto rscore0

lscore3:
    movlw d'3'
    cpfseq score1
    goto lscore4
    clrf LATH
    movlw b'01001111'
    movwf LATJ
    bsf LATH, 1
    call delay
    goto rscore0

lscore4:
    movlw d'4'
    cpfseq score1
    goto lscore5
    clrf LATH
    movlw b'01100110'
    movwf LATJ
    bsf LATH, 1
    call delay
    goto rscore0

lscore5:
    clrf LATH
    movlw b'01101101'
    movwf LATJ
    bsf LATH, 1
    call delay
    goto rscore0

rscore0:
    movlw d'0'
    cpfseq score2
    goto rscore1
    clrf LATH
    movlw b'00111111'
    movwf LATJ
    bsf LATH, 3
    call delay
    return

rscore1:
    movlw d'1'
    cpfseq score2
    goto rscore2
    clrf LATH
    movlw b'00000110'
    movwf LATJ
    bsf LATH, 3
    call delay
    return

rscore2:
    movlw d'2'
    cpfseq score2
    goto rscore3
    clrf LATH
    movlw b'01011011'
    movwf LATJ
    bsf LATH, 3
    call delay
    return

rscore3:
    movlw d'3'
    cpfseq score2
    goto rscore4
    clrf LATH
    movlw b'01001111'
    movwf LATJ
    bsf LATH, 3
    call delay
    return

rscore4:
    movlw d'4'
    cpfseq score2
    goto rscore5
    clrf LATH
    movlw b'01100110'
    movwf LATJ
    bsf LATH, 3
    call delay
    return

rscore5:
    clrf LATH
    movlw b'01101101'
    movwf LATJ
    bsf LATH, 3
    call delay
    return

led0:
    movlw d'0'
    cpfseq state
    goto led1
    btfss PORTA, 0
    incf score2
    btfss PORTA, 0
    bsf gmain, 0
    btfss PORTA, 0
    bsf PORTA, 0
    movlw d'1'
    movwf dir

led1:
    movlw d'1'
    cpfseq state
    goto led2
    btfss PORTA, 1
    incf score2
    btfss PORTA, 1
    bsf gmain, 0
    btfss PORTA, 1
    bsf PORTA, 1
    movlw d'1'
    movwf dir

led2:
    movlw d'2'
    cpfseq state
    goto led3
    btfss PORTA, 2
    incf score2
    btfss PORTA, 2
    bsf gmain, 0
    btfss PORTA, 2
    bsf PORTA, 2
    movlw d'1'
    movwf dir

led3:
    movlw d'3'
    cpfseq state
    goto led4
    btfss PORTA, 3
    incf score2
    btfss PORTA, 3
    bsf gmain, 0
    btfss PORTA, 3
    bsf PORTA, 3
    movlw d'1'
    movwf dir

led4:
    movlw d'4'
    cpfseq state
    goto led5
    btfss PORTA, 4
    incf score2
    btfss PORTA, 4
    bsf gmain, 0
    btfss PORTA, 4
    bsf PORTA, 4
    movlw d'1'
    movwf dir

led5:
    movlw d'5'
    cpfseq state
    goto led6
    btfss PORTA, 5
    incf score2
    btfss PORTA, 5
    bsf gmain, 0
    btfss PORTA, 5
    bsf PORTA, 5
    movlw d'1'
    movwf dir

led6:
    movlw d'6'
    cpfseq state
    goto led7
    bsf LATB, 0

led7:
    movlw d'7'
    cpfseq state
    goto led8
    bsf LATB, 1

led8:
    movlw d'8'
    cpfseq state
    goto led9
    bsf LATB, 2

led9:
    movlw d'9'
    cpfseq state
    goto led10
    bsf LATB, 3

led10:
    movlw d'10'
    cpfseq state
    goto led11
    bsf LATB, 4

led11:
    movlw d'11'
    cpfseq state
    goto led12
    bsf LATB, 5

led12:
    movlw d'12'
    cpfseq state
    goto led13
    bsf LATC, 0

led13:
    movlw d'13'
    cpfseq state
    goto led14
    bsf LATC, 1

led14:
    movlw d'14'
    cpfseq state
    goto led15
    bsf LATC, 2

led15:
    movlw d'15'
    cpfseq state
    goto led16
    bsf LATC, 3

led16:
    movlw d'16'
    cpfseq state
    goto led17
    bsf LATC, 4

led17:
    movlw d'17'
    cpfseq state
    goto led18
    bsf LATC, 5

led18:
    movlw d'18'
    cpfseq state
    goto led19
    bsf LATD, 0

led19:
    movlw d'19'
    cpfseq state
    goto led20
    bsf LATD, 1

led20:
    movlw d'20'
    cpfseq state
    goto led21
    bsf LATD, 2

led21:
    movlw d'21'
    cpfseq state
    goto led22
    bsf LATD, 3

led22:
    movlw d'22'
    cpfseq state
    goto led23
    bsf LATD, 4

led23:
    movlw d'23'
    cpfseq state
    goto led24
    bsf LATD, 5

led24:
    movlw d'24'
    cpfseq state
    goto led25
    bsf LATE, 0

led25:
    movlw d'25'
    cpfseq state
    goto led26
    bsf LATE, 1

led26:
    movlw d'26'
    cpfseq state
    goto led27
    bsf LATE, 2

led27:
    movlw d'27'
    cpfseq state
    goto led28
    bsf LATE, 3

led28:
    movlw d'28'
    cpfseq state
    goto led29
    bsf LATE, 4

led29:
    movlw d'29'
    cpfseq state
    goto led30
    bsf LATE, 5

led30:
    movlw d'30'
    cpfseq state
    goto led31
    btfss PORTF, 0
    incf score1
    btfss PORTF, 0
    bsf gmain, 0
    btfss PORTF, 0
    bsf PORTF, 0
    movlw d'0'
    movwf dir

led31:
    movlw d'31'
    cpfseq state
    goto led32
    btfss PORTF, 1
    incf score1
    btfss PORTF, 1
    bsf gmain, 0
    btfss PORTF, 1
    bsf PORTF, 1
    movlw d'0'
    movwf dir

led32:
    movlw d'32'
    cpfseq state
    goto led33
    btfss PORTF, 2
    incf score1
    btfss PORTF, 2
    bsf gmain, 0
    btfss PORTF, 2
    bsf PORTF, 2
    movlw d'0'
    movwf dir

led33:
    movlw d'33'
    cpfseq state
    goto led34
    btfss PORTF, 3
    incf score1
    btfss PORTF, 3
    bsf gmain, 0
    btfss PORTF, 3
    bsf PORTF, 3
    movlw d'0'
    movwf dir

led34:
    movlw d'34'
    cpfseq state
    goto led35
    btfss PORTF, 4
    incf score1
    btfss PORTF, 4
    bsf gmain, 0
    btfss PORTF, 4
    bsf PORTF, 4
    movlw d'0'
    movwf dir

led35:
    movlw d'35'
    cpfseq state
    return
    btfss PORTF, 5
    incf score1
    btfss PORTF, 5
    bsf gmain, 0
    btfss PORTF, 5
    bsf PORTF, 5
    movlw d'0'
    movwf dir
    return

increment:
    movf flagg,w
    addwf state, f
    return

decrement:
    movf flagg,w
    subwf state, f
    return


ball01:
    movlw d'0'
    cpfseq state
    goto ball011
    movlw d'5'
    movwf flagg
    return

ball011:
    movlw d'6'
    cpfseq state
    goto ball012
    movlw d'5'
    movwf flagg
    return

ball012:
    movlw d'12'
    cpfseq state
    goto ball013
    movlw d'5'
    movwf flagg
    return

ball013:
    movlw d'18'
    cpfseq state
    goto ball014
    movlw d'5'
    movwf flagg
    return

ball014:
    movlw d'24'
    cpfseq state
    goto ball015
    movlw d'5'
    movwf flagg
    return

ball015:
    movlw d'30'
    cpfseq state
    goto ball016
    movlw d'5'
    movwf flagg
    return

ball016:
    movlw d'7'
    movwf flagg
    return

rball01:
    movlw d'0'
    cpfseq state
    goto rball011
    movlw d'7'
    movwf flagg
    return

rball011:
    movlw d'6'
    cpfseq state
    goto rball012
    movlw d'7'
    movwf flagg
    return

rball012:
    movlw d'12'
    cpfseq state
    goto rball013
    movlw d'7'
    movwf flagg
    return

rball013:
    movlw d'18'
    cpfseq state
    goto rball014
    movlw d'7'
    movwf flagg
    return

rball014:
    movlw d'24'
    cpfseq state
    goto rball015
    movlw d'7'
    movwf flagg
    return

rball015:
    movlw d'30'
    cpfseq state
    goto rball016
    movlw d'7'
    movwf flagg
    return

rball016:
    movlw d'5'
    movwf flagg
    return

ball10:
    movlw d'5'
    cpfseq state
    goto ball101
    movlw d'7'
    movwf flagg
    return

ball101:
    movlw d'11'
    cpfseq state
    goto ball102
    movlw d'7'
    movwf flagg
    return

ball102:
    movlw d'17'
    cpfseq state
    goto ball103
    movlw d'7'
    movwf flagg
    return

ball103:
    movlw d'23'
    cpfseq state
    goto ball104
    movlw d'7'
    movwf flagg
    return

ball104:
    movlw d'29'
    cpfseq state
    goto ball105
    movlw d'7'
    movwf flagg
    return

ball105:
    movlw d'35'
    cpfseq state
    goto ball106
    movlw d'7'
    movwf flagg
    return

ball106:
    movlw d'5'
    movwf flagg
    return

rball10:
    movlw d'5'
    cpfseq state
    goto rball101
    movlw d'5'
    movwf flagg
    return

rball101:
    movlw d'11'
    cpfseq state
    goto rball102
    movlw d'5'
    movwf flagg
    return

rball102:
    movlw d'17'
    cpfseq state
    goto rball103
    movlw d'5'
    movwf flagg
    return

rball103:
    movlw d'23'
    cpfseq state
    goto rball104
    movlw d'5'
    movwf flagg
    return

rball104:
    movlw d'29'
    cpfseq state
    goto rball105
    movlw d'5'
    movwf flagg
    return

rball105:
    movlw d'35'
    cpfseq state
    goto rball106
    movlw d'5'
    movwf flagg
    return

rball106:
    movlw d'7'
    movwf flagg
    return

ballxx:
    btfsc tmrx, 0
    call ballx1
    btfss tmrx, 0
    call ballx0
    return

ballx0:
    btfsc tmrx, 1
    call ball10
    btfss tmrx, 1
    call ball00
    return

rballxx:
    btfsc tmrx, 0
    call rballx1
    btfss tmrx, 0
    call rballx0
    return

rballx0:
    btfsc tmrx, 1
    call rball10
    btfss tmrx, 1
    call rball00
    return

ball00:
    movlw d'6'
    movwf flagg
    return

rball00:
    clrf LATE
    movlw d'6'
    movwf flagg
    return

ballx1:
    btfsc tmrx, 1
    call ball11
    btfss tmrx, 1
    call ball01
    return

rballx1:
    btfsc tmrx, 1
    call rball11
    btfss tmrx, 1
    call rball01
    return

ball11:
    movlw d'6'
    movwf flagg
    return

rball11:
    movlw d'6'
    movwf flagg
    return


isr:
   call   save_registers
   goto timer_interrupt

timer_interrupt:
    incf	counter, f
    movf	counter, w
    sublw	d'6'
    btfss	STATUS, Z
    goto	timer_interrupt_exit
    clrf	counter
    clrf LATB
    clrf LATC
    clrf LATD
    clrf LATE
    btfss dir,0
    call ballxx
    btfss dir, 0
    call decrement
    btfsc dir,0
    call rballxx
    btfsc dir, 0
    call increment
    ;btfss STATUS, 4
    call led0
    ;btfsc STATUS, 4

    rlncf tmrx, 1

timer_interrupt_exit:
    bcf	    INTCON, 2
    movlw	d'255'
    movwf	TMR0
    ;movlw   b'00000000'
   ; btfsc   gmain,0
   ; movwf   INTCON
    call	restore_registers

    retfie

save_registers:
    movwf 	w_temp
    swapf 	STATUS, w
    clrf 	STATUS
    movwf 	status_temp
    movf 	PCLATH, w
    movwf 	pclath_temp
    clrf 	PCLATH
    return

restore_registers:
    movf 	pclath_temp, w
    movwf 	PCLATH
    swapf 	status_temp, w
    movwf 	STATUS
    swapf 	w_temp, f
    swapf 	w_temp, w
    return

end

