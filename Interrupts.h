
void ENABLEBUTTON();

void BUTTONINTERRUPT();

void ENABLETACO();

void TACOINTERRUPT();
void TACOPULSEPERIOD();

void PWMTACOHIGH();
void PWMTACOLOW();

void ENABLEPWM();

void PINSTATUSHIGH();
void PINSTATUSLOW();

void dutyCycleUpdate(float dcIn);

int getRevs(bool reset);

int getRPM(bool reset);

int getMode();
