// smooths the data coming in from the sensor out to achieve more accurate readings

int sensorAveraging(int sensorVal) 
{
  int sum = 0;
  int mean;
  
  indexForAverager++;
  
  if (indexForAverager > numOfReadsForAverager)
  {
    indexForAverager = 0;
  }
  
  arrayOfVals[indexForAverager] = sensorVal;
  
  // sum all values stored in array (i.e. last 10 sensor readings)
  for (int i = 0; i < numOfReadsForAverager; i++)
  {
    sum = sum + arrayOfVals[i];
  }
  
  // calculate mean
  mean = sum / numOfReadsForAverager;
  
  return mean;
}
