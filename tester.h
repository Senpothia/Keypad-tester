
void initialConditions(bool *, bool *, bool *);
void pressBP1(bool active);
void pressBP2(bool active);
void alimenter(bool active);
bool testR1(bool active);
bool testR2(bool active);
bool testR3(bool active);
bool testOK(bool active);
bool testNOK(bool active);
void ledNonConforme(bool active);
void ledConforme(bool active);
void ledProgession(bool active);
void attenteDemarrage(bool *, bool *);
void alerteDefaut(char etape[], bool *, bool *);
bool reponseOperateur(bool automatique);
bool controlVisuel();
void setHorloge(bool active);
void setP1(bool active);
void setP2(bool active);
void activerBuzzer();
void activerTouche(void);
void startAlert(void);
void errorAlert(void);
void okAlert(void);
void attenteDemarrage2(bool *, bool *);


