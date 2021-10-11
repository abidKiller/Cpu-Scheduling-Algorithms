//Author: ABID AHSAN SAMIN
//I tried to make the code OOP as possible

//Scheduling class is the base class. other algo(SJFP,RR,PP,S) classes inherits only its properites


#include<bits/stdc++.h>
using namespace std;

typedef struct process
{
    int id=0;
    int startTime;
    int arrivalTime=0;
    int burstTime=0;
    int priority=0;
    int waitingTime=0;
    int completionTime=0;
    int turnAroundTime=0;
    int responseTime=0;
    bool isCompleted=false;
    int remainingTime=0;


} process ;

class Scheduling
{
protected:
    vector<process> processes;
    int numOfProcesses;
    string schName;

public:
    Scheduling(vector<process>& pops):processes(pops)
    {
        numOfProcesses=pops.size();
    }

    virtual void run()=0;
    void print()
    {
        int sumResTime=0;
        int sumComTime=0;
        int sumWaitingTime=0;
        int sumTurnTime=0;
        cout<<"\n\n#####################################################"<<endl;
        cout<<"                    "<<schName<<"               "<<endl;
        cout<<"#####################################################"<<endl;


        for(int i=0; i<numOfProcesses; i++)
        {
            cout<<"\nP"<<processes[i].id<<"\n\n";

            //  cout<<"      burst arrival "<<processes[i].burstTime<<" "<<processes[i].<<endl;

            cout<<"     "<<"Start Time:    "<<processes[i].startTime<<"\n";
            cout<<"     "<<"Finish Time:   "<<processes[i].completionTime<<"\n";

            processes[i].turnAroundTime=processes[i].completionTime-processes[i].arrivalTime;
            processes[i].waitingTime= processes[i].turnAroundTime-processes[i].burstTime;

            cout<<"     "<<"Waiting Time:  "<<processes[i].waitingTime<<"\n";
            cout<<"     "<<"Response Time: "<<processes[i].responseTime<<"\n\n";
            //cout<<"     "<<"Response Time: "<<processes[i].responseTime<<"\n";
            sumResTime+=processes[i].responseTime;
            sumWaitingTime+=processes[i].waitingTime;
        }

        cout<<"Average Response Time: "<<(float) sumResTime/numOfProcesses<<"\n";
        cout<<"Average Waiting Time : "<< (float) sumWaitingTime/numOfProcesses<<"\n";
    }


};

class FCFS:public Scheduling
{
public:
    FCFS(vector<process> pops):Scheduling(pops)
    {
        schName="FCFS";
    }
    void run()
    {
        for(int i=0;i<numOfProcesses-1;i++)
            for(int j=i+1;j<numOfProcesses;j++)
                if(processes[i].arrivalTime>processes[j].arrivalTime)
                 swap(processes[i],processes[j]);
        int cumTime=0;
        for(int i=0;i<numOfProcesses;i++)
        {
            processes[i].completionTime=cumTime+processes[i].burstTime;\
            cumTime+=processes[i].burstTime;
            if(i==0)
            {
                processes[i].startTime=0;
                processes[i].responseTime=0;
            }
            else{
                processes[i].startTime=processes[i-1].completionTime;
                processes[i].responseTime=processes[i].startTime-processes[i].arrivalTime;
            }
        }
    }

};
class SJFP:public Scheduling
{
public:
    SJFP(vector<process> pops):Scheduling(pops)
    {
        schName="SJFP";
    }
    void run()
    {
      /*  for(int i=0; i<numOfProcesses-1; i++)
            for(int j=i+1; j<numOfProcesses; j++)
                if(processes[j].arrivalTime<processes[i].arrivalTime)
                    swap(processes[i],processes[j]);
  */

        int curTime=0;

        while(true)   //time loop
        {
            int curShortProcIdx=-1;
            int curShortProcRemTime=INT_MAX;
            bool isAllCompleted=true;

            for(int i=0; i<numOfProcesses; i++) //find loop
                if(processes[i].remainingTime>0)
                {
                    isAllCompleted=false;
                    if(processes[i].arrivalTime<=curTime)
                        if(processes[i].remainingTime<curShortProcRemTime)
                        {
                            curShortProcRemTime=processes[i].remainingTime;
                            curShortProcIdx=i;
                        }

                }
            if(isAllCompleted)
                break ;
            if(processes[curShortProcIdx].remainingTime==processes[curShortProcIdx].burstTime)
               {
                 processes[curShortProcIdx].startTime=curTime;
                 processes[curShortProcIdx].responseTime=curTime-processes[curShortProcIdx].arrivalTime;
               }
            processes[curShortProcIdx].remainingTime--;
            curTime++;
            if(processes[curShortProcIdx].remainingTime==0)
                processes[curShortProcIdx].completionTime=curTime;

        }

    }
};

