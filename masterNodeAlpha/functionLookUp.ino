// compensates for rolling index of the superFrameArray
int lookUp(long timeIn)
{  
  // determine which note of the n-note composition is imminent
  // n = (framesPerAct * actsPerComposition), i.e. 27-note
  int noteNumber = (timeIn % actRealtimeDataDuration) / ruleDuration;
  
  // should round down to whole number of completed "acts" into masterWrapTime
  // then multiplied by framesPerAct to give number of first "frame" of "act"
  int firstFrameOfCurrentAct = ((timeIn % masterWrapTime) / actRealtimeDataDuration) * framesPerAct;

  // get last frame of last act, not just last frame
  int lastFrameOfPreviousActNumber = (firstFrameOfCurrentAct + superArrayLength - 1) % superArrayLength;

  // some debugging checks
  if (debugMode == true)
  {
    Serial.print(" ### noteNumber: ");
    Serial.print(noteNumber);
    Serial.print(" ### firstFrameOfCurrentAct: ");
    Serial.print(firstFrameOfCurrentAct);
    Serial.print(" ### lastFrameOfPreviousActNumber: ");
    Serial.println(lastFrameOfPreviousActNumber);
  }

  int moduloDividend = lastFrameOfPreviousActNumber + 2 + noteNumber;
  int moduloDivisor  = superArrayLength;

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


