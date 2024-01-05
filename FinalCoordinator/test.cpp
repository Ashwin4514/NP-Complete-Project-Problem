#include <iostream>
#include <unistd.h>
#include <sys/wait.h>  // Include this for waitpid

using namespace std;

int main() {
    cout << "This is Parent Process." << endl;

    pid_t child1 = fork();

    if (child1 == 0) {
        cout << "This is child process." << endl;
        
        for(int i=0; i<20; i++)
        {
          cout<<i<<",";
        }
        
        cout<<endl;
        // Child process logic here
    } else if (child1 == -1) {
        cerr << "Unable to fork child." << endl;
    } else {
        // This code is executed in the parent process.
        int status;
        waitpid(child1, &status, 0);  // Wait for the child to exit.
        if (WIFEXITED(status)) {
            cout << "Child process exited with status: " << WEXITSTATUS(status) << endl;
        } else {
            cerr << "Child process did not exit normally." << endl;
        }
    }

    return 0;
}
		

