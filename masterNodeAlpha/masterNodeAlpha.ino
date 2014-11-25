const long frameDuration     = 420000; // in milliseconds, i.e. 7 minutes
const int framesPerAct       = 9;      // number of "frames" in each "act" of composition ("act" is 1/3 of composition)
const int actsPerComposition = 3;      // number of "acts" in the composition
const int ruleDuration       = 5000;   // in milliseconds
const int playbackBuffer     = 100;    // in milliseconds; the lead time before a note is due to be played which, when entered, Arduino focuses only on preparing to play that note (i.e. ignores Serial Port buffer)
const int numberOfRules      = 50;

int maxFrameTraffic          = 200;    // rough estimate; will be dynamically adjusted as max is exceeded

int lastAccessedFrame        = 0;      // used in determination of frame change

long actDuration;
long compositionDuration;
long superCompositionDuration;
int framesPerComposition;

// 4-act long array of counts
int superFrameArray[framesPerAct * (actsPerComposition + 1)];

boolean debugMode = true;

void setup()
{
  compositionDuration = (long) framesPerAct * actsPerComposition * ruleDuration;
  actDuration = frameDuration * (long) framesPerAct;
  superCompositionDuration = actDuration * (actsPerComposition + 1);
  framesPerComposition = framesPerAct * actsPerComposition;

  // initialise superFrameArray (i.e. the traffic count bank) with -1 to indicate lack of sensor data
  for(int i = 0; i < (framesPerAct * (actsPerComposition + 1)); i++)
  {
    superFrameArray[i] = -1;
  }

  // initialise Serial Port at baud rate of 9600 bps
  Serial.begin(9600);
  
  // some debugging checks
  if (debugMode == true)
  {
    Serial.print("compositionDuration: ");
    Serial.println(compositionDuration);
    Serial.print("actDuration: ");
    Serial.println(actDuration);
    Serial.print("superCompositionDuration: ");
    Serial.println(superCompositionDuration);
    Serial.print("framesPerComposition: ");
    Serial.println(framesPerComposition);
    Serial.println("...");
  }
}

void loop()
{
  concentrateOnMusic();
  serviceSerial();
}
