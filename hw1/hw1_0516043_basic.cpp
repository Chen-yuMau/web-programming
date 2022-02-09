#include <iostream>
#include <pthread.h>
#include <queue>
#include <unistd.h>
#include <fstream>
#include <mutex>

using namespace std;

int cur[100][3] = {0};
//0 played
//1 rested
//2 total played
queue <int> line;
int t=0;
int numofwaiter = 0;
int waiter[100] = {0};
int player = 0;
int dude = -1;
int cus[100][4];
int p;
int G;
int k=0;// people finished playing
int rinr = 0;
mutex mtx;

	pthread_t threads[3];
    pthread_attr_t attr;

void *wait(void *threadid)
{
	for(int i=0;i<numofwaiter;i++)
	{
//			cout<<"numofwaiter = "<<numofwaiter<<endl;
//			cout<<"t = "<<t<<endl;
//			cout<<"i = "<<i<<endl;
//			cout<<"waiter[i] = "<<waiter[i]<<endl;
//			cout<<"cur[waiter[i]] = "<<cur[waiter[i]][0]<<" "<<cur[waiter[i]][1]<<" "<<cur[waiter[i]][2]<<endl;
//			cout<<"cus[waiter[i]] = "<<cus[waiter[i]][0]<<" "<<cus[waiter[i]][1]<<" "<<cus[waiter[i]][2]<<" "<<cus[waiter[i]][3]<<endl<<endl<<endl<<endl;
		if(cur[waiter[i]][1]==(cus[waiter[i]][2]))/////////////////////////////////////////////////////////////
		{
			cur[waiter[i]][1]=0;
			numofwaiter--;
			line.push(waiter[i]);
			if(dude!=-1)
			{
			cout<<t<<" "<<waiter[i]+1<<" wait in line"<<endl;
			}
			//cout<<t<<" "<<waiter[i]+1<<"wait in line"<<endl;
			//cout<<line.back()+1 <<"in da back"<<endl;
			for(int j=i;j<=numofwaiter;j++)
			{
				waiter[j]=waiter[j+1];
			}
		}
		cur[waiter[i]][1]++;
	}
      mtx.unlock();
	pthread_exit(NULL);
}
void *machine(void *threadid)
{
//	for(int i=0;i<numofwaiter;i++)
//	{
//		cout<<waiter[i]+1<<" "<<cur[waiter[i]][i]<<endl;
//	}
		if( dude == -1)
		{
			rinr=0;
			if(line.size()!=0)
			{
				dude = line.front();
		    	cout<<t<<" "<<dude+1<<" start playing"<<endl;
		    	line.pop();
		    	//int did = line.front();
		    	//cout<<t<<" "<<did+1<<"check if same"<<endl;
			}
			else
			{
				t++;
				goto skip;
			}
		}
		if(cur[dude][0]==cus[dude][1])
		{
			dude = line.front();////////////////////////////////////////////////////////////////////////////////////
		    cout<<t<<" "<<dude+1<<" start playing"<<endl;
		    line.pop();
		}
		//cout<<"dude:"<<line.back()<<endl;
		cur[dude][0]++;
		cur[dude][2]++;
		rinr++;
		t++;
		if(rinr==G)
		{
		    cur[dude][0] = 0;
		    cout<<t<<" "<<dude+1<<" finish playing YES"<<endl;
		    k++;
		    rinr=0;
		    if(line.size()!=0)
		    {
			    dude = line.front();
			    cout<<t<<" "<<dude+1<<" start playing"<<endl;
			    line.pop();
		    }
		    else
		    {
		    	dude = -1;
		    }
		}
		else if(cur[dude][2]==cus[dude][3])
		{
		    cur[dude][0] = 0;
		    cout<<t<<" "<<dude+1<<" finish playing YES"<<endl;
		    k++;
		    rinr=0;

			if(line.size()!=0)
		    {
			    dude = line.front();
			    cout<<t<<" "<<dude+1<<" start playing"<<endl;
			    line.pop();
		    }
		    else
		    {
		    	dude = -1;
		    }
		}
		else if(cur[dude][0]==cus[dude][1])
		{
			waiter[numofwaiter] = dude;
			numofwaiter++;
		    cur[dude][0] = 0;
		    cur[dude][1] = 0;
		    cout<<t<<" "<<dude+1<<" finish playing NO"<<endl;

		    if(line.size()!=0)
		    {
			    dude = line.front();
			    cout<<t<<" "<<dude+1<<" start playing"<<endl;
			    line.pop();
		    }
		    else
		    {
		    	dude = -1;
		    }
		}
		skip:

			pthread_create(&threads[2], NULL, wait, (void *)2);
		pthread_exit(NULL);

}
void *arrive(void *threadid)
{
		for (int i=0;i<p;i++)
		{
			if(cus[i][0]==t)
			{
				if(line.size()!=0)
				{
				}
				line.push(i);
				if(dude!=-1)
				{
					cout<<t<<" "<<i+1<<" wait in line"<<endl;
				}
				//cout<<i+1<<"in queue"<<endl;
				//cout<<line.back()+1<<"in the back"<<endl;
			}
		}

			pthread_create(&threads[1], NULL, machine, (void *)1);
	pthread_exit(NULL);
}

int main(int argc, char **argv) 
{
	//int cur[100][3]
	//0 played
	//1 rested
	//2 total played

	//int cus[100][4];
	//0  arrive time
	//1	continuous play round
	//2	rest time
	//3	total play round
	//int G;//num of rounds needed to get prize
	
	fstream fin;	
    fin.open(*(argv+1),ios::in);
	
	fin>>G;
	fin>>p;
	//int p;//num of people

	for (int i=0;i<p;i++)
	{
		for(int j=0;j<4;j++)
		{
			fin>>cus[i][j];
		}
	}
	while(k!=p)
	{	
		//cout<<"                  "<<t<<endl;

		if(mtx.try_lock())
		{
			pthread_create(&threads[0], NULL, arrive, (void *)0);
	    	//system("PAUSE");
	    	//system("PAUSE");
			//lock things and wait for previous thread	
		}
	}
}


