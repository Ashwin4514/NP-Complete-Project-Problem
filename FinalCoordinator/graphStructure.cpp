#include "graphStructure.hpp"
#include "edge.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <stack>
#include <algorithm>
#include <string>
#include <fstream>
#include <cstdlib>
#include <list>
#include <regex>


graphStructure::graphStructure(){                                        //Calling a constructor to initialize an object of type graphStructure.
        this->vertices=0 ;
        this-> edges = std::vector<edge>(0);
    }

int graphStructure::getVertex(std::string line){
        std::stringstream ss(line);
        int verts;
        ss>>verts;
        return verts;
    }

std::vector<int> graphStructure::shortestPathBFS(int vertNum, std::vector<edge> allEdges, std::string endPoints) {

    std::istringstream iss(endPoints);                                  //stream to extract the endpoints.
    std::vector<int> points = std::vector<int>(0);                      //vector to hold the endpoints.
    std::unordered_map<int, std::list<int>> adjList;                    //Tracks vertices connected to a particular vertex.
    std::unordered_map<int, bool> nodesVisited;                         // Mark the nodes already visited in the code.
    std::unordered_map<int, int> parentTracker;                         // stores parent of each node for path reconstruction.
    std::queue<int> graphItr;                                           // Used to iterate through the graph.
    std::vector<int> finalPath = std::vector<int>(0);                   // Tracks the final path by tracking backwards.
    int sourceNode;                                                     // Node to find the path from.
    int destinationNode;                                                // Node to find the path to.
    int num;                                                            // Temporary variable to help extract source and destination node from a string.

    std::regex str_expr (R"(\d+)");                                     //Regex to help extract the endpoints for shortest path.
    std::smatch match;
    
    
    //Running while loop to find those two points for shortest path.
    
    while(std::regex_search(endPoints, match, str_expr)){
        num = stoi(match.str());
        endPoints = match.suffix();
        points.push_back(num);
    }

    //Getting the Endpoints to find the path between them.
    sourceNode = points[0];
    destinationNode = points[1];                                       
    
    
    if(sourceNode==0 || destinationNode==0){
        std::cerr<<"Error: 0 is not a valid vertex."<<std::endl;
        finalPath.push_back(-2);
        return finalPath;
    }
    
    //If the sourceNode and destinationNode are given more than that number of vertices. Throw an error.
    if(sourceNode > vertNum || destinationNode > vertNum){
        std::cerr<<"Error: Number of vertices not enough."<<std::endl;
        finalPath.push_back(-1);
        return finalPath;
    }
    
    //If the sourceNode and destinationNode is same, return the self edge.
    if(sourceNode == vertNum && destinationNode == vertNum){
        
        finalPath.push_back(vertNum);
        return finalPath;
    }
    // std::cout<<"Source Node = "<<sourceNode<<std::endl;
    // std::cout<<"Destination Node = "<<destinationNode<<std::endl;
    
    //Creating an Adjacency List for each of the nodes.
    //Since the vector.size() method returns a long unsigned int, Matching the case of our iterator with it.

    for(std::vector<int>::size_type i=0; i<allEdges.size(); i++)
    {
        int start = allEdges[i].startVertex;
        int stop = allEdges[i].endVertex;

        adjList[start].push_back(stop);
        adjList[stop].push_back(start);
    }
    
    //Pushing the origin node for our condition before starting to traverse the graph.

    //If none of the nodes are found, return empty path.[Optional]
    if(adjList.find(sourceNode)== adjList.end() || adjList.find(destinationNode)== adjList.end())
    {
        return finalPath;
    }
    
    
    graphItr.push(sourceNode);                                          //Pushing the sourceNode in queue.            
    nodesVisited[sourceNode] = true;                                    //Marking the node as traversed.
    parentTracker[sourceNode] = -1;                                     //Since its the source node, we assume the graph to be starting from here.
    
    //Traverse the nodes and capture a parent map which shows the dependency of one vertex on the other one.
    while (!graphItr.empty()) {
        int currNode = graphItr.front();
        graphItr.pop();                                            
        
        for(auto i : adjList[currNode]) {
            if(!nodesVisited[i]) {
                nodesVisited[i]=true;
                parentTracker[i] = currNode;
                graphItr.push(i);
            }
        }
    }
    //After getting the parent map ready you now basically have to traverse backawards from your required destination node to the source node
    //By using the parent information collected.

    //Taking the Destination Node in our current Node.
    int currNode = destinationNode;
    finalPath.push_back(currNode);
    
    //Traversing the Parent Directory to find the parent of the current node and thus the path.
    while(currNode != sourceNode){
        currNode = parentTracker[currNode];
        finalPath.push_back(currNode);
    }
    
    //As the finalPath is found through tracking back from destination to source. Need to reverse it and return to the main function.
    reverse(finalPath.begin(),finalPath.end());
    return finalPath; 
}


std::vector<edge> graphStructure::edgeMaker(int vertNum, const std::string& edgeLine) {
    
    std::vector<edge> edges;                                            //To collect Edges.


    //Iterating through the line with edges.
    for (std::vector<int>::size_type  i = 0; i < edgeLine.length(); i++) {

        //Whenever we come across the '<' means the start of an edge described.
        if (edgeLine[i] == '<') {
            int start, end;

            // Move past the '<'
            i++;

            // Read the first vertex
            int j = i;

            //Checking the type of vertex given. If not numeric, move ahead.
            while (isdigit(edgeLine[j])) j++;
            start = stoi(edgeLine.substr(i, j - i));
            i = j;

            // Move past the ','
            i++;

           // Read the second vertex
            j = i;
            while (isdigit(edgeLine[j])) j++;
            end = stoi(edgeLine.substr(i, j - i));
            i = j;

            // Create edge and add it to the list
            
            if(start > vertNum || end > vertNum){
                std::cerr<<"Error: Incorrect Edge(s) given. Will wait for next V."<<std::endl;
                return edges;
            }
            
            edge edgeInstance;
            edgeInstance.startVertex = start;
            edgeInstance.endVertex = end;
            edges.push_back(edgeInstance);
        }
    }

    return edges;
}

