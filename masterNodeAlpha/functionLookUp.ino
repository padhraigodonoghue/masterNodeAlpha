// compensates for rolling index of the superFrameArray
int lookUp(unsigned long timeIn)
{  
  // determine which note of the n-note composition is imminent...
  // where n = (framesPerAct * actsPerComposition), i.e. 27-note...
  // and number of first note is 0 and last note, 26
  int noteNumber = (int) ((timeIn % (unsigned long) actRealtimeDataDuration) / (unsigned long) ruleDuration);
  
  // should round down to the (whole) number of completed "acts" into masterWrapTime
  // then multiplied by framesPerAct to give number of first "frame" of "act"
  int firstFrameOfCurrentAct = (int) (((timeIn % masterWrapTime) / (unsigned long) actRealtimeDataDuration) * (unsigned long) framesPerAct);

  // get last frame of last act, not just last frame
  int lastFrameOfPreviousAct = (firstFrameOfCurrentAct + superArrayLength - 1) % superArrayLength;

  // some debugging checks
  if (debugMode == true)
  {
    Serial.print(" ### noteNumber: ");
    Serial.print(noteNumber);
    Serial.print(" (firstFrameOfCurrentAct: ");
    Serial.print(firstFrameOfCurrentAct);
    Serial.print(", lastFrameOfPreviousAct: ");
    Serial.print(lastFrameOfPreviousAct);
    Serial.print(", previous frame: ");
    Serial.print((frameTracker() + superArrayLength - 1) % superArrayLength);
    Serial.println(") ");
  }

  int moduloDividend = lastFrameOfPreviousAct + 2 + noteNumber;
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
