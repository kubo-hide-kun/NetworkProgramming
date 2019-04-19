#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

struct in_addr{
    unsigned long s_addr;
};

struct sockaddr_in {
    unsigned short sin_family;
    unsigned short sin_port;
    struct in_addr sin_addr;
    char sin_zero[8];
};

int main(int argc, char **argv){
    int sock = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_addr.s_addr = inet_addr("10.13.64.20");
    target.sin_port = htons(10001);
    connect(sock,(struct sockaddr *)&target,sizeof(target));
    close(sock);
    return 0;
}