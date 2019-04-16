#include <iostream>
#include <queue>
#include <chrono>
#include <fstream>
#include <sstream>
using namespace std;

struct priorityItem{
	string itemName;
	int primaryPriority;
	int secondaryPriority;
};

class PriorityQueue{
	
	public:
		PriorityQueue();
		~PriorityQueue();
		bool enqueue(string, int, int); 
		bool dequeue();
		void buildQueue();
		void printPQ();
		bool peek();
		void dequeueAll();

	private:
		int lastIndex = 0;
		int maxIndex = 880;
		priorityItem* pQueue[880];

};

PriorityQueue::PriorityQueue(){
	for(int i = 0; i < maxIndex; i++){
		pQueue[i] = nullptr;
	}
}

PriorityQueue::~PriorityQueue(){
	for(int i = 1; i < maxIndex; i++){
		delete pQueue[i];
	}
}

bool PriorityQueue::enqueue(string thingName, int primP, int secP){
	//if you wanted to dynamically resize the heap, extra points for doing so.
	priorityItem* itemToAdd = new priorityItem; 
	if(lastIndex < maxIndex){
		//priorityItem* itemToAdd = new priorityItem;
		itemToAdd->itemName = thingName;
		itemToAdd->primaryPriority = primP;
		itemToAdd->secondaryPriority = secP;
		lastIndex++;
		//put the new thing at last index
		pQueue[lastIndex] = itemToAdd;
		//reheap up
		int currentIndex = lastIndex;
		bool itemSwapped = true;
		while(currentIndex > 1 && itemSwapped){ //while we're not at the root and we havent swapped and item
			//compare thing at currentIndex to parent
			if(pQueue[currentIndex]->primaryPriority < pQueue[currentIndex/2]->primaryPriority){
				//cout << "adding: " << pQueue[currentIndex]->itemName << endl;
				priorityItem* temp = pQueue[currentIndex/2];
				pQueue[currentIndex/2] = pQueue[currentIndex];
				pQueue[currentIndex] = temp;
				currentIndex = currentIndex / 2;
			}
			else if((pQueue[currentIndex]->primaryPriority == pQueue[currentIndex/2]->primaryPriority) && (pQueue[currentIndex]->secondaryPriority < pQueue[currentIndex/2]->secondaryPriority)){
				priorityItem* temp = pQueue[currentIndex/2];
				pQueue[currentIndex/2] = pQueue[currentIndex];
				pQueue[currentIndex] = temp;
				currentIndex = currentIndex / 2;
			}
			else{
				itemSwapped = false;
			}
		}
	}
	else{
		//queue is full
		cout << "Queue is full" << endl;
		return false;
	}
	return true;

}
void PriorityQueue::buildQueue(){
	ifstream myFile;
	string line, name, priority, treatment;
	int count = 0;
	int lastIndex = 0;
	int index = 0;
	myFile.open("patientData2270.csv"); //fix this
	if(myFile.fail()){
		cout << "file issues" << endl;
	}
	else{
		while(myFile >> line && index <= 200){
			if(count == 0){
				count++; //skip first line of file --> do not care header information
				continue;
			}
			stringstream ss(line);
			getline(ss, name, ',');
			getline(ss, priority, ',');
			getline(ss, treatment);
			enqueue(name, stoi(priority), stoi(treatment)); 
			index++;
		}
	}
}

bool PriorityQueue::dequeue(){
	if(lastIndex > 0) {
		//cout << pQueue[1]->itemName << endl; ----> Print here to get correct order!!!!!!!
		delete pQueue[1];
		//check if priorityQueue is empty
		if(lastIndex == 1){
			lastIndex = 0;
			pQueue[1] = nullptr;
		}
		else{
			//set item 1 to thing at end
			pQueue[1] = pQueue[lastIndex];
			//set last thing equal to nullptr
			pQueue[lastIndex] = nullptr;
			lastIndex--;
			//reheap down
			int currentIndex = 1;
			int leftChild, rightChild, largerChild;
			bool itemSwapped = true;
			while(currentIndex < maxIndex && itemSwapped){
				//cout << "here" << endl;
				leftChild = currentIndex * 2;
				rightChild = currentIndex * 2 + 1;
				//check if you're out of bounds
				if(leftChild > lastIndex){
					itemSwapped = false;
				}
				else{
					if(rightChild > lastIndex){
						largerChild = leftChild;
					}
					else{
						//cout << "here" << endl;
						largerChild = (pQueue[leftChild]->primaryPriority < pQueue[rightChild]->primaryPriority) || ((pQueue[leftChild]->primaryPriority == pQueue[rightChild]->primaryPriority) && ((pQueue[leftChild]->secondaryPriority < pQueue[rightChild]->secondaryPriority))) ? leftChild : rightChild;
					}
					if((pQueue[currentIndex]->primaryPriority > pQueue[largerChild]->primaryPriority)){
						priorityItem* temp = pQueue[largerChild];
						pQueue[largerChild] = pQueue[currentIndex];
						pQueue[currentIndex] = temp;
						//move current index to the index of larger child
						currentIndex = largerChild;
					}
					else if((pQueue[currentIndex]->primaryPriority == pQueue[largerChild]->primaryPriority) && (pQueue[currentIndex]->secondaryPriority > pQueue[largerChild]->secondaryPriority)){
						priorityItem* temp = pQueue[largerChild];
						pQueue[largerChild] = pQueue[currentIndex];
						pQueue[currentIndex] = temp;
						//move current index to the index of larger child
						currentIndex = largerChild;
					}
					else{
						itemSwapped = false;
					}
				}
			}
		}
	}
	else{
		cout << "empty " << lastIndex << endl;
	}
	return true;
}

void PriorityQueue::printPQ(){
	//does not print in correct order!!!!!! need to print as we delete such that the array gets reheaped. 
	int count = 1;
	if(lastIndex == 0){
		cout << "priority queue is empty" << endl;
		return;
	}
	else{
		cout << "Rank " << "patient, " << "Priority, " << "Treatment" << endl;
		for(int i = 1; i <= lastIndex; i++){
			cout << count << ":  " << pQueue[i]->itemName << ",  " << pQueue[i]->primaryPriority << ",  " << pQueue[i]->secondaryPriority << endl;
			count++;
		}
	}
}

void PriorityQueue::dequeueAll(){
	if(lastIndex == 0){
		cout << "empty!!" << endl;
	}
	while(lastIndex != 0){
		dequeue();
	}
}

int main(){
	ofstream myFile;
	myFile.open("heaptest.csv"); //THIS IS NOT FILE FOR PATIENT INFO
	int numRunThroughs = 500;
	for(int i = 0; i < numRunThroughs; i++){
		auto start = chrono::system_clock::now();
		PriorityQueue instance;
		instance.buildQueue();
		instance.dequeueAll();
		auto end = chrono::system_clock::now();
		auto timeElapsed = chrono::duration<float>(end - start);
		cout << timeElapsed.count() << endl;
		myFile << timeElapsed.count() << endl;
	}
}