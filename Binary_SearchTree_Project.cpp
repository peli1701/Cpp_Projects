#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

struct MovieNodeLL{
    int ranking;
    std::string title;
    int year;
    int quantity;
    MovieNodeLL* next;
    MovieNodeLL(){};
    
    MovieNodeLL(int in_ranking, std::string in_title, int in_year, int in_quantity)
    {
        ranking = in_ranking;
        title = in_title;
        year = in_year;
        quantity = in_quantity;
        next = NULL;
    };
};

struct MovieNodeBST{
    char letter;
    MovieNodeBST* parent;
    MovieNodeBST* leftChild;
    MovieNodeBST* rightChild;
    MovieNodeLL* head;
    
    MovieNodeBST(){};
    
    MovieNodeBST(char in_letter)
    {
        letter = in_letter;
        parent = NULL;
        leftChild = NULL;
        rightChild = NULL;
        head = NULL;
    };
    
};

class MovieTree
{
    
public:
    MovieTree();
    ~MovieTree();
    void printMovieInventory();
    int countMovieNodes();
    void deleteMovieNode(std::string title);
    void addMovieNode(int ranking, std::string title, int releaseYear, int quantity);
    void findMovie(std::string title);
    void rentMovie(std::string title);
    
protected:
    
private:
    void DeleteAll(MovieNodeBST * node); //use this for the post-order traversal deletion of the tree
    void printMovieInventory(MovieNodeBST * node);
    void countMovieNodes(MovieNodeBST *node, int *c);
    MovieNodeBST* searchBST(MovieNodeBST *node, std::string title); //use this recursive function to find a pointer to a node in the BST, given a MOVIE TITLE
    MovieNodeLL* searchLL(MovieNodeLL* head, std::string title); //use this to return a pointer to a node in a linked list, given a MOVIE TITLE and the head of the linked list
    MovieNodeBST* treeMinimum(MovieNodeBST *node); //use this to find the left most leaf node of the BST, you'll need this in the delete function
    MovieNodeBST* root;
};


int main(int argv, char* argc[]){
    
    MovieTree instance;
    ifstream myFile;
    string line;
    string number;
    string title;
    string year;
    string ranking;
    int menuChoice;
    string movieChoice;

    myFile.open(argc[1]);

    while(getline(myFile, line)){
        stringstream ss(line);
        getline(ss, number, ',');
        getline(ss, title, ',');
        getline(ss, year, ',');
        getline(ss, ranking);
        instance.addMovieNode(stoi(number), title, stoi(year), stoi(ranking));
    }

    cout << "======Main Menu======" << endl;
    cout << "1. Find a movie" << endl;
    cout << "2. Rent a movie" << endl;
    cout << "3. Print the inventory" << endl;
    cout << "4. Delete a movie" << endl; 
    cout << "5. Count the movies" << endl; 
    cout << "6. Quit" << endl;
    cin >> menuChoice;
    while(menuChoice != 7){
        if(menuChoice == 1){
            cout << "Enter title:" << endl;
            cin.ignore();
            getline(cin,movieChoice);
            instance.findMovie(movieChoice);
        }
        if(menuChoice == 2){
            cout << "Enter title:" << endl;
            cin.ignore();
            getline(cin, movieChoice);
            instance.rentMovie(movieChoice);
        }
        if(menuChoice == 3){
            instance.printMovieInventory();
        }
        if(menuChoice == 4){
            cout << "Enter title:" << endl;
            cin.ignore();
            getline(cin,movieChoice);
            instance.deleteMovieNode(movieChoice);
        }

        if(menuChoice == 5){
            cout << "Tree contains: " << instance.countMovieNodes() << " movies." << endl;
        }
        if(menuChoice == 6){
            cout << "Goodbye!" << endl;
            break;
        }
        cout << "======Main Menu======" << endl;
        cout << "1. Find a movie" << endl;
        cout << "2. Rent a movie" << endl;
        cout << "3. Print the inventory" << endl;
        cout << "4. Delete a movie" << endl; 
        cout << "5. Count the movies" << endl; 
        cout << "6. Quit" << endl;

        cin >> menuChoice;
    }
}

MovieTree::MovieTree(){
    root = nullptr;
}

MovieTree::~MovieTree(){
    DeleteAll(root);
    root = nullptr;
}


MovieNodeBST* MovieTree::treeMinimum(MovieNodeBST* node){
	node = root;
	while(node != nullptr){
		node = node->leftChild;
	}
	return node;
}

MovieNodeLL* MovieTree::searchLL(MovieNodeLL* head, string title){
	MovieNodeLL* currentNode = head;
	if(head == nullptr){
		return currentNode;
	}
	while(head != nullptr){
		if(head->title == title){
			break;
		}
		head = head->next;
	}
	return head;
}

