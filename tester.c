/*
 * File:   tester.c
 * Author: Michel
 *
 * Created on 31 août 2023, 15:28
 */


#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include "I2C_LCD.h"
#include "tester.h"
#include "display.h"

void alimenter(bool active) {

    if (active) {

        REL1_SetHigh();
    } else {
        REL1_SetLow();
    }
}

void pressBP1(bool active) {

    if (active) {

        REL2_SetHigh();
    } else {
        REL2_SetLow();
    }
}

void pressBP2(bool active) {

    if (active) {

        REL3_SetHigh();
    } else {
        REL3_SetLow();
    }
}

void setHorloge(bool active) {

    if (active) {

        REL4_SetHigh();
    } else {
        REL4_SetLow();
    }

}

bool testR1(bool active) {

    bool result = false;
    if (active) {

        if (IN1_GetValue() == 1 && IN2_GetValue() == 0) {

            result = true;
        }
    }

    if (!active) {

        if (IN1_GetValue() == 0 && IN2_GetValue() == 1) {


            result = true;
        }
    }

    return result;
}

bool testR2(bool active) {

    bool result = false;
    if (active) {

        if (IN4_GetValue() == 1 && IN5_GetValue() == 0) {

            result = true;
        }
    }

    if (!active) {

        if (IN4_GetValue() == 0 && IN5_GetValue() == 1) {

            result = true;
        }
    }

    return result;
}

bool testR3(bool active) {


    bool result = false;
    if (active) {

        if (IN6_GetValue() == 1 && IN7_GetValue() == 0) {

            result = true;
        }
    }

    if (!active) {

        if (IN6_GetValue() == 0 && IN7_GetValue() == 1) {

            result = true;
        }
    }

    return result;
}

bool testOK(bool active) {

    bool result = false;
    if (active) {

        if (IN3_GetValue() == 0) {

            result = true;
        }
    }

    if (!active) {

        if (IN3_GetValue() == 1) {

            result = true;
        }
    }

    return result;

}

bool testNOK(bool active) {

    bool result = false;
    if (active) {

        if (IN8_GetValue() == 0) {

            result = true;
        }
    }

    if (!active) {

        if (IN8_GetValue() == 1) {

            result = true;
        }
    }

    return result;
}

bool controlVisuel() {

    bool result = false;


    if (IN3_GetValue() == 0) {

        result = true;
    }

    if (IN3_GetValue() == 1) {

        result = false;

    }

}

void ledNonConforme(bool active) {

    if (active) {

        C3_SetHigh();
    } else {
        C3_SetLow();
    }
}

void ledConforme(bool active) {

    if (active) {

        C4_SetHigh();
    } else {
        C4_SetLow();
    }
}

void ledProgession(bool active) {

    if (active) {

        C2_SetHigh();

    } else {

        C2_SetLow();
    }
}

void attenteDemarrage() {

    while (IN3_GetValue() == 1) {
    }
}

void alerteDefaut(char etape[], bool *testAct, bool *testVoy) {

    ledNonConforme(true);
    ledProgession(false);
    ledConforme(false);
    alimenter(false);
    displayManager(etape, NON_CONFORME, ACQ, LIGNE_VIDE);
    errorAlert();
    while (IN3_GetValue() == 1) {
    }
    while (IN3_GetValue() == 0) {
    }
    ledNonConforme(false);
    *testAct = false;
    *testVoy = false;

}

bool reponseOperateur() {

    bool reponse = false;
    while (testOK(false) && testNOK(false)) {
    }
    if (testNOK(true)) {
        reponse = false;
    }
    if (testOK(true)) {
        reponse = true;
    }

    while (testOK(true) || testNOK(true)) {
    }
    return reponse;

}

void setP1(bool active) {

    if (active) {

        REL5_SetHigh();
    } else {
        REL5_SetLow();
    }

}

void setP2(bool active) {

    if (active) {

        REL6_SetHigh();
    } else {
        REL6_SetLow();
    }

}

void initialConditions(bool *testAct, bool *testVoy) {

    *testAct = false;
    *testVoy = false;
    ledConforme(false);
    ledNonConforme(false);
    ledProgession(false);
    alimenter(false);
    pressBP1(false);
    pressBP2(false);
    setP1(false);
    setP2(false);

}

void activerBuzzer() {


    for (int i = 0; i < 50; i++) {

        BUZ_SetHigh();
        // __delay_ms(1);
        __delay_us(1000);

        BUZ_SetLow();
        // __delay_ms(1);
        __delay_us(1000);

    }

}

void activerTouche(void) {

    REL7_SetHigh();
    __delay_ms(250);
    REL7_SetLow();
    __delay_ms(250);

}

void startAlert(void) {

    for (int i = 0; i < 2; i++) {

        activerBuzzer();
        __delay_ms(500);

    }

}

void errorAlert(void) {

    for (int j = 0; j < 4; j++) {

        for (int i = 0; i < 4; i++) {

            activerBuzzer();
            __delay_ms(500);

        }
        __delay_ms(500);
    }


}

void okAlert(void) {

    for (int i = 0; i < 2; i++) {

        startAlert();

    }


}
