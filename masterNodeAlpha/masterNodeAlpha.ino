const int frameDuration      = 420000; // in milliseconds, i.e. 7 minutes
const int framesPerAct       = 9;      // number of "frames" in each "act" of composition ("act" is 1/3 of composition)
const int actsPerComposition = 3;      // number of "acts" in the composition
const int ruleDuration       = 5000;   // in milliseconds
const int playbackBuffer     = 100;    // in milliseconds
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
  actDuration = frameDuration * framesPerAct;
  compositionDuration = actDuration * actsPerComposition;
  superCompositionDuration = actDuration * (actsPerComposition + 1);
  framesPerComposition = framesPerAct * actsPerComposition;

  // initialise superFrameArray (i.e. the traffic count bank) with -1 to indicate lack of sensor data
  for(int i = 0; i < (framesPerAct * (actsPerComposition + 1)); i++)
  {
    superFrameArray[i] = -1;
  }

  // initialise Serial Port at baud rate of 9600 bps
  Serial.begin(9600);
}

void loop()
{
  concentrateOnMusic();
  serviceSerial();
}
