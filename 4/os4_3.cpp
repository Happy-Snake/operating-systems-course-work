#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int child_proc(void *arg)
{   
    cout << "os4_3 child pid: " << getpid() << endl;
    execlp("./os4_1", "five", "six", "seven", "eight", NULL);
}

#define STACK_SIZE (1024*1024)

int main(int argc, char* argv[])
{
    cout << "os4_3 started" << endl << flush;
    cout << "os4_3 pid: " << getpid() << endl << flush;
    char *stack;
    char *stackTop;
    stack = (char*)malloc(STACK_SIZE);
    stackTop = stack + STACK_SIZE;
    pid_t pid;
    int status;
   
    pid = clone(child_proc, stackTop, SIGCHLD, NULL);
    
    while(waitpid(pid, &status, WNOHANG) == 0)
        sleep(0.5);

    cout << "Process returned " << (WEXITSTATUS(status)) << endl;

    exit(0);
}