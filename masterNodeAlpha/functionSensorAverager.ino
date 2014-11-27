// smooths the data coming in from the sensor out to achieve more accurate readings

int sensorAveraging(int sensorVal) 
{
  int sum = 0;
  int mean;
  int dynamicNumberOfReads = numOfReadsForAverager;

  if (calibrating != true)
  {
    dynamicNumberOfReads = numOfReadsForAverager / 4;
  }

  if (indexForAverager > dynamicNumberOfReads)
  {
    indexForAverager = 0;
  }

  if (sensorReadDebugMode == true)
  {
    Serial.print("indexForAverager: ");
    Serial.println(indexForAverager);
  }

  arrayOfVals[indexForAverager] = sensorVal;

  // sum all values stored in array (i.e. last 10 sensor readings)
  for (int i = 0; i < dynamicNumberOfReads; i++)
  {
    if (sensorReadDebugMode == true)
    {
      Serial.print("averaging... i: ");
      Serial.print(i);
      Serial.print(" (arrayOfVals[i]: ");
      Serial.print(arrayOfVals[i]);
      Serial.println(")");
    }
    sum = sum + arrayOfVals[i];
  }

  // calculate mean
  mean = sum / dynamicNumberOfReads;

  indexForAverager++;

  return mean;
}


