//Kübra Alt?n-2035624
//Mert Bekar-2035749


#include <p18cxxx.h>
#include <p18f8722.h>
#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#include <stdio.h>
#include <stdlib.h>
#include "Includes.h"
#include "lcd.h"

/*
 *
 */
int counter500;  // Counters for interrupts
int counter250;
int counter100;
int counter120;
int adcon_counter;

int sharpOn; // Flag to blinking in LCD

int pressed6; // Flags to button press-relased check
int pressed7;

int completed; // Flag to indicate pin is set or not set

int start;
int blink;
int breaking;

unsigned char number1; // Determined pin from user
unsigned char number2;
unsigned char number3;
unsigned char number4;

unsigned char try1; // Tried pin from user
unsigned char try2;
unsigned char try3;
unsigned char try4;

int setting;

int count1; // 7-Segment value variables
int count2;
int count3;
int count4;

int phase2; // Flag for phase1 and phase2
int flag=0;
unsigned char attempt;
int completed2;


int left_time;
int wait;
int current_time;
int fre; // Flag for decide to '#' character will blinks or not
int counter6;

int tempRB;

int seq; // Flag for indicate which digit of number is setting
int seq2;
int ad;

char global; // Variable that used for initial variable of potentiometer

unsigned char  const segment_map[11] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x40}; // Used for numbers and '-' patterns as a segment_map[0] = 0 , segment_map[0] = 1 ...

void gotosharp(); // Shows "Set a Pin", "Enter Pin" displays
void gotocompletedsharp();

void ADC_interrupt(); // Used for interrupt enables

char setNumber(int x); // Indicate user input according to potentiometer

void sevensegment(); //Shows 7-Segment display

void delay();

void init(); //Used for initializing

void interrupt isr(){ // Isr function

    if(INTCONbits.TMR0IF){

          INTCONbits.TMR0IF =0;
        counter500++;
        counter250++;
        counter100++;


        if(counter250==305){ // Determines 250ms
            counter250=0;
            if(sharpOn==0)
                sharpOn=1; // Flag for blinking
            else
                sharpOn=0;
        }



        if(counter500==610){ // Determines 500ms
            counter500=0;
            if(completed)
                 counter6++;
            if(counter6==6)
                breaking=1; // Determine 3sn
            if(blink==0)
                blink=1; // Flag for blinking
            else
                blink=0;
        }

        if(counter100==122){ // Determines 100ms
            counter100=0;
                if(ADIF){
                        ADIF=0;

                        if(ad==0){
                            ad=1;
                            ADCON0bits.GO=1;
                            while(ADCON0bits.GO);
                            global = setNumber(ADRES); // Gets last value of potentiometer
                        }


                        else{
                            if(global!= setNumber(ADRES)){ // Checks potentiometer value changes or not
                                fre=1;
                               if(phase2==0){ // Checks phase1 or phase2
                                        if(seq==1)
                                            number1=setNumber(ADRES); // If phase1, get a pin and set it
                                        else if(seq==2)
                                            number2=setNumber(ADRES);
                                        else if(seq==3)
                                            number3=setNumber(ADRES);
                                        else if(seq==4)
                                            number4=setNumber(ADRES);
                                }

                                else if(phase2==1){ // If phase2, get a tried pin

                                        if(seq==1)
                                            try1=setNumber(ADRES);
                                        else if(seq==2)
                                            try2=setNumber(ADRES);
                                        else if(seq==3)
                                            try3=setNumber(ADRES);
                                        else if(seq==4)
                                            try4=setNumber(ADRES);

                                }
                               global = '!'; // Sets invalid value to global variable that uses for initial potentiometer value
                            }
                        }


                    ADC_interrupt();

                }
        }


    }


    else if(INTCONbits.RBIF==1){
        tempRB = PORTB;
        INTCONbits.RBIF=0; //button interrupt
        tempRB = PORTB;
        if(PORTBbits.RB6==0 && pressed6==1){ // Checks RB6 is pressed or not
            pressed6=0;
        }
        else if(PORTBbits.RB6==1 && pressed6==0){ // Checks RB6 is released if RB6 is pressed
            pressed6=1;
            seq++;
            ad=0;
            fre=0;
        }

        else if(PORTBbits.RB7==0 && pressed7==1){ // Checks RB7 is pressed or not
            pressed7=0;
        }
        else if(PORTBbits.RB7==1 && pressed7==0){ // Checks RB6 is released if RB6 is pressed
            pressed7=1;
            ad=0;
            if(completed==1){
               if(try1==number1 && try2==number2 && try3==number3 && try4==number4){ // Checks decided pin and tried pins are same
                   completed2=1;
              }
               else if(completed2==0){ // If tried pin is wrong, shows related display
                if(attempt=='2'){
                      attempt='1';
                      seq=1;
                      try1='#';
                      try2='#';
                      try3='#';
                      try4='#';
                }
                else if(attempt=='1'){ // If tried pin is wrong second time, waiting period (20sn) is begins
                  wait=1;
                  current_time=left_time;
                }
               }
            }


            else if(completed==0) // If RB7 is pressed and released, "complated" flag set to 1
                completed=1;
            }
        }



    else if(PIR1bits.TMR1IF){
        sevensegment(); // Shows 7-segment display
        if(start){ // Check "start" flag for 7-segment display begins or not
        counter120++;
        if(counter120==38){ // Determines 1 sn
            counter120=0;
            left_time--;
            if(!completed2){
            if(count4==0){ // Arranges the numbers in 7-Segment
                count4 = 9;
                if(count3>0){
                    count3--;
                }
                else
                {
                    if(count2>0)
                    {
                        count4 = 9;
                        count3 = 9;
                        count2--;
                    }
                    else
                    {
                        count2 = 0;
                        count3 = 0;
                        count4 = 0;
                    }
                }
            }

            else{
                count4--;
            }
           }
        }
                            sevensegment();

        if(count1==0 && count2==0 && count3==0 && count4==0) // If 120sn period is end, returns begin
            RESET();
        }
        PIR1bits.TMR1IF=0;

    }
            return;


}

