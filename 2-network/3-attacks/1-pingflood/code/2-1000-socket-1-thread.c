#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2) {
        sum += *buf++;
    }

    if (len == 1) {
        sum += *(unsigned char *)buf;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = (unsigned short)~sum;

    return result;
}

int main(int argc, char *argv[]) {
    // Set up 1000 raw socket
    int sockfd[1000];

    for (int i = 0; i < 1000; ++i) {
        sockfd[i] = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

        if (sockfd[i] < 0) {
            fprintf(stderr, "Error in creating raw socket\n");
            exit(1);
        }
    }

    // Set up sock address
    char *ip = argv[1];
    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_addr.s_addr = inet_addr(ip);

    // Set up send buff
    uint8_t send_buff[64];
    uint8_t send_buff_size = 64;

    memset(send_buff, 0, sizeof(uint8_t) * 64);


    // Set up icmp header
    struct icmphdr *icmp = (struct icmphdr *) send_buff;

    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->un.echo.id = getpid();
    icmp->un.echo.sequence = 0;
    icmp->checksum = checksum(send_buff, sizeof(uint8_t) * send_buff_size);

    for (int i = 0; i < 1000; ++i) {
        sendto(sockfd[i], send_buff, sizeof(uint8_t) * send_buff_size, 0, (struct sockaddr *)&target, sizeof(target));
    }
    

    return 0;
}