#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <time.h>

int main() 
{

    int port = 9000;
    char address[30] = "127.0.0.1";
    char file[30] = "sv_log";
    printf("tcp_server ");
    scanf("%s %d", address, &port);

while (1)
{

    // Tạo socket chờ kết nối
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int option = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (listener != -1)
        printf("socket created: %d\n", listener);
    else
    {
        perror("socket() failed");
        exit(1);
    }

    // Khai báo cấu trúc địa chỉ của server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(address);
    addr.sin_port = htons(port);

    // Gắn socket với cấu trúc địa chỉ
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))) 
    {
        perror("bind() failed");
        exit(1);
    }

    // Chuyển socket sang trạng thái chờ kết nối
    if (listen(listener, 5)) 
    {
        perror("listen() failed");
        exit(1);
    }


    printf("waiting for a new client ...\n");

    // Chờ và chấp nhận kết nối
    int client = accept(listener, NULL, NULL);
    if (client == -1)
    {
        perror("accept() failed");
        exit(1);
    }
    printf("new client connected: %d\n", client);

    // Nhận dữ liệu từ client
    char buf[256];
    int ret = recv(client, buf, sizeof(buf), 0);

    // Kiểm tra kết nối có bị đóng hoặc hủy không
    if (ret <= 0)
    {
        printf("recv() failed.\n");
        exit(1);
    }

    // Thêm ký tự kết thúc xâu và in ra màn hình
    if (ret < sizeof(buf))
        buf[ret] = 0;


    char separator[4] = ",.,";

    char client_addr[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &addr, client_addr, INET_ADDRSTRLEN );
    strcat(buf, separator);
    strcat(buf, client_addr);

    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strcat(buf, separator);
    strcat(buf, asctime (timeinfo));
    

    puts(buf);

    
    FILE *pFile;
    pFile = fopen(file, "a");
    fprintf(pFile, buf);
    fprintf(pFile, "\n");
    fclose(pFile);
    

    // Đóng kết nối
    close(client);
    close(listener);    
}

    return 0;
}
