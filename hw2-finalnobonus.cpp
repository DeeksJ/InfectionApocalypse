#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cmath>

using namespace std;

class Person{
	private:
		char status;						//S, E, I, R, S
		int timeInfect = 0;						//Time while I (infectious period)
//		int timeRec;						//Time being at R (recovered period; for bonus)
//		int latency;						//Latency/Exposed period between S and I (for bonus)
		int infectThresh = 0;					//How many infected are nearby
	public:
//---------------------------Setters-------------------------------------
		void setStatus(char health)
			{ status = health; }
		void setTimeInfect(int time)
			{ timeInfect = time; }
//		void setTimeRec(int rec)
//			{ timeRec = rec; }
//		void setLatency(int late)
//			{ latency = late; }
		void setInfectThresh(int infT)
			{ infectThresh = infT; }
//---------------------------Getters-------------------------------------
		char getStatus()
			{ return status; }
		int getTimeInfect()
			{ return timeInfect; }
//		int getTimeRec()
//			{ return timeRec; }
//		int getLatency()
//			{ return latency; }
		int getInfectThresh()
			{ return infectThresh; }
	};

void loadFile(char choice, string file, int& threshold, int& infectPeriod, int& display, int& maxDays, vector<vector<Person> > &land, int& size, int& infectCount);
void run(vector<vector<Person> > &land, int& infectCount, int size, int threshold, int infectPeriod, vector<int> &peakInfect);
void print(vector<vector<Person> >&land, int size);

int main(){

	int threshold, infectPeriod, display, x, y, maxDays, size, infectCount, bigInfect, bigInfectHolder, peakInfectDay;
	int numSus = 0;
	int numRec = 0;
	int numVac = 0;
	int dayCounter = 0;
	string file;
	char choice = 'h';

//	Make 2D vector, pass into load function
	vector<vector<Person> > land;					//Creates a 2D vector called land
	vector<int> peakInfect;

	cout << "What type of simulation would you like to run?\n - (a) SIR\n - (b) SEIRS\n - (q) TO QUIT" << endl;


	cin >> choice;

	while(choice != 'a' && choice != 'b' && choice != 'q')
	{
		cout << "Invalid choice... \n\n"
		"What type of simulation would you like to run?\n - (a) SIR\n - (b) SEIRS\n - (q) TO QUIT" << endl;

		cin >> choice;
	}

	if(choice == 'a')
	{
		cout << "Enter the name of the file you would like to open: " << endl;
		cin >> file;

		loadFile(choice, file, threshold, infectPeriod, display, maxDays, land, size, infectCount);
	}
	else if(choice == 'b')
	{
		cout << "Are you sure?" << endl;
	}
	else if(choice == 'q')
	{
		cout << "Exiting program" << endl;
		return 0;
	}


	cout << "Infection on Day 0" << endl;
	print(land, size);

	cout << endl;

	while(infectCount != 0)
	{
		infectCount = 0;

		run(land, infectCount, size, threshold, infectPeriod, peakInfect);

		if((dayCounter % display == 0) && (dayCounter != 0))
		{
			cout << "Infection on Day: " << dayCounter << endl;
			print(land, size);
			cout << endl;
		}

		dayCounter++;
	}

	bigInfect = 0;

	for(unsigned int i = 0; i < peakInfect.size(); i++)
	{
		bigInfectHolder = peakInfect.at(i);

		if(bigInfect < bigInfectHolder)
		{
			bigInfect = bigInfectHolder;
			peakInfectDay = i;
		}
	}

	for(int i = 0; i < (size*2 - 1); i++)
	{
		cout << "-";
	}

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			switch(land[i].at(j).getStatus())
			{
				case 'v':
					numVac++;
					break;
				case 'r':
					numRec++;
					break;
				case 's':
					numSus++;
					break;
			}
		}
	}

	cout << "\n\nFinal Day: " << dayCounter << endl;
	cout << "Peak Infection Amount: " << bigInfect << endl;
	cout << "Day of Peak Infection: " << peakInfectDay << endl;
	cout << "Final Counts: " << endl;
	cout << " - Susceptible Peoples : " << numSus << endl;
	cout << " - Recovered Peoples   : " << numRec << endl;
	cout << " - Vaccinated Peoples  : " << numVac << endl;
	cout << " - Infected Peoples    : " << infectCount << endl;
	cout << "\nFinal Grid" << endl;
	print(land, size);

	return 0;
}