void RB_interrupt(){  // Initialize RB interrupt
    PORTB=0;
    LATB=0;
    TRISB=0B11000000; // RB6 and RB7 is input the others output
    RBIE=1; // Enable
    INTCON2bits.RBPU=0; //Pull up
}


void gotosharp(){ // State 1
          if(!sharpOn && completed==0){  // Show "Set a pin" display if state is not completed
              WriteCommandToLCD(0x80);
              WriteStringToLCD(" Set a pin:");
              WriteDataToLCD(number1);
              WriteDataToLCD(number2);
              WriteDataToLCD(number3);
              WriteDataToLCD(number4);
              WriteDataToLCD(' ');
              WriteCommandToLCD(0xC0);
              WriteStringToLCD("                ");
          }
          else if(sharpOn && completed==0 && fre==0){ // Show "Set a pin" display with blink if state is not completed and initial potentiometer value does not change
              if(seq==1){
              WriteCommandToLCD(0x80);
              WriteStringToLCD(" Set a pin:");
              WriteDataToLCD(' ');
              WriteDataToLCD(number2);
              WriteDataToLCD(number3);
              WriteDataToLCD(number4);

              WriteCommandToLCD(0xC0);
              WriteStringToLCD("                ");
              }
              else if(seq==2){
              WriteCommandToLCD(0x80);
              WriteStringToLCD(" Set a pin:");
              WriteDataToLCD(number1);
              WriteDataToLCD(' ');
              WriteDataToLCD(number3);
              WriteDataToLCD(number4);

              WriteCommandToLCD(0xC0);
              WriteStringToLCD("                ");
              }

              else if(seq==3){
              WriteCommandToLCD(0x80);
              WriteStringToLCD(" Set a pin:");
              WriteDataToLCD(number1);
              WriteDataToLCD(number2);
              WriteDataToLCD(' ');
              WriteDataToLCD(number4);

              WriteCommandToLCD(0xC0);
              WriteStringToLCD("                ");
              }

              else if(seq==4){
              WriteCommandToLCD(0x80);
              WriteStringToLCD(" Set a pin:");
              WriteDataToLCD(number1);
              WriteDataToLCD(number2);
              WriteDataToLCD(number3);
              WriteDataToLCD(' ');

              WriteCommandToLCD(0xC0);
              WriteStringToLCD("                ");
              }
          }

          else if(completed){

              PORTH = 0x0F;
              PORTJ = 0x40;

              while(!breaking){ // Shows "The new pin is .." display when setting pin state is completed
                  if(blink==0){
                  WriteCommandToLCD(0x80);
                  WriteStringToLCD(" The new pin is ");
                  WriteCommandToLCD(0xC0);
                  WriteStringToLCD("   ---");
                  WriteDataToLCD(number1);
                  WriteDataToLCD(number2);
                  WriteDataToLCD(number3);
                  WriteDataToLCD(number4);
                  WriteStringToLCD("---   ");
                  }
                  else{ // Blink part for this display
                      WriteCommandToLCD(0x80);
                      WriteStringToLCD("                ");
                      WriteCommandToLCD(0xC0);
                      WriteStringToLCD("                ");
                  }
             }

              phase2=1;
              seq=1;
              start=1;
              gotocompletedsharp();

         }
}