MovieNodeBST* MovieTree::searchBST(MovieNodeBST *node, string value){
   if(node != nullptr){
	   if(value[0] < node->letter && node->leftChild != nullptr){
	       return searchBST(node->leftChild, value);
	   }
	   else if(value[0] > node->letter && node->rightChild != nullptr){
	       return searchBST(node->rightChild, value);
	   }
	   else if(node->letter == value[0]){
	       return node;
	   }
	   else{
	   	return nullptr;
	   }

	}
	else{
		return nullptr;
	}

}

void MovieTree::deleteMovieNode(string title){
    MovieNodeBST* currentNode = searchBST(root, title);
    MovieNodeBST* BSTdelete = searchBST(root, title);
    MovieNodeBST* minNode;
    MovieNodeBST* x = new MovieNodeBST;
    if(currentNode == nullptr){
        cout << "Movie not found." << endl;
        return;
    }
    
    MovieNodeLL* movieNode = searchLL(currentNode->head, title);
    if(movieNode == nullptr){
        cout << "Movie not found." << endl;
        return;
    }
    if(movieNode == currentNode->head && currentNode->head->next != nullptr){
        currentNode->head = currentNode->head->next;
        delete movieNode;
        return;
    }
    else if(movieNode != currentNode->head && currentNode->head->next != nullptr){
        MovieNodeLL* nodeFind = currentNode->head;
        while(nodeFind != nullptr){
            if(nodeFind->next->title == title){
                break;
            }
            nodeFind = nodeFind->next; 
        }
        nodeFind->next = movieNode->next;
        delete movieNode;
        return;
    }
    else{
        if(currentNode != root){
            if(currentNode->leftChild == nullptr && currentNode->rightChild == nullptr){
                if(currentNode->parent->leftChild == currentNode){
                     currentNode->parent->leftChild = nullptr;
                }
                else{
                    currentNode->parent->rightChild = nullptr;
                }
            }
            else if(currentNode->leftChild != nullptr && currentNode->rightChild    != nullptr){
                minNode = treeMinimum(currentNode->rightChild);
                cout << minNode->letter << endl;
                if(minNode == currentNode->rightChild){
                    currentNode->parent->leftChild = minNode;
                    minNode->parent = currentNode->parent;
                    minNode->leftChild = currentNode->leftChild;
                    minNode->leftChild->parent = minNode;
                }
                else{
                    minNode->parent->leftChild = minNode->rightChild; 
                    minNode->rightChild->parent = minNode->parent; 
                    minNode->parent = currentNode->parent; 
                    currentNode->parent->leftChild = minNode;
                    minNode->leftChild = currentNode->leftChild;
                    minNode->rightChild = currentNode->rightChild;
                    currentNode->rightChild->parent = minNode;
                    currentNode->rightChild->parent = minNode;
                }
                
            }
            else{
                if(currentNode->rightChild != nullptr){
                    currentNode->rightChild->parent = currentNode->parent;
                    currentNode->parent->rightChild = currentNode->rightChild;
                }
                else{
                    currentNode->leftChild->parent = currentNode->parent;
                    currentNode->parent->leftChild = currentNode->leftChild;
                }
                delete currentNode;
            }
            
            
        }
        else{
            if(currentNode->leftChild == nullptr && currentNode->rightChild == nullptr){
                if(currentNode->parent->leftChild == currentNode){
                     currentNode->parent->leftChild = nullptr;
                }
                else{
                    currentNode->parent->rightChild = nullptr;
                }
            }
            else if(currentNode->leftChild != nullptr && currentNode->rightChild    != nullptr){
                minNode = treeMinimum(currentNode->rightChild);
                if(minNode == currentNode->rightChild){
                    currentNode->parent->rightChild = minNode;
                    minNode->parent = currentNode->parent;
                    minNode->leftChild = currentNode->leftChild;
                    minNode->leftChild->parent = minNode;
                }
                else{
                    minNode->parent->leftChild = minNode->rightChild; 
                    minNode->rightChild->parent = minNode->parent; 
                    minNode->parent = currentNode->parent; 
                    currentNode->parent->rightChild = minNode;
                    minNode->leftChild = currentNode->leftChild;
                    minNode->rightChild = currentNode->rightChild;
                    currentNode->rightChild->parent = minNode;
                    currentNode->rightChild->parent = minNode;
                }
                
            }
            else{
                if(currentNode->rightChild != nullptr){
                    currentNode->rightChild->parent = currentNode->parent;
                    currentNode->parent->rightChild = currentNode->rightChild;
                }
                else{
                    currentNode->leftChild->parent = currentNode->parent;
                    currentNode->parent->leftChild = currentNode->leftChild;
                }
                delete currentNode;
            }
        }
    }
}


