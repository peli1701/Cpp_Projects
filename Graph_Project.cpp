#Peter Lindee

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;

struct vertex;

struct adjVertex{
    vertex *v;
    int weight;
};

struct vertex{
    std::string name;
    std::vector<adjVertex> adj;
};

class Graph
{
    public:
        Graph(){};  // Already implemented
        ~Graph(){}; // Already implemented
        void addEdge(std::string v1, std::string v2, int weight);
        void addVertex(std::string name);
        int isAdjacent(std::string v1, std::string v2);
        void displayEdges();
        
    protected:
    private:
        std::vector<vertex> vertices;
};


int main(int argc, char* argv[]){
	Graph instance;
	int i = 0;
	string line;
	string cities;
	string newCity;
	string weight;
	vector<string> myVector;
	ifstream myFile;
	myFile.open(argv[1]);
	getline(myFile, line);
	//cout << line << endl;
	stringstream ss(line);
	while(getline(ss, cities, ',')){
		if(cities != "cities"){
			cout << "adding " << cities << " to vertex" << endl;
			myVector.push_back(cities);
			instance.addVertex(cities);
		}
	}
	while(getline(myFile, line)){
		i = 0;
		//cout << line << endl;
		stringstream edge(line);
		getline(edge, newCity, ',');
		while(getline(edge, weight,',')){
			if(stoi(weight) > 0){
				cout << "adding path between " << newCity << " and " << myVector[i] << endl;
				instance.addEdge(newCity, myVector[i], stoi(weight));
			}
			i++;
		}
	}

	cout << "======Main Menu======" << endl;
	cout << "1. Print vertices" << endl;
	cout << "2. Vertex adjacent" << endl;
	cout << "3. Quit" << endl; 

	int optionChoice;
	string firstCity;
	string secondCity;
	cin >> optionChoice;

	while(optionChoice != 4){

		if(optionChoice == 1){
			instance.displayEdges();
		}

		if(optionChoice == 2){
			cout << "Enter first city:" << endl;
			cin.ignore();
			getline(cin, firstCity);
			cout << "Enter second city:" << endl;
			getline(cin, secondCity);
			//instance.isAdjacent(firstCity, secondCity);
		}

		if(optionChoice == 3){
			cout << "Goodbye!" << endl;
			break;
		}
		cout << "======Main Menu======" << endl;
		cout << "1. Print vertices" << endl;
		cout << "2. Vertex adjacent" << endl;
		cout << "3. Quit" << endl; 
		cin >> optionChoice;
	}
}

void Graph::addVertex(string name){
	bool found = false;
	int i = 0;
	while(i < vertices.size() && !found){
		if(vertices[i].name == name){
			found = true;
			cout << name << " already in the graph." << endl;
			break;
		}
		i++;
	}
	if(!found){
		vertex v;
		v.name = name;
		vertices.push_back(v);
	}
}

int Graph::isAdjacent(std::string v1, std::string v2)
{
    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i].name == v1){
            for(int x = 0; x < vertices.size(); x++){
                if(vertices[i].adj[x].v->name == v2){
                    return true;
                }
                return false;
            }
        }
    }
}

void Graph::displayEdges(){
    //loop through all vertices and adjacent vertices
    int j;
    for(int i = 0; i < vertices.size(); i++){
        cout<<vertices[i].name<<"-->";
        j = 0;
        while(j < vertices[i].adj.size()-1){
            cout<< vertices[i].adj[j].v->name;
            cout << "***";
            j++;
        }
       cout<< vertices[i].adj[j].v->name;
       cout<<endl;
    }
}

void Graph::addEdge(string v1, string v2, int weight){

    for(int i = 0; i < vertices.size(); i++){

        if(vertices[i].name == v1){

            for(int j = 0; j < vertices.size(); j++){

                if(vertices[j].name == v2 && i != j){

                    adjVertex av;
                    av.v = &vertices[j];
                    av.weight = weight;
                    vertices[i].adj.push_back(av);
                    //another vertex for edge in other direction
                }
            }
        }
    }
}
