#include <func.h>

int send_n(int newfd,char *p,int total){
    int len = 0;
    int cur=0;
    while(cur<total){
        len = send(newfd,p+cur,total-cur,0);
//        printf("%d\n",len);
        if(len==-1){
            return -1;
        }
        cur+=len;
    }
    return 0;
}
int recv_n(int newfd,char *p,int total)
{
    int len =0;
    int cur=0;
    while(cur<total){
        len = recv(newfd,p+cur,total-cur,0);
        cur+=len;	
        if(len==0){
            return -1;
        }

    }

    return 0;
}
