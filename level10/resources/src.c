#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 6969

int main() {
  int sockfd, newsockfd;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;

  // Create a socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("ERROR opening socket");
    exit(1);
  }

  // Initialize socket structure
  memset((char *) &serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(PORT);

  // Bind the host address
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    perror("ERROR on binding");
    exit(1);
  }

  // Start listening for the clients
  listen(sockfd, 5);
  clilen = sizeof(cli_addr);

  // Declare a dynamic array to store the socket file descriptors
  int *sockets = malloc(0);
  int num_sockets = 0;

  while (1) {
    // Accept a connection from a client
    printf("Waiting for a client to connect...\n");
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
    }
    printf("Client connected!\n");

    // Add the new socket to the array
    sockets = realloc(sockets, (num_sockets + 1) * sizeof(int));
    sockets[num_sockets] = newsockfd;
    num_sockets++;

    // Wait for enter to be pressed
    printf("Press enter to receive a message or 'q' to wait for another client: ");
    char c = getchar();
    if (c == '\n') {
      // Read from the last socket
      printf("Received: ");
      char buffer[256];
      int n = read(sockets[num_sockets - 1], buffer, 255);
      if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
      }
      printf("%s\n", buffer);
    } else if (c == 'q') {
      continue;
    }
  }

  return 0;
}