// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
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
#include "minisat/core/Solver.h"
#include "graphStructure.hpp"
#include "edge.hpp"

//Using the parse_line function taken from the calc.cpp tutorial taught in class.
//if you want to find a vertex cover of size 2, 
//you are essentially trying to determine which two positions within the vertex cover set will correspond to the vertices to be included in the cover. 
//These positions can be represented as follows:
// Position 1: (Empty slot)
// Position 2: (Empty slot)

std::vector<int> VertexCoverFinder(int numVertices, int VertexCover, std::vector<int> edges){

std::vector<int> origin;
Minisat::Solver minVC;
std::vector<std::vector<Minisat::Lit>> vertexVars(numVertices);
std::vector<int> destiny;

for (long unsigned int i = 0; i < edges.size(); ++i) {
        if (i % 2 == 0) 
            origin.push_back(edges[i]);  //Saves all origin Vertices.
        else 
            destiny.push_back(edges[i]);   //Saves all Destination vertices.
}

//Creating Literals for Each Vertex
for(int iter1 = 0; iter1<numVertices; iter1++)
{
    for(int iter2 = 0; iter2<VertexCover; iter2++){
        vertexVars[iter1].push_back(Minisat::mkLit(minVC.newVar())); //Number of Literals in Each row Added is equal to the Vertex Cover Size Proposed.
    }
}
//Clause - At least one vertex is the ith vertex in the vertex cover:
//This rule enforces that for each vertex position (i) in the vertex cover 
//(from 1 to j), at least one vertex from the graph must be chosen to fill that position.
//This ensures that the vertex cover includes a vertex for each position.

for(int iter1=0; iter1<VertexCover; iter1++){
    Minisat::vec<Minisat::Lit> literals;
    for (int iter2 = 0; iter2 < numVertices; iter2++) 
	  { 
        literals.push(vertexVars[iter2][iter1]);
      }
    minVC.addClause(literals);
        //std::cout<<"Clause 1"<<std::endl;
    literals.clear();
}

//Clause - No vertex can appear twice in a vertex cover:
//this rule ensures that no vertex occupies more than one slot in the vertex cover.

for(int iter1 = 0; iter1 < numVertices; iter1++){ 
    for (int iter2 = 0; iter2 < VertexCover - 1; iter2++){ 
        for (int iter3 = iter2 + 1; iter3 < VertexCover; iter3++) 
                minVC.addClause(~vertexVars[iter1][iter2], ~vertexVars[iter1][iter3]);
    }
}

//Clause - Every Edge has atleast one of its vertices present in the vertex cover  
for (long unsigned int iter1 = 0; iter1 < origin.size(); iter1++) 
	{
        Minisat::vec<Minisat::Lit> literals;
        for (int iter2 = 0; iter2 < VertexCover; iter2++){
            literals.push(vertexVars[origin[iter1]][iter2]);
            literals.push(vertexVars[destiny[iter1]][iter2]);
        }
        minVC.addClause(literals);
        literals.clear();
}

//Clause - No more than one vertex appears in the mth position of the vertex cover:
//this rule ensures that no slot in the vertex cover is occupied by more than one vertex.

for(int iter1 = 0; iter1 < VertexCover; iter1++){ 
    for (int iter2 = 0; iter2 < numVertices - 1; iter2++){ 
        for (int iter3 = iter2 + 1; iter3 < numVertices; iter3++) 
            minVC.addClause(~vertexVars[iter2][iter1], ~vertexVars[iter3][iter1]);
    }
}

    bool sat_solver = minVC.solve();
    std::vector<int> minimumCover;

    if (sat_solver){
      for (int iter1 = 0; iter1 < numVertices; iter1++){
        for (int iter2 = 0; iter2 < VertexCover; iter2++){
                if (Minisat::toInt(minVC.modelValue(vertexVars[iter1][iter2])) == 0){
                    //std::cout<<"Inside solver.."<<std::endl;
                    minimumCover.push_back(iter1);
                }
        }
      }
      return minimumCover;
    } 
    else {
      return {-1};
   }
} 


