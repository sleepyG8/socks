#include <WinSock2.h>
#include <stdio.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib") // Link the Winsock library becuase i was using cl on command line 

#define WEBSITE yourwebsite.com;

struct sockie {
    int af;
    int type;
    int protocol;
};
struct sockie create_sockie() {
    struct sockie s;
    s.af = AF_INET;
    s.type = SOCK_STREAM;
    s.protocol = 0;
    return s;
};

int main() {
    SOCKET sock = INVALID_SOCKET;
    WSADATA wsadata;
    char *message = NULL;
    char server_reply[2000];
    int recv_size;
    struct sockaddr_in server;
    struct sockie my_sockie = create_sockie();

    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        printf("WSAStartup failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    struct hostent *he;
    const char *hostname = "WEBSITE"; 
    sock = socket(my_sockie.af, my_sockie.type, my_sockie.protocol);
    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    if ((he = gethostbyname(hostname)) == NULL) {
        printf("gethostbyname failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    memcpy(&server.sin_addr, he->h_addr_list[0], he->h_length);
    server.sin_family = my_sockie.af;
    server.sin_port = htons(80);

    connect(sock, (struct sockaddr*)&server, sizeof(server));

    message = "GET / HTTP/1.1\r\nHost: WEBSITE\r\n\r\n";
    if (send(sock, message, strlen(message), 0) < 0) {
        printf("Send failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if ((recv_size = recv(sock, server_reply, 2000, 0 )) == SOCKET_ERROR) {
        printf("Error sending Get Request");
    } else {
        server_reply[recv_size] = '\0';
        printf("Data:\n %s", server_reply);
    }

    closesocket(sock);
    WSACleanup();
}
