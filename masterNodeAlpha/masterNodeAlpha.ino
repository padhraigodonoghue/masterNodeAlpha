const int frameDuration      = 420000; // in milliseconds, i.e. 7 minutes
const int framesPerAct       = 9;      // number of "frames" in each "act" of composition ("act" is 1/3 of composition)
const int actsPerComposition = 3;      // number of "acts" in the composition
const int ruleDuration       = 5000;   // in milliseconds
const int playbackBuffer     = 100;    // in milliseconds

int maxFrameTraffic          = 200;    // rough estimate; will be dynamically adjusted as max is exceeded

long actDuration;
int framesPerComposition;

// 4-act long array of counts
int superFrameArray[(framesPerAct * (actsPerComposition + 1))];

void setup()
{
  actDuration = frameDuration * framesPerAct;
  framesPerComposition = framesPerAct * actsPerComposition;

  for(int i = 0; i < (framesPerAct * (actsPerComposition + 1)); i++)
  {
    superFrameArray[i] = maxFrameTraffic;
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
  
  int whichBin = (superCompositionWrapTime / frameDuration);
  
  superFrameArray[whichBin]++;
}

void playComposition()
{
  // calculate delay
  // delay
  
  long compositionWrapTime = (millis() % (actDuration * (actsPerComposition)));
  
  int lastFrame = whatLastFrame();
  
  // play note, i.e. send rule
  playNote(rule);
}

void whatLastFrame()
{
  
}

void playNote(int ruleNumber)
{
//  Serial.println(ruleNumber);
  Serial.write(ruleNumber);
}

