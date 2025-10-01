#ifndef TASK6_H
#define TASK6_H

/* Konstanter */
#define BUFFER_SIZE 2048   /* Bufferstørrelse for å motta kryptert data */
#define BLOCK_SIZE 8       /* TEA bruker blokker på 64 bits(8-byte) */
#define TEA_ROUNDS 64      /* Antall TEA_runder for tilstrekkelig sikkerhet */

/* Dekrypterer en 64-bits blokk med TEA-algoritmen */
void tea_decrypt(unsigned int* v, unsigned int* k);

/* Gjetter TEA-nøkkelen under antakelse om at alle bytes i nøkkelen er like */
int guess_key(unsigned char* buf, int len, unsigned int* found_key);

/* Dekrypterer en buffer med TEA-blokker og skriver resultatet til en fil */
void write_decrypted_blocks(unsigned char* buffer, int receiveD_bytes,
                           unsigned int* key, FILE* fp);

#endif /* TASK6_H */
