void pollSensor()
{
  // check the voltage at sensorPin to read sensor
  sensorValue = analogRead(sensorPin);
  // smooth reading
  int smoothRead = sensorAveraging(sensorValue);

  if ((millis() - debouncingSince) > ((unsigned long) debounceDuration))
  {
    bounceGuard = false;
  }

  // before a successive IR sensor break can be detected, the debouncing round-trip of minimum (2 * debounceDuration) must be made
  if (bounceGuard == false)
  {
    // check if beam has been broken or check if it has been restored
    if (((float) smoothRead) >= (((float) sensorBaseLevel) * breakThresholdMultiplier) && sensorStateChangeFlag == false)
    {
      beamBroken();
    }
    else if (((float) smoothRead) < (((float) sensorBaseLevel) * restoreThresholdMultiplier) && sensorStateChangeFlag == true)
    {
      beamBack();
    }

    // simulate sensor break using Serial Monitor input (only in debug mode)
    if (debugMode == true)
    {
      // this behaviour is blocking, but occurs only in debug mode
      while (Serial.available())
      {
        if ((millis() - debouncingSince) > ((unsigned long) debounceDuration))
        {
          bounceGuard = false;
        }

        if (bounceGuard == false)
        {
          if (Serial.read() == 49)
          {
            beamBroken();
          }
        }
      }
    }

    // some debugging checks
    if (sensorReadDebugMode == true)
    {
      Serial.print("sensorValue: ");
      Serial.println(sensorValue);
      Serial.print("smoothRead: ");
      Serial.println(smoothRead);
    }
  }
}

// do when IR beam is broken
void beamBroken()
{
  incrementTrafficCount();
  digitalWrite(ledPin, HIGH);
  sensorStateChangeFlag = true;
  bounceGuard = true;
  debouncingSince = millis();
}

// do when IR beam is restored
void beamBack()
{
  digitalWrite(ledPin, LOW);
  sensorStateChangeFlag = false;
  bounceGuard = true;
  debouncingSince = millis();
}
