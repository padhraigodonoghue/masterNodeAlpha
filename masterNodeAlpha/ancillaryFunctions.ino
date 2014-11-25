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
    Serial.print("[rolling index status] indexOut: ");
    Serial.print(indexOut);
    Serial.print(" ### (moduloDividend: ");
    Serial.print(moduloDividend);
    Serial.print(" ### moduloDivisor: ");
    Serial.print(moduloDivisor);
    Serial.println(")");
  }

  return indexOut;
}

// derive ruleNumber by comparing count of relevant bin (i.e. frame) to a reference maximum
int countConversion(int index)
{
  int ruleOut;

  int binCount = superFrameArray[index];

  if (binCount != -1)
  {

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
      Serial.print(" ### floatyBinCount: ");
      Serial.print(floatyBinCount);
      Serial.print(" ### floatyMaxFrameTraffic: ");
      Serial.print(floatyMaxFrameTraffic);
      Serial.print(" ### ratio: ");
      Serial.print(ratio);
      Serial.print(" ### scaled: ");
      Serial.println(scaled);
    }
  }
  else
  {
    ruleOut = 255;
  }

  /**
   * // some debugging checks
   * if (debugMode == true)
   * {
   * Serial.print("ruleOut: ");
   * Serial.println(ruleOut);
   * }
   **/

  return ruleOut;
}

// executes a delay before sending rule number over Serial Port
void playNote(int waitTimeIn, int ruleNumberIn)
{
  delay(waitTimeIn);

  // some debugging checks
  if (debugMode == true)
  {
    Serial.print("SENDING RULE (ruleNumberIn): ");
    Serial.println(ruleNumberIn);
    Serial.println("");
  }
  else
  {
    Serial.write(ruleNumberIn);
  }
}

void incrementTrafficCount()
{
  // look only at window of time equivalent to full composition plus 1/3, i.e. 4 "acts"
  long superCompositionWrapTime = (millis() % (actRealtimeDataDuration * (actsPerComposition + 1)));

  // should always round down to current frame
  int whichBin = (superCompositionWrapTime / frameDuration);

  // some debugging checks
  if (debugMode == true)
  {
    Serial.print("lastAccessedFrame: ");
    Serial.print(lastAccessedFrame);
    Serial.print(" ### whichBin: ");
    Serial.print(whichBin);
  }

  if (whichBin != lastAccessedFrame)
  {
    // reset count in this bin to zero
    superFrameArray[whichBin] = 0;

    // update lastAccessedFrame variable
    lastAccessedFrame = whichBin;
  }

  superFrameArray[whichBin]++;
  cobwebs = false;

  // some debugging checks
  if (debugMode == true)
  {
    Serial.print(" ### superFrameArray[");
    Serial.print(whichBin);
    Serial.print("]: ");
    Serial.println(superFrameArray[whichBin]);
    Serial.println("");
  }
}