void vertexCoverProblem(graphStructure graph)
{

    int start = 1;
    int end = graph.vertices;
    std::vector<int> res;
    std::vector<int> finalResult;
    std::vector<int> temp;
    std::unordered_map<int,int> eachVertOccurence;
    std::vector<int> tempVert; //Hold Vertices with a freq=1.
    temp = {-1};
    
    std::vector<int> vertexPoints;
       //Optimizer 
    //Right now you have Original V and E Pair
    //Find vertices with only one edge.
    //Use double for loop, Pick One Edge[That has its both verts..]
    int flag = 0; //Indicating that there exists no vertex with a freq. 0

    //Need a while loop to capsulate this here .. 
    for (edge i in graph.edges){
      edge e = i;
      for (edge j in graph.edges){
          if(i!=j) {
          
          //See if source vertext is in any pair or not... If yes, increment its occurence by 1.  
          if(e.startVertex == j.startVertex || e.startVertex == j.endVertex){
            eachVertOccurence[e.startVertex]++;
          }
          if(e.endVertex == j.startVertex || e.endVertex == j.endVertex){
            eachVertOccurence[e.endVertex]++;
          }
        }
      }
    }
    //Now we have a map where we have the frequency of every vertex in our pool of edges.
    std::map<int, int>::iterator it;
    for (it = eachVertOccurence.begin(); it != eachVertOccurence.end(); it++){
        if(it->second == 1) tempVert.push_back(it->first);
    }
    
    for (int vert in tempVert){
     for (edge E in edges) {
        
     }
    }









    for(auto edgeIter : graph.edges)
    {
      vertexPoints.push_back(edgeIter.startVertex);
      vertexPoints.push_back(edgeIter.endVertex);  
    }

    //Temporary Vertex that just takes the max vertex value.
    auto maxElement = std::max_element(myVector.begin(), myVector.end());
    if (maxElement == myVector.end()) {
        std::cout<<"No further Process is needed."<<std::endl;  //Only Happens if you have mapped all the values in vertext cover.
    }

    while(start <= end) {

        int mid = start + (end-start)/2;
        res = VertexCoverFinder(graph.vertices + 1, mid, vertexPoints);
        bool flag = std::equal(res.begin(), res.end(), temp.begin());

        if(!flag){
            end = mid - 1;
            finalResult.clear();
            finalResult = res;
        } else {
            start = mid + 1;
        }
    }

    for(auto vert: finalResult){
        std::cout << vert << " ";
    }
    std::cout<<std::endl;

}

bool parse_line (const std::string &line,
                 char &cmd, std::string &arg, std::string &err_msg) {


    //To strem the input.
    std::istringstream input(line);
    // remove whitespace
    input >> std::ws;


    //If the input does not have a command, return Empty command.
    if (input.eof()) {
        err_msg = "Error: Empty command";
        return false;
    }


    std::string ch;
    input >> ch;

    //If input is unable to stream it into another variable, return false.
    if (input.fail()) {
        err_msg = "Error: Failed to read input";
        return false;
    }

    cmd = line[0];                                             //Finding the command and returning it here. [If its 'V', 'E', or 's']
    arg = line.substr(1);                                      //Getting the values passed in that command.

    return true;
}

int main(int argc, char** argv) {

    std::stringstream ss;                                     //To stream through the string line provided as command.      
    std::string line;                                         //collect the line parsed.
    std::string arg;                                          //collect the arguments after parsing.
    graphStructure graph;                            //Using the graphStructure which will be used for all operations.
    
    //Reading the standard input through this loop continuously till no empty lines are found in the testfile.
    while (std::getline(std::cin, line)){

        if (line.empty() || std::all_of(line.begin(), line.end(), ::isspace)) {
            // Handle the empty line, for example,
//            std::cerr << "Error: Empty Line." << std::endl;
            continue;
        }

        char cmd;                                             //Tp collect the command in each loop iteration.
        std::string err_msg;                                  //To collect error messages if any.
        std::string verts;                                    //To collect vertices.
        std::string edgeInfo;                                 //To collect edges.

        
        if(parse_line(line,cmd,arg,err_msg)) {                //If we are able to parse the line.
                            
            //Switch case running for three commands required.
            //V -> Adding Vertices.
            //E -> Adding Edges.
            //s -> Finding the Shortest Path between two vertices.
            int flag_valid = 0;
            switch(cmd) {  
                case 'V': 
                    verts = arg;
                    //Getting the number of vertices for the current graph instance required.
                    graph.vertices = graph.getVertex(verts);
                    
                    //Break the code if there are no vertices.
                    if(graph.vertices==0){                                                                           
                        std::cerr<<"Error: Not enough Vertices for Graph."<<std::endl; 
                        break;
                    }
                    graph.edges.clear();

                    break;
                
                case 'E':
                    
                    //If edges are called before passing the number of vertices, break it.
                    if(graph.vertices==0){
                        std::cerr<<"Error: No vertices. Can't add Edges."<<std::endl;
                    }

                    edgeInfo = arg;
                    
                    //Calling the function to create all the edges if not populated previously only
                    //If a valid V E pair is found.
                    
                    if(graph.edges.empty() && graph.vertices != -1){
                        graph.edges = graph.edgeMaker(graph.vertices, edgeInfo);
                           
                        if(graph.edges.empty()){
                            graph.vertices = -1;
                            std::cerr<<"Error: No Edges"<<std::endl;
                            break;
                        }
                    }
                    //Program is going to wait unless a valid V is passed after encountering invalid Edges..
                    else if(graph.vertices == -1){
                        std::cerr<<"Error: Waiting for next valid V E pair."<<std::endl;
                        break;
                    }

                    if(graph.vertices == -1)
                    {
                        std::cerr<<"Error: Waiting for Next Valid V E pair."<<std::endl;
                        break;
                    }

                    else if(graph.vertices==0) {
                         std::cerr<<"Error: No vertices."<<std::endl;
                         break;
                    }
                    
                    else if(graph.edges.size()==0) {
                        std::cerr<<"Error: No Edges"<<std::endl;
                        break;
                    }

                    flag_valid = 1;
                    break;
            }
            if(flag_valid == 1) vertexCoverProblem(graph);
        }

        else{
            std::cerr<<err_msg<<std::endl;
            continue;
        }
    }
    return 0;
}

