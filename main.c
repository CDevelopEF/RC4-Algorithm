#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#define KEYLEN 32
#define MOD 256


void swap(uint8_t *a, uint8_t *b)
{
    uint8_t tmp = *a;
    *a = *b;
    *b = tmp;
}

void key_scheduling_rc4(uint8_t *S, uint8_t* key)
{

    for(uint32_t i = 0; i < MOD; i++)
    {
        S[i] = i;
    }

    uint32_t j = 0;

    for(uint32_t i = 0 ; i < MOD ; i++)
        {

        j = (j + S[i] + key[i % KEYLEN]) % MOD;

        swap(&S[i], &S[j]);
        }
}


void PRGA(uint8_t *S, uint8_t *plaintext)
{

    int j = 0;
    int i = 0;

    i = (i + 1)    % MOD;
    j = (j + S[i]) % MOD;

    swap(&S[i], &S[j]);

    int p = ((S[i] + S[j]) % MOD);

    *plaintext = *plaintext ^ S[p];


}
void RC4_cipher(FILE *source, FILE *dest, uint8_t *key)
{
    uint8_t S[256];
    key_scheduling_rc4(S, key);

    while(!feof(source))
    {
        uint8_t buffer;

        fread(&buffer, sizeof(buffer), 1, source);
        if(feof(source)) break;
        PRGA(S, &buffer);
        fwrite(&buffer, sizeof(buffer), 1, dest);

    }
}

int main(int args, char *argv[])
{

    uint8_t key[32]= {
                      0x01, 0x02, 0x03, 0x04, 0x05,
                      0x06, 0x07, 0x08, 0x09, 0x0a,
                      0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                      0x10, 0x11, 0x12, 0x13, 0x14,
                      0x15, 0x16, 0x17, 0x18, 0x19,
                      0x1a, 0x1b, 0x1c, 0x1d, 0x1e,
                      0x1f, 0x20
                      };


    FILE* inputFile = fopen(argv[1], "rb+");
    FILE* destFile  = fopen("test_dest.bin", "wb");




    RC4_cipher(inputFile, destFile, key);

    fclose(inputFile);
    fclose(destFile);
    return 0;
}
