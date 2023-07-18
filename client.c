/*
 ============================================================================
 Name        : client.c
 Author      : Celeste Popoca
 Version     :
 Copyright   : Your copyright notice
 Description : This program is the client to a server. It will ask for a
               message input, send it to the server, and return it with the 
               first letter capitalized.
 ============================================================================
 */
#define DEFAULT_PORT 6789
// CHANGE ADDRESS
#define DEFAULT_ADDR "xx.xx.xx.xx"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(void) {
  // Create variables
  int sock;
  struct sockaddr_in addr;
  socklen_t addr_size;
  char buf_msg[512];
  int n;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("Socket error!");
    exit(1);
  }
  printf("Socket created!\n");

  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = DEFAULT_PORT;
  addr.sin_addr.s_addr = inet_addr(DEFAULT_ADDR);

  connect(sock, (struct sockaddr*) &addr, sizeof(addr));
  printf("Connected to server! :)\n");

  // Send OK. Connected to server.
  bzero(buf_msg, 512);   // Clear string variable
  strcpy(buf_msg, "OK");
  printf("Client: %s\n", buf_msg);
  send(sock, buf_msg, strlen(buf_msg), 0);

  // Recieve welcome message
  bzero(buf_msg, 512);  // Clear string variable
  recv(sock, buf_msg, sizeof(buf_msg), 0);
  printf("Server: %s\n\n", buf_msg);

  // Begin message loop for 5 iterations
  int i = 0;
  while (i < 5) {
    // Send message
    bzero(buf_msg, 512);   // Clear string variable
    printf("Message > ");
    fgets(buf_msg, sizeof(buf_msg), stdin);
    send(sock, buf_msg, strlen(buf_msg), 0);

    // Receive capitalized message
    bzero(buf_msg, 512);  // Clear string variable
    recv(sock, buf_msg, sizeof(buf_msg), 0);
    printf("Server: %s\n\n", buf_msg);

    i++;
  }

  shutdown(sock, 2);
  printf("Disconnected from server.\n");
  
  return 0;
}