void gotocompletedsharp(){ // State 2
           if(!wait){
              if(completed2==1){ // If tried pin correct
                  WriteCommandToLCD(0x80);
                  WriteStringToLCD("Safe is opening!");
                  WriteCommandToLCD(0xC0);
                  WriteStringToLCD("$$$$$$$$$$$$$$$$");
              }

              else if(!sharpOn && completed==0){  // Show "Enter pin" display if state is not completed
              WriteCommandToLCD(0x80);
              WriteStringToLCD(" Enter pin:");
              WriteDataToLCD(try1);
              WriteDataToLCD(try2);
              WriteDataToLCD(try3);
              WriteDataToLCD(try4);
          }
            else if(sharpOn && completed==0 && fre==0){ // Show "Enter pin" display with blink if state is not completed and initial potentiometer value does not change
              if(seq==1){
              WriteCommandToLCD(0x80);
              WriteStringToLCD(" Enter pin:");
              WriteDataToLCD(' ');
              WriteDataToLCD(try2);
              WriteDataToLCD(try3);
              WriteDataToLCD(try4);
              }
              else if(seq==2){
              WriteCommandToLCD(0x80);
              WriteStringToLCD(" Enter pin:");
              WriteDataToLCD(try1);
              WriteDataToLCD(' ');
              WriteDataToLCD(try3);
              WriteDataToLCD(try4);
              }

              else if(seq==3){
              WriteCommandToLCD(0x80);
              WriteStringToLCD(" Enter pin:");
              WriteDataToLCD(try1);
              WriteDataToLCD(try2);
              WriteDataToLCD(' ');
              WriteDataToLCD(try4);
              }

              else if(seq==4){
              WriteCommandToLCD(0x80);
              WriteStringToLCD(" Enter pin:");
              WriteDataToLCD(try1);
              WriteDataToLCD(try2);
              WriteDataToLCD(try3);
              WriteDataToLCD(' ');
              }
          }

          else if(!sharpOn && completed==1 && completed2==0){ // Show "Enter pin" display if state is not completed and first try was incorrect
              WriteCommandToLCD(0x80);
              WriteStringToLCD(" Enter pin:");
              WriteDataToLCD(try1);
              WriteDataToLCD(try2);
              WriteDataToLCD(try3);
              WriteDataToLCD(try4);
              WriteCommandToLCD(0xC0);
              WriteStringToLCD("  Attempts:");
              WriteDataToLCD(attempt);
              sharpOn = 1;

          }
          else if(sharpOn && completed==1 && completed2==0 && fre==0){ // Show "Enter pin" display with blink if state is not completed, initial potentiometer value does not change and first try was incorrect
                if(seq==1){
              WriteCommandToLCD(0x80);
              WriteStringToLCD(" Enter pin:");
              WriteDataToLCD(' ');
              WriteDataToLCD(try2);
              WriteDataToLCD(try3);
              WriteDataToLCD(try4);
              WriteCommandToLCD(0xC0);
              WriteStringToLCD("  Attempts:");
              WriteDataToLCD(attempt);
              WriteStringToLCD("    ");
              }
              else if(seq==2){
              WriteCommandToLCD(0x80);
              WriteStringToLCD(" Enter pin:");
              WriteDataToLCD(try1);
              WriteDataToLCD(' ');
              WriteDataToLCD(try3);
              WriteDataToLCD(try4);
              WriteCommandToLCD(0xC0);
              WriteStringToLCD("  Attempts:");
              WriteDataToLCD(attempt);
              WriteStringToLCD("    ");
              }

              else if(seq==3){
              WriteCommandToLCD(0x80);
              WriteStringToLCD(" Enter pin:");
              WriteDataToLCD(try1);
              WriteDataToLCD(try2);
              WriteDataToLCD(' ');
              WriteDataToLCD(try4);
              WriteCommandToLCD(0xC0);
              WriteStringToLCD("  Attempts:");
              WriteDataToLCD(attempt);
              WriteStringToLCD("    ");
              }

              else if(seq==4){
              WriteCommandToLCD(0x80);
              WriteStringToLCD(" Enter pin:");
              WriteDataToLCD(try1);
              WriteDataToLCD(try2);
              WriteDataToLCD(try3);
              WriteDataToLCD(' ');
              WriteCommandToLCD(0xC0);
              WriteStringToLCD("  Attempts:");
              WriteDataToLCD(attempt);
              WriteStringToLCD("    ");

              }

          }

           }

           else{
               if(current_time-left_time==21){ // Shows "Enter pin" displays again after 2 incorrect try and 20 second period
                   wait =0;
                   ad=0;
                   attempt='2';
                   try1='#';
                   try2='#';
                   try3='#';
                   try4='#';
                   seq=1;
               }
               else{ // Shows related display for 20 sec
                  WriteCommandToLCD(0x80);
                  WriteStringToLCD(" Enter pin:XXXX ");
                  WriteCommandToLCD(0xC0);
                  WriteStringToLCD("Try after 20 sec.");
               }
           }
}