void loadFile(char choice, string file, int& threshold, int& infectPeriod, int& display, int& maxDays, vector<vector<Person> > &land, int& size, int& infectCount)		//loads a file for SIR running
{
	char people, personHold;
	int counter = 0;
	string firstLines;
	ifstream inf;
	int stringSize;
	Person pusher;
	vector <Person> temp;
	int numi = 0;
	int nums = 0;
	int numv = 0;
	int numr = 0;

	land.clear();

	inf.open(file.c_str());						//replae with the file name when finishing
	if(inf.fail())
	{
		cout << "File \"" << file << "\" failed to open.\nExiting program..." << endl;
		EXIT_FAILURE;

		return;
	}

	while(counter < 3)						//Possible way to load the input. Uses
	{								//size_t to find the position before the :
		getline(inf, firstLines, ':');				//the puts the number in an int

		switch(counter){
			case 0:
				inf >> threshold;
				cout << "Threshold:" << threshold << endl;
				break;
			case 1:
				inf >> infectPeriod;
				cout << "infectPeriod:" << infectPeriod << endl;
				break;
			case 2:
				inf >> display;
				cout << "Display:" << display << endl;
				break;
		}
		counter++;
	}
	cin.ignore();

	counter = 0;
	inf.get(personHold);

	if(choice == 'a')
	{
		do
		{
			inf.get(personHold);
			//cout << personHold << "-" << endl;

			if(personHold == '\n')
                        {
				land.push_back(temp);
				temp.clear();
                        }
			else if(personHold != ',')
			{
				pusher.setStatus(personHold);
				temp.push_back(pusher);

				counter++;

				switch(personHold)
				{
					case 's':
						nums++;
						break;
					case 'i':
						numi++;
						break;
					case 'v':
						numv++;
						break;
					case 'r':
						numr++;
						break;
				}
			}

		}while(!inf.eof());

/*		cout << "Intial Suseptible: " << nums << endl;
		cout << "Intial Infected  : " << numi << endl;
		cout << "Intial Vaccinated: " << numv << endl;
		cout << "Intial Recovered : " << numr << endl;
*/

		infectCount = numi;
		size = sqrt(nums + numi + numv + numr);
	}

	inf.close();

	return;
}

void print(vector<vector<Person> >&land, int size)
{
	for(int i = 0; i < size;i++)
	{
		for(int j = 0; j < size;j++)
		{
			cout << land[i].at(j).getStatus() << " ";
		}
		cout << endl;
	}

	return;
}

void run(vector<vector<Person> > &land, int& infectCount, int size, int threshold, int infectPeriod, vector<int> &peakInfect)
{
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			if(land[i].at(j).getStatus() == 'i')
			{
				if((i + 1 < size) && (i - 1 > -1))
				{
					if(j + 1 == size)
					{
						land[i].at(0).setInfectThresh(land[i].at(0).getInfectThresh() + 1);
						land[i - 1].at(0).setInfectThresh(land[i].at(0).getInfectThresh() + 1);
						land[i + 1].at(0).setInfectThresh(land[i].at(0).getInfectThresh() + 1);
                        land[i - 1].at(j).setInfectThresh(land[i - 1].at(j).getInfectThresh() + 1);;
                        land[i - 1].at(j - 1).setInfectThresh(land[i - 1].at(j - 1).getInfectThresh() + 1);
                        land[i + 1].at(j - 1).setInfectThresh(land[i + 1].at(j - 1).getInfectThresh() + 1);
                        land[i + 1].at(j).setInfectThresh(land[i + 1].at(j).getInfectThresh() + 1);
						land[i].at(j - 1).setInfectThresh(land[i].at(j - 1).getInfectThresh() + 1);
					}
					else if(j - 1 < 0)
					{
						land[i].at(size - 1).setInfectThresh(land[i].at(size - 1).getInfectThresh() + 1);
						land[i - 1].at(size - 1).setInfectThresh(land[i - 1].at(size - 1).getInfectThresh() + 1);;
						land[i + 1].at(size - 1).setInfectThresh(land[i - 1].at(size - 1).getInfectThresh() + 1);
                        land[i - 1].at(j).setInfectThresh(land[i - 1].at(j).getInfectThresh() + 1);
                        land[i - 1].at(j + 1).setInfectThresh(land[i - 1].at(j + 1).getInfectThresh() + 1);
                        land[i].at(j + 1).setInfectThresh(land[i].at(j + 1).getInfectThresh() + 1);
                        land[i + 1].at(j + 1).setInfectThresh(land[i + 1].at(j + 1).getInfectThresh() + 1);
                        land[i + 1].at(j).setInfectThresh(land[i + 1].at(j).getInfectThresh() + 1);
					}
					else
					{
						land[i - 1].at(j).setInfectThresh(land[i - 1].at(j).getInfectThresh() + 1);
                        land[i - 1].at(j + 1).setInfectThresh(land[i - 1].at(j + 1).getInfectThresh() + 1);
                        land[i].at(j + 1).setInfectThresh(land[i].at(j + 1).getInfectThresh() + 1);
                        land[i + 1].at(j + 1).setInfectThresh(land[i + 1].at(j + 1).getInfectThresh() + 1);
                        land[i + 1].at(j).setInfectThresh(land[i + 1].at(j).getInfectThresh() + 1);
                        land[i + 1].at(j - 1).setInfectThresh(land[i + 1].at(j - 1).getInfectThresh() + 1);
                        land[i].at(j - 1).setInfectThresh(land[i].at(j - 1).getInfectThresh() + 1);
                        land[i - 1].at(j - 1).setInfectThresh(land[i - 1].at(j - 1).getInfectThresh() + 1);
					}
				}
			}
		}
	}
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			if(land[i].at(j).getStatus() == 'i')
			{
				if(land[i].at(j).getTimeInfect() == infectPeriod - 1)
				{
					land[i].at(j).setStatus('r');
				}
				else
				{
					land[i].at(j).setTimeInfect(land[i].at(j).getTimeInfect() + 1);
					infectCount++;
				}
			}
			if(land[i].at(j).getInfectThresh() >= threshold)
			{
				if(land[i].at(j).getStatus() == 's')
				{
					land[i].at(j).setStatus('i');
					infectCount++;
				}
			}

			land[i].at(j).setInfectThresh(0);
		}
	}

	peakInfect.push_back(infectCount);

	return;
}
