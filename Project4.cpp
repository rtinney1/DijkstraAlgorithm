/*
	Dijkstra's Algorithm. This program uses Dijkstra's Algorithm to determine the shortest path
	possible between cities. The cities are read in from a file that includes a four(4) character
	abreviation of the city and the upper triangle of the adjacency matrix associated with it.
	Along with main(), this program uses nine(9) other functions along with a struct.
	
	Programmer: Randi Tinney
	Date Last Modified: 01 Dec 2016
*/

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <ctype.h>
#include <iomanip>

using namespace std;

struct Node
{
	char name[4];
	int previous;
	int weight;
	bool inP;
};

void check(char* fileName);
void getWeights(char* filename, int adjMatrix[][10], int& size, Node arrayOfPoints[]);
void start(char* startPoint, int size, int adjMatrix[][10], Node arrayOfPoints[], int& locationOfStart);
void dijkstra(char* startPoint, int size, int adjMatrix[][10], Node arrayOfPoints[], int& locationOfStart);
int findLowest(int size, Node arrayOfPoints[]);
void showAdjMatrix(int size, int adjMatrix[][10], Node arrayOfPoints[]);
void figurePath(Node node, Node arrayOfPoints[]);
void data(int size, Node arrayOfPoints[], int locationOfStart);
void toUpper(char* array, int size);

ifstream cityFile;

int main()
{
	char filename[51];
	char startPoint[4];
	int size = 0;
	int  adjMatrix[10][10];
	Node* arrayOfPoints = new Node[46];
	int locationOfStart;
	
	cout << "Enter name of file" << endl;
	cin >> filename;
	
	cityFile.open(filename);
	
	check(filename);
	
	getWeights(filename, adjMatrix, size, arrayOfPoints);
	
	cout << "Choose starting city" << endl;
	cin >> startPoint;
		
	toUpper(startPoint, 4);
	
	dijkstra(startPoint, size, adjMatrix, arrayOfPoints, locationOfStart);	
	
	showAdjMatrix(size, adjMatrix, arrayOfPoints);
	
	data(size, arrayOfPoints, locationOfStart);	
	
	cityFile.close();
}//end main()

/*toUpper()
	Changes a char array to all upper case
*/
void toUpper(char* array, int size)
{
	int n = 0;
	char c;
	
	while(n < size)
	{
		c = array[n];
    	array[n] = toupper(c);
    	n++;
	}
}//end toUpper()

/*data()
	This method shows the overall data after Dijkstra's Algorithm has been performed.
	Outputs the shortest distance from the starting city to all the cities and the path
	one would use to get there.
*/
void data(int size, Node arrayOfPoints[], int locationOfStart)
{
	for(int i = 0; i < size; i++)
	{
		if(i != locationOfStart)
		{
			system("PAUSE");
		
			cout << "****************************" << endl;
			
			cout << "The distance from " << arrayOfPoints[locationOfStart].name << " to " << arrayOfPoints[i].name << " is: " << arrayOfPoints[i].weight << endl; 
			
			figurePath(arrayOfPoints[i], arrayOfPoints);
			
			cout << endl;
		}	
	}
}//end data()

/*figurePath()
	A recursive functions that finds the shortest path from the starting city to the
	current city.
*/
void figurePath(Node node, Node arrayOfPoints[])
{
	if(node.weight > 0)
	{
		figurePath(arrayOfPoints[node.previous], arrayOfPoints);
		cout << " to " << node.name;
	}
	else
		cout << node.name << " ";
}//end figurePath()

/*showAdjMatrix()
	This method simply shows the adjacency matrix that is constructed from the file.
*/
void showAdjMatrix(int size, int adjMatrix[][10], Node arrayOfPoints[])
{
	cout << "     ";
	
	for(int i = 0; i < size; i++)
	{
		cout << setw(10)  << arrayOfPoints[i].name;
	}
	
	cout << endl;
	
	for(int n = 0; n < size; n++)
	{
		cout << arrayOfPoints[n].name;
		
		for(int m = 0; m < size; m++)
		{
			cout << setw(10) << adjMatrix[n][m];	
		}
		
		cout << endl;
	}
}//end showAdjMatrix()

