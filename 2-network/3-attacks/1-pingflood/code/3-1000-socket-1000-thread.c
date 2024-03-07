#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>

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

void *thread_worker(void *vargp) {
    char *ip = (char*) vargp;
    // Set up raw socket 
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        fprintf(stderr, "Error in creating raw socket\n");
        exit(1);
    }

    // Set up sock address
    
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

    sendto(sockfd, send_buff, sizeof(uint8_t) * send_buff_size, 0, (struct sockaddr *)&target, sizeof(target));
}

int main(int argc, char *argv[]) {
    char *ip = argv[1];

    // Set up 1000 threads 
    pthread_t thread_id[1000];


    for (int i = 0; i < 1000; ++i) {
        pthread_create(&thread_id[i], NULL, thread_worker, (void*)ip);
    }

    for (int i = 0; i < 1000; ++i) {
        pthread_join(thread_id[i], NULL);
    }

    return 0;
}