class SJFNP:public Scheduling
{
public:
    SJFNP(vector<process> pops):Scheduling(pops) { schName="SJFNP";}
    void run()
    {
     /*   for(int i=0; i<numOfProcesses-1; i++)
            for(int j=i+1; j<numOfProcesses; j++)
                if(processes[j].arrivalTime<processes[i].arrivalTime)
                    swap(processes[i],processes[j]);
    */

        int curTime=0;

        while(true)
        {
            int curShortProcIdx=-1;
            int curShortProcTime=INT_MAX;
            bool isAllCompleted=true;

            for(int i=0; i<numOfProcesses; i++)
            {
                if(!processes[i].isCompleted)
                {
                    isAllCompleted=false;
                    if(processes[i].arrivalTime<=curTime)
                    {
                        if(processes[i].burstTime<curShortProcTime)
                        {
                            curShortProcIdx=i;
                            curShortProcTime=processes[i].burstTime;

                        }

                    }
                }
            }

            if(isAllCompleted) break;
            processes[curShortProcIdx].startTime=curTime;
            processes[curShortProcIdx].responseTime=curTime-processes[curShortProcIdx].arrivalTime;
            curTime+=processes[curShortProcIdx].burstTime;
            processes[curShortProcIdx].isCompleted=true;
            processes[curShortProcIdx].completionTime=curTime;
        }

    }

};

class PP:public Scheduling
{
public:
    PP(vector<process> pops):Scheduling(pops) {schName="PP";}
    void run()
    {

        int curTime=0;
        while(true)
        {
            int curHighProcIdx=-1;
            int curHighProcPriority=INT_MIN;
            bool isAllCompleted=true;

            for(int i=0;i<numOfProcesses;i++)
            {
              //  cout<<"in"<<endl;
                if(processes[i].remainingTime>0)
                {
                    isAllCompleted=false;
                    if(processes[i].arrivalTime<=curTime)
                        if(processes[i].priority==curHighProcPriority)
                        {
                            if(processes[i].remainingTime>processes[curHighProcIdx].remainingTime)
                                continue;
                            else{
                                curHighProcIdx=i;
                                 curHighProcPriority=processes[i].priority;
                            }

                        }
                        else if(processes[i].priority>curHighProcPriority)
                        {
                            curHighProcIdx=i;
                            curHighProcPriority=processes[i].priority;
                        }
                }
            }
            if(isAllCompleted) break;
            if(processes[curHighProcIdx].remainingTime==processes[curHighProcIdx].burstTime)
            {
                processes[curHighProcIdx].startTime=curTime;
                processes[curHighProcIdx].responseTime=curTime-processes[curHighProcIdx].arrivalTime;
            }
            processes[curHighProcIdx].remainingTime--;
            curTime++;
            if(processes[curHighProcIdx].remainingTime==0)
                processes[curHighProcIdx].completionTime=curTime;

        }
    }
};

class NPP:public Scheduling
{
public:
    NPP(vector<process> pops):Scheduling(pops) {schName="NPP";}

    void run()
    {

        int curTime=0;
        while(true)
        {
            int curHighProcIdx=-1;
            int curHighProcPriority=INT_MIN;
            bool isAllCompleted=true;

            for(int i=0;i<numOfProcesses;i++)
            {
                if(!processes[i].isCompleted)
                {
                    isAllCompleted=false;
                    if(processes[i].arrivalTime<=curTime)
                        if(processes[i].priority>curHighProcPriority)
                        {
                            curHighProcIdx=i;
                            curHighProcPriority=processes[i].priority;
                        }
                }
            }
            if(isAllCompleted) break;

            processes[curHighProcIdx].startTime=curTime;
            processes[curHighProcIdx].responseTime=curTime-processes[curHighProcIdx].arrivalTime;
            curTime+=processes[curHighProcIdx].burstTime;
            processes[curHighProcIdx].isCompleted=true;
            processes[curHighProcIdx].completionTime=curTime;

        }


    }
};


class RR:public Scheduling
{

public:
    int quantum;
    RR(vector<process> pops):Scheduling(pops) {
      schName="RR";
      quantum=4;
    }
    void run()
    {
        int curTime=processes[0].arrivalTime;
        int remainingProc=numOfProcesses;
        for(int i=0;i<numOfProcesses;i=(i+1)%numOfProcesses)
        {
            if(processes[i].remainingTime>0 && processes[i].arrivalTime<=curTime)
            {
                if(processes[i].remainingTime==processes[i].burstTime)
                {
                    processes[i].startTime=curTime;
                    processes[i].responseTime=curTime-processes[i].arrivalTime;
                }
                if(processes[i].remainingTime<=quantum)
                {
                    curTime+=processes[i].remainingTime;
                    processes[i].completionTime=curTime;
                    processes[i].remainingTime=0;
                    remainingProc--;
                }
                else
                {
                    curTime+=quantum;
                    processes[i].remainingTime-=quantum;
                }
            }
            if(remainingProc==0)
                break;
        }

    }
};



int main()
{
    vector<process> processes; // primary input sotrage
//take input
    freopen("input.txt","r",stdin);
    int numberOfProcesses;
    cin>>numberOfProcesses;
    for(int i=1; i<=numberOfProcesses; i++)
    {
        process temp;
        temp.id=i;
        cin>>temp.burstTime>>temp.priority>>temp.arrivalTime;
        temp.remainingTime=temp.burstTime;
        processes.push_back(temp);
    }


    SJFNP algo1(processes);
    algo1.run();
    algo1.print();

    SJFP algo2(processes);
    algo2.run();
    algo2.print();

    NPP algo3(processes);
    algo3.run();
    algo3.print();

    PP algo4(processes);
    algo4.run();
    algo4.print();

    RR algo5(processes);
    algo5.run();
    algo5.print();

    FCFS algo6(processes);
    algo6.run();
    algo6.print();




}

