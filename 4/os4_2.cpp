#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    cout << "os4_2 started" << endl << flush;

    pid_t pid = fork();
    int status;
    
    if (pid == 0)
    {
        execlp("./os4_1", "one", "two", "three", "four", NULL);
    }
    else
    {   
        cout << "os4_2 parent pid: " << getppid() << endl << flush;
        cout << "os4_2 pid: " << getpid() << endl << flush;

        while(waitpid(pid, &status, WNOHANG) == 0)
            sleep(0.5);

        cout << "Process returned " << (WEXITSTATUS(status)) << endl;
    }

    exit(0);
}