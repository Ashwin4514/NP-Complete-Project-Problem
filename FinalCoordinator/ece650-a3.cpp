#include<vector>
#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<cstdlib>

using namespace std;

int main(int argc, char **argv)
{

    vector<pid_t> childProcessToKill;
    opterr = 0;
 	  string temp;
    int retries = 0;
 	  int snum = 10, nnum = 5, lnum = 5, cnum = 20, c;
	
	  while ((c = getopt (argc, argv, "s:n:l:c:")) != -1)
    {
          
          
          if(c=='s')
          {
               temp = optarg;
               snum = atoi(temp.c_str());
               if(snum < 2){
                    cerr << "Error: Minimum 2 streets are required for intersection" << endl;
                    exit(0);
                } 
          }
          else if(c=='n')
          {
              temp = optarg;
                nnum = atoi(temp.c_str());
              
                if(nnum < 1){
                    cerr << "Error: A Street should have minimum 1 line(street line)" << endl;
                    exit(0);
                }
          }
          else if (c=='l')
          {
              temp = optarg;
                lnum = atoi(temp.c_str());
               
                if(lnum < 5){
                    cerr << "Error: Seconds should be more than 5." << endl;
                    exit(0);
                }
          }
          else if(c=='c')
          {
                    temp = optarg;
                cnum = atoi(temp.c_str());
               
                if(cnum < 1){
                    cerr << "Error: K less than 1." << endl;
                    exit(0);
                }
          }
          else if(c=='?')
          {
              if (optopt == 's' || optopt == 'n' || optopt == 'l' || optopt == 'c')
                    cerr << "Error: No argument for the option given" << endl;
                else
                    cerr << "Error: Invalid option." << endl;
                return 1;
          }
          else
          {
              return 0;
          }
       
    }
	

    char* rgenarg[10];
    rgenarg[0] = (char*)"./rgen";
    rgenarg[1] = (char*)"-s";
    rgenarg[2] = (char*)to_string(snum).c_str();
    rgenarg[3] = (char*)"-n";
    rgenarg[4] = (char*)to_string(nnum).c_str();
    rgenarg[5] = (char*)"-l";
    rgenarg[6] = (char*)to_string(lnum).c_str();
    rgenarg[7] = (char*)"-c";
    rgenarg[8] = (char*)to_string(cnum).c_str();
    rgenarg[9] = nullptr;

    char* pyarg[3];
    pyarg[0] = (char*)"/usr/bin/python3";
   	pyarg[1] = (char*)"ece650-a1.py";
   	pyarg[2] = nullptr;

    char* shortest_args[2];
    shortest_args[0] = (char*)"./ece650-a2";
    shortest_args[1] = nullptr;


    int rgenToA1[2];
    int a1ToA2[2];
    
    pipe(rgenToA1);
    pipe(a1ToA2);
     
    pid_t rgenProcess = fork();

    if(rgenProcess < 0){
        std::cerr<<"Forking Rgen Failed.";
        return 1;
    } 

    if (rgenProcess == 0){
        
        close(rgenToA1[0]);
        dup2(rgenToA1[1], 1);
        
        execvp("./rgen", rgenarg);
        
        std::cerr << "execvp failed for rgen: " << std::endl;
    }
    
    childProcessToKill.push_back(rgenProcess);

    pid_t a1Process = fork();
    if (a1Process < 0) {
        std::cerr << "Forking Python Failed.";
        return 1;
    }

    if (a1Process == 0){
    
        close(rgenToA1[1]);

        dup2(rgenToA1[0], 0);
        close(a1ToA2[0]);
        dup2(a1ToA2[1],1);
        execv("/usr/bin/python3", pyarg);
        
        std::cerr << "execvp failed for python: " << std::endl;
    }

    childProcessToKill.push_back(a1Process);

    pid_t a2Process = fork();
    if(a2Process == 0) {
    
        close(a1ToA2[1]);
        dup2(a1ToA2[0], 0);
        
        execv("./ece650-a2", shortest_args);
        
        std::cerr << "execvp failed for a2 : " << std::endl;
    
    }
    
    childProcessToKill.push_back(a2Process);

    dup2(a1ToA2[1],1);
    
    while(!std::cin.eof() && waitpid(rgenProcess, &retries, WNOHANG) != rgenProcess){
    
       std::string line;
       std::getline(std::cin,line);
       std::cout<<line<<std::endl;
       
    }
    
    close(rgenToA1[0]);
    close(rgenToA1[1]);
    close(a1ToA2[0]);
    close(a1ToA2[1]);


    int status;

    for(auto process : childProcessToKill){
      kill(process, SIGTERM);
      waitpid(process, &status, 0);
    }

    return 0;
}