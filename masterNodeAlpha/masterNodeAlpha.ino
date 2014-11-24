const int frameDuration      = 420000; // in milliseconds, i.e. 7 minutes
const int framesPerAct       = 9;      // number of "frames" in each "act" of composition ("act" is 1/3 of composition)
const int actsPerComposition = 3;      // number of "acts" in the composition
const int ruleDuration       = 5000;   // in milliseconds
const int playbackBuffer     = 100;    // in milliseconds

int maxFrameTraffic          = 200;    // rough estimate; will be dynamically adjusted as max is exceeded

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

// get into the groove, etc.
void concentrateOnMusic()
{
  long lookAheadMillis = millis() + playbackBuffer;
  
  // is the current time within the playback zone?
  if ((lookAheadMillis % actDuration) < compositionDuration)
  {
    // is the current time within the leadtime of the next note?
    // (i.e. is it between time of next note minus buffer to time of next note?)
    if ((lookAheadMillis % ruleDuration) < playbackBuffer)
    {
      // determine which note of the n-note composition is imminent
      // n = (framesPerAct * actsPerComposition), i.e. 27-note
      int noteNumber = (lookAheadMillis % actDuration) / ruleDuration;
      
      // determine the previousFrameNumber
      // consider only modulus of superCompositionDuration and divide by frameDuration
      // (should only ever be equal to [(n * 9) - 1], where n ∈ N < (actsPerComposition + 1 ... because of first if condition)
      int previousFrameNumber = (lookAheadMillis % superCompositionDuration) / frameDuration;
      
      // lookup superFrameArray (i.e. the traffic count bank) for current index corresponding to noteNumber
      // (index shifts dynamically, hence requires lookup with reference to current time and previousFrameNumber)
      int superArrayAddress = lookUp(previousFrameNumber, noteNumber);
      
      // derive ruleNumber by comparing to a reference maximum; then determine necessary wait time before rule should be sent
      int ruleNumber = countConversion(superArrayAddress);
      int waitTime   = (millis() + playbackBuffer) % ruleDuration;
      
      // some debugging checks
      if (debugMode == true)
      {
        print("lookAheadMillis: ");
        println(lookAheadMillis);
      }
      
      // call the function that executes a delay and send rule number over Serial Port
      playNote(waitTime, ruleNumber);
    }
}

// compensates for rolling index of the superFrameArray (which has an extra act for ... no good reason, now that I think about it)
int lookUp(int previousFrameNumberIn, int noteNumberIn)
{
  int moduloDividend = previousFrameNumberIn + (framesPerAct + 1) + noteNumberIn;
  int moduloDivisor  = framesPerAct * (actsPerComposition + 1);
  
  int indexOut =  moduloDividend % moduloDivisor;
  
  return indexOut;
}

int countConversion(int index)
{
  int ruleOut = 
  
  return ruleOut;
}

void playNote(int waitTimeIn, int ruleNumberIn)
{
  delay(waitTimeIn);
//  Serial.println(ruleNumber);
  Serial.write(ruleNumberIn);
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

