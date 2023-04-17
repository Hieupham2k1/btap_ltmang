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
    // scanf("%s %d", address, &port);


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
    
    // Gửi tin nhắn đến server
    char input[64] = "";
    char msg[256] = "";
    char separator[4] = ",.,";

    printf("Ten may tinh: ");
    //getchar();
    fgets(input, sizeof(input), stdin);
    strtok(input, "\n");
    strcat(msg, input);
    strcat(msg, separator);

    int driveNum = 0;
    printf("So o dia: ");
    scanf("%d", &driveNum);
    getchar();
    char str[10];

    sprintf(str, "%d", driveNum);
    strcat(msg, str);
    strcat(msg, separator);

    for(int i = 0; i < driveNum; i++) {
        printf("O dia thu %d: ", i+1);
        fgets(input, sizeof(input), stdin);
        strtok(input, "\n");
        strcat(msg, input);
        strcat(msg, separator);
    }

    printf("%s", msg);


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