#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task2_hash.h"
#include "task2_count.h"
#include "task2_sum.h"
#include "pgdbglog.h"

/* Struktur for å lagre metadata */
/* Pragma for å sikre struct uten padding */
#pragma pack(push, 1)
struct TASK2_FILE_METADATA {
   char szFileName[32];
   int iFileSize;
   char byHash[4];
   int iSumOfChars;
   char aAlphaCount[26];
};
#pragma pack(pop)

int main() {
   const char* szInputFile = "pgexam25_test.txt";
   const char* szOutputFile = "pgexam25_output.bin";

   /* Åpner inputfilen for lesing */
   pgdebug("Starter programmet, åpner inputfil %s", szInputFile);
   FILE* fp = fopen(szInputFile, "r");
   if (!fp) {
      pgerror("Feil: Kunne ikke åpne inputfilen %s", szInputFile);
      return 1;
   }

   /* Initialiserer strukturen med nuller */
   struct TASK2_FILE_METADATA metadata;
   memset(&metadata, 0, sizeof(metadata));
   strncpy(metadata.szFileName, szInputFile, sizeof(metadata.szFileName) -1);

   /* Beregner DJB2-hash for filen */
   unsigned int hash = 0;
   if (Task2_SimpleDjb2Hash(fp, &hash) != 0) {
      pgerror("Feil under beregning av hash");
      fclose(fp);
      return 1;
   }
   /* Kopierer de første 4 bytene */
   memcpy(metadata.byHash, &hash, sizeof(metadata.byHash));
   pgdebug("Hash beregnet: %u", hash);

   /* Teller bokstaver */
   if (Task2_CountEachCharacter(fp, metadata.aAlphaCount) != 0) {
      pgerror("Feil under telling av bokstaver");
      fclose(fp);
      return 1;
   }
   pgdebug("Bokstavtelling fullført");

   /* Beregner filstørrelse og summerer alle tegn */
   if (Task2_SizeAndSumOfCharacters
         (fp, &metadata.iFileSize, &metadata.iSumOfChars)!= 0) {
            pgerror("Feil under beregning av størrelse og sum");
            fclose(fp);
            return 1;
   }
   pgdebug("Filstørrelse: %d, Sum av tegn: %d", metadata.iFileSize,
                                                metadata.iSumOfChars);

   /* Lukker input filen */
   fclose(fp);
   pgdebug("Inputfil lukket");

   /* Åpner output filen for skriving */
   FILE* outFp = fopen(szOutputFile, "wb");
   if (!outFp) {
      pgerror("Feil: Kunne ikke åpne outputfilen %s", szOutputFile);
   }

   /* Skriver data til outputfilen */
   if (fwrite(&metadata, sizeof(metadata), 1, outFp) != 1) {
      pgerror("Feil: Kunne ikke skrive metadata til outputfilen");
      fclose(outFp);
      return 1;
   }

   /* Lukker output filen */
   fclose(outFp);
   pgdebug("Outputfil lukket, metadata skrevet til %s", szOutputFile);

   /* Lukker programmet med suksessmelding */
   printf("Metadata skrevet til %s\n", szOutputFile);
   pgdebug("Program avsluttet normalt");
   return 0;
}
      
