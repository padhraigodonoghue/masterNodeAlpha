void pollSensor()
{
  // check the voltage at sensorPin to read sensor
  sensorValue = analogRead(sensorPin);
  // smooth reading
  int smoothRead = sensorAveraging(sensorValue);

  // check if beam has been broken or check if it has been restored
  if (smoothRead >= (sensorBaseLevel * 1.2) && sensorStateChangeFlag == false)
  {
    if (sensorReadDebugMode == true)
    {
      Serial.print("sensorValue: ");
      Serial.println(sensorValue);
      Serial.print("smoothRead: ");
      Serial.println(smoothRead);
    }
   
    beamBroken();
  }
  else if (smoothRead < (sensorBaseLevel * 1.125) && sensorStateChangeFlag == true)
  {
    beamBack();
  }
}

// two simple functions that act accordingly when the IR beam is broken and restored

void beamBroken()
{
//  digitalWrite(ledPin, HIGH);
  incrementTrafficCount();
  sensorStateChangeFlag = true;
}

void beamBack()
{
//  digitalWrite(ledPin, LOW);
  sensorStateChangeFlag = false;
}
