// get into the groove, etc.
void concentrateOnMusic()
{
  long lookAheadMillis = millis() + playbackBuffer;

  // is the current time within the playback zone?
  if ((lookAheadMillis % actRealtimeDataDuration) < compositionDuration)
  {
    // is the current time within the leadtime of the next note?
    // (i.e. is it between time of next note minus buffer to time of next note?)
    if ((lookAheadMillis % ruleDuration) < playbackBuffer)
    {
      // determine which note of the n-note composition is imminent
      // n = (framesPerAct * actsPerComposition), i.e. 27-note
      int noteNumber = (lookAheadMillis % actRealtimeDataDuration) / ruleDuration;

      // determine the previousFrameNumber
      // consider only modulus of superCompositionDuration and divide by frameDuration
      // (should only ever be equal to [(n * 9) - 1], where n ∈ N < (actsPerComposition + 1 ... because of first if condition)
      int currentFrameNumber  = (lookAheadMillis % superRealtimeDataDuration) / frameDuration;
      int previousFrameNumber = (currentFrameNumber + ((framesPerComposition + framesPerAct) - 1)) % (framesPerAct * (actsPerComposition + 1));

      // some debugging checks
      if (debugMode == true)
      {
        Serial.print("lookAheadMillis: ");
        Serial.print(lookAheadMillis);
        Serial.print(" ### noteNumber: ");
        Serial.print(noteNumber);
        Serial.print(" ### previousFrameNumber: ");
        Serial.println(previousFrameNumber);
      }

      // lookup superFrameArray (i.e. the traffic count bank) for current index corresponding to noteNumber
      // (index shifts dynamically, hence requires lookup with reference to current time and previousFrameNumber)
      int superArrayAddress = lookUp(previousFrameNumber, noteNumber);

      // some debugging checks
      if (debugMode == true)
      {
        Serial.print("superArrayAddress: ");
        Serial.print(superArrayAddress);
      }

      // derive ruleNumber by comparing to a reference maximum; then determine necessary wait time before rule should be sent
      int ruleNumber = countConversion(superArrayAddress);
      int waitTime   = (millis() + playbackBuffer) % ruleDuration;


      // call the function that executes a delay before sending rule number over Serial Port
      playNote(waitTime, ruleNumber);
    }
  }
}



void serviceSerial()
{
  // some debugging checks
  if (debugMode == true)
  {
//    Serial.println("checking Serial Port buffer...");
  }

  while(Serial.available())
  {
    // in debug mode, Serial Port will be received the value of the ACSCII code for the character '1', i.e. 49
    if (debugMode == true)
    {
      if(Serial.read() == 49)
      {
        // increment count on whatever frame for every byte in the serial port buffer
        incrementTrafficCount();
      }
    }
    else
    {
      if(Serial.read() == 1)
      {
        // increment count on whatever frame for every byte in the serial port buffer
        incrementTrafficCount();
      }
    }
  }

  // look only at window of time equivalent to full composition plus 1/3, i.e. 4 "acts"
  long superCompositionWrapTime = (millis() % (actRealtimeDataDuration * (actsPerComposition + 1)));

  // should always round down to current frame
  int currentBin = (superCompositionWrapTime / frameDuration);

  if (currentBin != lastFrameCheck)
  {
    lastFrameCheck = currentBin;

    if (cobwebs == true)
    {
      superFrameArray[(currentBin + ((sizeof(superFrameArray) / sizeof(int)) - 1)) % (sizeof(superFrameArray) / sizeof(int))] = 0;
    }
    else
    {
      cobwebs = true;
    }
  }
}
