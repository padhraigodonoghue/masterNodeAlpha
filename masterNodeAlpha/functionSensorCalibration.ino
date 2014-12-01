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
  
  int calibrationElapsed = (int) (millis() - calibrationStartTime);

  while (calibrationElapsed < ((unsigned long) calibrationDuration))
  {
    if (debugMode == true && (calibrationElapsed % 500) == 0)
    {
      Serial.print(".");
    }

    sensorValue = analogRead(sensorPin);
    
    sensorBaseLevel = sensorAveraging(sensorValue);
    
    // if the hardware sensor is not connected and debug mode is enabled...
    // but the below Serial communication statements are commented out, then the sensorBaseLevel is calculated as usually around 24
    // when the statements are "reinstated" (i.e. comment tags removed), the sensorBaseLevel is calculated differently (i.e. as around 300)
    // the floating pin may be affected by Serial Communication or something
    /*
    if (debugMode == true)
    {
      Serial.print("sensorBaseLevel: ");
      Serial.println(sensorBaseLevel);
    }
    */
    
    nonBlockingBlink();
    
    calibrationElapsed = (int) (millis() - calibrationStartTime);
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
