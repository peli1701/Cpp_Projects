#Peter Lindee

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

class Queue{
	public:
		Queue();
		Queue(int);
		~Queue();
		void enqueue(string);
		void dequeue();
		void printQueue();
		void enqueueSentence(string);

	private:
		bool queueIsFull();
		bool queueIsEmpty();
		int queueSize;
		int queueHead;
		int queueTail;
		int queueCount;
		string* arrayQueue;

};

int main(){

	Queue instance(10);
	int optionChoice;

	cout << "======Main Menu======" << endl;
	cout << "1. Enqueue word" << endl;
	cout << "2. Dequeue word" << endl;
	cout << "3. Print queue" << endl;
	cout << "4. Enqueue sentence" << endl;
	cout << "5. Quit" << endl;

	cin >> optionChoice;

	while(optionChoice != 9){
		
		if(optionChoice == 1){
			string word;
			cout << "word: " << endl;
			cin >> word;
			instance.enqueue(word);
			//enqueue 
		}
		if(optionChoice == 2){
			instance.dequeue();
		}
		if(optionChoice == 3){
			instance.printQueue();
		}
		if(optionChoice == 4){
			string sentence;
			cin >> sentence;
			instance.enqueueSentence(sentence);
			//enqueue sentence	
		}
		if(optionChoice == 5){
			cout << "Goodbye" << endl;
			break;
		}
		cout << "======Main Menu======" << endl;
		cout << "1. Enqueue word" << endl;
		cout << "2. Dequeue word" << endl;
		cout << "3. Print queue" << endl;
		cout << "4. Enqueue sentence" << endl;
		cout << "5. Quit" << endl;

		cin >> optionChoice;

	}
}

Queue::Queue(){

}

Queue::Queue(int qs){
	queueSize = qs;
	arrayQueue = new string[queueSize];
	queueHead = 0;
	queueTail = 0;
	queueCount = 0;
}

Queue::~Queue(){
	delete [] arrayQueue;

	arrayQueue = nullptr;
}

void Queue::enqueue(string word){
	if(!queueIsFull()){
		arrayQueue[queueTail] = word;
		queueCount++;
		queueTail++;
		if(queueTail == queueSize){
			queueTail = 0;
		}
	}
	else{
		cout << "Queue is full" << endl;
	}
	cout << "E: " << word << endl;
	cout << "H: " << queueHead << endl;
	cout << "T: " << queueTail << endl;
	return;
}

void Queue::dequeue(){
    if(!queueIsEmpty()){
	    string word = arrayQueue[queueHead];
	    queueCount--;
	    queueHead++;
		if(queueHead == queueSize){
			queueHead = 0;
		}
		cout << "H: " << queueHead << endl;
		cout << "T: " << queueTail << endl;
		cout << "word: " << word << endl;
	}
	else{
	    cout << "Queue is empty" << endl;
	}

}

void Queue::printQueue(){
    int count = queueHead;
    if(queueCount == 0){
        cout << "Empty" << endl;
    }
    else{
        for(int i = 0; i < queueCount; i++){
            cout << count << ": " << arrayQueue[count] << endl;
            count++;
            if(count == queueCount){
               count = 0;
            }
           
        }
    
    }
}

void Queue::enqueueSentence(string word){
    string line;
    Queue banana;
    stringstream ss(word);
    while(getline(ss,line, ' ')){
         enqueue(line);
    }
}

bool Queue::queueIsFull(){
	if (queueCount == queueSize){
		return true;
	}
	return false;
}

bool Queue::queueIsEmpty(){
	if(queueCount == 0){
		return true;
	}
	return false;
}
