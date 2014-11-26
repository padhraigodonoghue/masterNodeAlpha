// calibrate the sensors max and min values which are used to set the threshold that breakc the beam
void sensorCalibration()
{
  if (debugMode == true)
  {
    Serial.print("calibrating sensor");
  }

  for (long i= 0; i < 30000; i++) 
  {
    if (debugMode == true && (i % 10000) == (10000 - 1))
    {
      Serial.print(".");
    }
    
    sensorValue = analogRead(sensorPin);
    int incoming = sensorAveraging(sensorValue);

    if(incoming > maxVal)
    {
      maxVal = incoming;
    }
    else if(incoming < minVal)
    {
      minVal = incoming;
    }
    nonBlockingBlink(); 
  }
  
  if (debugMode == true)
  {
    Serial.println(" CALIBRATION COMPLETE!");
    Serial.print("maxVal: ");
    Serial.print(maxVal);
    Serial.print(" ### minVal: ");
    Serial.println(minVal);
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
