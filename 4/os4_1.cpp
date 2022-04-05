#include <sys/types.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
{
    cout << "os4_1 started" << endl << flush;

    for (int i = 0; argv[i] != 0; i++)
    {
        cout << argv[i] << ", ";
        sleep(1);
    }

    cout << endl << "os4_1 pid: " << getpid() << endl << flush;
    cout << "os4_1 parent pid: " << getppid() << endl << flush;

    exit (argc*5);
}