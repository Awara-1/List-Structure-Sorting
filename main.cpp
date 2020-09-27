//**********************************************
//Name: Osama Awara
//Login: oa1612
//e-mail: sam.awara12@imperial.ac.uk
//**********************************************


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h> // include all relevant and required libraries

using namespace std; // For cin cout objects

typedef int Item; // Item used to make program more readable

struct Node   // created structure of an element in the link list
{
	Item pnumber;
	Node *next;
};

typedef Node *NodePtr; // made NodePtr a pointer to a Node
NodePtr hdList = NULL; //Set hdList to a free memory location

bool sort = false;
string number; //Variables repeated different functions  declared as global.

void addToList (Item number, NodePtr &hdList);
//Add the given paramaters as a node to the current linked list

void outputfile(string number, NodePtr &hdList);
//Outputs the link list into a specific output file with relevant index number

void createlinklist (string number, NodePtr &hdList);
//Reads from the input data file with specified index in order to populate a link list

void insertItem(Item pnumber, NodePtr &hdList, bool sort);
//Reads from data file, a specific vallue, that is inserted into the link list
//The orders of the link list is maintained, unless link list is unordered

void deleteItem (Item pnumber, NodePtr &hdList);
// Reads from relevant data file and deletes element according to value specified in CommandFile
//note that the order of the link list is preserved. Also for multiple entries, only the first instance is deleted

void average (NodePtr hdList, string number);
//This	command	adds to the	linked	list, by adding	an	extra node between every
//two	adjacent	nodes of the list with a data value equal to the average value of the data of
//the	two	nodes. The average value is converted to an interger

void findmaximum (NodePtr hdList, string number);
//Finds the minimum value of all the elements in the link list and outputs
// this value to the specified output file

int Threshold (Item T, NodePtr hdList);
//Finds the number of elements in the link list that is greater than a certain threshold value given in the command file

void ThresholdtoFile(string number, Item T, int Thresh);
//Function to allow for insertion of number of elements > threshold value in the output_x file

void bubblesort (NodePtr &hdList, bool &sort);
// Applies the bubblesort algorithm to sort elements of the link list in ascending order




int main(int argc, char* argv[]) //specify command line arguments
{
	string line;
	int newNumber; // used in the insert and delete function

	ifstream myfile; //opens command file
	string temp = argv[1]; // uses a temporary string to define what the argument is

	myfile.open (temp.c_str()); // open the relevant textfile (this will be a command file)
	if (myfile.is_open()) // Check to see if the file succesfully opened / exists in directory
	{

		while(!myfile.eof()) // go through the CommandFile
		{
			myfile>>line;  //read the first line of the command file

			if (line.compare("r") == 0)  // compare is used to look for the specific commands
			{
				myfile>>number; // read the number after 'r', to determine which data file and output file to operate on
                createlinklist(number,hdList); //creates the link list hdList
			}

			if (line.compare("i") == 0)
			{
				myfile>>newNumber; //obtains the value after i in the CommandFile to insert into the link list
                insertItem(newNumber, hdList, sort); // inserts the new node into link list from data file
			}

			if (line.compare("w") == 0)
			{
				outputfile(number, hdList); // outputs link list to relevant output file (output file index corresponds to input file index)
			}

			if (line.compare("d") == 0)
			{
				myfile>>newNumber;
				deleteItem(newNumber, hdList); // deletes relevant node of link list
			}

			if (line.compare("e") == 0)
			{
			    int Thresh =0;// number of values above threshold = Thresh. initialise to 0.

			    myfile >>newNumber; //Take from the CommandFile the threshold limit.
                Thresh = Threshold(newNumber, hdList);

                ThresholdtoFile(number, newNumber, Thresh);

                cout << "Numbers above threshold is:" << Thresh << endl;

			}

			if (line.compare("s") == 0)
			{
				bubblesort(hdList,sort); // sorts the link list in ascending order
			}


			if (line.compare("a") == 0)
			{
				average(hdList, number); // finds the average of the date values of the link list
			}

			if (line.compare("m") == 0)
			{
				findmaximum(hdList, number); // finds the maximum of the data values of the link list
			}
			line=""; // For endlines at the end of the text file.

		}
		myfile.close(); // close file after all its contents have been read.

		NodePtr tempPtr2;
		while (hdList != NULL)
		{
			tempPtr2 = hdList;
			hdList = hdList->next;
			delete tempPtr2; // delete and frees the allocated memory after the program finishes, to save cost.
		}
	}
	else cout << "Unable to open file"; // error message if file failed to open.


}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void addToList (Item number, NodePtr &hdList)
{
	NodePtr newNode = new Node;
	newNode->pnumber = number;
	newNode->next = hdList;  // Puts a new node before hdList (containg Item, pointer)
	hdList = newNode;
}


