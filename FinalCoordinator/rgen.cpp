// an example of reading random numbers from /dev/urandom
// https://stackoverflow.com/questions/35726331/c-extracting-random-numbers-from-dev-urandom
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class vertex {
  public:
  int x;
  int y;

  vertex() : x(-10000), y(-10000) {}
  vertex(int x, int y) {

    this -> x = x;
    this -> y = y;

  }

  bool operator==(const vertex& other) const {
        return x == other.x && y == other.y;
  }

  struct Hash {
        std::size_t operator()(const vertex& v) const {
        return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
      }
   };
};

class line{
  public:
  vertex start,end;
  double slope;

  line(vertex V1, vertex V2) {
    this->start = V1;
    this->end = V2;
    this->slope = (std::abs(V1.y - V2.y)/std::abs(V1.x -V2.x));
  }

   bool operator==(const line& other) const {
        // Compare the start and end vertices and the slope
        return (start == other.start) && (end == other.end) && (slope == other.slope);
    }

  struct Hash {
      std::size_t operator()(const line& L) const{
        std::size_t hashStart = std::hash<double>()(L.start.x) ^ std::hash<double>()(L.start.y);
        std::size_t hashEnd = std::hash<double>()(L.end.x) ^ std::hash<double>()(L.end.y);
        return hashStart ^ hashEnd;
      }
   };
};

bool areLinesOverlapping(line L1, line L2){

if(L1.slope == L2.slope) {
    return true;
}

return false;
}


std::unordered_set<vertex, vertex::Hash> verticesSet;
// std::unordered_map<std::string,std::vector<Vertex>> RoadMap;
std::unordered_set<line, line::Hash> linesSet;
std::unordered_set<std::string> streetNamesSet;


int numberGenWithinARange(char ch, int rangeNumber){

std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
if (!urandom) {
        std::cerr << "Failed to open /dev/urandom" << std::endl;
        return 1;
    }

int randomValue, minimum, maximum;
urandom.read(reinterpret_cast<char*>(&randomValue), sizeof(randomValue));
urandom.close();

if(ch == 's') //Number of Streets.
{
  minimum = 2;
  maximum = rangeNumber;
}

if(ch == 'n')//Number of Lanes in the Street.
{
  minimum = 1;
  maximum = rangeNumber;
}

if(ch == 'l') //Delay 
{
  minimum = 5;
  maximum = rangeNumber;
}

return minimum + (std::abs(randomValue) % (maximum - minimum + 1));

}

std::string streetNameGenerator(long unsigned int length){

  std::string randString;
  std::ifstream urandom("/dev/urandom", std::ios::binary);


  if(urandom) {
    while (randString.length() < length)
    {
      char randomCharacter;
      urandom.read(&randomCharacter, 1);
      if(std::isalnum(randomCharacter)) randString += randomCharacter;
    }

  urandom.close();
  } else {
    std::cerr <<"Failed to open /dev/urandom " <<std::endl;
  }

return randString;
}

std::vector<vertex> vertexGenerator(int noOfVertices, int Range){

std::vector<vertex> vertexVector;
std::ifstream urandom("/dev/urandom", std::ios::binary);
int connectingFlag = 0;
vertex connectingVert = vertex(0,0); // To track prev vertex in a street. //Clear it whenever a new street is taken.

if(!urandom) {
  std::cerr <<"Failed to open /dev/urandom" <<std::endl;
  return vertexVector;
}
int count = 0; //No of vertices in an add statement.
int retriesCount = 0;

int minimum = -Range;
int maximum = Range;

while(count < noOfVertices)
{
  int randomValue1, randomValue2;
  urandom.read(reinterpret_cast<char*>(&randomValue1), sizeof(randomValue1));
  urandom.read(reinterpret_cast<char*>(&randomValue2), sizeof(randomValue2));


  randomValue1 = minimum + (std::abs(randomValue1) % (maximum - minimum + 1));
  randomValue2 = minimum + (std::abs(randomValue2) % (maximum - minimum + 1));


  vertex Vertex(randomValue1,randomValue2);

  if(verticesSet.find(Vertex) == verticesSet.end())    //Checking if a vertex is already present.
  {

    if(connectingFlag != 0)
    {
      line temp = line(connectingVert, Vertex);  //Creating a Temp Line.
      bool reject = false;
      
      if(linesSet.size() != 0){
          for(auto itr : linesSet){
            reject = areLinesOverlapping(temp,itr); //Comparing with all the lines if it is overlapping.
            if(reject) break;
         }
      }

      if(reject) continue;  // If the lines are parallel, rejcet them. Recompute Vertices.
      linesSet.insert(temp); //If its valid, add it in lineset as well.
      verticesSet.insert(Vertex); //Insert the Valid Point.
      vertexVector.push_back(Vertex);
      connectingVert = vertex(Vertex.x, Vertex.y);  //Setting prev vertex as current vertex and get ready to capture next correct vertex in our set.
      connectingFlag = 1;       //Next Vertex wont be taken as the first one of the street.
      count++;

    }

    if(connectingFlag == 0){
      
      verticesSet.insert(Vertex);
      vertexVector.push_back(Vertex);
      connectingVert = vertex(Vertex.x, Vertex.y);
      count++;
    
    }
  }

  else{
    retriesCount++;
    if(retriesCount >= 25){
      std::cerr<<"Error: Tried More than 25 Times. Restart the Program."<<std::endl;
      exit(1);
    }
    else{
     continue;
    }
  }
}

urandom.close();
return vertexVector;
}

