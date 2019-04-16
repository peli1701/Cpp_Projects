#Peter Lindee

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;

struct adjVertex;

/*
 This is the struct for each vertex in the graph.
 When you construct the shortest path between two vertices, 
 you will need to store a pointer to the parent vertex. 
 Also, you will use the distance members in your shortest
 path implementations.  The ID member should be an index
 that is increased every time a vertex is added to the graph.
 The first vertex will have an ID equal to zero.
*/
struct vertex{
    int ID;
    vertex *parent;
    std::string name;
    int districtID = -1;
    bool visited;
    int unweightedDistance;
    int weightedDistance;
    std::vector<adjVertex> adj;
};

/*
 This is the struct for the adjacent vertices for each
 vertex in the graph. It contains a weight of the edge
 connecting the vertex and the adjacent vertex as well
 as a pointer to the adjacent vertex.
*/

struct adjVertex{
    vertex *v;
    int weight;
};

class Graph
{
    public:
        Graph();
        ~Graph();
        void addEdge(std::string v1, std::string v2, int weight);
        void addVertex(std::string name);
        void displayEdges();
        void assignDistricts();
        // Breadth First Search
        void shortestPath(std::string startingCity, std::string endingCity);
        // Dijkstras
        void shortestWeightedPath(std::string startingCity, std::string endingCity);

    protected:
    private:
        std::vector<vertex > vertices;
        vertex * findVertex(std::string name);
        /*
          Call this from within assignDistricts to label the districts.
          This method should implement a depth first search from the 
          startingCity and assign all cities encountered the distID value.
        */
        void DFSLabel(std::string startingCity, int distID);
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
			//cout << "adding " << cities << " to vertex" << endl;
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
				//cout << "adding path between " << newCity << " and " << myVector[i] << endl;
				instance.addEdge(newCity, myVector[i], stoi(weight));
			}
			i++;
		}
	}

	cout << "======Main Menu======" << endl;
	cout << "1. Print vertices" << endl;
	cout << "2. Vertex adjacent" << endl;
	cout << "3. Find shortest path" << endl;
	cout << "4. Find shortest weighted path" << endl;
	cout << "5. Quit" << endl; 

	int optionChoice;
	string start, end;
	cin >> optionChoice;

	while(optionChoice != 5){

		if(optionChoice == 1){
			instance.displayEdges();
		}

		if(optionChoice == 2){
			instance.assignDistricts();
		}

		if(optionChoice == 3){
			cin.ignore();
			cout << "Enter a starting city: " << endl;
			getline(cin, start);
			cout << "Enter an ending city: " << endl;
			getline(cin, end);
			instance.shortestPath(start, end);
		}
		if(optionChoice == 4){
			cin.ignore();
			cout << "Enter a starting city: " << endl;
			getline(cin, start);
			cout << "Enter an ending city: " << endl;
			getline(cin, end);
			instance.shortestWeightedPath(start, end);
		}
		if(optionChoice == 5){
			cout << "Goodbye!" << endl;
			break;
		}
		cout << "======Main Menu======" << endl;
		cout << "1. Print vertices" << endl;
		cout << "2. Vertex adjacent" << endl;
		cout << "3. Find shortest path" << endl;
		cout << "4. Find shortest weighted path" << endl;
		cout << "5. Quit" << endl; 
		cin >> optionChoice;
	}
}

Graph::Graph(){
    
}

