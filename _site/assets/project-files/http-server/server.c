#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 8080
#define BUFFER_SIZE 512
#define MAX_REQUEST 2048

typedef struct{
    char method[16];
    char path[256];
    char version[16];
}HttpRequest;

void parse_request_line(char *line, HttpRequest *req){
    char *token;
    int part = 0;
    token = strtok(line, " ");
    while(token != NULL && part < 3) {
        if(part == 0) {
            strcpy(req->method, token);
        } else if(part == 1) {
            strcpy(req->path, token);
        } else if(part == 2) {
            strcpy(req->version, token);
        }
        token = strtok(NULL," ");
        part++;
    }
}


char* process_path(char *path) {
    char *response =malloc(1024);
    sprintf(response, 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "<html>"
            "<body>"
            "<h1>Memory Safe Server</h1>"
            "<p>You requested: %s</p>"
            "</body>"
            "</html>",path);
    
    return response;
}

void handle_client(int client_sock) {
    char buffer[BUFFER_SIZE];
    char request_data[MAX_REQUEST];
    int bytes_read;
    memset(buffer, 0, sizeof(buffer));
    memset(request_data, 0, sizeof(request_data));
    bytes_read = recv(client_sock, buffer, sizeof(buffer)-1, 0);
    if(bytes_read <= 0) {
        close(client_sock);
        return;
    }
    
    buffer[bytes_read] ='\0';
    strncpy(request_data, buffer, sizeof(request_data)-1);
    
    HttpRequest req;
    memset(&req, 0, sizeof(req));
    
    char *line = strtok(request_data, "\r\n");
    if(line) {
        parse_request_line(line, &req);
        
        printf("[REQUEST] %s%s%s\n", req.method, req.path, req.version);
        
        char *response = process_path(req.path);
        send(client_sock, response, strlen(response),0);
        free(response);
    }
    close(client_sock);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    printf("Starting Server on port %d\n",PORT);
    
    server_sock = socket(AF_INET, SOCK_STREAM,0);
    if(server_sock < 0) {
        perror("socket failed");
        return 1;
    }
    
    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);
    if(bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_sock);
        return 1;
    }
    
    if(listen(server_sock, 10) < 0) {
        perror("listen failed");
        close(server_sock);
        return 1;
    }
    
    printf("Server listening... \n");
    printf("Server is accessible at http://localhost:%d/\n\n", PORT);
    
    while(1) {
        client_sock = accept(server_sock,(struct sockaddr*)&client_addr,&client_len);
        if(client_sock < 0) {
            perror("Accept failed");
            continue;
        }
        handle_client(client_sock);
    }
    

    close(server_sock);
    return 0;
}
