void setup()
{
}

void loop()
{
  scheduler();
}

void scheduler()
{
  prioritiser();
  if (priorityServiceSerial == true)
  {
    serviceSerial();
  }
  else
  {
    playComposition();
  }
}
