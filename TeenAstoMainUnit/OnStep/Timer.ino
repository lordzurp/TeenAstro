// -----------------------------------------------------------------------------------
// Timers and interrupt handling



#if (defined(__arm__) && defined(TEENSYDUINO))
#define ISR(f)  void f (void)
void                TIMER1_COMPA_vect(void);
volatile boolean    clearAxis1 = true;
volatile boolean    takeStepAxis1 = false;
volatile boolean    clearAxis2 = true;
volatile boolean    takeStepAxis2 = false;

#if defined(__arm__) && defined(TEENSYDUINO)
IntervalTimer       itimer1;
#endif

// Energia does not have IntervalTimer so the timers were already initialised in OnStep.ino
#endif
#if defined(AXIS1_MODE) && defined(AXIS1_MODE_GOTO)
volatile long       modeAxis1_next = AXIS1_MODE;
volatile boolean    gotoModeAxis1 = false;
#endif
#if defined(AXIS2_MODE) && defined(AXIS2_MODE_GOTO)
volatile long       modeAxis2_next = AXIS2_MODE;
volatile boolean    gotoModeAxis2 = false;
#endif
#ifdef AUTO_POWER_DOWN_AXIS2_ON
volatile long       Axis2PowerOffTimer = 0;
volatile bool       axis2Powered = false;
#endif


double getV(long rate)
{
  return 1000000.0 / (rate / 16L);
}

long getRate(double V)
{
  return max(16. * 1000000.0 / V, maxRate);

}


//--------------------------------------------------------------------------------------------------

// set timer1 to rate (in microseconds*16)
void Timer1SetRate(long rate)
{
  itimer1.begin(TIMER1_COMPA_vect, (float)rate * 0.0625);
}

// set the master sidereal clock rate, also forces rate update for RA/Dec timer rates so that PPS adjustments take hold immediately
volatile long   isrTimerRateAxis1 = 0;
volatile long   isrTimerRateAxis2 = 0;
volatile long   runtimerRateAxis1 = 0;
volatile long   runTimerRateAxis2 = 0;
void SetSiderealClockRate(long Interval)
{
  if (trackingState == TrackingMoveTo)
    Timer1SetRate(Interval / 100);
  else
    Timer1SetRate(Interval / 300);
  isrTimerRateAxis1 = 0;
  isrTimerRateAxis2 = 0;
}

// set timer3 to rate (in microseconds*16)
volatile uint32_t   nextAxis1Rate = 100000UL;
void Timer3SetRate(long rate)
{
  cli();
  nextAxis1Rate = (F_BUS / 1000000) * (rate * 0.0625) * 0.5 - 1;
  sei();
}

// set timer4 to rate (in microseconds*16)
volatile uint32_t   nextAxis2Rate = 100000UL;
void Timer4SetRate(long rate)
{
  cli();
  nextAxis2Rate = (F_BUS / 1000000) * (rate * 0.0625) * 0.5 - 1;
  sei();
}

//--------------------------------------------------------------------------------------------------
// Timer1 handles sidereal time and programming the drive rates
volatile boolean    wasInbacklashAxis1 = false;
volatile boolean    wasInbacklashAxis2 = false;

volatile boolean    gotoRateAxis1 = false;
volatile boolean    gotoRateAxis2 = false;
volatile byte       cnt = 0;

volatile double     guideTimerRateAxis1A = 0;
volatile double     guideTimerRateAxis2A = 0;


void updateDeltaTarget()
{
  cli();
  deltaTargetAxis1 = distStepAxis1(posAxis1, (long)targetAxis1.part.m);
  deltaTargetAxis2 = distStepAxis2(posAxis2, (long)targetAxis2.part.m);
  sei();
}

ISR(TIMER1_COMPA_vect)

