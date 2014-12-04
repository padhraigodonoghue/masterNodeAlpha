// get into the groove, etc.
void concentrateOnMusic()
{
  // playbackBuffer value must be cast to unsigned long in order to do maths with millis() value
  unsigned long lookAheadMillis = millis() + (unsigned long) playbackBuffer;

  // used in preparation for triggering board's own sound-making mechanism (on solenoidPin)
  unsigned long selfSoundLookAhead;

  // is the current time within the playback zone?
  if ((lookAheadMillis % ((unsigned long) actRealtimeDataDuration)) < ((unsigned long) compositionDuration))
  {
    // is the current time within the lead-time of the next note?
    // (i.e. is it between [time of next note minus buffer] to [time of next note]?)
    if ((lookAheadMillis % ((unsigned long) ruleDuration)) < ((unsigned long)  playbackBuffer))
    {
      // some debugging checks
      if (debugMode == true)
      {
        Serial.print("[composition playback info...] lookAheadMillis: ");
        Serial.print(lookAheadMillis);
      }

      // lookup superFrameArray (i.e. the traffic count bank) for current index corresponding to noteNumber
      // (index shifts dynamically, hence requires lookup with reference to current time and previousFrameNumber)
      int superArrayAddress = lookUp(lookAheadMillis);

      // some debugging checks
      if (debugMode == true)
      {
        Serial.print(" ### superArrayAddress: ");
        Serial.println(superArrayAddress);
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
      int waitTime = (int) (((unsigned long) playbackBuffer) - ((millis() + ((unsigned long) playbackBuffer)) % ((unsigned long) ruleDuration)));

      // call the function that executes a delay before sending rule number over Serial Port
      playNote(waitTime, ruleNumber);
    }

    // checks necessary for identifying if it is almost time to play own note
    else if (rulePlayed == true)
    {
      selfSoundLookAhead = millis() + (unsigned long) selfSoundBuffer;

      // calculates time interval in milliseconds between each self-sounding note
      int selfSoundInterval = (ruleDuration / selfSoundFrequency);

      // check whether the current time is within the lead-time of the next note?
      if ((selfSoundLookAhead % ((unsigned long) selfSoundInterval)) < ((unsigned long) selfSoundBuffer))
      {
        int selfSoundWaitTime = (int) (((unsigned long) selfSoundBuffer) - ((millis() + ((unsigned long) selfSoundBuffer)) % ((unsigned long) selfSoundInterval)));
        delay(selfSoundWaitTime);
        solenoider(hardVelocity, true);

        // some debugging checks
        if (debugMode == true)
        {
          Serial.println("self-sounding!");
        }
      }
      // have all beats been played
      else if (((selfSoundLookAhead % ((unsigned long) ruleDuration)) / ((unsigned long) selfSoundFrequency)) == ((unsigned long) selfSoundFrequency - 1))
      {
        // some debugging checks
        if (debugMode == true)
        {
          Serial.println("");
        }
        rulePlayed == false;
      }
    }
  }
}

