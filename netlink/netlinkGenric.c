#include<stdio.h>
#include<linux/netlink.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<errno.h>


#define NETLINK_PROTO_KATHIR 26
#define PAYLOAD_SIZE 256


int main() {
    int sock_fd;
    struct sockaddr_nl sa;
    struct nlmsghdr *nh = NULL;
    struct iovec iov;
    struct msghdr msg;
    int seq = 1;

    if((sock_fd = socket(AF_NETLINK,SOCK_RAW,NETLINK_PROTO_KATHIR)) < 0) {
        perror("socket:");
        return -1;
    }
    sa.nl_family = AF_NETLINK;
    sa.nl_groups = 0;
    sa.nl_pid = 0;

    if(bind(sock_fd,(struct sockaddr *)&sa,sizeof(sa)) < 0){
      
        perror("bind:");
        close(sock_fd);
        return -1;
    }


    if((nh = malloc(sizeof(struct nlmsghdr)+ NLMSG_SPACE(PAYLOAD_SIZE))) == NULL) {
        perror("Malloc:");
        close(sock_fd);
        return -1;
    }
    nh->nlmsg_len = NLMSG_SPACE(PAYLOAD_SIZE);
    nh->nlmsg_pid = 0;
    nh->nlmsg_flags = 0;
    nh->nlmsg_seq = seq++;

    strncpy((char *)NLMSG_DATA(nh),"Hello kernel",PAYLOAD_SIZE);
    
    memset(&iov,0,sizeof(struct iovec));
    iov.iov_base = (void *)nh;
    iov.iov_len = nh->nlmsg_len;

    memset(&msg,0,sizeof(struct msghdr));
    msg.msg_name = &sa;
    msg.msg_namelen = sizeof(sa);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;


    if(sendmsg(sock_fd,&msg,0)<0){
          if(errno ) {
            printf("%X\n",errno);
        }
        perror("sendmsg:\n");
    }

    
    

    close(sock_fd);

    return 0;
}