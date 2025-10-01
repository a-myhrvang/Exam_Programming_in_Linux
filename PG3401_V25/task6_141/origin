#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "task6.h"
#include "pgdbglog.h"

#undef TEA_ROUNDS
#define TEA_ROUNDS 64

/* TEA-dekryptering */
/* Tar inn peker til 64-bit blokk og en 128-bit nøkkel */
void tea_decrypt(unsigned int* v, unsigned int* k) {  /* Initierer v0 og v1 */
   unsigned int v0 = v[0], v1 = v[1];                 /* Startverdi for sum */
   unsigned int sum = 0xC6EF3720;                     /* Konstant brukt i TEA */
   unsigned int delta = 0x9E3779B9;
   int i;

   for ( int i = 0; i < TEA_ROUNDS; i++) {
      v1 -= ((v0 << 5) + k[3]) ^ (v0 + sum) ^ ((v0 >> 4) + k[2]);
      v0 -= ((v1 << 5) + k[1]) ^ (v1 + sum) ^ ((v1 >> 4) + k[0]);
      sum -= delta;
   }

   v[0] = v0;  /* Skriv tilbake dekryptert v0 */
   v[1] = v1;  /* Skriv tilbake dekryptert v1 */
}


/* Gjetter TEA-nøkkel basert på at alle 4 nøkkelord er like */
/* Forutsetter at plaintext inneholder kun ASCII-tegn */
int guess_key(unsigned char* buf, int len, unsigned int* found_key) {
   unsigned char c;
   unsigned int k[4];
   unsigned int v[2];
   unsigned char* p;
   int valid, kbyte, j, b;

   printf("Guessing key...\n");   /* Debug */
   pgdebug("Gjetter nøkkel...");

   for (c = 0; c < 255; c++) {
      printf("Trying key byte: %d\n", c); /* Debug */
      pgdebug("Prøver key byte: %d\n", c);

      /* Lag 128-bit nøkkel med like bytes */ 
      unsigned int val = ((unsigned int)c << 24) | ((unsigned int)c << 16) |
                         ((unsigned int)c << 8) | (unsigned int)c;
      k[0] = k[1] = k[2] = k[3] = val;


      /* Test hvert 8-byte blokk i bufferen */
      for (j = 0; j + BLOCK_SIZE <= len; j += BLOCK_SIZE) {
         memcpy(v, buf + j, BLOCK_SIZE);  /* Kopier 8 byte inn i v */
         tea_decrypt(v, k);               /* Dekrypter blokken */
         valid = 1;
      
         for (b = 0; b < 2; b++) {
            p = (unsigned char*)&v[b];
            for (kbyte = 0; kbyte < 4; kbyte++) {
               if (p[kbyte] < 32 || p[kbyte] > 126) {
                  valid = 0;  /* Ikke utskrivbart tegn */
                  break;
               }
            }
            if (!valid) break;   /* Gå ut av ytre løkke hvis ugyldig blokk */
      
         }
         if (valid) {
         memcpy(found_key, k, sizeof(unsigned int) * 4); /* Lagre funnet nøkkel */
         printf("Found key: %02X\n", c);
         pgdebug("Fant nøkkel: %02X\n");
         return 1;
         }
      }
   }

   printf("Key guess failed after all attempts.\n");  /* Debug */
   pgdebug("Feilet å gjette krypteringsnøkkel");
   return 0;
}


/* Skriver dekrypterte blokker til fil hvis de inneholder utskrivbare tegn*/
void write_decrypted_blocks(unsigned char* buffer, int received_bytes,
                            unsigned int* key, FILE* fp) {

   int blocks = 0;
   int i;
   
   for (i = 0; i + BLOCK_SIZE <= received_bytes; i += BLOCK_SIZE){
      /* Les blokk i korrekt endian-rekkefølge */
      unsigned int v[2];
      memcpy(v, buffer + i, BLOCK_SIZE);  /* Kopier blokk */
      tea_decrypt(v, key);                /* Dekrypter blokk */

      /* Skriv ut hver byte som tekst */      
      unsigned char* p = (unsigned char*)v;
      int valid = 1;
      int b;

      for (b = 0; b < BLOCK_SIZE; b++) {
         if (p[b] < 32 || p[b] > 126) {
            valid = 0;  /* Hvis ugyldig tegn, hopp over */
            break;
         }
      }
      if (valid) {
         fwrite(p, 1, BLOCK_SIZE, fp); /* Skriv blokk til fil */
         blocks++;
      }
   }
   printf("Decrypted %d blocks (%d bytes).\n", blocks, blocks * BLOCK_SIZE);
   printf("Decrypted file written to decrypted_output.txt\n");
   pgdebug("Dekryptert fil skrevet til decrypted_output.txt\n");
}
