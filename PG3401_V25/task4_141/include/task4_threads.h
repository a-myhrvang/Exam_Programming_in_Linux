#ifndef TASK4_THREADS_H
#define TASK4_THREADS_H

#include <semaphore.h>

#define BUFFER_SIZE 4096   /* Størrelse på buffer */
#define BYTE_RANGE 256     /* Antall mulige byteverdier */

/* Struct for delt data mellom trådene */
/* Bruker pragma pack for å sikre at structen pakkes tett uten padding */
#pragma pack(push, 1)
typedef struct {
/* //unsigned char* buffer;  // Delt buffer  
   //int count[BYTE_RANGE];  // Tellearray for byte-verdier
   //int bytes_in_buffer;    // Antall bytes i bufferet nå
   //sem_t sem_full;         // Semafor: signal om buffer fylt
   //pthread_mutex_t mutex;  // Mutex for beskyttelse av data mellom tråder
   //sem_t sem_empty;        // Semafor: signal om buffer klart */
   const char* filename;     /* Navn på inputfil */
} SharedData;
#pragma pack(pop)

/* Deklarasjon av tråd-funksjoner */
void* thread_A(void* arg);
void* thread_B(void* arg);

#endif /* TASK4_THREADS_H */