/*start()
	The start of Dijkstra's Algorithm. Finds the location of the starting city as indicated
	by the user and sets previous to -1, places it into the set "P" and gives it a weight of 0.
	It then runs through the rest of the cities and adjusts their weights and previous accordingly.
*/
void start(char* startPoint, int size, int adjMatrix[][10], Node arrayOfPoints[], int& locationOfStart)
{	
	for(int i = 0; i < size; i++)
	{
		if(strcmp(startPoint, arrayOfPoints[i].name) == 0)
			locationOfStart = i;
	}
	
	arrayOfPoints[locationOfStart].previous = -1;
	arrayOfPoints[locationOfStart].inP = true;
	arrayOfPoints[locationOfStart].weight = 0;
	
	for(int i = 0; i < size; i++)
	{
		if(i != locationOfStart)
		{
			arrayOfPoints[i].previous = locationOfStart;
			arrayOfPoints[i].weight = adjMatrix[locationOfStart][i];
			arrayOfPoints[i].inP = false;
		}
	}
}//end start()

/*dijkstra()
	This method performs all of Dijkstra's algorithm. Calls on start()
	and findLowest()
*/
void dijkstra(char* startPoint, int size, int adjMatrix[][10], Node arrayOfPoints[], int& locationOfStart)
{
	int locationOfLowest;
	int tillDone = 0;
	
	start(startPoint, size, adjMatrix, arrayOfPoints, locationOfStart);
	
	do
	{
		locationOfLowest = findLowest(size, arrayOfPoints);
		
		arrayOfPoints[locationOfLowest].inP = true;
		
		for(int i = 0; i < size; i++)
		{
			if(!arrayOfPoints[i].inP && adjMatrix[locationOfLowest][i] < 999)
			{
				if(arrayOfPoints[i].weight > adjMatrix[locationOfLowest][i] + arrayOfPoints[locationOfLowest].weight)
				{
					arrayOfPoints[i].weight = adjMatrix[locationOfLowest][i] + arrayOfPoints[locationOfLowest].weight;
					arrayOfPoints[i].previous = locationOfLowest;
				}
			}
		}
		
		tillDone++;	
		
	}while(tillDone < size);	
}//end dijkstra()

/*findLowest()
	returns the location of the city with the lowest weight
*/
int findLowest(int size, Node arrayOfPoints[])
{
	int lowest = 999;
	int location;
	
	for(int i = 0; i < size; i++)
	{
		if(!arrayOfPoints[i].inP && arrayOfPoints[i].weight < lowest)
		{
			lowest = arrayOfPoints[i].weight;
			location = i;
		}
	}
	
	return location;
}//end findLowest()

/*getWeights()
	Obtains the names of the cities and the accompanied weights for each path. Places
	the weights in the adjacency matrix and changes all 0's (both for no path and no
	cycles) to 999 to indicate infinity (no path).
*/
void getWeights(char* filename, int adjMatrix[][10], int& size, Node arrayOfPoints[])
{
	char cityName[4];
	int weight;
	int weights[46];
	int x = 0;
	int i = 0;
	
	while(!cityFile.eof())
	{
		//cout << "WE good to begin" << endl;
		
		if(!isdigit(cityFile.peek()))//to see if we are still reading city names
		{
			//cout << "getting name" << endl;
			cityFile.getline(arrayOfPoints[x].name, 5);
			
			toUpper(arrayOfPoints[x].name, 4);
			
			//cout << "YOOOOOOO" << endl;
			
			//strcpy(arrayOfPoints[x].name, cityName);
			//cout << names[x][i];
			x++;
			size++;
			cityFile >> ws;	
		}
		else
		{	
			//cout << "getting numbers" << endl;
			cityFile >> weights[i] >> ws;
			if(weights[i] == 0)
				weights[i] = 999;
			//cout << weights[i] << endl;
			i++;
		}
	}
	
	//placing weights in adjacency matrix
	for(int n = 0, x = 0, i = 1; n < size; n++, i++)
	{
		for(int m = 0; m < size; m++)
		{
			if(n == m)//city cannot connect to itself
				adjMatrix[n][m] = 999;
			else if(m >= i)//this is so we only work with the upper triangle portion
			{
				adjMatrix[n][m] = weights[x];
				adjMatrix[m][n] = weights[x];
				x++;
			}
			//cout << adjMatrix[n][m] << " ";
		}
	}
}//end getWeights()

/*check()
	Checks to see if the file is an actual file
*/
void check(char* fileName)
{
	while (!cityFile)
	{
		cout << "Error opening file: " << fileName << endl;
		cout << "Please try again." << endl;
		cityFile.clear();
		cout << "Enter name of file: ";
		cin >> fileName;
		cout << endl;
		
		cityFile.open(fileName);
	}	
}//end Check()
