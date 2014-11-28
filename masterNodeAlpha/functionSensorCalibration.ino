// calibrate the sensors sensor base level used as basis for break threshold of beam
void sensorCalibration()
{
  int calibrationDuration = 3000;
  
  calibrating = true;
  calibrationStartTime = millis();
  
  if (debugMode == true)
  {
    Serial.print("calibrating sensor");
  }
  
  int calibrationElapsed = (int) (millis() - ((unsigned long) calibrationStartTime));

  while (calibrationElapsed < calibrationDuration)
  {
    if (debugMode == true && (calibrationElapsed % 500) == 0)
    {
      Serial.print(".");
    }

    sensorValue = analogRead(sensorPin);
    
    sensorBaseLevel = sensorAveraging(sensorValue);
    
    nonBlockingBlink();
    
    calibrationElapsed = (int) (millis() - ((unsigned long) calibrationStartTime));
  }
  
  calibrating = false;
  
  digitalWrite(ledPin, false);

  if (debugMode == true)
  {
    Serial.println(" CALIBRATION COMPLETE!");
    Serial.print("sensorBaseLevel: ");
    Serial.println(sensorBaseLevel);
    Serial.println("");
  }
}

// source: Arduino Examples - "BlinkWithoutDelay"
void nonBlockingBlink()
{
  int blinkDuration = 100;

  long currentTime = millis();

  if (currentTime - previousBlinkTime >= blinkDuration)
  {
    previousBlinkTime = currentTime;
    ledState = !ledState;
  }

  digitalWrite(ledPin, ledState);
}