char setNumber(int address){ // Specify value of potentiometer

    if(0 <= address && address <= 99)
        return '0';
    else if(100 < address && address <= 199)
        return '1';
    else if(200 < address && address <= 299)
        return '2';
    else if(300 < address && address <= 399)
        return '3';
    else if(400 <= address && address <= 499)
        return '4';
    else if(500 < address && address <= 599)
        return '5';
    else if(600 < address && address <= 699)
        return '6';
    else if(700 < address && address <= 799)
        return '7';
    else if(800 < address && address <= 899)
        return '8';
    else if(900 < address && address <= 1024)
        return '9';
}



void TM0_interrupt(){ // Enable of Timer0 interrupt
   T0CON= 0b11000100;
   TMR0IF =1; // Set interrupt flag
   counter500=0; // Set 0 to counters for timer interrupts
   counter250=0;
   counter100=0;
   sharpOn=0;
}


void TM1_interrupt(){ // Enable of Timer1 interrupt
    T1CON = 0B00100001;
    TMR1IE =1; // Set interrupt flag
    counter120=0; // Set 0 to counters for timer interrupts
}


void ADC_interrupt(){ // Enable of ADC interrupt

    ADCON0 = 0x33; // Channel 12, A/D on
    ADCON1 = 2;
    ADFM = 1;
    TRISH4 = 1;

}


void delay(){ // Delay for 3 Seconds (busy wait)

    for(int i=0;i<255;i++){
        for(int i=0;i<250;i++){
            ;
        }
    }
}


void sevensegment(){ // 7-segment display

    if(!completed){
        PORTH = 0x0F;
        PORTJ = 0x40;
    }
    else if(completed && start){


        PORTH =0x01;
        PORTJ=0;
        PORTJ = segment_map[count1];
        __delay_ms(2);


        PORTH =0x02;
        PORTJ=0;
        PORTJ = segment_map[count2];
        __delay_ms(2);


        PORTH =0x04;
        PORTJ=0;
        PORTJ = segment_map[count3];
        __delay_ms(2);

        PORTH =0x8;
        PORTJ=0;
        PORTJ = segment_map[count4];
        __delay_ms(2);

    }

}


void init(){ // Initializing
    __delay_ms(15); __delay_ms(15); __delay_ms(15); __delay_ms(15);


    InitLCD();	// Initialize LCD, Flags, TRIS, Ports and other values
    ClearLCDScreen();
    pressed6=1;
    pressed7=1;
    seq=1;
    completed=0;
    adcon_counter=0;
    ad=0;
    fre=0;
    blink=0;
    counter6=0;
    breaking=0;

    TRISE = 0x2; // RE1 input .
    PORTE = 0;
    LATE = 0;

    number1='#';
    number2='#';
    number3='#';
    number4='#';
    try1='#';
    try2='#';
    try3='#';
    try4='#';
    count1 = 0;
    count2 = 1;
    count3 = 2;
    count4 = 0;
    wait=0;
    start=0;
    completed2=0;
    attempt='2';

    phase2=0;
    left_time=120;

    INTCON=0xE0; // Initialize INTCON

    WriteCommandToLCD(0x80); // Shows initial display
    WriteStringToLCD(" $>Very  Safe<$ ");
    WriteCommandToLCD(0xC0);
    WriteStringToLCD(" $$$$$$$$$$$$$$ ");

    while(1){  // Checks RE1 button is pressed and released
        if(PORTEbits.RE1==0x00){
            while(PORTEbits.RE1==0){
            }
            break;
        }
    }

for(int i=0;i<26;i++ )
                 delay();


    TM0_interrupt(); // Initializing interrupts
    TM1_interrupt();
    ADC_interrupt();
    RB_interrupt();

    ClearLCDScreen(); // Initializing LCD Screen

    while(1){  // Shows related display according to phase1 or phase2
        if(phase2==0)
            gotosharp();
        else
            gotocompletedsharp();
    }
}



int main(int argc, char** argv) {
    init();
    return (EXIT_SUCCESS);
}

