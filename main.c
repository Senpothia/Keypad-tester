/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16LF1939
        Driver Version    :  2.00
 */

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
 */

#include "mcc_generated_files/mcc.h"
#include "I2C_LCD.h"
#include "tester.h"
#include "display.h"

/*
                         Main application
 */
void main(void) {
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();



    // lCD
    I2C_Master_Init();
    LCD_Init(0x4E); // Initialize LCD module with I2C address = 0x4E
    bool testActif = false;
    bool testVoyants = false;
    int lectureAN1;
    bool testLeds = true;

    // Affichage message d'accueil

    displayManager(TITRE, BOARD_REQUEST, OK_REQUEST, LIGNE_VIDE);
    __delay_ms(1000);

    while (1) {

        /*
        if(GPIO1_GetValue() == 1){
        
            REL8_SetHigh();
        }else{
            REL8_SetLow();
        }
         **/

        if (GPIO1_GetValue() == 1) {

            REL8_SetHigh();
            testLeds = true;

        } else {

            testLeds = false;
            REL8_SetLow();
        }




        // Attente de démarrage

        displayManager(TITRE, ATTENTE, CAVALIERS, OK_REQUEST);
        __delay_ms(100);
        attenteDemarrage();

        displayManager("ETAPE 1", "TEST 3 RELAIS ON", LIGNE_VIDE, LIGNE_VIDE);
        testActif = true;
        ledConforme(false);
        ledNonConforme(false);
        ledProgession(true);
        REL8_SetLow();
        // entrée dans la séquence de test

        // ETAPE 1


        pressBP1(true);
        pressBP2(true);
        __delay_ms(100);
        alimenter(true);
        __delay_ms(2000);


        if (testR1(true) && testR2(true) && testR3(true)) {

        } else {

            testActif = false;
            pressBP1(false);
            pressBP2(false);
            alerteDefaut("ETAPE 1", &testActif, &testVoyants);

        }

        __delay_ms(1000);

        pressBP1(false);
        pressBP2(false);

        // ETAPE 2

        if (testActif) {

            displayManager("ETAPE 2", "TEST 3 RELAIS OFF", LIGNE_VIDE, LIGNE_VIDE);
            pressBP1(false);
            pressBP2(false);
            __delay_ms(500);
            if (!testR1(true) && !testR2(true) && !testR3(true)) {

            } else {

                testActif = false;
                alerteDefaut("ETAPE 2", &testActif, &testVoyants);
            }
        }


        // ETAPE 3

        if (testActif) {

            displayManager("ETAPE 3", "TEST LED ROUGE", LIGNE_VIDE, LIGNE_VIDE);
            pressBP1(true);
            __delay_ms(250);
            pressBP1(false);
            if (testLeds) {

                testVoyants = reponseOperateur();
                if (!testVoyants) {

                    testActif = false;
                    alerteDefaut("ETAPE 3", &testActif, &testVoyants);
                }
            }


        }

        // ETAPE 4

        if (testActif) {

            displayManager("ETAPE 4", "TEST LED BLEUE", LIGNE_VIDE, LIGNE_VIDE);
            pressBP1(true);
            __delay_ms(250);
            pressBP1(false);
            if (testLeds) {

                testVoyants = reponseOperateur();
                if (!testVoyants) {

                    testActif = false;
                    alerteDefaut("ETAPE 4", &testActif, &testVoyants);
                }
            }


        }

        // ETAPE 5

        if (testActif) {

            displayManager("ETAPE 5", "TEST LED VERTE", LIGNE_VIDE, LIGNE_VIDE);
            pressBP1(true);
            __delay_ms(250);
            pressBP1(false);
            if (testLeds) {

                testVoyants = reponseOperateur();
                if (!testVoyants) {

                    testActif = false;
                    alerteDefaut("ETAPE 5", &testActif, &testVoyants);
                }
            }

        }



        // ETAPE 6

        if (testActif) {

            displayManager("ETAPE 6", "TEST R1 ON", LIGNE_VIDE, LIGNE_VIDE);
            pressBP1(true);
            __delay_ms(1000);
            pressBP1(false);

            __delay_ms(1000);

            if (testR1(true)) {

            } else {

                testActif = false;
                alerteDefaut("ETAPE 6", &testActif, &testVoyants);
            }

        }

        // ETAPE 7

        if (testActif) {

            displayManager("ETAPE 7", "TEST R1 OFF - R2 ON", LIGNE_VIDE, LIGNE_VIDE);
            pressBP1(true);
            __delay_ms(1000);
            pressBP1(false);

            __delay_ms(1000);

            if (testR1(false) && testR2(true)) {

            } else {

                testActif = false;
                alerteDefaut("ETAPE 7", &testActif, &testVoyants);
            }

        }

        // ETAPE 8

        if (testActif) {

            displayManager("ETAPE 8", "TEST R2 OFF - R3 ON", LIGNE_VIDE, LIGNE_VIDE);
            pressBP1(true);
            __delay_ms(1000);
            pressBP1(false);

            __delay_ms(1000);

            if (testR2(false) && testR3(true)) {

            } else {

                testActif = false;
                alerteDefaut("ETAPE 8", &testActif, &testVoyants);
            }

        }

        // ETAPE 9

        if (testActif) {

            displayManager("ETAPE 9", "TEST LED CLAVIER", "CLAVIER ECLAIRE?", LIGNE_VIDE);
            pressBP1(true);
            __delay_ms(250);
            pressBP1(false);
            __delay_ms(500);
            // Lecture analogique
            // Led on: Van = 433
            // Led off: Van = 587

            lectureAN1 = ADC_GetConversion(VIN1);
            if (lectureAN1 < 480) {


                REL8_SetHigh();

            } else {

                alerteDefaut("ETAPE 9", &testActif, &testVoyants);
                REL8_SetLow();

            }

            __delay_ms(2000);

        }

        // ETAPE 10

        if (testActif) {

            displayManager("ETAPE 10", "TEST LED CLAVIER", "CLAVIER ETEINT?", LIGNE_VIDE);
            pressBP1(true);
            __delay_ms(250);
            pressBP1(false);

            // Lecture analogique
            // Led on: Van = 433
            // Led off: Van = 587
            __delay_ms(500);
            lectureAN1 = ADC_GetConversion(VIN1);
            if (lectureAN1 > 480) {


                REL8_SetLow();

            } else {

                alerteDefaut("ETAPE 10", &testActif, &testVoyants);
                REL8_SetHigh();

            }
            __delay_ms(2000);

        }


        // ETAPE 12

        if (testActif) {

            displayManager("ETAPE 12", "TEST SFLASH", LIGNE_VIDE, LIGNE_VIDE);
            __delay_ms(500);
            pressBP1(true);
            __delay_ms(250);
            pressBP1(false);

            __delay_ms(3000);

            pressBP1(true);
            __delay_ms(250);
            pressBP1(false);
            __delay_ms(750);

            if (testR1(true) && testR2(true) && testR3(false)) {

            } else {

                testActif = false;
                pressBP1(false);
                pressBP2(false);
                alerteDefaut("ETAPE 12", &testActif, &testVoyants);

            }

            __delay_ms(1000);

            pressBP1(false);
            pressBP2(false);

        }

        // ETAPE 13


        if (testActif) {

            displayManager("ETAPE 13", "TEST LEDS CARTE", "LEDS ALLUMEES", "PRESSER OK / NOK");
            pressBP1(true);
            __delay_ms(250);
            pressBP1(false);

            testVoyants = reponseOperateur();
            if (!testVoyants) {

                testActif = false;
                alerteDefaut("ETAPE 13", &testActif, &testVoyants);
            }
        }

        // ETAPE 14

        if (testActif) {

            displayManager("ETAPE 14", "TEST BP2", LIGNE_VIDE, LIGNE_VIDE);
            pressBP2(true);
            __delay_ms(250);
            pressBP2(false);
            __delay_ms(500);

            if (testR1(true) && testR2(true) && testR3(true)) {

            } else {

                testActif = false;
                alerteDefaut("ETAPE 14", &testActif, &testVoyants);
            }

        }

        // ETAPE 15

        if (testActif) {

            displayManager("ETAPE 15", "TEST HORLOGE", LIGNE_VIDE, LIGNE_VIDE);
            setHorloge(true);
            __delay_ms(250);
            setHorloge(false);
            __delay_ms(500);

            if (testR1(false) && testR2(false) && testR3(false)) {

            } else {

                testActif = false;
                alerteDefaut("ETAPE 15", &testActif, &testVoyants);
            }

        }

        // ETAPE 16

        if (testActif) {

            displayManager("ETAPE 16", "TEST P1", LIGNE_VIDE, LIGNE_VIDE);
            setP1(true);
            __delay_ms(1200);
            setP1(false);
            __delay_ms(500);
            if (testR1(true) && testR2(true) && testR3(true)) {

            } else {

                testActif = false;
                alerteDefaut("ETAPE 16", &testActif, &testVoyants);
            }

        }

        // ETAPE 17

        if (testActif) {

            displayManager("ETAPE 17", "TEST P2", LIGNE_VIDE, LIGNE_VIDE);
            setP2(true);
            __delay_ms(1200);
            setP2(false);
            __delay_ms(500);

            if (testR1(false) && testR2(false) && testR3(false)) {

            } else {

                testActif = false;
                alerteDefaut("ETAPE 17", &testActif, &testVoyants);
            }

        }


        // ETAPE 18

        if (testActif) {

            displayManager("ETAPE 18", "TEST BLUETOOTH", "VOIR APPLI", "PRESSER OK / NOK");
            activerTouche();
            testVoyants = reponseOperateur();
            if (!testVoyants) {

                testActif = false;
                alerteDefaut("ETAPE 18", &testActif, &testVoyants);
            }


        }


        // ETAPE: SORTIE

        if (testActif) {

            displayManager("FIN DE TEST", "CONFORME", "RETIRER CARTE", ACQ);
            ledConforme(true);
            attenteDemarrage();
            initialConditions(&testActif, &testVoyants);
            __delay_ms(2000);

        }

    } // fin boucle infinie



} // fin fonction main

/**
 End of File
 */