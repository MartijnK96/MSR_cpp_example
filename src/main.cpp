// To compile, install g++, and run
//
// g++ -Wall -o robot main.cpp

#include <iostream>
#include <list>
#include <string>
#include <set>

using namespace std;


class Robot
{
	list<string> job;
	int robotID;
	int speed;
	
public:

	Robot(list<string> const & job, int robotID)
	: job(job), robotID(robotID), speed(robotID * 10)
	{}

	bool nextstate(int wallclock, int & duration)
	{
		if (job.empty())
			return false;

		string task = job.front();
		job.pop_front();

		cout << wallclock << ", Robot " << robotID << ", " << task << ", " << speed << "\n";

		duration = speed;
		return true;
	}

	string getCurrentTask() const
	{
		if (job.empty())
			return "";
		return job.front();
	}
};


int main()
{
	cout << "Test!\n";

	list<string> job1 = {"Move to P2", "Move to M1S1", "Calibrate M1S1", "Swab M1S1", "Move to M1S2", "Calibrate M1S2", "Swab M1S2", "Move to P2", "Move to P1"};
	list<string> job2 = {"Move to P4", "Move to M2S1", "Calibrate M2S1", "Swab M2S1", "Move to M2S2", "Calibrate M2S2", "Swab M2S2", "Move to P4", "Move to P2"};
	int wallclock = 0;

	Robot r1(job1, 1);
	Robot r2(job2, 2);
	Robot r3(job1, 3);

	cout << "Wall clock, Robot, Task, Speed\n";


	list<pair<Robot &, int> > updatelist = { {r1, 35}, {r2, 0}, {r3, 0} };

	while (true)
	{
		// Find smallest possible time step
		list<pair<Robot &, int> >::const_iterator it = updatelist.begin();
		int smallestduration = it->second;
		it++;
		while (it != updatelist.end())
		{
			if (it->second < smallestduration)
				smallestduration = it->second;
			it++;
		}

		// Smallest timestep is known
		wallclock += smallestduration;
		
		// Process all robots with smallestduration as duration
		list<pair<Robot &, int> > updatelist_new;
		for (pair<Robot &, int> & robot : updatelist)
		{
			if (robot.second == smallestduration)
			{
				int duration;
				if (!robot.first.nextstate(wallclock, duration))
					continue;
				updatelist_new.push_back(pair<Robot &, int>(robot.first, duration));
			}
			else
			{
				robot.second -= smallestduration;
				updatelist_new.push_back(robot);
			}
		}

		// World-consistency-check
		set<string> tasks;
		tasks.insert(r1.getCurrentTask());
		tasks.insert(r2.getCurrentTask());
		tasks.insert(r3.getCurrentTask());
		if (tasks.size() != 3)
			cout << "CONFLICT!\n";
		

		if (updatelist_new.empty())
			break;

		updatelist = updatelist_new;
	}
	
	cin.get();
	return 0;

}
