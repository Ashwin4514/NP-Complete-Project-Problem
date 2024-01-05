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
#include "graphStructure.hpp"
#include "edge.hpp"

//Using the parse_line function taken from the calc.cpp tutorial taught in class.
bool parse_line (const std::string &line,
                 char &cmd, std::string &arg, std::string &err_msg) {


    //To strem the input.
    std::istringstream input(line);

    // remove whitespace
    std::ws(input);


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
    graphStructure graphStructure;                            //Using the graphStructure which will be used for all operations.
    
    //Reading the standard input through this loop continuously till no empty lines are found in the testfile.
    
    //std::cout<<"Hi Giri"<<std::endl;
    
    while (std::getline(std::cin, line)){
        //std::cout<<"Hi Giri inside while"<<std::endl;
        //std::cout<<line<<std::endl;
        
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
            
            switch(cmd) {  
                case 'V':    
                    std::cout<<line<<std::endl;                            
                    verts = arg;
                    //Getting the number of vertices for the current graph instance required.
                    graphStructure.vertices = graphStructure.getVertex(verts);
                    
                    //Break the code if there are no vertices.
                    if(graphStructure.vertices==0){                                                                           
                        std::cerr<<"Error: Not enough Vertices for Graph."<<std::endl; 
                        break;
                    }
                    graphStructure.edges.clear();

                    break;
                
                case 'E':
                    std::cout<<line<<std::endl;
                    //If edges are called before passing the number of vertices, break it.
                    if(graphStructure.vertices==0){
                        std::cerr<<"Error: No vertices. Can't add Edges."<<std::endl;
                    }
                    edgeInfo = arg;
                    
                    //Calling the function to create all the edges if not populated previously only
                    //If a valid V E pair is found.
                    
                    if(graphStructure.edges.empty() && graphStructure.vertices != -1){
                        graphStructure.edges = graphStructure.edgeMaker(graphStructure.vertices, edgeInfo);
                           
                        if(graphStructure.edges.empty()){
                            graphStructure.vertices = -1;
                            break;
                        }
                    }
                    //Program is going to wait unless a valid V is passed after encountering invalid Edges..
                    else if(graphStructure.vertices == -1){
//                        std::cerr<<"Error: Waiting for next valid V E pair."<<std::endl;
                        break;
                    }    
                    break;
                
                case 's':
                    std::string shortestPathEnds = arg;

                    //If incorrect edges given and no V after that, wait for a valid V E pair before we start finding shortest path again. 
                    if(graphStructure.vertices == -1)
                    {
//                      std::cerr<<"Error: Waiting for next valid V E pair."<<std::endl;
                        break;
                    }
                    //If no vertices provided, throw error.
                    if(graphStructure.vertices==0) {
                        std::cerr<<"Error: No vertices."<<std::endl;
                        break;
                    }
                    
                    //If no edges provided, throw error.
                    else if(graphStructure.edges.size()==0) {
                        std::cerr<<"Error: No Edges"<<std::endl;
                        break;
                    }
                    
                    //Calling the shortestPath Finder function that uses BFS.
                    std::vector<int> result = 
                    graphStructure.shortestPathBFS(graphStructure.vertices, graphStructure.edges, shortestPathEnds);
                    
                    //If no path is found, return as Error.
                    if(result.empty()){
                        std::cerr<<"Error: No Path Found."<<std::endl;
                        continue;
                    }
                    
                    //If both the vertices passed find the shortest path is same, return Error.
                    else if(result.size()==1){
                        if(result[0]==-1){ continue;}
                        if(result[0]==-2){ continue;}
                        std::cout<<result[0]<<std::endl;
                        continue;
                    }
                    //Print the final shortest path computed.
                    for (std::vector<int>::size_type i=0; i<result.size()-1; i++){
                        std::cout<<result[i]<<"-";
                    }
                    std::cout<<result[result.size()-1]<<std::endl;
                    break;
            }
        }

        else{
            std::cerr<<err_msg<<std::endl;
            continue;
        }
    }
    return 0;
}

