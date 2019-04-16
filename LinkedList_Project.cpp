//Peter Lindee
//Communication Network


#include <iostream>
#include <fstream>
using namespace std;

struct City{
    std::string cityName;
    std::string message;
    City *next;
    City *previous;

    City(){}; // default constructor

    City(std::string initName, City *initNext, City *initPrevious, std::string initMessage)
    {
        cityName = initName;
        next = initNext;
        previous = initPrevious;
        message = initMessage;
    }
};

class CommunicationNetwork
{
    
public:
    CommunicationNetwork();
    ~CommunicationNetwork();
    void addCity(std::string, std::string);
    void buildNetwork();
    void transmitMsg(char *); //this is like a string
    void printNetwork();
    void deleteCity(std::string);
    void deleteNetwork();

protected:

private:
    City *head;
    City *tail;
};

int main(int argc, char* argv[]){

	CommunicationNetwork instance;
	City* currentNode;
	int optionChoice;

	cout << "======Main Menu======" << endl;
	cout << "1. Build Network" << endl;
	cout << "2. Print Network Path" << endl;
	cout << "3. Transmit Message Coast-To-Coast" << endl;
	cout << "4. Add City" << endl;
	cout << "5. Delete City" << endl;
	cout << "6. Clear Network" << endl;
	cout << "7. Quit" << endl;

	cin >> optionChoice;

	while(optionChoice != 9){
		
		if(optionChoice == 1){
			instance.buildNetwork(); 
			//insta.printNetwork();
		}
		if(optionChoice == 2){
			instance.printNetwork();
		}
		if(optionChoice == 3){
			instance.transmitMsg(argv[1]);
		}
		if(optionChoice == 4){
			cin.ignore();
			string newCityName;
			string previousCityName;
			cout << "Enter a city name:" << endl;
		    getline(cin,newCityName);
		    cout << "Enter a previous city name:" << endl;
		    getline(cin,previousCityName);
		    instance.addCity(newCityName, previousCityName);		
		}
		if(optionChoice == 5){
			cin.ignore();
			string removeCity;
			cout << "Enter a city name:" << endl;
			getline(cin,removeCity);
			instance.deleteCity(removeCity);
		}
		if(optionChoice == 6){
			instance.deleteNetwork();
		}
		if(optionChoice == 7){
			cout << "Goodbye!" << endl;
			break;
		}
	
		cout << "======Main Menu======" << endl;
		cout << "1. Build Network" << endl;
		cout << "2. Print Network Path" << endl;
		cout << "3. Transmit Message Coast-To-Coast" << endl;
		cout << "4. Add City" << endl;
		cout << "5. Delete City" << endl;
		cout << "6. Clear Network" << endl;
		cout << "7. Quit" << endl;

		cin >> optionChoice;

	}
}

CommunicationNetwork::CommunicationNetwork(){
    //default constructor
}

CommunicationNetwork::~CommunicationNetwork(){
    deleteNetwork();
    head = nullptr;
    tail = nullptr;
}

void CommunicationNetwork::addCity(string newCityName, string previousCityName){
   City* newNode = new City;
   bool found = false;
   newNode->cityName = newCityName;
   
   if(head == nullptr){
       head = newNode;
       tail = newNode;
   }
   
   City* currentNode = head;
   while(currentNode != nullptr){
       if(currentNode->cityName == previousCityName){
           found = true;
           break;
       }
       currentNode = currentNode->next;
   }
   
   if(previousCityName == "First"){
      newNode->next = head;
      newNode->previous = nullptr;
      head = newNode;
   }
   else if(previousCityName == ""){
       tail->next = newNode;
       newNode->next = nullptr;
       tail = newNode;
   }
   else{
       newNode->next = currentNode->next;
       newNode->previous = currentNode;
       currentNode->next = newNode;
   }
   
}