int main(int argc, char **argv) {

  int  noOfStreets = 10;
  int  noOfLineSegments = 5;
  int  delay = 5;
  int  range = 20;

  int switchCaseHandler;

  // expected options are '-a', '-b', and '-c value'
  while ((switchCaseHandler= getopt(argc, argv, "s:n:l:c:")) != -1)   
  switch (switchCaseHandler) {
    case 's':
      if (std::stoi(optarg) < 2){
        std::cerr <<"Error: No. of streets must be >= 2. Using range to choose s - [2,10]" << std::endl;
      }
      else {
        noOfStreets = std::stoi(optarg);
      }
      break;
    
    case 'n':
      if (std::stoi(optarg) < 1){
        std::cerr <<"Error: No. of line segments in each street must be >= 2. Using range to choose n - [1,5]" << std::endl;
      }
      else {
        noOfLineSegments = std::stoi(optarg);
      }
      break;

    case 'l':
      if (std::stoi(optarg) < 5){
        std::cerr <<"Error: The delay specified should be >= 5. Using range to choose l - [5,5]" <<std::endl;
      }
      else{
        delay = std::stoi(optarg);
      }
      break;

    case 'c':
      if (std::stoi(optarg) < 1){
        std::cerr<<"Error: The range value given should be >= 1. Using range to choose c - [-20,20]" <<std::endl;
      }
      else{
        range = std::stoi(optarg);
      }
      break;

    default:
        std::cout << "Using Defaults." << optarg << std::endl;
    }

//    std::cout << "noOfStreets= " << noOfStreets  << " "
//            << "noOfLineSegments= " << noOfLineSegments << " "
//            << "delay= " << delay << " "
//           << "range= " << "[-" << range <<"," << range << "]"<< std::endl;




  while(true) {  
     int addStatementsGenerated = 0;
     int streets = numberGenWithinARange('s', noOfStreets);
     delay = numberGenWithinARange('l', delay);
    
     //Add Generator
    while(addStatementsGenerated < streets)
    {
      int noOfLanes = numberGenWithinARange('n', noOfLineSegments);
      delay = numberGenWithinARange('l', delay);
      std::string result = streetNameGenerator(streets); //Name Generator
      if(result.empty()) continue;
      streetNamesSet.insert(result);
      std::vector<vertex> vertices = vertexGenerator(noOfLanes+1,range); //Since two points can make a line. N lines will need N+1 Vertices.
      std::cout<<"add \""<<result<< "\" ";
      for(vertex v : vertices){
         std::cout<<"("<<v.x<<","<<v.y<<")"<<" ";
      }
      std::cout<<std::endl;
      addStatementsGenerated++;
    }
    
    //Graph Generator
    std::cout<<"gg"<<std::endl;

    //Delay    
    std::this_thread::sleep_for(std::chrono::seconds(delay));

    //Remove Generator
    for(auto iter = streetNamesSet.begin(); iter != streetNamesSet.end(); ++iter){
      std::cout <<"rm \""<<*iter<<"\"" <<std::endl;
    }

    verticesSet.clear();
    linesSet.clear();
    streetNamesSet.clear();
  }

  return 0;
}
