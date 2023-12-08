void enable_Button();

void BUTTONINTERRUPT();

void enableRotaryEncoder();

void ROTENCINTERRUPT();

bool getRotEncRotated();

void setRotEncRotated(bool newVal);

void setRotEncSetPoint(float setPoint);

void setRotEncMax(float max);

void setRotEncMin(float min);

void setRotEncResolution(float res);

float getPulseCount();

int getMode();

bool getModeChanged();

void resetModeChange();
