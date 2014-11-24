int frameDuration      = 420000; // in milliseconds, i.e. 7 minutes
int framesPerAct       = 9;      // number of "frames" in each "act" of composition ("act" is 1/3 of composition)
int actsPerComposition = 3;      // number of "acts" in the composition
int ruleDuration       = 5000    // in milliseconds
int playbackBuffer     = 100     // in milliseconds

long actDuration;
int framesPerComposition;

void setup()
{
  actDuration = frameDuration * framesPerAct;
  framesPerComposition = framesPerAct * actsPerComposition;
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
 long wrapTime = millis() % actDuration;
 
 // what rule number is coming up next
 int ruleVicinity = (wrapTime / ruleDuration);
 
 // if we are in range of playable rules
 if (ruleVicinity < framesPerComposition)
 {
   // check if we are within the playback buffer range of playing a note
   if ((wrapTime % ruleDuration) > (ruleDuration - playbackBuffer))
   {
     return false;
   }
 }
 else
 {
   return true;
 }
}

void serviceSerial()
{
  while(Serial.available())
  {
    // increment count on whatever frame for every byte in the serial port buffer
    incrementer();
  }
}

void incrementer()
{
  
}
