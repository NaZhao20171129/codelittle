#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

#include<libgen.h>

#include<string.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<strings.h>

#include<assert.h>


#include<arpa/inet.h>
#include<fcntl.h>

#include<sys/wait.h>

#include<errno.h>

#include<time.h>

//deal with select
#include<sys/select.h>
//deal with epoll
#include<sys/epoll.h>

//deal with shared memory
#include<sys/mman.h>
#include<sys/stat.h>

//deal with the semaphore,for multiple threads
#include<semaphore.h>


#include<list>



int setnonblock(int fd)
{
        int oldoption=fcntl(fd,F_GETFL);
        int newoption=oldoption|O_NONBLOCK;
        fcntl(fd,F_SETFL,newoption);
        return oldoption;

}
