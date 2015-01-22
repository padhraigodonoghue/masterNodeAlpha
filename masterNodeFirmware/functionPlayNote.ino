// executes a delay before sending rule number over Serial Port
void playNote(int waitTimeIn, int ruleNumberIn)
{
  // in case Arduino falls behind unexpectedly, do not allow a negative wait value reach 
  if (waitTimeIn > 0)
  {
    delay(waitTimeIn);
  }

  // some debugging checks
  if (debugMode == true)
  {
    Serial.print("waitTimeIn: ");
    Serial.print(waitTimeIn);
    Serial.print(" ### SENDING RULE: ");
    Serial.println(ruleNumberIn);

    if (ruleNumberIn != 255)
    {
      for (int i = 0; i < ruleNumberIn; i++)
      {
        Serial.print("|");
      }
    }
    else
    {
      Serial.print(":P");
    }

    Serial.println("");
  }
  else
  {
    Serial.write(ruleNumberIn);
  }

  if (ruleNumberIn != 0)
  {
    solenoider(hardVelocity, true);

    // enable self-sounding on first beat of each bar of n-bar rule (where n is determined by selfSoundFrequency; not enabled in the case of debug rule 255)
    if (ruleNumberIn != 255)
    {
      rulePlayed = true;
      rulePlayedAt = millisOffset();
    }
  }
  else
  {
    // rule number 0 (i.e. don't send anything to slave nodes) is to be played
    solenoidCoil = true;
    coiledAtTime = millis();
    rulePlayed   = false;
  }
}