void MovieTree::addMovieNode(int ranking, string title, int releaseYear, int quantity){
    
	MovieNodeBST* currentNode = new MovieNodeBST(title[0]);  
    MovieNodeBST* newBST = new MovieNodeBST(title[0]);
    MovieNodeBST* temp;
    MovieNodeBST* newNode;
    MovieNodeLL* temp2;
    
    if(root == NULL){
        root = currentNode;
        root->parent = NULL;
        root->head = new MovieNodeLL(ranking, title, releaseYear, quantity);
    }
    else{
        currentNode = root;
        while(currentNode != NULL){
            temp = currentNode;
            if(currentNode->letter == title[0]){
                break;
            }
            if(currentNode->letter > title[0]){
                currentNode = currentNode->leftChild;
            }
            else if(currentNode->letter < title[0]){
                currentNode = currentNode->rightChild;
            }
        }
        
        if(currentNode == NULL){
            if(temp->letter > title[0]){
                temp->leftChild = newBST;
                newBST->parent = temp;
            }
            else if(temp->letter < title[0]){
                temp->rightChild = newBST;
                newBST->parent = temp;
            }
        }
        else{
            newBST = currentNode;
        }
            MovieNodeLL* newMovie = new MovieNodeLL(ranking, title, releaseYear, quantity);
            MovieNodeLL* previous = newMovie;
            if(newBST->head == NULL){
               // cout << "head null" << endl;
                newBST->head = newMovie;
                return;
            }
                temp2 = newBST->head;
                while(temp2 != NULL){
                    if(temp2->title >= title){
                        if(temp2 == newBST->head){
                            newBST->head = newMovie;
                            newMovie->next = temp2;
                            return;
                        }
                    break;
                    }
                    previous = temp2;
                    temp2 = temp2->next;
                }
                newMovie->next = previous->next;
                previous->next = newMovie;
       } 
}


void MovieTree::findMovie(string title){
    MovieNodeBST* currentNode = searchBST(root, title);
    if(currentNode == nullptr){
        cout << "Movie not found." << endl;
        return;
    }
    else{
        MovieNodeLL* movieNode = searchLL(currentNode->head, title);
        if(movieNode == nullptr){
            cout << "Movie not found." << endl;
            return;
        }
        else{
            cout << "Movie Info:" << endl;
            cout << "===========" << endl;
            cout << "Ranking:" << movieNode->ranking << endl; 
            cout << "Title:" << movieNode->title << endl;
            cout << "Year:" << movieNode->year << endl;
            cout << "Quantity:" << movieNode->quantity << endl;
        }
    }
}

void MovieTree::rentMovie(string title){
	//cout << "pls dont seg fault" << endl;
	MovieNodeBST* currentNode = searchBST(root, title);
    if(currentNode == nullptr){
        cout << "Movie not found." << endl;
        return;
    }
    else{
        MovieNodeLL*  movieNode = searchLL(currentNode->head, title);
        if(movieNode == nullptr){
    	cout << "Movie not found." << endl;
    	return;
        }
        else if(movieNode->quantity != 0){
            movieNode->quantity--;
            cout << "Movie has been rented." << endl;
            cout << "Movie Info:" << endl;
            cout << "===========" << endl;
            cout << "Ranking:" << movieNode->ranking << endl; 
            cout << "Title:" << movieNode->title << endl;
            cout << "Year:" << movieNode->year << endl;
            cout << "Quantity:" << movieNode->quantity << endl;
            if(movieNode->quantity == 0){
                deleteMovieNode(title);//why does moodle not account for this?
            }
        }
    }
}

int MovieTree::countMovieNodes(){
	int c = 0;
	countMovieNodes(root, &c);
	return c;
}

void MovieTree::countMovieNodes(MovieNodeBST *node, int *c){
   //cout << "1" << endl;
   if(node != nullptr){ //no return type, can I use recursive methods here?
        countMovieNodes(node->leftChild, c);
        countMovieNodes(node->rightChild, c);
        MovieNodeLL* movieCounter = node->head;
        while(movieCounter != nullptr){
            (*c)++;
            movieCounter = movieCounter->next;
        }
        
    }
    //cout << *c << endl;
}


void MovieTree::DeleteAll(MovieNodeBST* node){
	if(node != nullptr){
        DeleteAll(node->leftChild);
        DeleteAll(node->rightChild);
        MovieNodeLL* movieNode = node->head;
        while(movieNode != nullptr){
            cout << "Deleting: " << movieNode->title << endl;
            delete movieNode;
            movieNode = movieNode->next;
         }
    }
}


void MovieTree::printMovieInventory(){
	printMovieInventory(root);
}


void MovieTree::printMovieInventory(MovieNodeBST* node){
	//cout << "a suh dude" << endl;
	if(node != nullptr){
		printMovieInventory(node->leftChild);
		MovieNodeLL* LLnode = node->head;
		while(LLnode != nullptr){
			cout << "Movie: " << LLnode->title << " " << LLnode->quantity << endl;
			LLnode = LLnode->next;
		}
        //cout << "1" << endl;
        printMovieInventory(node->rightChild);
	}
}