Graph::~Graph(){
    
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

void Graph::displayEdges(){
    //loop through all vertices and adjacent vertices
    int j;
    for(int i = 0; i < vertices.size(); i++){
        cout << vertices[i].districtID << ":";
        cout<<vertices[i].name<<"-->";
        j = 0;
        while(j < vertices[i].adj.size()-1){
            cout << vertices[i].adj[j].v->name;
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

void Graph::assignDistricts(){
    int i = 0;
    int district = 1;
    while(i <= vertices.size()){
        if(vertices[i].districtID == -1){
            DFSLabel(vertices[i].name, district);
            district++;
        }
        i++;
    }
}

void Graph::DFSLabel(string startingCity, int distID)
{
    vertex* currentVert = findVertex(startingCity);
    if(currentVert != nullptr){
        currentVert->visited = true;
        currentVert->districtID = distID;
        stack <vertex*> DFStack;
        DFStack.push(currentVert);
        while(!DFStack.empty()){
            vertex* curV = DFStack.top();
            DFStack.pop();
            for(int i = 0; i < curV->adj.size(); i++){
                if(!curV->adj[i].v->visited){
                    curV->adj[i].v->districtID = distID;
                    curV->adj[i].v->visited = true;
                    DFStack.push(curV->adj[i].v);
                }
            }
        }
    }
}

void Graph::shortestPath(string startingCity, string endingCity)
{

    queue <vertex*> vertexQ;
    bool pathComplete = false;
    
    vertex* startVert = findVertex(startingCity);
    vertex* endVert = findVertex(endingCity);
    
    if(!startVert || !endVert){
        cout << "One or more cities doesn't exist" << endl;
        return;
    }
    else if(startVert->districtID != endVert->districtID){
        cout << "No safe path between cities" << endl;
        return;
    }
    else if(startVert->districtID == -1 || endVert->districtID == -1){
        cout << "Please identify the districts before checking distances" << endl;
        return;
    }
    else{
        int x = 0;
        while(x < vertices.size()){
            vertices[x].visited = false;
            vertices[x].unweightedDistance = numeric_limits<int>::max();
            //vertices[x].weightedDistance = numeric_limits<int>::max();
            //vertices[x].parent = nullptr;
            x++;
        }
        startVert->unweightedDistance = 0;
        vertexQ.push(startVert);
        while(vertexQ.size() != 0){
            vertex* curV = vertexQ.front();
            vertexQ.pop();
            for(int i = 0; i < curV->adj.size(); i++){
                if(!curV->adj[i].v->visited){
                    if (curV->adj[i].v->unweightedDistance > curV->unweightedDistance + 1) {
                        curV->adj[i].v->unweightedDistance = curV->unweightedDistance + 1;
                        curV->adj[i].v->parent = curV;
                    }              
    
                    if(curV->adj[i].v != startVert){
                        //curV->adj[i].v->unweightedDistance = curV->unweightedDistance + 1;
                    }
                    if(curV->adj[i].v == endVert){
                        break;
                    }
                    else{
                        curV->adj[i].v->visited = true;
                        vertexQ.push(curV->adj[i].v);
                    }
                }
            }
        }
            int hopCount = 0;
            vertex* hopCounter = endVert;
            vector <string> myVector;
            //myVector.push_back(endVert->name);
            while(hopCounter->parent != nullptr){
                hopCount++;
                myVector.push_back(hopCounter->name);
                hopCounter = hopCounter->parent;
            }
            myVector.push_back(startVert->name);
    
            cout << hopCount;
            for(int i = myVector.size() - 1; i >= 0; i--){
                cout << ", " << myVector[i];
            }
            cout << endl;
        }
}

void Graph::shortestWeightedPath(string startingCity, string endingCity)
{

    queue <vertex*> vertexQ;
    bool pathComplete = false;
    
    vertex* startVert = findVertex(startingCity);
    vertex* endVert = findVertex(endingCity);
    
    if(!startVert || !endVert){
        cout << "One or more cities doesn't exist" << endl;
        return;
    }
    else if(startVert->districtID != endVert->districtID){
        cout << "No safe path between cities" << endl;
        return;
    }
    else if(startVert->districtID == -1 || endVert->districtID == -1){
        cout << "Please identify the districts before checking distances" << endl;
        return;
    }
    else{
        int x = 0;
        while(x < vertices.size()){
            vertices[x].visited = false;
            vertices[x].weightedDistance = numeric_limits<int>::max();
            vertices[x].parent = NULL;
            x++;
        }
        startVert->weightedDistance = 0;
        vertexQ.push(startVert);
        while(vertexQ.size() != 0){
            vertex* curV = vertexQ.front();
            vertexQ.pop();
            for(int i = 0; i < curV->adj.size(); i++){
                if(!curV->adj[i].v->visited){
                    if (curV->adj[i].v->weightedDistance > curV->weightedDistance + 1) {
                        curV->adj[i].v->weightedDistance = curV->weightedDistance + curV->adj[i].weight;
                        //curV->adj[i].v->parent = curV;
                    }              
    
                    //else{
                        curV->adj[i].v->visited = true;
                        vertexQ.push(curV->adj[i].v);
                        curV->adj[i].v->parent = curV;
                    //}
                }
            }
        }
            int hopCount = 0;
            vertex* hopCounter = endVert;
            vector <string> myVector;
            //myVector.push_back(endVert->name);
            while(hopCounter != startVert){
                hopCount++;
                myVector.push_back(hopCounter->name);
                hopCounter = hopCounter->parent;
            }
            myVector.push_back(startVert->name);
    
            cout << endVert->weightedDistance;
            for(int i = myVector.size() - 1; i >= 0; i--){
                cout << ", " << myVector[i];
            }
            cout << endl;
        }

}
