#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#include "task4_threads.h"
#include "djb2.h"
#include "tea.h"
#include "pgdbglog.h"

/* Tråd A: leser data fra fil inn i delt buffer */
/* Tråd A er ikke i bruk i PART 2, kode fjernet */
void* thread_A(void* arg) {
   pthread_exit(NULL);
}


/* Tråd B: behandler data fra bufferet og teller byte-frekvens */
void* thread_B(void* arg) {
   SharedData* pData = (SharedData*)arg;  /* Hent ut peker til delt data */

   /* Åpne fil for hashing */
   FILE* fp = fopen(pData->filename, "rb");  /* Åpner inputfulen i binary-read */
   pgdebug("Åpner fil for hashing");
   if (!fp) {  /* Sjekk om fopen feilet */
      perror("Kunne ikke åpne fil for hash");
      pgdebug("Kunne ikke åpne fil for hash");
      pthread_exit(NULL);  /* Avslutt tråden dersom fil ikke kan åpnes */
   }

   /* Kalkuler DJB2-hash */
   unsigned int hash = 0;  /* Variabel for å lagre hash-verdien */
   Task2_SimpleDjb2Hash(fp, &hash);
   
   /* Skriv hash til .hash-fil */
   FILE* hashFile = fopen("task4_pg2265.hash", "w");  /* Åpne hash-utfil */
   pgdebug("Åpner task4_pg2265.hash");
   if (!hashFile) {
      perror("Kunne ikke åpne hash-fil");
      pgdebug("Kunne ikke åpne hash-fil");
      fclose(fp); /* Lukk inputfilen */
      pgdebug("Lukker inputfilen");
      pthread_exit(NULL);
   }

   fprintf(hashFile, "%u\n", hash); /* Skriv hash-verdien til filen */
   pgdebug("Skriver hash-verdier..");
   fclose(hashFile); /* Lukk hash-filen */
   pgdebug("Lukker hash-filen");
   fclose(fp);

   /* Åpne fil på nytt for kryptering */
   fp = fopen(pData->filename, "rb");  /* Åpner inputfilen igjen i binary-read */
   pgdebug("Åpner inpitfil i <rb>");
   if (!fp) {
      perror("Kunne ikke åpne fil for kryptering");
      pgdebug("Kunne ikke åpne fil for kryptering");
      pthread_exit(NULL);
   }

   FILE* encFile = fopen("task4_pg2265.enc", "wb");
   pgdebug("Åpner fil task4_pg2265.enc");
   if (!encFile) {
      perror("Kunne ikke åpne enc-fil");
      pgdebug("Kunne ikke åpne enc-fil");
      fclose(fp);
      pgdebug("Lukker enc-fil..");
      pthread_exit(NULL);
   }

   /* Krypteringsnøkkel (4 x 32-bit heltall, hardkodet) */
   unsigned int key[4] = {0x12345678, 0x9abcdef0, 0x0fedcba9, 0x87654321};
   unsigned char block[8]; /* 8-byte blokk (64-bit) */
   size_t bytesRead; /* Antall bytes lest per runde */

   while ((bytesRead = fread(block, 1, 8, fp)) > 0) { // Les blokk for blokk
      /* PKCS5-padding hvis siste blokk */
      int pad = 8 - bytesRead;   /* Beregn hvor mange padding-bytes som trengs */
      for (size_t i = bytesRead; i < 8; i++) {
         block[i] = pad;   /* Fyll på med padding verdier */
      }

      unsigned int v[2], w[2]; /* To 32-bit heltall inn, to ut */
      memcpy(v, block, 8); /* Kopier 8 bytes inn i to 32-bit verdier */
   
      /* Krypter blokken */
      encipher(v, w, key); // Kall TEA-kryperingsfunksjonen
      pgdebug("Krypterer innhold..");

      /* Skriv kryptert blokk til utfil */
      fwrite(w, sizeof(unsigned int), 2, encFile); // Skriv to 32-bit heltall(8-bit)
      pgdebug("Skriver kryptert innhold til fil");
   }

   fclose(fp); /* Lukk inputfilen */
   pgdebug("Lukker inputfil");
   fclose(encFile);   /* Lukk utfilen */
   pgdebug("Lukker outputfil");

   pthread_exit(NULL);  /* Avslutt tråden */
}
                                                      

int main(int argc, char* argv[]) {
   if (argc != 2) {
      fprintf(stderr, "Bruk: %s <filnavn>\n", argv[0]);
      return EXIT_FAILURE; /* Sjekk at filnavn er gitt som argument */
   }

   SharedData data;
   data.filename = argv[1];

   pthread_t threadB;
   pgdebug("Starter threadB");
   /* Start kun thread B, som nå gjør hash + kryptering */
   if (pthread_create(&threadB, NULL, thread_B, &data) != 0) {
      perror("Klarte ikke å starte thread B");
      pgdebug("Kunne ikke åpne threadB");
      return EXIT_FAILURE;
   }

   /* Vent til thread B blir ferdig */
   pthread_join(threadB, NULL);

   return EXIT_SUCCESS;
}
