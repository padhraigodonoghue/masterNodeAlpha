// calibrate the sensors sensor base level used as basis for break threshold of beam
void sensorCalibration()
{
  if (debugMode == true)
  {
    Serial.print("calibrating sensor");
  }

  for (long i= 0; i < 20000; i++) 
  {
    if (debugMode == true && (i % 5000) == (0) && i != 0)
    {
      Serial.print(".");
    }

    sensorValue = analogRead(sensorPin);
    
    sensorBaseLevel = sensorAveraging(sensorValue);
    
    nonBlockingBlink();
  }
  
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
