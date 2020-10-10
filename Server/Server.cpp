#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 1024

const char* get_result(char*);
bool is_number(char);
bool is_charecter(char);

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Please enter the port\n");
        return 1;
    }

    printf("Server is running in port %s\n", argv[1]);

    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, argv[1], &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    char recvbuf[DEFAULT_BUFLEN + 1];
    int recvbuflen = DEFAULT_BUFLEN;

    while (true)
    {
        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }

        while (true)
        {
            int bytesReceived = recv(ClientSocket, recvbuf, recvbuflen, 0);

            if (bytesReceived == SOCKET_ERROR) {
                break;
            }

            if (bytesReceived < recvbuflen) {
                recvbuf[bytesReceived] = '\0';
            }

            printf("Listened: %s\n", recvbuf);


            // Echo the buffer back to the sender
            const char* sendBuf = get_result(recvbuf);
            int receivedbuflen = send(ClientSocket, sendBuf, strlen(sendBuf), 0);
        }

        closesocket(ClientSocket);
    }

    closesocket(ListenSocket);
    WSACleanup();

    return 0;
}

const char* get_result(char *data) {
    int length = strlen(data);

    char numbers[1024];
    char characters[1024];

    int i_number = 0;
    int i_character = 0;
    
    for (int i = 0; i < length; i++) {
        if (is_number(data[i]) == false && is_charecter(data[i]) == false) {
            return "Error: String contains special charaacter.";
        }

        if (is_number(data[i])){
            numbers[i_number] = data[i];
            i_number++;
            continue;
        }

        characters[i_character] = data[i];
        i_character++;
    }

    characters[i_character] = '\0';
    numbers[i_number] = '\0';

    char result[1024];

    strcpy_s(result, "Result:\n");
    strcat_s(result, characters);
    strcat_s(result, "\n");
    strcat_s(result, numbers);

    return result;
}

bool is_number(char c) {
    return c >= '0' && c <= '9';
}

bool is_charecter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}