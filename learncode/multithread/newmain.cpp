#include "thread.h"
#include <iostream>
using namespace std;
const int EXIT_SUCCESS =1;
class MultiThread : public Thread
{
public:

    void run()
    {
        int number = 0;
        for (int i = 0; i < 10; i++)
        {
            cout << "Current number is " << number++;
            cout << " PID is " << getpid() << " TID is " << getThreadID() << endl;
            sleep(1);
        }
    }
};


int main(int argc, char** argv)
{
    bool ret;
    MultiThread *mt;
    mt = new MultiThread();
    ret = mt->start();
    mt->join(6000000);
    return (EXIT_SUCCESS);
}

