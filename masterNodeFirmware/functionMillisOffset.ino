// set up Arduino to play at 5-to-hour based on a 'on-the-hour' power-on time, e.g. 6:00 am
// i.e. add 5 minutes to current time (assuming performance scheduled every 2 hours)
unsigned long millisOffset()
{
  return (millis() + 300000);
}
