#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>

ssize_t readn(int fd, void *vptr, size_t n)
{
    size_t nleft, nread;
    char *ptr;

    ptr = vptr;
    nleft = n;
    while(nleft > 0){
        if(nread = (read(fd, ptr, nleft)) < 0){
            if(errno == EINTR)
                nread = 0;
            else
                return -1;
        }
        else if(nread == 0){
            break;
        }

        ptr += nread;
        nleft -= nread;
    }

    return (n - nleft);

}

ssize_t writen(int fd, void *vptr, size_t n)
{
    size_t nleft, nwrite;
    char *ptr;

    nleft = n;

    while (nleft > 0)
    {
       if((nwrite = write(fd, ptr, nleft)) < 0){
            if(nwrite < 0 && errno == EINTR)
                nwrite = 0;
            else
                return -1;
       }

       nleft -= nwrite;
       ptr += nwrite; 
    }
    
    return n;
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t n, rc;
    char *ptr;

    ptr = vptr;
    for(n = 1; n < maxlen; n++){
        if((rc = read(fd, ptr, 1) == 1)){
            if(*ptr++ == '\n')
                break;
        }else if(rc == 0){
            *ptr = 0;
            return (n - 1);
        }else{
            if(errno == EINTR)
                n--;
            else
                return -1;
        }
    }

    *ptr = 0;
    return n;
}

