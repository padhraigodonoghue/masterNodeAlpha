// get into the groove, etc.
void concentrateOnMusic()
{
  // millis() value must be cast to signed long in order to do maths
  unsigned long lookAheadMillis = millis() + playbackBuffer;

  // is the current time within the playback zone?
  if ((lookAheadMillis % actRealtimeDataDuration) < compositionDuration)
  {
    // is the current time within the lead-time of the next note?
    // (i.e. is it between [time of next note minus buffer] to [time of next note]?)
    if ((lookAheadMillis % ruleDuration) < playbackBuffer)
    {
      // some debugging checks
      if (debugMode == true)
      {
        Serial.print("lookAheadMillis: ");
        Serial.print(lookAheadMillis);
      }

      // lookup superFrameArray (i.e. the traffic count bank) for current index corresponding to noteNumber
      // (index shifts dynamically, hence requires lookup with reference to current time and previousFrameNumber)
      int superArrayAddress = lookUp(lookAheadMillis);

      // some debugging checks
      if (debugMode == true)
      {
        Serial.print("superArrayAddress: ");
        Serial.print(superArrayAddress);
      }

      // derive ruleNumber by comparing to a reference maximum
      int ruleNumber = countConversion(superArrayAddress);

      // some debugging checks
      if (debugMode == true)
      {
        unsigned long checkTime = millis();
        Serial.print("new millis(): ");
        Serial.println(checkTime);
      }

      // determine necessary wait time before rule should be sent
      // add buffer time to current system time, modulo compare with ruleDuration, subtract result from buffer time
      int waitTime   = playbackBuffer - ((millis() + playbackBuffer) % ruleDuration);

      // call the function that executes a delay before sending rule number over Serial Port
      playNote(waitTime, ruleNumber);
    }
  }
}

