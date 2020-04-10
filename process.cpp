//Written by Ziad Tarek

#include "process.h"

Process::Process(int id,QString n, float aT, float rT, int p=0)
{
  pid = id;
  name =n;
  arrivalTime = aT;
  requiredTime = rT;
  priority = p;
}


void Process::decReqTime(float t)
{
  if(t >= requiredTime)
    requiredTime =0;
  else
    requiredTime -= t;
}

int Process::getPID(){return pid;}
QString Process::getName() {return name;}
float Process::getArrivalTime(){return arrivalTime;}
float Process::getRequiredTime(){return requiredTime;}
int Process::getPriority(){return priority;}
