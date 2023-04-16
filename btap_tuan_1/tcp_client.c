#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main() {

    int port = 9000;
    char address[30] = "127.0.0.1";
    printf("tcp_client ");
    scanf("%s %d", address, &port);


    // Khai báo socket kết nối đến server
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Khai báo địa chỉ của server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(address);
    addr.sin_port = htons(port); 

    // Kết nối đến server
    int res = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1) {
        perror("connect() failed");
        return 1;
    }

    char greetingMessage[2048];
    int glen = recv(client, greetingMessage, sizeof(greetingMessage), 0);
    if (glen <= 0)
    {
        printf("recv() failed.\n");
        exit(1);
    }

    greetingMessage[glen] = 0;
    printf("Greeting Message: %s\n", greetingMessage);
    
    // Gửi tin nhắn đến server
    char msg[30] = "Hello server";
    printf("Send message: ");
    // scanf("%s", msg);
    getchar();
    fgets(msg, sizeof(msg), stdin);
    send(client, msg, strlen(msg), 0);

    // Nhận tin nhắn từ server
    char buf[2048];
    int len = recv(client, buf, sizeof(buf), 0);
    if (len <= 0)
    {
        printf("recv() failed.\n");
        exit(1);
    }

    buf[len] = 0;
    printf("Data received: %s\n", buf);

    // Kết thúc, đóng socket
    close(client);

    return 0;
}