void CommunicationNetwork::transmitMsg(char* filename){
    string line;
    City* currentNode = head;
    ifstream myFile;
    myFile.open(filename);
    if(myFile.fail()){
        cout << "File failed to open" << endl;
    }
    else{
        while((myFile >> line)){
            while(currentNode->next != nullptr){
                currentNode->message = line;
                cout << currentNode->cityName << " received " << currentNode -> message << endl;
                currentNode->message = "";
                currentNode = currentNode -> next;
            }
            currentNode = tail;
            while(currentNode->previous != nullptr){
                currentNode->message = line;
                cout << currentNode->cityName << " received " << currentNode -> message << endl;
                currentNode->message = "";
                currentNode = currentNode -> previous;
            }
            currentNode = head;
            currentNode->message = line;
            cout << currentNode->cityName << " received " << currentNode -> message << endl;
        }
    }
}

void CommunicationNetwork::printNetwork(){
    cout << "===CURRENT PATH===" << endl;
    if(head == nullptr){
        cout << "empty list" << endl;
    }
    else{
        City* currentNode = head;
        cout << "NULL <- ";
        //skips last node so that cout statements(arrows)would be correct. 
        while(currentNode->next != nullptr){
            cout << currentNode->cityName << " <-> ";
            currentNode = currentNode->next;
        }
        cout << currentNode->cityName << " -> NULL" << endl;
    }
    cout << "==================" << endl;
}

void CommunicationNetwork::buildNetwork(){
    City *node1 = new City;
    node1->cityName = "Los Angeles";
    node1->previous = nullptr;
    node1->message = "";
    City* node2 = new City;
    node2->cityName = "Phoenix";
    node2->previous = node1;
    node2->message = "";
    node1->next = node2; 
    City* node3 = new City;
    node3->cityName = "Denver";
    node3->previous = node2;
    node3->message = "";
    node2->next = node3;
    City* node4 = new City;
    node4->cityName = "Dallas";
    node4->previous = node3;
    node4->message = "";
    node3->next = node4;
    City* node5 = new City;
    node5->cityName = "St. Louis";
    node5->previous = node4;
    node5->message = "";
    node4->next = node5;
    City* node6 = new City;
    node6->cityName = "Chicago";
    node6->previous = node5;
    node6->message = "";
    node5->next = node6;
    City *node7 = new City;
    node7->cityName = "Atlanta";
    node7->previous = node6;
    node7->message = "";
    node6->next = node7;
    City *node8 = new City;
    node8->cityName = "Washington, D.C.";
    node8->previous = node7;
    node8->message = "";
    node7->next = node8;
    City* node9 = new City;
    node9->cityName = "New York";
    node9->previous = node8;
    node9->message = "";
    node8->next = node9;
    City* node10 = new City;
    node10->cityName = "Boston";
    node10->previous = node9;
    node10->message = "";
    node9->next = node10;
    node10->next = nullptr;

    head = node1; //head
    tail = node10; //tail
}

void CommunicationNetwork::deleteCity(string removeCity){
    City* currentNode = head;
    //cout << currentNode->cityName << endl;
    bool found = false;
    while(currentNode != nullptr){
        if(currentNode->cityName == removeCity){
            found = true;
            break;
        }
        currentNode = currentNode->next;
    }
    if(found == true){
        if(currentNode == head){
            head = head->next;
            head->previous = nullptr;
            delete currentNode;
        }
        else{
            if(currentNode->next == nullptr){
                currentNode->next = nullptr;
                currentNode->previous->next = nullptr;
                tail = currentNode->previous;
                delete currentNode;
            }
            else{
                currentNode->previous->next = currentNode->next;
                currentNode->next->previous = currentNode->previous;
                delete currentNode;
            }
        }
    }
    else{
        cout << removeCity << " not found" << endl;
    }
}

void CommunicationNetwork::deleteNetwork(){
    City* currentNode = head;
    City* temp = new City;
    while(currentNode != nullptr){
        cout << "Deleting " << currentNode->cityName << endl;
        temp = currentNode->next;
        delete currentNode;
        currentNode = temp;
    }
    head = nullptr;
    tail = nullptr;
}