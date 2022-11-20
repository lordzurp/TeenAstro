// -----------------------------------------------------------------------------------
// Timers and interrupt handling
#define stepAxis   1

#define ISR(f)  void f (void)
void TIMER1_COMPA_vect(void);
void TIMER3_COMPA_vect(void);
void TIMER4_COMPA_vect(void);

static IntervalTimer  itimer1;
static IntervalTimer  itimer3;
static IntervalTimer  itimer4;

static volatile double isrIntervalAxis1 = 0;
static volatile double isrIntervalAxis2 = 0;

speed interval2speed(interval i) //Speed in step per second
{
  return masterClockSpeed / i;
}
interval speed2interval(speed V, speed minV)
{
  if (V == 0)
  {
    return minV;
  }
  return min(masterClockSpeed / V, minV);
}


// set the master sidereal clock rate, also forces rate update for RA/Dec timer rates so that PPS adjustments take hold immediately
void SetsiderealClockSpeed(double cs)
{
  Timer1SetInterval(cs / 100);
  isrIntervalAxis1 = 0;
  isrIntervalAxis2 = 0;
}
void beginTimers()
{
  // set the system timer for millis() to the second highest priority
  SCB_SHPR3 = (32 << 24) | (SCB_SHPR3 & 0x00FFFFFF);
  itimer3.begin(TIMER3_COMPA_vect, 100);
  itimer4.begin(TIMER4_COMPA_vect, 100);
  // set the 1/100 second sidereal clock timer to run at the second highest priority
  NVIC_SET_PRIORITY(IRQ_PIT_CH0, 32);

  // set the motor timers to run at the highest priority
  NVIC_SET_PRIORITY(IRQ_PIT_CH1, 0);
  NVIC_SET_PRIORITY(IRQ_PIT_CH2, 0);
}
// set timer1 to interval (in microseconds)
static void Timer1SetInterval(interval i)
{
  itimer1.begin(TIMER1_COMPA_vect, i );
}

// set timer3 to interval (in microseconds)
static volatile uint32_t   nextIntervalAxis1 = 100000UL;


static void Timer3SetInterval(interval i)
{
  cli();
  nextIntervalAxis1 = (F_BUS / masterClockSpeed) * i * 0.5 - 1;
  sei();
}

// set timer4 to interval (in microseconds)
static volatile uint32_t   nextIntervalAxis2 = 100000UL;
static void Timer4SetInterval(interval i)
{
  cli();
  nextIntervalAxis2 = (F_BUS / masterClockSpeed) * i * 0.5 - 1;
  sei();
}


