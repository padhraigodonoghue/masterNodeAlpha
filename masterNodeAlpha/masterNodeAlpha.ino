int frameDuration      = 4200; // in milliseconds, i.e. 7 minutes
int framesPerAct       = 9;    // number of "frames" in each "act" of composition ("act" is 1/3 of composition)
int actsPerComposition = 3;    // number of "acts" in the composition

void setup()
{
  
}

void loop()
{
  scheduler();
}

void scheduler()
{
  if (priorityServiceSerial() == true)
  {
    serviceSerial();
  }
  else
  {
    playComposition();
  }
}

// is it time to play anything or can serial port buffer be serviced?
boolean priorityServiceSerial()
{
  checkTime();
  
}

void checkTime()
{
 long currentTime = millis();
 
}
