
#include <vector>
#include <string>
#include "edge.hpp"

class graphStructure {
public:
       int vertices;
       
       std::vector<edge> edges;
       
       graphStructure();
       
       int getVertex(std::string line);
       
       std::vector<int> shortestPathBFS(int vertNum, std::vector<edge> allEdges, std::string endPoints);
       
       std::vector<edge> edgeMaker(int vertNum, const std::string& edgeLine);
       
};
