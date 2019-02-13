#include <func.h>

int init(int *sofd,char *ip,char *port)

{
    int sfd;
    sfd = socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1){
        perror("socket");
        return -1;
    }
    struct sockaddr_in ser;
    ser.sin_family=AF_INET;
    ser.sin_port = htons(atoi(port));
    ser.sin_addr.s_addr = inet_addr(ip);
    socklen_t addrlen = sizeof(struct sockaddr);
    int reuse=1;
    setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    int ret = bind(sfd,(struct sockaddr*)&ser,addrlen);
    if(-1==ret){
        perror("bind");
        return -1;
    }
    ret = listen(sfd,5);
    if(-1==ret){
        perror("listen");
        return -1;
    }
    *sofd = sfd;
    return 0;
}