void outputfile(string number, NodePtr &hdList)
{
	string base = "output_n.txt";
	base.replace(7,1,number); // base is a string with output_(x).txt, x is determined by the string(number)
	ofstream myfile;
	myfile.open(base.c_str(),ios::app);// appends/creates the relevant output file

	NodePtr printout = hdList; //printout is pointer that points to the first element in the list


	while(printout!=NULL) //prints all the elements of the link list into the output file
	{
	    cout << "Output file value is:" << printout->pnumber << endl;
		myfile << printout->pnumber << endl;
		printout=printout->next;
	}
	myfile << "W command has been called!" << endl;
	myfile.close();
}

void ThresholdtoFile(string number, Item T, int Thresh)
{
	string base = "output_n.txt";
	base.replace(7,1,number); // base is a string with output_(x).txt, x is determined by the string(number)
	ofstream myfile;
	myfile.open(base.c_str(),ios::app);// appends/creates the relevant output file

    myfile << "Number of elements with value greater than T="<< T <<  " is:" << Thresh <<  endl;
}

void createlinklist(string number, NodePtr &hdList)
{
	int datanumber = 0; // initialise data number.
	string base = "data_n.txt";
	base.replace(5,1,number); // base is a string with data_(x).txt
	ifstream myfile;
	myfile.open(base.c_str());

	myfile>>datanumber; //reads in the first Item

	if ((datanumber < 1) || (datanumber > 151))
		return;

	hdList = new Node;
	hdList->pnumber = datanumber;
	cout << "Number read into list is:" << datanumber << endl; // used for debugging
	hdList->next = NULL;    // initialising hdList to first element in the datafile

	Node newdata;
	while(!myfile.eof())
	{
		while (myfile>>newdata.pnumber) //retrieves the numbers from data file
		{
		    cout << "Number read into list is:" << newdata.pnumber << endl; //used for debugging
			addToList (newdata.pnumber, hdList); // adds these new elements to the link list
		}
	}
}

void insertItem(Item pnumber, NodePtr &hdList, bool sort)
{
	bool found = false;
	NodePtr searchPtr, lastPtr, newPtr;

	//create node
	newPtr = new Node;
	newPtr->pnumber = pnumber;
	newPtr->next = NULL;

	if (sort == false) // if the link list has not been sorted, just add the new element as the head of the list
	{
		newPtr->next = hdList;
		hdList = newPtr;
	}
	else
	{

		//empty list is a special case
		if (hdList == NULL)
		{
			hdList = newPtr;
			return;
		}
		//insert at head is special too
		else if (hdList->pnumber >= pnumber)
		{
			newPtr->next = hdList;
			hdList = newPtr;
		}

		//normal insertion
		else
		{
			found = false;
			searchPtr = hdList;
			lastPtr = hdList;
			while ((searchPtr != NULL) && (!found))
			{
				if (searchPtr->pnumber >= pnumber)
					found = true;
				else
				{
					lastPtr = searchPtr;
					searchPtr = searchPtr->next;
				}
			}
			newPtr->next = searchPtr;
			lastPtr->next = newPtr;
		}
	}
}