{
  // run 1/3 of the time at 3x the rate, unless a goto is happening
  if (trackingState != TrackingMoveTo)
  {
    cnt++;
    if (cnt % 3 != 0) return;
    cnt = 0;
  }
  rtk.m_lst++;

  if (trackingState != TrackingMoveTo)
  {
    double maxguideTimerRate = 4;
    // automatic rate calculation HA
    {

    long    calculatedTimerRateAxis1;

    // guide rate acceleration/deceleration and control
    updateDeltaTarget();

    double  x = deltaTargetAxis1;
    bool other_axis_done = fabs(deltaTargetAxis2) < BreakDistAxis2;

    if ((!inbacklashAxis1) && (guideDirAxis1))
    {
      if ((fabs(guideTimerRateAxis1) < maxguideTimerRate) &&
        (fabs(guideTimerRateAxis1A) < maxguideTimerRate))
      {
        // break mode
        if (guideDirAxis1 == 'b')
        {
          guideTimerRateAxis1 = trackingTimerRateAxis1;
          if (guideTimerRateAxis1 >= 0)
            guideTimerRateAxis1 = 1.0;
          else
            guideTimerRateAxis1 = -1.0;
        }

        // slow speed guiding, no acceleration
        guideTimerRateAxis1A = guideTimerRateAxis1;
      }
      else
      {
        // use acceleration
        DecayModeGoto();
        double z = guideDirAxis1 == 'b' ? getRate(sqrt(fabs(x) * 2 * DccAxis1)) : getRate(sqrt(fabs(x) * 2 * AccAxis1));
        guideTimerRateAxis1A = (1.0 / ((StepsPerDegreeAxis1 * (z / 1000000.0))) * 3600.0);
        if (guideTimerRateAxis1A < maxguideTimerRate) guideTimerRateAxis1A = maxguideTimerRate;
      }

      // stop guiding
      if (guideDirAxis1 == 'b')
      {
        if (fabs(x) < BreakDistAxis1)
        {
          guideDirAxis1 = 0;
          guideTimerRateAxis1 = 0;
          guideTimerRateAxis1A = 0;
          if (other_axis_done)
            DecayModeTracking();
        }
      }
    }
    double  timerRateAxis1A = trackingTimerRateAxis1;
    double  timerRateAxis1B = fabs(guideTimerRateAxis1A + timerRateAxis1A);

    // round up to run the motor timers just a tiny bit slow, then adjust below if we start to fall behind during sidereal tracking
    if (timerRateAxis1B > 0.5)
      calculatedTimerRateAxis1 =
      ceil((double)SiderealRate / timerRateAxis1B)
      + 5;
    else
      calculatedTimerRateAxis1 = ceil((double)SiderealRate * 2.0);

    // remember our "running" rate and only update the actual rate when it changes
    if (runtimerRateAxis1 != calculatedTimerRateAxis1)
    {
      timerRateAxis1 = calculatedTimerRateAxis1;
      runtimerRateAxis1 = calculatedTimerRateAxis1;
    }

    // dynamic rate adjust
    // in pre-scaler /64 mode the motor timers might be slow (relative to the sidereal timer) by as much as 0.000004 seconds/second (16000000/64)
    // so a 0.01% (0.0001x) increase is always enough to correct for this, it happens very slowly - about a single step worth of movement over an hours time
    if (x > 10.0) x = 10.0;
    if (x < -10.0) x = -10.0;
    x = 10000.00 - x;
    x = x / 10000.0;
    timerRateAxis1 = max((long)(calculatedTimerRateAxis1 * x), maxRate);      // up to 0.01% faster or slower (or as little as 0.001%)
    runtimerRateAxis1 = timerRateAxis1;

  }
    // automatic rate calculation Dec
    {
      long    calculatedTimerRateAxis2;

      // guide rate acceleration/deceleration

      updateDeltaTarget();

      double x = fabs(deltaTargetAxis2);
      bool other_axis_done = fabs(deltaTargetAxis1) < BreakDistAxis1;

      if (!inbacklashAxis2 && guideDirAxis2)
      {
        if ((fabs(guideTimerRateAxis2) < maxguideTimerRate) &&
          (fabs(guideTimerRateAxis2A) < maxguideTimerRate))
        {
          // break mode
          if (guideDirAxis2 == 'b')
          {
            guideTimerRateAxis2 = trackingTimerRateAxis2;
            if (guideTimerRateAxis2 >= 0)
              guideTimerRateAxis2 = 1.0;
            else
              guideTimerRateAxis2 = -1.0;
          }

          // slow speed guiding, no acceleration
          guideTimerRateAxis2A = guideTimerRateAxis2;
        }
        else
        {
          // use acceleration
          DecayModeGoto();
          double  z = guideDirAxis1 == 'b' ? getRate(sqrt(fabs(x) * 2 * DccAxis2)) : getRate(sqrt(fabs(x) * 2 * AccAxis2));
          guideTimerRateAxis2A = (1.0 / (((double)StepsPerDegreeAxis2 * (z / 1000000.0))) * 3600.0);
          if (guideTimerRateAxis2A < maxguideTimerRate) guideTimerRateAxis2A = maxguideTimerRate;
        }

        // stop guiding
        if (guideDirAxis2 == 'b')
        {
          if (x < BreakDistAxis2)
          {
            guideDirAxis2 = 0;
            guideTimerRateAxis2 = 0;
            guideTimerRateAxis2A = 0;
            if (other_axis_done)
              DecayModeTracking();
          }
        }
      }
      double  timerRateAxis2A = trackingTimerRateAxis2;
      double  timerRateAxis2B = fabs(guideTimerRateAxis2A + timerRateAxis2A);

      // round up to run the motor timers just a tiny bit slow, then adjust below if we start to fall behind during sidereal tracking
      if (timerRateAxis2B > 0.5)
        calculatedTimerRateAxis2 =
        ceil((double)SiderealRate / timerRateAxis2B)
        + 5;
      else
        calculatedTimerRateAxis2 = ceil((double)SiderealRate * 2.0);

      // remember our "running" rate and only update the actual rate when it changes
      if (runTimerRateAxis2 != calculatedTimerRateAxis2)
      {
        timerRateAxis2 = calculatedTimerRateAxis2;
        runTimerRateAxis2 = calculatedTimerRateAxis2;
      }

      // dynamic rate adjust
      if (x > 1.0)
      {
        x = x - 1.0;
        if (x > 10.0) x = 10.0;
        x = 10000.00 - x;
        x = x / 10000.0;
        timerRateAxis2 = calculatedTimerRateAxis2 * x;  // up to 0.01% faster (or as little as 0.001%)
        runTimerRateAxis2 = timerRateAxis2;
      }
    }

    if (!guideDirAxis1 && !guideDirAxis2)
    {
      GuidingState = GuidingOFF;
    }
  }
  long    thisTimerRateAxis1 = timerRateAxis1;
  long    thisTimerRateAxis2 = useTimerRateRatio ? timerRateAxis2 * timerRateRatio : timerRateAxis2;


  timerRateAxis2 = max(timerRateAxis2, maxRate);     
  thisTimerRateAxis2 = timerRateAxis2;

  // override rate during backlash compensation
  if (inbacklashAxis1)
  {
    thisTimerRateAxis1 = timerRateBacklashAxis1;
    wasInbacklashAxis1 = true;
  }

  // override rate during backlash compensation
  if (inbacklashAxis2)
  {
    thisTimerRateAxis2 = timerRateBacklashAxis2;
    wasInbacklashAxis2 = true;
  }
  if ((trackingState == TrackingON))
  {
    // travel through the backlash is done, but we weren't following the target while it was happening!
    // so now get us back to near where we need to be
    updateDeltaTarget();
    if ((!inbacklashAxis1) && (wasInbacklashAxis1) && (!guideDirAxis1))
    {
      
      cli();
      if (abs(deltaTargetAxis1) > 2)
        thisTimerRateAxis1 = TakeupRate;
      else
        wasInbacklashAxis1 = false;
      sei();
    }
    if ((!inbacklashAxis2) && (wasInbacklashAxis2) && (!guideDirAxis2))
    {
      cli();
      if (abs(deltaTargetAxis2) > 2)
        thisTimerRateAxis2 = TakeupRate;
      else
        wasInbacklashAxis2 = false;
      sei();
    }
  }
  if (trackingState == TrackingMoveTo)
  {
    // trigger Goto step mode, rapid acceleration (low StepsForRateChange) can leave too little time
    // until the home position arrives to actually switch to tracking micro-step mode. the larger step size
    // then causes backlash compensation to activate which in-turn keeps goto micro-step mode from turning off
#if defined(AXIS2_MODE) && defined(AXIS2_MODE_GOTO)
    gotoRateAxis2 = (thisTimerRateAxis2 < 128 * 16L);   // activate <128us rate
#endif
#if defined(AXIS1_MODE) && defined(AXIS1_MODE_GOTO)
    gotoRateAxis1 = (thisTimerRateAxis1 < 128 * 16L);   // activate <128us rate
#endif
  }

    // set the rates
  if (thisTimerRateAxis1 != isrTimerRateAxis1)
  {
    Timer3SetRate(thisTimerRateAxis1 / pps.m_rateRatio);
    isrTimerRateAxis1 = thisTimerRateAxis1;
  }
  if (thisTimerRateAxis2 != isrTimerRateAxis2)
  {
    Timer4SetRate(thisTimerRateAxis2 / pps.m_rateRatio);
    isrTimerRateAxis2 = thisTimerRateAxis2;
  }
}
ISR(TIMER3_COMPA_vect)
{

  // drivers step on the rising edge, need >=1.9uS to settle (for DRV8825 or A4988) so this is early in the routine
#if defined(__arm__) && defined(TEENSYDUINO)
  digitalWriteFast(Axis1StepPin, LOW);
#else
  CLR(Axis1StepPORT, Axis1StepBit);
#endif
#if (defined(__arm__) && defined(TEENSYDUINO)) 

  // on the much faster Teensy run this ISR at twice the normal rate and pull the step pin low every other call
  if (clearAxis1)
  {
    takeStepAxis1 = false;
#endif
#if defined(AXIS1_MODE) && defined(AXIS1_MODE_GOTO) && !defined(MODE_SWITCH_BEFORE_SLEW_ON)
    // switch micro-step mode
    if (gotoModeAxis1 != gotoRateAxis1)
    {
      // only when at the home position
      if (((posAxis1 + blAxis1) - trueAxis1) % 256 == 0)
      {                   // was 1024 in support of 256x drivers... if they work like the DRV8825, A4988 this should still be ok
          // switch mode
        if (gotoModeAxis1)
        {
          stepAxis1 = 1;
          modeAxis1_next = AXIS1_MODE;
          gotoModeAxis1 = false;
        }
        else
        {
          stepAxis1 = AXIS1_STEP_GOTO;
          modeAxis1_next = AXIS1_MODE_GOTO;
          gotoModeAxis1 = true;
        }

        digitalWrite(Axis1_M0, (modeAxis1_next & 1));
        digitalWrite(Axis1_M1, (modeAxis1_next >> 1 & 1));
        digitalWrite(Axis1_M2, (modeAxis1_next >> 2 & 1));
      }
    }
#endif


    // Guessing about 4+4+1+ 4+4+1+ 1+ 2+1+2+ 13=37 clocks between here and the step signal which is 2.3uS
    updateDeltaTarget();
    if (deltaTargetAxis1 !=0)
    {                       // Move the RA stepper to the target

      if ( 0 < deltaTargetAxis1)
      {
        //dirAxis1 = 1;
        //else
        //dirAxis1 = 0;   // Direction control
        if (deltaTargetAxis1 <= halfRotAxis1)
        {
          dirAxis1 = 1;
        }
        else
        {
          dirAxis1 = 0;
        }
      }
      else
      {
        if (-deltaTargetAxis1 <= halfRotAxis1)
        {
          dirAxis1 = 0;
        }
        else
        {
          dirAxis1 = 1;
        }

      }
      

    // Guessing about 1+2+1+4+4+1=13 clocks between here and the step signal which is 0.81uS
    // Set direction.  Needs >=0.65uS before/after rising step signal (DRV8825 or A4988).

      if (ReverseAxis1)
      {
        if (HADir == dirAxis1)
          digitalWriteFast(Axis1DirPin, LOW);
        else
          digitalWriteFast(Axis1DirPin, HIGH);
      }
      else
      {
        if (HADir == dirAxis1)

          digitalWriteFast(Axis1DirPin, HIGH);
        else
          digitalWriteFast(Axis1DirPin, LOW);
      }

      // telescope moves WEST with the sky, blAxis1 is the amount of EAST backlash
      if (dirAxis1 == 1)
      {
        if (blAxis1 < backlashAxis1)
        {
          blAxis1 += stepAxis1;
          inbacklashAxis1 = true;
        }
        else
        {
          inbacklashAxis1 = false;
          posAxis1 += stepAxis1;
        }
      }
      else
      {
        if (blAxis1 > 0)
        {
          blAxis1 -= stepAxis1;
          inbacklashAxis1 = true;
        }
        else
        {
          inbacklashAxis1 = false;
          posAxis1 -= stepAxis1;
        }
      }

#if (defined(__arm__) && defined(TEENSYDUINO))
      takeStepAxis1 = true;
    }

    clearAxis1 = false;
  }
  else
  {
    if (takeStepAxis1)
    {
#if defined(__arm__) && defined(TEENSYDUINO)
      digitalWriteFast(Axis1StepPin, HIGH);
#else
      SET(Axis1StepPORT, Axis1StepBit);
#endif
    }

    clearAxis1 = true;

#if defined(__arm__) && defined(TEENSYDUINO)
    PIT_LDVAL1 = nextAxis1Rate * stepAxis1;
#endif
  }

#else
      SET(Axis1StepPORT, Axis1StepBit);
}
#endif
}
  ISR(TIMER4_COMPA_vect)
  {
#if defined(__arm__) && defined(TEENSYDUINO)
    digitalWriteFast(Axis2StepPin, LOW);
#else
    // drivers step on the rising edge
    CLR(Axis2StepPORT, Axis2StepBit);
#endif
#if (defined(__arm__) && defined(TEENSYDUINO))
    // on the much faster Teensy and Tiva TM4C run this ISR at twice the normal rate and pull the step pin low every other call
    if (clearAxis2)
    {
      takeStepAxis2 = false;
#endif
#if defined(AXIS2_MODE) && defined(AXIS2_MODE_GOTO) && !defined(MODE_SWITCH_BEFORE_SLEW_ON)
      // switch micro-step mode
      if (gotoModeAxis2 != gotoRateAxis2)
      {
        // only when at home position
        if (((posAxis2 + blAxis2) - trueAxis2) % 256 == 0)
        {                   // was 1024 in support of 256x drivers... if they work like the DRV8825, A4988 this should still be ok
            // switch mode
          if (gotoModeAxis2)
          {
            stepAxis2 = 1;
            modeAxis2_next = AXIS2_MODE;
            gotoModeAxis2 = false;
          }
          else
          {
            stepAxis2 = AXIS2_STEP_GOTO;
            modeAxis2_next = AXIS2_MODE_GOTO;
            gotoModeAxis2 = true;
          }

          digitalWrite(Axis2_M0, (modeAxis2_next & 1));
          digitalWrite(Axis2_M1, (modeAxis2_next >> 1 & 1));
          digitalWrite(Axis2_M2, (modeAxis2_next >> 2 & 1));
        }
      }
#endif
#if defined(__AVR_ATmega2560__)
      OCR4A = nextAxis2Rate * stepAxis2;
#endif
      updateDeltaTarget();
      if (deltaTargetAxis2 !=0 )
      {                       // move the Dec stepper to the target
          // telescope normally starts on the EAST side of the pier looking at the WEST sky
        if (0 < deltaTargetAxis2)
          dirAxis2 = 1;
        else
          dirAxis2 = 0;   // Direction control

      // Set direction.  Needs >=0.65uS before/after rising step signal (DRV8825 or A4988).

        if (ReverseAxis2)
        {
          if (DecDir == dirAxis2)
            digitalWriteFast(Axis2DirPin, LOW);
          else
            digitalWriteFast(Axis2DirPin, HIGH);
        }
        else
        {
          if (DecDir == dirAxis2)
            digitalWriteFast(Axis2DirPin, HIGH);
          else
            digitalWriteFast(Axis2DirPin, LOW);
        }

        // telescope moving toward celestial pole in the sky, blAxis2 is the amount of opposite backlash
        if (dirAxis2 == 1)
        {
          if (blAxis2 < backlashAxis2)
          {
            blAxis2 += stepAxis2;
            inbacklashAxis2 = true;
          }
          else
          {
            inbacklashAxis2 = false;
            posAxis2 += stepAxis2;
          }
        }
        else
        {
          if (blAxis2 > 0)
          {
            blAxis2 -= stepAxis2;
            inbacklashAxis2 = true;
          }
          else
          {
            inbacklashAxis2 = false;
            posAxis2 -= stepAxis2;
          }
        }

#if (defined(__arm__) && defined(TEENSYDUINO))
        takeStepAxis2 = true;
      }

      clearAxis2 = false;
    }
    else
    {
      if (takeStepAxis2)
      {
#if defined(__arm__) && defined(TEENSYDUINO)
        digitalWriteFast(Axis2StepPin, HIGH);
#else
        SET(Axis2StepPORT, Axis2StepBit);
#endif
      }

      clearAxis2 = true;

#if defined(__arm__) && defined(TEENSYDUINO)
      PIT_LDVAL2 = nextAxis2Rate * stepAxis2;
#endif
    }

#else
        SET(Axis2StepPORT, Axis2StepBit);
  }
#endif
  }
#ifdef PPS_SENSE_ON

    // PPS interrupt
    void ClockSync()
    {
      unsigned long   t = micros();
      unsigned long   oneS = (t - PPSlastMicroS);
      if ((oneS > 1000000 - 1000) && (oneS < 1000000 + 1000))
      {
        PPSavgMicroS = (PPSavgMicroS * 19 + oneS) / 20;
        PPSsynced = true;
      }

      PPSlastMicroS = t;
    }
#endif

