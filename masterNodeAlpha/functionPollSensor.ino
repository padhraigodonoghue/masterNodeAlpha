void pollSensor()
{
  // check the voltage at sensorPin to read sensor
  sensorValue = analogRead(sensorPin);

  // smooth reading
  int smoothRead = sensorAveraging(sensorValue);
  
  if (debugMode == true)
  {
    Serial.print("smoothRead: ");
    Serial.println(smoothRead);
  }

  // check if beam has been broken or check if it has been restored
  if (smoothRead >= (minVal * 1.25) && sensorStateChangeFlag == false)
  {
    beamBroken();
  }
  else if (smoothRead < (minVal * 1.2) && sensorStateChangeFlag == true)
  {
    beamBack();
  }
  delay(100);
}

// two simple functions that act accordingly when the IR beam is broken and restored

void beamBroken()
{
  digitalWrite(ledPin, HIGH);
  Serial.println("break");
  incrementTrafficCount();
  sensorStateChangeFlag = true;
}

void beamBack()
{
  digitalWrite(ledPin, LOW);
  Serial.println("back");
  sensorStateChangeFlag = false;
}

