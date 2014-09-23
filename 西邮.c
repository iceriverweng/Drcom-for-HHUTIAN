#include <stdio.h> 
#include <string.h> 
//#include <netdb.h> 
#include <winsock2.h> 
#pragma   comment(lib,"WS2_32.lib") 
#include <windows.h>
#pragma comment(lib, "wsock32.lib")
#include <signal.h>
//#include <arpa/inet.h> 
//#include <netinet/in.h> 
#include <sys/types.h> 
//#include <sys/socket.h> 
//#include <sys/ioctl.h> 
//#include <net/if.h> 
//#include <net/if_arp.h> 
//#include <net/ethernet.h> 
//#include <netinet/ip.h> 
 
typedef struct _code2 { 
    char sign; 
    char type; 
    short len; 
    int id; 
    int code0; 
    int code1; 
}code2; 
 
typedef struct _code3 { 
    char sign; 
    char type; 
    short len; 
    int id; 
    int unk0; 
    int code1; 
    int unk1; 
    int code0; 
    int check; 
    int unk2; 
    char mac[6]; 
    char dhcp; 
    char devtype; 
    int ipaddr; 
    int netmask; 
    char padding[48]; 
}code3; 
 
int waitdata(int hsocket, int time) 
{ 
    fd_set fds; 
    struct timeval tv; 
 
    FD_ZERO(&fds); 
    FD_SET(hsocket,&fds); 
    tv.tv_usec = time; 
    tv.tv_sec = 0; 
    return select(hsocket+1,&fds,NULL,NULL,&tv); 
} 
 
 
main (int argc, char* argv[]) 
{ 
    struct in_addr ip,netmask,ptp; 
    char mac[6]; 
    int hsocket; 
    struct ifreq ifr; 
    struct sockaddr_in sin; 
    struct sockaddr *sa = (struct sockaddr *) &sin; 
    int fromlen; 
    struct sockaddr_in *sin_ptr; 
    u_char *ptr; 
    u_long *ptr_long;
    int n,workwell = 0;
    int check; 
    char c1[] = {7,1,8,0,1,0,0,0};
    code2 c2;
    code3 c3; 
    char c4[512]; 
    int sendlen, recvlen; 
 
    printf("\nFree Dr.COM for Linux  v0.2\n  mengxp works,2009 @ xiyou\n  http://www.deadc0de.com/\n\n"); 
    if ((hsocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    { 
        perror( "socket error!"); 
        return 0; 
    } 
 
    strcpy(ifr.ifr_name, "ppp0"); 

//    if (setsockopt(hsocket,SOL_SOCKET,SO_BINDTODEVICE,(char *)&ifr, sizeof(ifr)) < 0)
//        perror("SO_BINDTODEVICE failed");

    if (ioctl(hsocket, SIOCGIFADDR, &ifr) < 0) 
        perror("ioctl SIOCGIFADDR error"); 
    sin_ptr = (struct sockaddr_in *)&ifr.ifr_addr; 
    ip = sin_ptr->sin_addr; 
 
    if (ioctl(hsocket, SIOCGIFNETMASK, &ifr) < 0) 
        perror("ioctl SIOCGIFNETMASK error"); 
    sin_ptr = (struct sockaddr_in *)&ifr.ifr_addr; 
    netmask = sin_ptr->sin_addr;

    if (ioctl(hsocket, SIOCGIFDSTADDR, &ifr) < 0) 
        perror("ioctl SIOCGIFDSTADDR error"); 
    sin_ptr = (struct sockaddr_in *)&ifr.ifr_addr; 
    ptp = sin_ptr->sin_addr;
    memset(&mac,0,6);
 
    printf("ppp0 interface:\n"); 
    printf(" IP:      %s\n", inet_ntoa(ip)); 
    printf(" NetMask: %s\n", inet_ntoa(netmask)); 
    printf(" Server:  %s\n\n", inet_ntoa(ptp)); 

    sin.sin_addr = ip; 
    sin.sin_port = htons(61440); 
    sin.sin_family = AF_INET; 
    if ((bind(hsocket,sa,sizeof(sin))) < 0) 
    { 
        printf( "can't bind port 61440!\n"); 
        return 0; 
    } 

    sin.sin_addr = ptp; 

keepalive:
    if(!workwell) 
        printf("Try to connect Dr.COM Server...\n");
    while(1) 
    { 
        sendlen = sendto(hsocket,&c1,8,0,sa,sizeof(sin));
        if(sendlen < 0)
            perror("send1 "); 
        if(waitdata(hsocket,1000*1000)) 
        { 
            fromlen = sizeof(sin); 
            recvlen = recvfrom(hsocket,&c2,sizeof(c2),0,sa,&fromlen);
            ip = sin.sin_addr;
            break; 
        } 
        else 
            printf("recv2 timed out\n"); 
    }
    
    memset(&c3,0,96); 
    c3.sign = 7;
    c3.len = 96;
    c3.id = 3;
    c3.code1 = c2.code1;
    c3.unk1 = 0x2000200;
    c3.code0 = c2.code0;
    c3.check = 0x1312fc7;
    c3.unk2 = 0x7e;
    memcpy(&c3.mac,&mac,6);
    c3.devtype = 0x17;
    memcpy(&c3.ipaddr,&ip,4);
    memcpy(&c3.netmask,&netmask,4);
    
    ptr_long = (u_long *)&c3; 
    for(n=0,check=0;n<96/4;n++)
        check ^= *(ptr_long + n);
    c3.check = check * 0x12C4B7E;
    c3.unk2 = 0;
    
    ptr = (u_char *)&c3.mac; 
    for(n=0;n<64;n++)
        *(ptr + n) = (*(ptr + n) >> 8 - (n & 7)) + (*(ptr + n) << (n & 7));
    
    sendlen = sendto(hsocket,&c3,96,0,sa,sizeof(sin));
    if(sendlen < 0)
        perror("send3 "); 
    if(waitdata(hsocket,1000*1000)) 
    { 
        fromlen = sizeof(sin); 
        recvlen = recvfrom(hsocket,&c2,sizeof(c2),0,sa,&fromlen);
        if(!workwell)
        {
        printf("Connected,Keeping connection active...\n"); 
        workwell = 1;
        }
    } 
    else
    { 
        printf("recv4 timed out\n");
        sleep(1);
        goto keepalive;
    } 

    sleep(15);
    goto keepalive;
    return 0; 
} 
