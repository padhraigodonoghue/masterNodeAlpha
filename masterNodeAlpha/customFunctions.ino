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
        Serial.print("lookAheadMillis: ");
        Serial.println(lookAheadMillis);
      }

      // call the function that executes a delay before sending rule number over Serial Port
      playNote(waitTime, ruleNumber);
    }
  }
}

// compensates for rolling index of the superFrameArray (which has an extra act for ... no good reason, now that I think about it)
// leaving it in for now
int lookUp(int previousFrameNumberIn, int noteNumberIn)
{
  int moduloDividend = previousFrameNumberIn + (framesPerAct + 1) + noteNumberIn;
  int moduloDivisor  = framesPerAct * (actsPerComposition + 1);

  int indexOut =  moduloDividend % moduloDivisor;

  // some debugging checks
  if (debugMode == true)
  {
    Serial.print("moduloDividend: ");
    Serial.println(moduloDividend);
    Serial.print("moduloDivisor: ");
    Serial.println(moduloDivisor);
    Serial.print("indexOut: ");
    Serial.println(indexOut);
    Serial.println("...");
  }

  return indexOut;
}

int countConversion(int index)
{
  int ruleOut;

  int binCount = superFrameArray[index];

  // high-water-mark approach to calibrating reference maximum
  if (binCount > maxFrameTraffic)
  {
    maxFrameTraffic = binCount;
  }

  // cast int data types to float for more precise rationing
  float floatyBinCount        = (float) binCount;
  float floatyMaxFrameTraffic = (float) maxFrameTraffic;

  float ratio  = floatyBinCount / floatyMaxFrameTraffic;
  float scaled = ratio * (float) (numberOfRules - 1);

  ruleOut = round(scaled);

  // some debugging checks
  if (debugMode == true)
  {
    Serial.print("floatyBinCount: ");
    Serial.println(floatyBinCount);
    Serial.print("floatyMaxFrameTraffic: ");
    Serial.println(floatyMaxFrameTraffic);
    Serial.print("ratio: ");
    Serial.println(ratio);
    Serial.print("scaled: ");
    Serial.println(scaled);
    Serial.print("ruleOut: ");
    Serial.println(ruleOut);
    Serial.println("...");
  }

  return ruleOut;
}

// executes a delay before sending rule number over Serial Port
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
    incrementTrafficCount();
  }
}

void incrementTrafficCount()
{
  // increment

  // look only at window of time equivalent to full composition plus 1/3, i.e. 4 "acts"
  long superCompositionWrapTime = (millis() % (actDuration * (actsPerComposition + 1)));

  // should always round down to current frame
  int whichBin = (superCompositionWrapTime / frameDuration);

  // some debugging checks
  if (debugMode == true)
  {
    Serial.print("lastAccessedFrame: ");
    Serial.println(lastAccessedFrame);
    Serial.print("whichBin: ");
    Serial.println(whichBin);
  }
  
  if (whichBin != lastAccessedFrame)
  {
    // reset count in this bin to zero
    superFrameArray[whichBin] = 0;

    // update lastAccessedFrame variable
    lastAccessedFrame = whichBin;
  }

  superFrameArray[whichBin]++;
    
  // some debugging checks
  if (debugMode == true)
  {
    Serial.print("superFrameArray[");
    Serial.print(whichBin);
    Serial.print("]: ");
    Serial.println(superFrameArray[whichBin]);
  }
}