void deleteItem (Item pnumber, NodePtr &hdList)
{
	bool found = false;
	NodePtr searchPtr, lastPtr, oldPtr;

	//empty list is special case
	if (hdList==NULL)
	{
		return;
	}
	else if (hdList->pnumber == pnumber)
	{
		oldPtr = hdList;
		hdList = hdList->next;
		delete oldPtr;
	}
	else
	{
		found = false; // flag used to determine if the Item has been found
		searchPtr = hdList;
		lastPtr = hdList;
		while ((searchPtr != NULL) && (!found))
		{
			if (searchPtr->pnumber == pnumber) // if the search pointer has found the right element
			{
				found = true;
				lastPtr->next = searchPtr->next;
				delete searchPtr;
			}
			else
			{
				lastPtr = searchPtr;
				searchPtr = searchPtr->next; //if not found, keep scanninbg the list
			}
		}
	}
}


void average (NodePtr hdList, string number)
{
	float sum = 0;
	//float count = 0;
	float avg;


	while (hdList !=NULL && hdList->next != NULL)
	{

		sum = 0;
		sum = sum + hdList->pnumber; //sums first data Item of adjacent pair in the link list
		//count++; //counts the number of elements in the list as it traversed
		NodePtr newNode = new Node; //New node to insert for the average (in between the existing links)
		newNode->next = hdList->next; //point the new node to the next link
		hdList->next = newNode; //point the previous node to the new link (the one inserted)

		if(hdList->next != NULL) {
			hdList = newNode->next; //point to the second link to add it to the sum
			sum = sum + hdList->pnumber; //sums second data Item to first of adjacent pair in the link list

			avg = sum / 2;

			avg = int (avg);
			cout << "Average value: " << avg << endl;
			newNode->pnumber = avg;

			//cout << "Average value: " << avg << endl;

		}
	}


}



void findmaximum (NodePtr hdList, string number)
{
	int maximum = hdList->pnumber; //sets the maximum value to the first Item element in the link list
	NodePtr nextPtr = new Node;
	nextPtr = hdList->next;
	while (nextPtr != NULL)
	{
		if ((nextPtr->pnumber) >  (maximum)) // compares the next Item to the previous one
			maximum = nextPtr->pnumber; //if greater, maximum is set to the next Item
		nextPtr = nextPtr->next; //continues tranversing the entire list
	}

	string base = "output_n.txt";
	base.replace(7,1,number); // base is a string with output_(x).txt, replaces the correct number for x
	ofstream myfile; //opens file as an output file
	myfile.open(base.c_str(),ios::app); //opens the file and appends to it
	myfile << "Maximum value: " << maximum << endl; //writes to the file
	myfile.close();
}

int Threshold (Item T, NodePtr hdList)
{
    int count = 0; // initialise count to = 0;
    while(hdList !=NULL && hdList->next != NULL)
    {

        if (hdList->pnumber > T)
        {
            count ++;

        }
        hdList = hdList -> next;
    }
    return count;
}

void bubblesort(NodePtr &hdList, bool &sort)
{
	NodePtr tempPtr = new Node;
	NodePtr lastPtr = new Node;
	NodePtr nextPtr = new Node;
	NodePtr currentPtr = new Node;
	NodePtr previousPtr = new Node;
	NodePtr beforeheaderPtr = new Node; // declare various pointers

	lastPtr = NULL;
	bool flag = false; //indicates when there has been no switch after an entire operation of traversing the list

	beforeheaderPtr->next = hdList;
	hdList = beforeheaderPtr;

	while(!flag)
	{
		currentPtr = beforeheaderPtr->next;
		previousPtr = beforeheaderPtr;
		nextPtr = currentPtr->next;
		flag = true;

		while(nextPtr!=lastPtr)
		{
			if((currentPtr->pnumber) > (nextPtr->pnumber)) //if the current Item is larger than the next, swap them
			{
				tempPtr = nextPtr->next;
				nextPtr->next = currentPtr;
				previousPtr->next = nextPtr;
				currentPtr->next = tempPtr;
				previousPtr = nextPtr;
				nextPtr = tempPtr;

				flag = false;
			}
			else
			{

				nextPtr = nextPtr->next; // these pointer are then shifted one position to continue the sorting
				currentPtr = currentPtr->next;
				previousPtr = previousPtr->next;
			}
		}
		lastPtr = currentPtr;
	}
	hdList = hdList->next;
	delete beforeheaderPtr;
	sort = true;

}
