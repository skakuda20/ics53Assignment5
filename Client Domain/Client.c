// Client.c 

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 9999
#define MAXARGS 3



// From the lect slides: open_clientfd; prob dont need this much

// int open_clientfd(char *hostname, char *port){
//   int clientfd;
//   struct addrinfo hints, *listp, *p;
//   memset(&hints, 0, sizeof(struct addrinfo));
//   hints.ai_socktype = SOCK_STREAM;
//   hints.ai_flags = AI_NUMERICSERV;
//   hints.ai_flags |= AI_ADDRCONFIG;
//   getadderinfo(hostname, port, &hints, &listp);

//   for (p = listp; p; p = p->ai_next) {
//     if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
//       continue;
//     if (connect(clientfd, p->ai_addr, p->ai_adderlen) != -1)
//       break;
//     close(clientfd);
//   }
// }



// establish connection:
//int connect(int clientfd, SA *adder, socklen_t addreln) {}


void tokenize(char *input, char *args[MAXARGS]){
  int i = 0;
  char *token = strtok(input, " \n");
  do{
    args[i] = token;
    i++;
  } while ((token = strtok(NULL, " \n")));
}


void pauseCmd(int length){
  // use sleep? eg: sleep(length);
  sleep(length);
  //pid_t wait(length);
}


void appendCmd(char* file_name){
  /*
  // open in append mode
  // char destination[] = '/Remote Directory/' + file_name?
  char* path = "\\RemoteDirectory\\\n";
  strcat(path, file_name);
  FILE *fptr;
  fptr = fopen(path, "wb");
  //int fd;
  //fd = popen(file_name,O_RDWR|O_APPEND);
  if (fptr == NULL)
  {
    printf("FILE [");
    printf("%s", file_name);
    printf("] could not be found in driectory\n");
    return;
  }
  while(1)
    {
      printf("Appending> ");
      char input[500];
      
      fgets(input, 500, stdin); 
      if (strcmp(input, "close") == 0){
        break;
      }
      else{
        fwrite(input, 1, sizeof(input), fptr);
      }
    }
  */
}


void uploadCmd(char* filename){ 
  // see example3 
}
 

void downloadCmd(char *filename, int client_socket){
  // see example3
  int received_size;
  char* path = "\\RemoteDirectory\\\n"; // idk if thats the right way to do the \ in strings but it stopped giving an error lol
  // path + filename?
  strcat(path, filename);
  //char destination_path[] = path; // do we need destination path if we can just use path?
  int chunk_size = 1000;
  char file_chunk[chunk_size];
  FILE *fptr;
  fptr = fopen(path, "wb");
  while(1){
    bzero(file_chunk, chunk_size);
    received_size = recv(client_socket, file_chunk, chunk_size, 0);
    if (received_size == 0){
      close(client_socket); // wait do we close socket here or after quit?
      fclose(fptr);
      break;
    }
    fwrite(&file_chunk, sizeof(char), received_size, fptr);
  }
}


void deleteCmd(char* filename){
  /*
  // remove function
  // char destination[] = '/Remote Directory/' + file_name?
  int r = remove(filename);

  if(r == 0) {
    printf("File deleted successfully");
  } else {
    printf("File [");
    printf(filename);
    printf("] could not be found in remote directory.\n");
    }
  */
}



void syncheckCmd(){
  // confusing TBD
  // hash function
}



void execute(char* argv[MAXARGS], int client_socket){
  char *command = argv[0];
  // pause
  if (strcmp(command, "pause") == 0)
    pauseCmd(atoi(argv[1]));
  // append
  else if (strcmp(command, "append") == 0)
    appendCmd(argv[1]);
  // upload
  else if (strcmp(command, "uplaod") == 0)
    uploadCmd(argv[1]);
  // download
  else if (strcmp(command, "download") == 0)
    downloadCmd(argv[1], client_socket);
  // delete
  else if (strcmp(command, "delete") == 0)
    deleteCmd(argv[1]);
  // syncheck
  else if (strcmp(command, "syncheck") == 0)
    syncheckCmd();
}


void loop(char* file, char* IP, int client_socket) {
  char input[500];
  char *args[MAXARGS];
  do {
    printf("> ");
    fgets(input, 500, stdin); // was fgets(input, 500, file); why file and not stdin?
    // arent we taking a file in the directory and read from that? not sure though
    // so maybe not enough fgets but read file?
    tokenize((char*)input, args);
    if (strcmp(args[0], "quit") == 0){
      exit(0);
    }
    execute(args, client_socket);
  } while (1);
}


int main() {

  /*
  // Create socket
  int sock;
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Socket creation error \n");
    return -1;
  }

  // Connect
  struct sockaddr_in serv_addr;
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\nConnection Failed \n");
    return -1;
  }
  */

  // read from terminal to know which file we reading
  // to be change later into reading straight from "Remote Directory" folder

  int client_socket;
  struct sockaddr_in serv_addr;
  client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  if (client_socket < 0){
    printf("\n Socket creation error \n");
    return -1;
  }
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  int addr_status = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
  if (addr_status <= 0){
    printf("\nInvalid address/ Adress not supported \n");
    return -1;
  }
  int connect_status = connect(client_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if (connect_status < 0){
    printf("\nConnection Failed\n");
    return -1;
  }
  

  char input[500];
  char *args[MAXARGS];
  
  fgets(input, 500, stdin);
  tokenize((char*)input, args);
  char* file = args[0]; 
  char* IP = args[1];
  
  printf("Welcome to ICS53 Online Cloud Storage.");
  
  loop(file, IP, client_socket);
  return 0;
}