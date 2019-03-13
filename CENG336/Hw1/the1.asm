LIST P=18f8722
#INCLUDE <p18f8722.inc>
CONFIG OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

d1 udata 0x20
d1

d2 udata 0x21
d2

d3 udata 0x22
d3

t4 udata 0x23
t4

t5 udata 0x24
t5

org 0

goto ZERO

fDELAY
    movlw 100
    movwf d3

fDELAY2
	movlw 100
	movwf d2

fDELAY3
    movlw 64
    movwf d1

fDELAY4
    decfsz d1,F
    goto fDELAY4
	decfsz d2,F
	goto fDELAY3
	decfsz d3,F
	goto fDELAY2
    return


WAIT2
    btfss PORTA, 4
    return
    movlw d'0'
    movwf t5
    call PRESS2
    return

PRESS2
    btfsc PORTA, 4
    return
    movlw d'0'
    movwf t4
    return

WAIT4
    btfss PORTA, 4
    return
    call PRESS4
    return

PRESS4
    btfsc PORTA, 4
    goto PRESS4
    movlw d'0'
    movwf t4
    return

DELAY
    movlw 75
    movwf d3

DELAY2
	movlw 66
	movwf d2

DELAY3
    movlw 30
    movwf d1

DELAY4
    call WAIT2
    btfss t5, 0
    call PRESS2
    btfss t4, 0
    return
    decfsz d1,F
    goto DELAY4
	decfsz d2,F
	goto DELAY3
	decfsz d3,F
	goto DELAY2
    return

WAIT
    call WAIT4
    btfss t4, 0
    return
    btfss PORTB, 5
    goto WAIT
    goto PRESS
    return
PRESS
    btfsc PORTB, 5
    goto PRESS
    return

rDELAY
    movlw 50
    movwf d3

rDELAY2
	movlw 50
	movwf d2

rDELAY3
    movlw 30
    movwf d1

rDELAY4
    call WAIT2
    btfss t5, 0
    call PRESS2
    btfss t4, 0
    return
    decfsz d1,F
    goto rDELAY4
	decfsz d2,F
	goto rDELAY3
	decfsz d3,F
	goto rDELAY2
    return


ZERO
    clrf TRISA
    clrf TRISB
    clrf TRISC
    clrf TRISD
    clrf LATA
    clrf LATB
    clrf LATC
    clrf LATD
    movlw b'00001111'
    movwf LATA
    movwf LATB
    movwf LATC
    movwf LATD
    call fDELAY
    goto FIRST
    return

FIRST
    clrf TRISA
    clrf TRISB
    clrf TRISC
    clrf TRISD
    clrf LATA
    clrf LATB
    clrf LATC
    clrf LATD
    movlw b'00100000'
    movwf TRISB
    movlw b'00010000'
    movwf TRISA
    goto a1b1

a1b1
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00000001'
    movwf LATA
    movwf LATB
    call DELAY
    clrf LATA
    clrf LATB
    btfss t4, 0
    goto xa1b1
    goto b1c1

b1c1
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00000001'
    movwf LATB
    movwf LATC
    call DELAY
    clrf LATB
    clrf LATC
    btfss t4, 0
    goto xa1b1
    goto c1d1

c1d1
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00000001'
    movwf LATC
    movwf LATD
    call WAIT
    clrf LATC
    clrf LATD
    btfss t4, 0
    goto xb1c1
    goto d1d2

d1d2
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00000011'
    movwf LATD
    call DELAY
    clrf LATD
    btfss t4, 0
    goto xd1d2
    goto d2d3

d2d3
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00000110'
    movwf LATD
    call DELAY
    clrf LATD
    btfss t4, 0
    goto xd1d2
    goto d3d4

d3d4
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00001100'
    movwf LATD
    call WAIT
    clrf LATD
    btfss t4, 0
    goto xd2d3
    goto c4d4

c4d4
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00001000'
    movwf LATD
    movwf LATC
    call DELAY
    clrf LATD
    clrf LATC
    btfss t4, 0
    goto xc4d4
    goto b4c4

b4c4
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00001000'
    movwf LATC
    movwf LATB
    call DELAY
    clrf LATC
    clrf LATB
    btfss t4, 0
    goto xc4d4
    goto a4b4

a4b4
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00001000'
    movwf LATB
    movwf LATA
    call WAIT
    clrf LATB
    clrf LATA
    btfss t4, 0
    goto xb4c4
    goto a3a4

a3a4
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00001100'
    movwf LATA
    call DELAY
    clrf LATA
    btfss t4, 0
    goto xa3a4
    goto a2a3

a2a3
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00000110'
    movwf LATA
    call DELAY
    clrf LATA
    btfss t4, 0
    goto xa3a4
    goto a1a2

a1a2
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00000011'
    movwf LATA
    call WAIT
    clrf LATA
    btfss t4, 0
    goto xa2a3
    goto a1b1

xa1b1
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00000001'
    movwf LATA
    movwf LATB
    call WAIT
    clrf LATA
    clrf LATB
    btfss t4, 0
    goto b1c1
    goto xa1a2

xb1c1
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00000001'
    movwf LATB
    movwf LATC
    call rDELAY
    clrf LATB
    clrf LATC
    btfss t4, 0
    goto c1d1
    goto xa1b1

xc1d1
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00000001'
    movwf LATC
    movwf LATD
    call rDELAY
    clrf LATC
    clrf LATD
    btfss t4, 0
    goto c1d1
    goto xb1c1

xd1d2
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00000011'
    movwf LATD
    call WAIT
    clrf LATD
    btfss t4, 0
    goto d2d3
    goto xc1d1

xd2d3
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00000110'
    movwf LATD
    call rDELAY
    clrf LATD
    btfss t4, 0
    goto d3d4
    goto xd1d2

xd3d4
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00001100'
    movwf LATD
    call rDELAY
    clrf LATD
    btfss t4, 0
    goto d3d4
    goto xd2d3

xc4d4
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00001000'
    movwf LATD
    movwf LATC
    call WAIT
    clrf LATD
    clrf LATC
    btfss t4, 0
    goto b4c4
    goto xd3d4

xb4c4
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00001000'
    movwf LATC
    movwf LATB
    call rDELAY
    clrf LATC
    clrf LATB
    btfss t4, 0
    goto a4b4
    goto xc4d4

xa4b4
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00001000'
    movwf LATB
    movwf LATA
    call rDELAY
    clrf LATB
    clrf LATA
    btfss t4, 0
    goto a4b4
    goto xb4c4

xa3a4
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00001100'
    movwf LATA
    call WAIT
    clrf LATA
    btfss t4, 0
    goto a2a3
    goto xa4b4

xa2a3
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00000110'
    movwf LATA
    call rDELAY
    clrf LATA
    btfss t4, 0
    goto a1a2
    goto xa3a4

xa1a2
    movlw d'1'
    movwf t4
    movwf t5
    movlw b'00000011'
    movwf LATA
    call rDELAY
    clrf LATA
    btfss t4, 0
    goto a1a2
    goto xa2a3


end


