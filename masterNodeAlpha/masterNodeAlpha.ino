const long frameDuration         = 40000; // in milliseconds, i.e. 7 minutes
const int framesPerAct           = 3;     // number of "frames" in each "act" of composition ("act" is 1/3 of composition)
const int actsPerComposition     = 2;     // number of "acts" in the composition
const int ruleDuration           = 5000;  // in milliseconds
const int playbackBuffer         = 100;   // in milliseconds; the lead time before a note is due to be played which, when entered, Arduino focuses only on preparing to play that note (i.e. ignores Serial Port buffer)
const int numberOfRules          = 50;

int maxFrameTraffic              = 10;    // rough estimate; will be dynamically adjusted when current max is exceeded

int lastAccessedFrame;                    // used in determination of "frame" change when writing to "frame" count
int lastFrameCheck               = 0;                      // used in determination of "frame" change when zeroing "frame" count

long actRealtimeDataDuration;             // in milliseconds; total duration of "act"
long compositionDuration;                 // in milliseconds; total duration of "composition"
long superRealtimeDataDuration;           // in milliseconds; total duration of composition plus extra act's worth of realtime data
int framesPerComposition;

boolean cobwebs                  = true; // for ensuring that zero activity frames are updated

// 4-act long array of counts
int superFrameArray[framesPerAct * (actsPerComposition + 1)];

boolean debugMode                = true;
boolean silentDebugMode          = false;  // for debugging without Serial Port
volatile int debugOutputPinState = LOW;
const int debugOutputPinNumber   = 13;

void setup()
{
  compositionDuration       = (long) framesPerAct * actsPerComposition * ruleDuration;
  actRealtimeDataDuration   = frameDuration * (long) framesPerAct;
  superRealtimeDataDuration = actRealtimeDataDuration * (actsPerComposition + 1);
  framesPerComposition      = framesPerAct * actsPerComposition;
  lastAccessedFrame         = (framesPerAct * (actsPerComposition + 1)) - 1;

  // initialise superFrameArray (i.e. the traffic count bank) with -1 to indicate lack of sensor data
  for(int i = 0; i < (framesPerAct * (actsPerComposition + 1)); i++)
  {
    superFrameArray[i] = 7;
  }

  // initialise Serial Port at baud rate of 9600 bps
  Serial.begin(9600);

  // some debugging checks
  if (debugMode == true)
  {
    Serial.print("compositionDuration: ");
    Serial.println(compositionDuration);
    Serial.print("actRealtimeDataDuration: ");
    Serial.println(actRealtimeDataDuration);
    Serial.print("superCompositionDuration: ");
    Serial.println(superRealtimeDataDuration);
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

  pinMode(debugOutputPinNumber, OUTPUT);

  if (silentDebugMode == true)
  {
    // set digital pin 3 to listen to a slave for testing without other electronics
    // source: http://arduino.cc/en/Reference/attachInterrupt
    attachInterrupt(1, blink, CHANGE);
  }
}

void loop()
{
  concentrateOnMusic();
  serviceSerial();

  if (silentDebugMode == true)
  {
    digitalWrite(debugOutputPinNumber, debugOutputPinState);
  }
  else
  {
    digitalWrite(debugOutputPinNumber, LOW);
  }

}

