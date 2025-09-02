#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include "a2l_generator.h"

#define PORT 5555

int main() {
    printf("🚗 Sample ECU Demo starting...\n");

    // Generate A2L
    Variable vars[] = {
        {"rpm", "UWORD", 0, 8000},
        {"throttle", "UBYTE", 0, 100},
        {"ignition_timing", "SWORD", -90, 90}
    };
    generate_a2l("sample_ecu_demo.a2l", "sample_ecu_demo", vars, 3);

    // Create UDP socket for XCP-like demo
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);
    char buffer[1024];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.228.92");  // <-- Fixed IP
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("✅ ECU server running on UDP port %d\n", PORT);
    printf("📡 Import sample_ecu_demo.a2l in CANape and connect to 127.0.0.1:%d\n", PORT);

    while (1) {
        int n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0,
                         (struct sockaddr *)&cliaddr, &len);
        if (n > 0) {
            buffer[n] = '\0';
            printf("📩 Received: %s\n", buffer);
            // Echo back
            sendto(sockfd, buffer, n, 0, (struct sockaddr *)&cliaddr, len);
        }
    }

    close(sockfd);
    return 0;
}
