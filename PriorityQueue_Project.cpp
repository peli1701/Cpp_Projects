#Peter Lindee

#include <iostream>
#include <fstream>
#include <sstream> 
#include <chrono>
using namespace std;

struct LLitem{
	string name;
	int priority;
	int treatment;
	LLitem* next;

	LLitem(){};

	LLitem(string name1, int priority1, int treatment1){
		name = name1;
		priority = priority1;
		treatment = treatment1;
		next = nullptr;
	}
};

class LLQueue{

	public:
		LLQueue();
		~LLQueue();
		void buildLLQueue();
		LLitem* enqueueLL(string, int, int);
		LLitem* dequeueLL();
		void printQueue();

	private:
		LLitem* head;
		
};

LLQueue::LLQueue(){

}

LLQueue::~LLQueue(){

}

void LLQueue::buildLLQueue(){
	
	ifstream myFile;
	LLQueue instance;
	string line, name, priority, treatment;
	int count = 0;
	int index = 0;
	
	myFile.open("patientData2270.csv"); //fix this
	if(myFile.fail()){
		cout << "file issues" << endl;
	}
	else{
		while(myFile >> line && index <= 100){
			if(count == 0){
				count++; //skip first line of file --> do not care header information
				continue;
			}
			stringstream ss(line);
			getline(ss, name, ',');
			getline(ss, priority, ',');
			getline(ss, treatment);
			enqueueLL(name, stoi(priority), stoi(treatment));
			index++;
		}
	}
}

LLitem* LLQueue::enqueueLL(string name, int priority, int treatment){
	LLitem* newNode = new LLitem(name, priority, treatment);
	LLitem* previous = newNode;
	if(head == nullptr){
		head = newNode;
		return head;
	}
	else{
		LLitem* temp = head;
		while(temp != nullptr){
			if(priority < temp->priority){ //if priority is less that an item in linked list, break and insert their
				if(temp == head){ //checks for if that node is the head.
					head = newNode;
					newNode->next = temp;
					return newNode;
				}
				break;
			}	
			else if(priority == temp->priority && treatment < temp->treatment){ //if priority is equal BUT treatment is less, break and insert their
				if(temp == head){
					head = newNode;
					newNode->next = temp;
					return newNode;
				}
				break;
			}
			previous = temp;
			temp = temp->next;
		}
		newNode->next = previous->next;
		previous->next = newNode;
	}
	return head;
}

LLitem* LLQueue::dequeueLL(){

	if(head == NULL){
		//cout << "null" << endl;
		return nullptr;
	}
	else if(head->next == nullptr){
		cout << head->name << endl;
		delete head;
		return nullptr;
	}

	else{
		LLitem* node = head;
		LLitem* temp;
		while(node != nullptr){
			cout << "deleting: " << node->name << endl;
			temp = node->next;
			//cout << node->name << endl;
			delete node;
			node = temp;
		}
		head = nullptr;
		return head;
	}
	//return nullptr;

}

void LLQueue::printQueue(){
	LLitem* temp = head;
	int queuePos = 1;
	if(temp == nullptr){
		cout << "Linked List is NULL" << endl;
	}
	else{
		cout << "Rank  " << "patient, " << "Priority, " << "Treatment" << endl;
		while(temp != nullptr){
			cout << queuePos << ":   "  << temp->name << ", " << temp->priority << ", " << temp->treatment << endl;
			temp = temp->next; 
			queuePos++;
		}
	}
}



int main(int argc, char* argv[]){
	ofstream myFile;
	myFile.open("LLtest.csv");
	int numRunThroughs = 500;
	//for(int i = 0; i < numRunThroughs; i++){
		//auto start = chrono::system_clock::now();
		LLQueue LLinstance;
		LLinstance.buildLLQueue();
		//LLinstance.printQueue();
		LLinstance.dequeueLL();
		//auto end = chrono::system_clock::now();
		//auto timeElapsed = chrono::duration<float>(end - start);
		//cout << timeElapsed.count() << endl;
		//myFile << timeElapsed.count() << endl;
	//}
}


