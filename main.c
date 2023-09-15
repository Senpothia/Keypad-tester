/**
 TEST CARTE D925ED4 - BOX GALEO BLTH
 * 
 * Septembre 2023
 * 
 * Michel LOPEZ
 * 
 */

/*
 Equipement: automate v1.0
 * PIC16LF1939
 * 
 * Relais:
 * 
 * R1: alimentation
 * R2: BP1
 * R3: BP2
 * R4: Horloge
 * R5: Cavalier P1
 * R6: Cavalier P2
 * R7: Touche clavier - "Doigt"
 * R8: Avant séquence de test = indicateur inhibition tests des leds; dans sequence de test = indicateur éclairage clavier
 * 
 * Entrées:
 * 
 * J13/(IN1 dans le code): NC1
 * J15/IN2: NO1
 * J16/IN4: NC2
 * J17/IN5: NO2
 * J19/IN6: NC3
 * J18/IN7: NO3
 * J14/IN3: Bouton de validation OK 
 * J20/IN8: Bouton de confirmation NOK
 * J21/AN1: test éclairage clavier par mesure analogique
 * 
 * Sorties:
 * J25.1/C2: led rouge (indication non conforme)
 * J25.2/C4: led verte (indication conforme)
 * J25.3/C2: led jaune (indication test en cours)
 * J26(serigraphie OV)/GPIO1: inhibition test des leds - starp au OV = tests des leds inhibé
 * 
 * Mode d'emploi:
 * 1- Sélectionner mode de test des leds (strap = inhibition)
 * 2- Appuyer sur OK, pour lancer la séquence
 * 3- Appuyer sur OK pour valider / sur NOK pour invalider
 * 4- En fin de séquence le résultat est donné par la led verte ou rouge
 * 5- Acquiter le résultat en appuyant sur OK
 * 
 * 
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
    bool automatique = false;




    // Affichage message d'accueil

    displayManager(TITRE, BOARD_REQUEST, OK_REQUEST, LIGNE_VIDE);
    __delay_ms(1000);


    while (1) {


        // sélection test individuel des leds
        // le test est inhibé si l'entrée GPIO1 est à zéro

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
        // attenteDemarrage(&automatique, &testActif);
        attenteDemarrage2(&automatique, &testActif);
        testActif = true;
        startAlert();
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

                printf("Attente validation led rouge\r\n");
                testVoyants = reponseOperateur(automatique);
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

                testVoyants = reponseOperateur(automatique);
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

                testVoyants = reponseOperateur(automatique);
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

            printf("ATTENTE VALIDATION LEDS\r\n");
            testVoyants = reponseOperateur(automatique);
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
            printf("ATTENTE VALIDATION BLUETOOTH\r\n");
            testVoyants = reponseOperateur(automatique);
            if (!testVoyants) {

                testActif = false;
                alerteDefaut("ETAPE 18", &testActif, &testVoyants);
            }
        }


        // ETAPE: SORTIE

        if (testActif) {

            displayManager("FIN DE TEST", "CONFORME", "RETIRER CARTE", ACQ);
            ledConforme(true);
            alimenter(false);
            okAlert();
            //attenteDemarrage(&automatique, &testActif);
            attenteAquittement(&automatique, &testActif);
            initialConditions(&testActif, &testVoyants, &automatique);
            __delay_ms(2000);

        }

    } // fin boucle infinie



} // fin fonction main

/**
 End of File
 */