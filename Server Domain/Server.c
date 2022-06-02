#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 9999
#define MAXLINE 500



int main() {
/* create and configure the listening socket */
  // Creating socket file descriptor
  int server_fd;
  if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  int opt = 1;
// Forcefully attaching socket to the port 8080
// Set socket options
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
&opt, sizeof(opt)))
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in address;
  //BIND
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );
  // Forcefully attaching socket to the port
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  } 

  
  //LISTEN
  if (listen(server_fd, 3) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  while(1) {
/* Accept(): wait for a connection request */
    //ACCEPT
    int new_socket;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    if ((new_socket = accept(server_fd, 
(struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    {
      perror("accept");
      exit(EXIT_FAILURE);
    }
/* echo(): read and echo input lines from client till EOF */
    size_t n;
    char buf[MAXLINE];
    while((n = read(new_socket, buf, MAXLINE)) != 0) { 
      printf("server received %d bytes\n", (int)n);
      write(new_socket, buf, n);
    }
/* Close(): close the connection */ 
    close(new_socket);
  }
  exit(0);
  return 0;
}