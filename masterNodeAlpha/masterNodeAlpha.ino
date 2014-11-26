const long frameDuration         = 10000; // in milliseconds, i.e. 7 minutes; duration of real-time "frame"
const int framesPerAct           = 9;      // number of "frames" in each "act" of composition ("act" is 1/3 of composition)
const int actsPerComposition     = 3;      // number of "acts" in the composition
const int ruleDuration           = 1000;   // in milliseconds
const int playbackBuffer         = 100;    // in milliseconds; the lead time before a note is due to be played which, when entered, Arduino focuses only on preparing to play that note (i.e. ignores Serial Port buffer)
const int numberOfRules          = 50;

int maxFrameTraffic              = 100;    // rough estimate; will be dynamically adjusted when current max is exceeded

int lastAccessedFrame;      // used in determination of "frame" change

long actRealtimeDataDuration;              // in milliseconds; total duration of real-time "act"
long compositionDuration;                  // in milliseconds; total duration of "composition"
long masterWrapTime;                       // in milliseconds; total duration of "composition" plus extra "frame's" worth of real-time data
int framesPerComposition;
int superArrayLength;                      // for storing length of array without having to calculate it each time

// array of counts, 3-acts-and-1-frame in length
int superFrameArray[(framesPerAct * actsPerComposition) + 1];

const boolean debugMode          = true;  // toggle serial port debug messages

void setup()
{
  compositionDuration            = (long) (framesPerAct * actsPerComposition * ruleDuration);
  actRealtimeDataDuration        = frameDuration * (long) framesPerAct;
  masterWrapTime                 = actRealtimeDataDuration * (long) (actsPerComposition + 1);
  framesPerComposition           = framesPerAct * actsPerComposition;
  superArrayLength               = (framesPerAct * actsPerComposition) + 1;
  lastAccessedFrame              = superArrayLength - 1;

  // initialise superFrameArray (i.e. the traffic count bank) with -1 to indicate lack of sensor data
  for(int i = 0; i < superArrayLength; i++)
  {
    superFrameArray[i] = 50;
  }

  /*
  initialise Serial Port at baud rate of 9600 bps
   (unless debug mode is on, in which case a higher baud rate is necessary...
   to prevent Serial.print function from becoming a spanner in the works)
   */
  if (debugMode == true)
  {
    Serial.begin(57600);
  }
  else
  {
    Serial.begin(9600);
  }

  // some debugging checks
  if (debugMode == true)
  {
    Serial.print("compositionDuration: ");
    Serial.println(compositionDuration);
    Serial.print("actRealtimeDataDuration: ");
    Serial.println(actRealtimeDataDuration);
    Serial.print("masterWrapTime: ");
    Serial.println(masterWrapTime);
    Serial.print("framesPerComposition: ");
    Serial.println(framesPerComposition);
    Serial.print("length of superFrameArray: ");
    Serial.println(sizeof(superFrameArray) / sizeof(int));
    Serial.println("...");
    Serial.println("SETUP COMPLETE!");
    Serial.print("current time: ");
    Serial.print(millis());
    Serial.println(" ms");
    Serial.println("");
  }
}

void loop()
{
  concentrateOnMusic();
  pollSensor();
}