ISR(TIMER1_COMPA_vect)
{
  static volatile bool   wasInbacklashAxis1 = false;
  static volatile bool   wasInbacklashAxis2 = false;
  static volatile double tmp_guideRateAxisA1 = 0;
  static volatile double tmp_1 = 0;
  static volatile double tmp_2 = 0;
  static volatile double tmp_guideRateAxisA2 = 0;

  static volatile double sign;
  rtk.m_lst++;
  // in this mode the target is always a bit faster than the scope because we move first the target!!
  if (!movingTo)
  {
    double max_guideRate = 16;
    // guide rate acceleration/deceleration
    {
      // guide rate acceleration/deceleration and control
      updateDeltaTarget();
      if (!backlashA1.correcting && guideA1.dir)
      {
        if ((fabs(guideA1.atRate) < max_guideRate) &&
          (fabs(tmp_guideRateAxisA1) < max_guideRate))
        {
          // slow speed guiding, no acceleration
          tmp_guideRateAxisA1 = guideA1.atRate;
        }
        else
        {
          DecayModeGoto();
          // for acceleration, we know run this routine this a fix amount of time 1/100 of a sideral second
          sign = guideA1.atRate < 0 ? -1 : 1;
          if (guideA1.dir == 'b')
          {      
            tmp_1 = tmp_guideRateAxisA1 - 2 * staA1.acc * 0.01 * sign / geoA1.stepsPerSecond;
            tmp_2 = sqrt(fabs(staA1.deltaTarget) * 4 * staA1.acc) * sign / geoA1.stepsPerSecond ;
            tmp_guideRateAxisA1 = guideA1.atRate < 0 ?
              max(tmp_1, tmp_2) :
              min(tmp_1, tmp_2);
 
          } 
          else
          {
            tmp_guideRateAxisA1 += 2 * staA1.acc * 0.01 * sign / geoA1.stepsPerSecond;
          }
          if (tmp_guideRateAxisA1 < 0)
          {
            tmp_guideRateAxisA1 = min(-max_guideRate, tmp_guideRateAxisA1);
            tmp_guideRateAxisA1 = max(guideA1.atRate, tmp_guideRateAxisA1);           
          }
          else
          {
            tmp_guideRateAxisA1 = max(max_guideRate, tmp_guideRateAxisA1);
            tmp_guideRateAxisA1 = min(guideA1.atRate, tmp_guideRateAxisA1);
          }
        }

        // stop guiding
        if (guideA1.dir == 'b')
        {
          if (staA1.atTarget(false))
          {
            guideA1.dir = 0;
            guideA1.atRate = 0;
            tmp_guideRateAxisA1 = 0;
            if (staA2.atTarget(false))
              DecayModeTracking();
          }
        }
      }
      // compute timerRateAxis  and avoid timerRate  to be extremly large
      volatile double timerRateAxis1 = fabs(tmp_guideRateAxisA1 + staA1.CurrentTrackingRate);
      staA1.interval_Step_Cur = max(staA1.interval_Step_Sid / timerRateAxis1, minInterval1);
      staA1.interval_Step_Cur = min(staA1.interval_Step_Cur, maxInterval1);
    }

    {

      updateDeltaTarget();
      if (!backlashA2.correcting && guideA2.dir)
      {
        if ((fabs(guideA2.atRate) < max_guideRate) &&
          (fabs(tmp_guideRateAxisA2) < max_guideRate))
        {
          // slow speed guiding, no acceleration
          tmp_guideRateAxisA2 = guideA2.atRate;
        }
        else
        {
          DecayModeGoto();
          // for acceleration, we know run this routine this a fix amount of time 1/100 of a sideral second
          sign = guideA2.atRate < 0 ? -1 : 1;
          if (guideA2.dir == 'b')
          {
            tmp_1 = tmp_guideRateAxisA2 - 2 * staA2.acc * 0.01 * sign / geoA2.stepsPerSecond;
            tmp_2 = sqrt(fabs(staA2.deltaTarget) * 4 * staA2.acc) * sign / geoA2.stepsPerSecond;
            tmp_guideRateAxisA2 = guideA2.atRate < 0 ?
              max(tmp_1, tmp_2) :
              min(tmp_1, tmp_2);
          }
          else
          {
            tmp_guideRateAxisA2 += 2 * staA2.acc * 0.01 * sign / geoA2.stepsPerSecond;
          }
          if (tmp_guideRateAxisA2 < 0)
          {
            tmp_guideRateAxisA2 = min(-max_guideRate, tmp_guideRateAxisA2);
            tmp_guideRateAxisA2 = max(guideA2.atRate, tmp_guideRateAxisA2);
          }
          else
          {
            tmp_guideRateAxisA2 = max(max_guideRate, tmp_guideRateAxisA2);
            tmp_guideRateAxisA2 = min(guideA2.atRate, tmp_guideRateAxisA2);
          }

        }

        // stop guiding
        if (guideA2.dir == 'b')
        {
          if (staA2.atTarget(false))
          {
            guideA2.dir = 0;
            guideA2.atRate = 0;
            tmp_guideRateAxisA2 = 0;
            if (staA1.atTarget(false))
              DecayModeTracking();
          }
        }
      }
      // compute timerRateAxis  and avoid timerRate  to be extremly large
      volatile double timerRateAxis2 = fabs(tmp_guideRateAxisA2 + staA2.CurrentTrackingRate);
      staA2.interval_Step_Cur = max(staA2.interval_Step_Sid / timerRateAxis2, minInterval2);
      staA2.interval_Step_Cur = min(staA2.interval_Step_Cur, maxInterval2);
    }

    if (!guideA1.dir && !guideA2.dir)
    {
      GuidingState = GuidingOFF;
    }
  }

  volatile double thisIntervalAxis1 = staA1.interval_Step_Cur;
  volatile double thisIntervalAxis2 = staA2.interval_Step_Cur;

  // override rate during backlash compensation
  if (backlashA1.correcting)
  {
    thisIntervalAxis1 = backlashA1.interval_Step;
    wasInbacklashAxis1 = true;
  }

  // override rate during backlash compensation
  if (backlashA2.correcting)
  {
    thisIntervalAxis2 = backlashA2.interval_Step;
    wasInbacklashAxis2 = true;
  }
  if (sideralTracking && !movingTo)
  {
    // travel through the backlash is done, but we weren't following the target while it was happening!
    // so now get us back to near where we need to be
    if (!backlashA1.correcting && wasInbacklashAxis1 && !guideA1.dir)
    {
      if (!staA1.atTarget(true))
      {
        cli();
        thisIntervalAxis1 = staA1.takeupInterval;
        sei();
      }
      else
      {
        cli();
        wasInbacklashAxis1 = false;
        sei();
      }
    }
    if (!backlashA2.correcting && wasInbacklashAxis2 && !guideA2.dir)
    {
      if (!staA2.atTarget(true))
      {
        cli();
        thisIntervalAxis2 = staA2.takeupInterval;
        sei();
      }
      else
      {
        cli();
        wasInbacklashAxis2 = false;
        sei();
      }
    }
  }
  // set the rates
  if (thisIntervalAxis1 != isrIntervalAxis1)
  {
    Timer3SetInterval(thisIntervalAxis1);
    isrIntervalAxis1 = thisIntervalAxis1;
  }
  if (thisIntervalAxis2 != isrIntervalAxis2)
  {
    Timer4SetInterval(thisIntervalAxis2);
    isrIntervalAxis2 = thisIntervalAxis2;
  }
}
ISR(TIMER3_COMPA_vect)
{
  static volatile bool clearAxis1 = true;
  static volatile bool takeStepAxis1 = false;
  digitalWriteFast(Axis1StepPin, LOW);
  if (clearAxis1)
  {
    takeStepAxis1 = false;
    staA1.updateDeltaTarget();
    if (staA1.deltaTarget != 0 || backlashA1.correcting)
    {
      // Move the RA stepper to the target
      staA1.dir = 0 < staA1.deltaTarget;
      // Direction control
      if (motorA1.reverse ^ Axis1Reverse)
      {
        digitalWriteFast(Axis1DirPin, HADir != staA1.dir);
      }
      else
      {
        digitalWriteFast(Axis1DirPin, HADir == staA1.dir);
      }

      // telescope moves WEST with the sky, blAxis1 is the amount of EAST backlash
      if (staA1.dir)
      {
        if (backlashA1.movedSteps < backlashA1.inSteps)
        {
          backlashA1.movedSteps += stepAxis;
          backlashA1.correcting = true;
        }
        else
        {
          backlashA1.correcting = false;
          staA1.pos += stepAxis;
        }
      }
      else
      {
        if (backlashA1.movedSteps > 0)
        {
          backlashA1.movedSteps -= stepAxis;
          backlashA1.correcting = true;
        }
        else
        {
          backlashA1.correcting = false;
          staA1.pos -= stepAxis;
        }
      }
      takeStepAxis1 = true;
    }
    clearAxis1 = false;
  }
  else
  {
    if (takeStepAxis1)
    {
      digitalWriteFast(Axis1StepPin, HIGH);
    }
    clearAxis1 = true;
    PIT_LDVAL1 = nextIntervalAxis1 * stepAxis;
  }
}
ISR(TIMER4_COMPA_vect)
{
  static volatile bool clearAxis2 = true;
  static volatile bool takeStepAxis2 = false;
  digitalWriteFast(Axis2StepPin, LOW);
  // on the much faster Teensy and Tiva TM4C run this ISR at twice the normal rate and pull the step pin low every other call
  if (clearAxis2)
  {
    takeStepAxis2 = false;
    staA2.updateDeltaTarget();
    if (staA2.deltaTarget != 0 || backlashA2.correcting)
    {
      // move the Dec stepper to the target
      // telescope normally starts on the EAST side of the pier looking at the WEST sky
      staA2.dir = 0 < staA2.deltaTarget;
      // Direction control
      if (motorA2.reverse ^ Axis2Reverse)
      {
        digitalWriteFast(Axis2DirPin, !staA2.dir);
      }
      else
      {
        digitalWriteFast(Axis2DirPin, staA2.dir);
      }

      // telescope moving toward celestial pole in the sky, blAxis2 is the amount of opposite backlash
      if (staA2.dir)
      {
        if (backlashA2.movedSteps < backlashA2.inSteps)
        {
          backlashA2.movedSteps += stepAxis;
          backlashA2.correcting = true;
        }
        else
        {
          backlashA2.correcting = false;
          staA2.pos += stepAxis;
        }
      }
      else
      {
        if (backlashA2.movedSteps > 0)
        {
          backlashA2.movedSteps -= stepAxis;
          backlashA2.correcting = true;
        }
        else
        {
          backlashA2.correcting = false;
          staA2.pos -= stepAxis;
        }
      }
      takeStepAxis2 = true;
    }
    clearAxis2 = false;
  }
  else
  {
    if (takeStepAxis2)
    {
      digitalWriteFast(Axis2StepPin, HIGH);
    }
    clearAxis2 = true;
    PIT_LDVAL2 = nextIntervalAxis2 * stepAxis;
  }
}