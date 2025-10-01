#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <ctype.h>

#include "ewpdef.h"
#include "pgdbglog.h"

#define BUFFER_SIZE 1024   /* Størrelse på mottaksbuffer */
#define FIXED_SIZE 64      /* Fast lengde for enkelte felt */

/* Hjelpefunksjon for å sjekke gyldig filnavn */
int isValidFilename(const char *filename) {
   const char *ptr = filename;
   size_t len = strlen(filename);

   while (*ptr) {
      if(!isalnum(*ptr) && *ptr != '_' && *ptr != '.' && *ptr != '-') {
         return 0; /* Ugyldig tegn funnet */
      }
      ptr++;
   }

   /* Sjekk at filnavnet er langt nok og slutter på .eml */
   len = strlen(filename);
   if (len < 5) return 0;
   if (strcmp(filename + len - 4, ".eml") != 0) {
      return 0;
   }
   
   return 1;
}

int main(int argc, char *argv[]) {
   int port;
   char id[50];
   int server_fd, client_fd;
   struct sockaddr_in server_addr, client_addr;
   char buffer[BUFFER_SIZE];

   if (argc != 5 || strcmp(argv[1], "-port") != 0 || strcmp(argv[3], "-id") != 0) {
      fprintf(stderr, "Bruk: %s -port <portnummer> -id <id>\n", argv[0]);
      return EXIT_FAILURE;
   }

   port = atoi(argv[2]); /* Hent port fra kommandolinje */
   strncpy(id, argv[4], sizeof(id) - 1);  /* Kopier id-argument */
   id[sizeof(id) - 1] = '\0'; /* Nullterminering */

   socklen_t client_len = sizeof(client_addr);
   memset(&client_addr, 0, sizeof(client_addr));
   client_fd = accept(server_fd, (struct sockaddr *)&client_addr,&client_len);
   

   /* Opprett TCP socket */
   server_fd = socket(AF_INET, SOCK_STREAM, 0);
   if (server_fd < 0) {
      perror("socket feil");
      pgdebug("Socket feil");
      
      return EXIT_FAILURE;
   }

   /* Konfigurer serveraddresse */
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
   server_addr.sin_port = htons(port);

   /* Bind socket til adresse og port */
   if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
      perror("bind feil");
      pgdebug("bind feil");
      close(server_fd);
      return EXIT_FAILURE;
   }

   /* Start lytting */
   if (listen(server_fd, 1) < 0) {
      perror("listen feil");
      pgdebug("listen feil");
      close(server_fd);
      return EXIT_FAILURE;
   }

   printf("Venter på klient...\n");
   pgdebug("Venter på klient..");

   /* Aksepter tilkobling fra klient */
   client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
   if (client_fd < 0) {
      perror("accept feil");
      pgdebug("accept feil");
      close(server_fd);
      return EXIT_FAILURE;
   }
   printf("Klient koblet til!\n");
   pgdebug("Klient koblet til");

   /* Send server accept melding */
   struct EWA_EXAM25_TASK5_PROTOCOL_SERVERACCEPT accept_msg = {0};
   strcpy(accept_msg.stHead.acMagicNumber, "EWP");
   strcpy(accept_msg.stHead.acDataSize, "0064");
   strcpy(accept_msg.stHead.acDelimeter, "|");
   strcpy(accept_msg.acStatusCode, "220");
   accept_msg.acHardSpace[0] = ' ';
   snprintf(accept_msg.acFormattedString, sizeof(accept_msg.acFormattedString),
            "%s SMTP %s %s", inet_ntoa(server_addr.sin_addr), id, __TIME__);
   accept_msg.acHardZero[0] = '\0';

   send(client_fd, &accept_msg, sizeof(accept_msg), 0);

   FILE *file = NULL;
   int dataMode = 0;
   char filename[100] = "";

   /* Hovedløkken for å motta og svare på kommandoer */
   while (1) {
      memset(buffer, 0, BUFFER_SIZE);  /* Nullstill bufferet */
      int bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0); /* Mottak fra klient */
      buffer[bytes] ='\0';    /* Nullterminer mottat data */
      if (bytes <= 0) break;  /* Avslutt ved feil/forbindelse lukket */

      printf("Mottok: %s\n", buffer);
      pgdebug("Mottok: %s\n", buffer);

      if (dataMode) {
      /* i DATA-modus -> skriv til fil */
         int done = strstr(buffer, "\r\n.\r\n") != NULL;

         /* Skriv til fil */
         if (file) {
            int len = bytes;
            if (done) {
               char *end = strstr(buffer, "\r\n.\r\n");
               len = end - buffer;
            }
            fwrite(buffer, 1, len, file);
            pgdebug("Skriver til fil..");
         }

         if (done) {
            dataMode = 0;
            if (file) {
               fclose(file);
               file= NULL;
            }
            printf("Fil lagret ferdig.\n");
            pgdebug("Fil lagret ferdig");
            
            /* Bekreftelse på mottatt data */
            struct EWA_EXAM25_TASK5_PROTOCOL_SERVERREPLY doneReply = {0};
            strcpy(doneReply.stHead.acMagicNumber, "EWP");
            strcpy(doneReply.stHead.acDataSize, "0064");
            strcpy(doneReply.stHead.acDelimeter, "|");
            strcpy(doneReply.acStatusCode, "250");
            doneReply.acHardSpace[0] = ' ';
            snprintf(doneReply.acFormattedString,
                     sizeof(doneReply.acFormattedString),
                     "Data mottatt OK");
            doneReply.acHardZero[0] = '\0';
            send(client_fd, &doneReply, sizeof(doneReply), 0);
         }
         continue;
      }

      /* Sjekk hvilken kommando vi fikk og svar riktig */
      if (strstr(buffer, "HELO")) {
         struct EWA_EXAM25_TASK5_PROTOCOL_SERVERREPLY reply = {0};
         strcpy(reply.stHead.acMagicNumber, "EWP");
         strcpy(reply.stHead.acDataSize, "0064"),
         strcpy(reply.stHead.acDelimeter, "|");
         strcpy(reply.acStatusCode, "250");
         reply.acHardSpace[0] = ' ';
         snprintf(reply.acFormattedString, sizeof(reply.acFormattedString),
                  "Hei Bengt!", inet_ntoa(client_addr.sin_addr), id);
         reply.acHardZero[0] = '\0';

         send(client_fd, &reply, sizeof(reply), 0);
      }
      else if (strstr(buffer, "MAIL FROM")) {
         /* Svarmelding: SERVERREPLY */
         struct EWA_EXAM25_TASK5_PROTOCOL_SERVERREPLY reply = {0};
         strcpy(reply.stHead.acMagicNumber, "EWP");
         strcpy(reply.stHead.acDataSize, "0064");
         strcpy(reply.stHead.acDelimeter, "|");
         strcpy(reply.acStatusCode, "250");
         reply.acHardSpace[0] = ' ';
         snprintf(reply.acFormattedString, sizeof(reply.acFormattedString),
                  "Sender adresse OK");
         reply.acHardZero[0] = '\0';


         send(client_fd, &reply, sizeof(reply), 0);
      }
      else if (strstr(buffer, "RCPT TO")) {  
         /* Svarmelding: SERVERREPLY */
         struct EWA_EXAM25_TASK5_PROTOCOL_SERVERREPLY reply = {0};
         strcpy(reply.stHead.acMagicNumber, "EWP");
         strcpy(reply.stHead.acDataSize, "0064");
         strcpy(reply.stHead.acDelimeter, "|");
         strcpy(reply.acStatusCode, "250");
         reply.acHardSpace[0] = ' ';
         snprintf(reply.acFormattedString, sizeof(reply.acFormattedString),
                  "Mottaker adresse OK");
         reply.acHardZero[0] = '\0';


         send(client_fd, &reply, sizeof(reply), 0);
      }
      else if (strstr(buffer, "DATA")) {
         /* Svarmelding: SERVERREPLY */
         struct EWA_EXAM25_TASK5_PROTOCOL_SERVERREPLY reply = {0};
         strcpy(reply.stHead.acMagicNumber, "EWP");
         strcpy(reply.stHead.acDataSize, "0064");
         strcpy(reply.stHead.acDelimeter, "|");

         /* Hent ut filnanvn */
         const char *filenameStart = strstr(buffer, "DATA ") + 5; /* Første ord: "DATA" */
         while (*filenameStart == ' ') filenameStart++;  /* Hopp over mellomrom */
         strncpy(filename, filenameStart, sizeof(filename) - 1);
         filename[sizeof(filename) -1] = '\0';

         /* Validerer filnavn */         
         if (isValidFilename(filename)) {
            printf("Åpner fil: %s\n", filename);
            pgdebug("Åpner fil: %s\n", filename);
            file = fopen(filename, "wb");
            if (!file) {
               perror("fopen");
               pgdebug("fopen feilet");
               strcpy(reply.acStatusCode, "550");
               snprintf(reply.acFormattedString, sizeof(reply.acFormattedString),
                        "Feil ved åpning av fil");
            } else {
               strcpy(reply.acStatusCode, "354");
               snprintf(reply.acFormattedString, sizeof(reply.acFormattedString),
                        "Skriv data, avslutt med . på linje alene");
               dataMode = 1;  /* Sett server i data-modus */
            }
         } else {
            strcpy(reply.acStatusCode, "550");
            snprintf(reply.acFormattedString, sizeof(reply.acFormattedString),
                     "Ugyldig filnavn");
         }
         reply.acHardSpace[0] = ' ';
         reply.acHardZero[0] = '\0';
         send(client_fd, &reply, sizeof(reply), 0);
         continue;

     } else if (strstr(buffer, "QUIT")) {
         /* Svarmelding: SERVERREPLY */
         struct EWA_EXAM25_TASK5_PROTOCOL_SERVERREPLY reply = {0};
         strcpy(reply.stHead.acMagicNumber, "EWP");
         strcpy(reply.stHead.acDataSize, "0064");
         strcpy(reply.stHead.acDelimeter, "|");
         strcpy(reply.acStatusCode, "221");
         reply.acHardSpace[0] = ' ';
         reply.acHardZero[0] = '\0';

         send(client_fd, &reply, sizeof(reply), 0);
         break;
      }
   }

   /* Lukk forbindelser og avslutt */
   if (file) fclose(file);
   pgdebug("Fil lukket");
   close(client_fd);
   close(server_fd);
   printf("Server avsluttet.\n");
   pgdebug("Server avsluttet");

   return EXIT_SUCCESS;
}
      
      
