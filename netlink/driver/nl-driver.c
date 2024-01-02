#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/netlink.h>
#include<net/netlink.h>
#include<net/net_namespace.h>

static struct sock *sk = NULL;
#define NETLINK_PROTO_KATHIR 26
static void recv_func_kathir(struct sk_buff *buff) {
    struct nlmsghdr *nh = NULL;
    printk("Entering %s\n",__FUNCTION__);
    nh = (struct nlmsghdr *)buff->data;
    printk("Message from userspace=%s\n",(char *)nlmsg_data(nh));
    return;
}


static int __init start_nl_module (void) {

    struct netlink_kernel_cfg cfg = {   .input = recv_func_kathir,
    };

    struct socket *l_sk;
    if(sock_create(AF_NETLINK,SOCK_RAW,NETLINK_PROTO_KATHIR,&l_sk) < 0){
        if((sk = netlink_kernel_create(&init_net,NETLINK_PROTO_KATHIR,&cfg)) == NULL){
            printk("Error creating netlink socket\n");
            return -1;
        } else {
            printk("created successfully\n");
        }
    } else {
        sock_release(l_sk);
        printk("Socket was already created\n");
    }


    return 0;
}



static void __exit end_nl_module(void) {
if(sk) {
        netlink_kernel_release(sk);
}
    return;
}




module_init(start_nl_module);
module_exit(end_nl_module);



MODULE_LICENSE("GPL");
MODULE_AUTHOR("KATHIR");
MODULE_DESCRIPTION("Genric nl module");
MODULE_VERSION("1");


