/*
 ============================================================================
 Name        : server.c
 Author      : Celeste Popoca
 Version     :
 Copyright   : Your copyright notice
 Description : This program is the server. It will return a message from a
               client for 5 rounds.
 ============================================================================

 Simple Messaging Protocol Description:
 ---------------------------------------
         Client 			        Server
         <<----TCP CONNECTION---->>
         <<----------OK----------
                  BEGINNING
           --------MESSAGE------->>
         <<----CAPITALIZE MSG----
            exit after 5 rounds
 ============================================================================
 */

#define DEFAULT_PORT 6789
// CHANGE ADDRESS
#define DEFAULT_ADDR "xx.xx.xx.xx"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(void) {
  // Create variables
  int server_sock, client_sock;
  struct sockaddr_in addr_server, addr_client;
  socklen_t addr_size;
  char buf_msg[512];
  int n;

  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0) {
    perror("Socket error!");
    exit(1);
  }
  printf("Socket created!\n");

  memset(&addr_server, '\0', sizeof(addr_server));
  addr_server.sin_family = AF_INET;
  addr_server.sin_port = DEFAULT_PORT;
  addr_server.sin_addr.s_addr = inet_addr(DEFAULT_ADDR);

  n = bind(server_sock, (struct sockaddr *)&addr_server, sizeof(addr_server));
  if (n < 0) {
    perror("Bind error!");
    exit(1);
  }
  printf("Bound to port number.\n");

  // Open the server to one client
  listen(server_sock, 1);
  printf("Listening for client...\n");

  // Connect the client to address and port
  addr_size = sizeof(addr_client);
  client_sock = accept(server_sock, (struct sockaddr *)&addr_client, &addr_size);
  printf("Client connected!\n");

  // Get OK
  bzero(buf_msg, 512);  // Clear string variable
  recv(client_sock, buf_msg, sizeof(buf_msg), 0);
  printf("Client: %s\n", buf_msg);

  // Send confirmation message
  bzero(buf_msg, 512);  // Clear string variable
  strcpy(buf_msg, "Welcome! :)");
  printf("Server sending: %s\n", buf_msg);
  send(client_sock, buf_msg, sizeof(buf_msg), 0);

  printf("Beginning message exchange with client.\n\n");

  // Begin message loop for 5 iterations
  int i = 0;
  while (i < 5) {
    // Receive message
    bzero(buf_msg, 512);
    recv(client_sock, buf_msg, sizeof(buf_msg), 0);
    printf("Client: %s", buf_msg);
    
    // Capitalize first letter of message
    buf_msg[0] = toupper(buf_msg[0]);

    // Send message
    printf("Server sending: %s\n\n", buf_msg);
    send(client_sock, buf_msg, sizeof(buf_msg), 0);

    i++;
  }

  shutdown(client_sock, 2);
  printf("Client disconnected.\n");

  return 0;
}
