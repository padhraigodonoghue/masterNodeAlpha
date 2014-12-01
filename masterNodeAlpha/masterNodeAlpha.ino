const long frameDuration          = 420000; // in milliseconds, i.e. 7 minutes; duration of real-time "frame"
const int framesPerAct            = 9;      // number of "frames" in each "act" of composition ("act" is 1/3 of composition)
const int actsPerComposition      = 3;      // number of "acts" in the composition
const int ruleDuration            = 5000;   // in milliseconds
const int playbackBuffer          = 80;     // in milliseconds; the lead-time before a note is due to be played; which, when entered, allows program to stall everything else and focus only on playing that note (i.e. ignore Serial Port buffer)
const int numberOfRules           = 50;

int maxFrameTraffic               = 120;    // rough estimate; will be dynamically adjusted when current max is exceeded
const int countLimit              = 800;    // if count exceeds this, something probably went wrong

int lastAccessedFrame;                      // used in determination of "frame" change

long actRealtimeDataDuration;               // in milliseconds; total duration of real-time "act"
long compositionDuration;                   // in milliseconds; total duration of "composition"
long masterWrapTime;                        // in milliseconds; total duration of "composition" plus extra "frame's" worth of real-time data
int framesPerComposition;
int superArrayLength;                       // for storing length of array without having to calculate it each time

// array of counts, 3-acts-and-1-frame in length
int superFrameArray[(framesPerAct * actsPerComposition) + 1];

const boolean debugMode           = false;  // toggle serial port debug messages
const boolean solenoidDebugMode   = false;  //
const boolean sensorMode          = true;   // toggle sensor functionality
const boolean dataTicking         = true;   // toggle solenoid "tick" sound on each sensor detection

boolean solenoidPressure          = false;  // state of solenoid; if true, solenoid is engaged and may require releasing
boolean solenoidCoil              = false;  // requested state of solenoid; if true, solenoid will need to be released
boolean solenoidDamp              = false;  // state of solenoid; if true, solenoid will need to be released after certain duration (i.e. solenoidDampDuration)
unsigned long struckAtTime;                 // store the time at which the solenoid was activated
unsigned long coiledAtTime;                 // store the time at which the solenoid was released in preparation for activation
unsigned long dampedAtTime;                 // store the time at which the solenoid was damped
const int solenoidHoldDuration    = 10;     // in milliseconds; duration for which solenoid is kept engaged
const int solenoidPreHoldDuration = 20;     // in milliseconds; duration for which solenoid is kept disengaged before engaging
const int solenoidDampDuration    = 1000;   // in milliseconds; duration for which solenoid is kept engaged
const int solenoidPin             = 10;     // pin for controlling voltage to gate of MOSTFET connected to solenoid
const int hardVelocity            = 255;    //
const int tickVelocity            = 255;    // 

const int selfSoundBuffer         = 50;
boolean rulePlayed                = false;
unsigned long rulePlayedAt;
unsigned long selfSoundFrequency  = 4;      // number of times master self-sounds per rule

/** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ **/
/** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ sensor code variables @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ **/
/** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ **/

const int sensorPin               = A0;     // main sensor pin input
const int ledPin                  = 13;     // assign output pin for visual feedback

boolean ledState                  = false;  // used in flashing pin 13 LED to indicate sensor calibration is taking place
unsigned long previousBlinkTime   = 0;

int sensorValue;                            // store last sensor reading

const boolean calibrateMode       = true;   // toggle sensor calibration during setup function
const boolean sensorReadDebugMode = false;  // toggle sensor value read out for debugging
boolean sensorStateChangeFlag     = false;  // to allow us to check previous state and run certain state-dependent functions
boolean calibrating               = false;  // state tracker
boolean bounceGuard               = false;

int sensorBaseLevel; // to hold min value from calibration

int indexForAverager              = 0;      // tracker for index of recent sensor values array
const int numOfReadsForAverager   = 32;     // length of array (i.e. sample size of averaging analysis); quartered when calibration is complete
int arrayOfVals[numOfReadsForAverager];     // array of sensor reads to be averaged
unsigned long calibrationStartTime;
unsigned long bounceFromTime;
unsigned long debouncingSince;
const int debounceDuration        = 60;     // in milliseconds; duration for which debouncing lasts

/** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ **/
/** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ program start @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ **/
/** @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ **/

void setup()
{
  // delay to allow all slaves to reach state of readiness
  delay(60000);
  
  compositionDuration             = ((long) framesPerAct * (long) actsPerComposition * (long) ruleDuration);
  actRealtimeDataDuration         = frameDuration * (long) framesPerAct;
  framesPerComposition            = framesPerAct * actsPerComposition;
  superArrayLength                = (framesPerAct * actsPerComposition) + 1;
  masterWrapTime                  = (long) superArrayLength * (long) frameDuration;
  lastAccessedFrame               = superArrayLength - 1; // initialise at end of array; monitoring/playing starts at 0

  // initialise superFrameArray (i.e. the traffic count bank) with -1 to indicate lack of sensor data
  for (int i = 0; i < superArrayLength; i++)
  {
    superFrameArray[i] = -1;
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

  // for sensor feedback
  pinMode(ledPin, OUTPUT);
  digitalWrite(solenoidPin, LOW);

  if (calibrateMode == true)
  {
    sensorCalibration();
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

  // depending on whether or not sensor monitoring is done on-board or externally...
  // poll either sensor or Serial Port for data
  if (sensorMode == true)
  {
    pollSensor();
  }
  else
  {
    serviceSerial();
  }
  
  // detects when a new "frame" has been entered
  frameTracker();
  
  // if true, solenoid is engaged and may require releasing
  if (solenoidPressure == true)
  {
    // some debugging checks
    if (solenoidDebugMode == true)
    {
      Serial.print("pressure's on...");
    }
    
    nonBlockingSolenoidRelease();
  }
  if (solenoidCoil == true)
  {
    // some debugging checks
    if (solenoidDebugMode == true)
    {
      Serial.println("coiling...");
    }
    
    nonBlockingSolenoidTick();
  }
  else if (solenoidDamp == true)
  {
    nonBlockingDamping();
  }
}
