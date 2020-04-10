//Written by Ziad Tarek

#ifndef PROCESS_H
#define PROCESS_H

#include "QString"
#include "vector"
#include "algorithm"
#include "queue"

using namespace std;

class Process
{
public:
  int pid;
  QString name;
  float arrivalTime;
  float requiredTime;
  int priority;


  Process(int id, QString n, float aT, float rT, int p);

  void decReqTime(float t);

  int getPID();
  QString getName();
  float getArrivalTime();
  float getRequiredTime();
  int getPriority();
};


static int noOfProceses;
static vector <Process> PList;

static Process PopFirst(vector <Process> &Q){
  Process P = Q[0];
  Q.erase(Q.begin());
  return P;
}

static float TotalTime(vector <Process> &Q){
  float tt=0;
  for(size_t i =0; i< Q.size(); i++)
    tt += Q[i].getRequiredTime();
  return tt;
}


// sort(Queue.begin(), Queue.end(), CompFunction);
// sort functions:
static bool CompArrivalTime(Process a, Process b){return (a.getArrivalTime() < b.getArrivalTime());}
static bool CompReqTime(Process a, Process b){return (a.getRequiredTime() < b.getRequiredTime());}
static bool CompPri(Process a, Process b){return (a.getPriority() < b.getPriority());}
struct CompRequiredTime
 {
   bool operator()(Process a, Process b)
   {
       return (a.getRequiredTime() < b.getRequiredTime());
   }
 };
struct CompPriority
 {
   bool operator()(Process a, Process b)
   {
       return (a.getPriority() < b.getPriority());
   }
 };

#endif // PROCESS_H
