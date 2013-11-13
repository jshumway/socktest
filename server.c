#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BACKLOG 25
#define MESSAGE_LENGTH_LIMIT 255
#define PORT "1337"

void caesar(char *str)
{
   int ndx;
   char chr, offset;

   for (ndx = 0; ndx < strlen(str); ndx++) {
      chr = str[ndx];

      if (isalpha(chr)) {
         offset = isupper(chr) ? 'A': 'a';

         chr = chr - offset + 3;
         chr %= 26;
         chr = chr + offset;

         str[ndx] = chr;
      }
   }
}

int accept_conn(int sockfd)
{
   char buffer[MESSAGE_LENGTH_LIMIT];
   int connfd, rv;
   struct sockaddr_storage remote_addr;
   unsigned int addr_size = sizeof(remote_addr);

   if ((connfd = accept(sockfd, (struct sockaddr *) &remote_addr, &addr_size))
         == -1) {
      fprintf(stderr, "Could not accpet: %s\n", strerror(errno));
      return 0;
   }

   memset(buffer, 0, MESSAGE_LENGTH_LIMIT);
   rv = recv(connfd, buffer, MESSAGE_LENGTH_LIMIT - 1, 0);

   if (rv == 0) {
      fprintf(stderr, "Remote host hung up\n");
   } else if (rv == -1) {
      perror("server: recv");
   } else {
      printf("server: recv %3d: %s", rv, buffer);

      caesar(buffer);

      if ((rv = send(connfd, buffer, strlen(buffer), 0)) == -1)
         perror("server: send");
      else
         printf("server: send %3d: %s", rv, buffer);
   }

   return 0;
}

int main(void)
{
   int sockfd, rv;
   struct addrinfo hints, *servinfo, *p;

   memset(&hints, 0, sizeof hints);
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_PASSIVE;

   if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
   }

   for (p = servinfo; p != NULL; p = p->ai_next) {
      if ((sockfd = socket(p->ai_family, p->ai_socktype,
            p->ai_protocol)) == -1) {
         perror("server: socket");
         continue;
      }

      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
         close(sockfd);
         perror("server: bind");
         continue;
      }

      break;
   }

   if (p == NULL) {
      fprintf(stderr, "server: failed to bind\n");
      return 2;
   }

   freeaddrinfo(servinfo);

   if (listen(sockfd, BACKLOG) == -1) {
      perror("listen");
      exit(1);
   }

   while (accept_conn(sockfd) == 0)
      ;

   close(sockfd);

   return 0;
}
