const int frameDuration      = 420000; // in milliseconds, i.e. 7 minutes
const int framesPerAct       = 9;      // number of "frames" in each "act" of composition ("act" is 1/3 of composition)
const int actsPerComposition = 3;      // number of "acts" in the composition
const int ruleDuration       = 5000;   // in milliseconds
const int playbackBuffer     = 100;    // in milliseconds

int maxFrameTraffic          = 200;    // rough estimate; will be dynamically adjusted as max is exceeded

long actDuration;
int framesPerComposition;

int act0[framesPerAct];
int act1[framesPerAct];
int act2[framesPerAct];
int act3[framesPerAct];

// 2-dimensional array of counts
int superFrameArray[actsPerComposition + 1][framesPerAct];

void setup()
{
  actDuration = frameDuration * framesPerAct;
  framesPerComposition = framesPerAct * actsPerComposition;

  for(int i = 0; i < actsPerComposition; i++)
  {
    for (int j = 0; j < framesPerAct; j++)
    {
      superFrameArray[i][j]= maxFrameTraffic;
    }
  }
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
  long actWrapTime = millis() % actDuration;

  // what rule number is coming up next
  int ruleVicinity = (actWrapTime / ruleDuration);

  // if we are in range of playable rules
  if (ruleVicinity < framesPerComposition)
  {
    // check if we are within the playback buffer range of playing a note
    if ((actWrapTime % ruleDuration) > (ruleDuration - playbackBuffer))
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
  // increment
  
  // look only at window of time equivalent to full composition and 1/3, i.e. 4 "acts"
  long superCompositionWrapTime = (millis() % (actDuration * (actsPerComposition + 1)));
  
  int whichAct = (superCompositionWrapTime / actDuration;
  int whichBin = (superCompositionWrapTime / frameDuration);
  
  superFrameArray[whichAct][whichAct]++;
}

void playComposition()
{
  // calculate delay
  // delay
  
  long CompositionWrapTime = (millis() % (actDuration * (actsPerComposition)));
  
  // play note, i.e. send rule
  playNote(

}

