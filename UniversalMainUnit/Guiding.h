void initGuiding(void);
void startGuiding(char dir, int milliseconds);
void stopGuidingAxis1(UNUSED(TimerHandle_t xTimer));
void stopGuidingAxis2(UNUSED(TimerHandle_t xTimer));
void spiralTask(UNUSED(TimerHandle_t xTimer));
void startSpiral(double);
void stopSpiral(void);
void getSpiralSpeeds(Speeds *);
