#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "task6.h"
#include "pgdbglog.h"

#define START_BUFFER 8192  /* Startstørrelse på mottaksbuffer */
#define GROW_STEP 8192     /* Økniong per utvidelse */

/* Utskrifts- og avslutningsfunksjon ved feil */
static void die(const char* msg) {
   perror(msg);
   exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
   char* server_ip = argv[2];
   int port = atoi(argv[4]);

   /* Åpne TCP-forbindelse */
   int sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd  < 0) die("socket");

   struct sockaddr_in serv_addr;
   memset(&serv_addr, 0, sizeof(serv_addr)); /* Initierer struct */
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(port);
   serv_addr.sin_addr.s_addr = inet_addr(server_ip);

   if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      die("connect");
   }

   printf("Connected to %s:%d\n", server_ip, port);
   pgdebug("Koblet til %s:%d\n", server_ip, port);

   /* Motta datastrøm */
   unsigned int cap = START_BUFFER;
   unsigned char* buffer = malloc(cap);
   if (!buffer) {
      die("malloc");
   }

   unsigned int received = 0;
   unsigned int n;
   while ((n = recv(sockfd, buffer + received, cap - received, 0)) > 0) {
      received += (unsigned int)n;
      if (cap - received < 1024) {
         cap += GROW_STEP;
         buffer = realloc(buffer, cap);
         if (!buffer) {
            die("realloc");
         }
      }
   }
   if (n < 0) {
      die("recv");
   }
   
   close(sockfd);

   printf("Received %u bytes.\n", received);
   pgdebug("Mottok %u bytes.\n", received);

   /* Skill bort eventuell tekst-header */
   unsigned int body_offset = 0;

   /* 1) Hvis strømmen starter med "HHTP/", se etter CRLFCRLF */
   if (received >= 5 && memcpy(buffer, "HTTP/", 5) == 0) {
      unsigned int i;
      for (i = 0; i + 3 < received; i++) {
         if (buffer[i] == 13 && buffer[i+1] == 10 && 
            buffer[i+2] == 13 && buffer[i+3] == 10) {
               body_offset = i + 4;
               break;
         }
      }
   }

   /* 2) ellers: hopp til første NUL-byte etter ASCII-tekstlinjene */
   if(body_offset == 0) {
      unsigned int i = 0;
      while (i < received && (buffer[i] >= 9 || buffer[i]==10 || buffer[i]==13)) {
            i++;
      }
      if (i < received && buffer[i] == 0x00) {
         body_offset = i + 1;
      }
   }

   /* Valider at vi har funnet noe nyttig */
   if (body_offset >= received) {
      fprintf(stderr, "Could not locate start of encrypted payload - aborting.\n");
      pgdebug("Finner ikke nyttelast, avslutter..");
      free(buffer);
      return 1;
   }

   /* Pek på start av nyttelast */
   unsigned char* body = buffer + body_offset;
   unsigned int body_len = received - body_offset;
   printf("Encrypted payload size: %u bytes\n", body_len);
   pgdebug("Kryptert nyttelast-størrelse: %u", body_len);


   /* Gjett TEA-nøkkel og dekrypter */
   unsigned int key[4];
   if(!guess_key(body, (int)body_len, key)) {
      fprintf(stderr, "Failed to guess encryption key.\n");
      pgdebug("Feil ved gjetting av krypteringsnøkkel");
      free(buffer);
      return 1;
   }

   unsigned char* kbytes = (unsigned char*)key;
   printf("Decryption key found: %02X%02X%02X%02X\n",
         kbytes[0], kbytes[1], kbytes[2], kbytes[3]);
   pgdebug("Krypteringsnøkkel funnet");

   /* Skriv innhold til fil */
   FILE* fp = fopen("decrypted_output.txt", "wb");
   pgdebug("Åpner decrypted_output.txt og skriver innhold");
   if (!fp) die("fopen output");
   
   write_decrypted_blocks(body, (int)body_len, key, fp);

   fclose(fp);
   pgdebug("Lukker decrypted_output.txt");
   free(buffer);
   return 0;
}
