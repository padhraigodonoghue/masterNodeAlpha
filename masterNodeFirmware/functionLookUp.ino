// compensates for rolling index of the superFrameArray
int lookUp(unsigned long timeIn)
{  
  // determine which note of the n-note composition is imminent...
  // where n = (framesPerAct * actsPerComposition), i.e. 27-note...
  // and number of first note is 0 and last note, 26
  int noteNumber = (int) ((timeIn % (unsigned long) actRealtimeDataDuration) / (unsigned long) ruleDuration);
  
  // step 1: calculate array index of currently active frame
  // step 2:
  int currentFrameIndex      = (int) ((timeIn % ((unsigned long) masterWrapTime)) / ((unsigned long) frameDuration));
  int actRelativeFrameNumber = (int) ((timeIn % ((unsigned long) actRealtimeDataDuration)) / ((unsigned long) frameDuration));
  
  // some debugging checks
  if (debugMode == true)
  {
    Serial.print(" ### currentFrameIndex: ");
    Serial.print(currentFrameIndex);
    Serial.print(" ... actRelativeFrameNumber: ");
    Serial.print(actRelativeFrameNumber);
  }
  
  int firstFrameOfCurrentAct = ((currentFrameIndex - actRelativeFrameNumber) + superArrayLength) % superArrayLength;

  // some debugging checks
  if (debugMode == true)
  {
    Serial.print(" ### noteNumber: ");
    Serial.print(noteNumber);
    Serial.print(" (firstFrameOfCurrentAct: ");
    Serial.print(firstFrameOfCurrentAct);
    Serial.print(", previous frame: ");
    Serial.print((currentFrameIndex + superArrayLength - 1) % superArrayLength);
    Serial.println(") ");
  }
  
  // 
  int moduloDividend = firstFrameOfCurrentAct + 1 + noteNumber;
  int moduloDivisor  = superArrayLength;

  int indexOut =  moduloDividend % moduloDivisor;

  // some debugging checks
  if (debugMode == true)
  {
    Serial.print("[rolling index status........] (moduloDividend: ");
    Serial.print(moduloDividend);
    Serial.print(", moduloDivisor: ");
    Serial.print(moduloDivisor);
    Serial.print(")");
    Serial.print(" ### indexOut: ");
    Serial.print(indexOut);
  }

  return indexOut;
}
