#Peter Lindee

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

//struct for holding the cities name, their message, and a pointer to the next city
struct city{
	string cityName;
	string message;
	city *next;

	city(){};

	city(string name, city *next, string message){
		cityName = name;
		next = next;
		message = message;
	}
};

//builds my linked list by initializing new cities and connecting them with next pointer
city* buildNetwork(){
    city *node1 = new city;
	node1->cityName = "Los Angeles";
	city* head = node1;
	city* node2 = new city;
	node2->cityName = "Phoenix";
	node1->next = node2; //head or node1?
	city* node3 = new city;
	node3->cityName = "Denver";
	node2->next = node3;
	city* node4 = new city;
	node4->cityName = "Dallas";
	node3->next = node4;
	city* node5 = new city;
	node5->cityName = "St. Louis";
	node4->next = node5;
	city* node6 = new city;
	node6->cityName = "Chicago";
	node5->next = node6;
	city *node7 = new city;
	node7->cityName = "Atlanta";
	node6->next = node7;
	city *node8 = new city;
	node8->cityName = "Washington, D.C.";
	node7->next = node8;
	city* node9 = new city;
	node9->cityName = "New York";
	node8->next = node9;
	city* node10 = new city;
	node10->cityName = "Boston";
	node9->next = node10;
	node10->next = NULL;
	return head;

}

//addCity add's a city to my linked list based on user input from main. If head is NULL,
//a new node is created. If the previous node is null, then a new node is created 
//that points to the head, If neither of these cases are true, then the new city is 
//inserted after the previous city.
city* addCity(city *head, city *previous, string cityName){
	city* newNode = new city;
	if(head==NULL){
		newNode->cityName = cityName;
		newNode->next = NULL;
		head = newNode;
	}
	else if(previous==NULL){
		newNode->next = head;
		newNode->cityName = cityName;
		head = newNode;
	}
    else{
       	newNode->next = previous->next;
       	newNode->cityName = cityName;
        previous->next = newNode;
    }
    return head;
}

//transmitMsg reads in a .txt file and sends each word as messages throughout my linked list.
//After a word is received, it is deleted before being set to the next node.
void transmitMsg(city *head){
	string line;
	city* currentNode = head;
	ifstream myFile;
	myFile.open("messageIn.txt");
	if(myFile.fail()){
		cout << "File failed to open" << endl;
	}
	else{
		while((myFile >> line)){
			while(currentNode != NULL){
				currentNode->message = line;
				cout << currentNode->cityName << " received " << currentNode -> message << endl;
				currentNode->message = "";
				currentNode = currentNode -> next;
			}
			currentNode = head;
		}
	}
} 

//prints linked list
void printPath(city *head){
	cout << "===CURRENT PATH===" << endl;
		city* currentNode = head;
		while(currentNode != NULL){
			cout << currentNode->cityName << " -> ";
			currentNode = currentNode->next;
		}
		if(currentNode == NULL){
			cout << " NULL" << endl;
		}
	cout << "==================" << endl;
}

//user interface for building and manipulating the linked list of cities. Continues
//to loop until user quits. Option 1 builds the linked list. Option 2 prints it. 
//Option 3 transmits the message. Option 4 gives user ability to insert a city into the
//linked list. When the previous city is inputted, the linked list is searched to find its position.
int main(){
	city *previousCity = new city;
	city *currentNode = new city;
	string newCity;
	int optionChoice;
	city* head = NULL;

	cout << "======Main Menu======" << endl;
	cout << "1. Build Network" << endl;
	cout << "2. Print Network Path" << endl;
	cout << "3. Transmit Message Coast-To-Coast" << endl;
	cout << "4. Add City" << endl;
	cout << "5. Quit" << endl;

	cin >> optionChoice;

	while(optionChoice != 9){
		
		if(optionChoice == 1){
			head = buildNetwork(); 
		}
		if(optionChoice == 2){
			printPath(head);
		}
		if(optionChoice == 3){
			transmitMsg(head);
		}
		if(optionChoice == 4){
			cout << "Enter a City name:" << endl;
			cin >> newCity;
			cout << "Enter a previous City name:" << endl;
			cin >> previousCity->cityName;
			currentNode = head;
			while(currentNode != NULL){
				if(currentNode->cityName == previousCity->cityName){
					break;
				}
				currentNode = currentNode->next;
			}
			addCity(head, currentNode, newCity);
			previousCity = new city;
		}
		if(optionChoice == 5){
			cout << "Goodbye!" << endl;
			break;
		}
		cout << "======Main Menu======" << endl;
		cout << "1. Build Network" << endl;
		cout << "2. Print Network Path" << endl;
		cout << "3. Transmit Message Coast-To-Coast" << endl;
		cout << "4. Add City" << endl;
		cout << "5. Quit" << endl;

		cin >> optionChoice;

	}
}
