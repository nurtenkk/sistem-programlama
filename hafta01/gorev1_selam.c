#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(void){

    printf("Kutuphane fonksiyonu ile selamlar \n");

    const char *ileti = "Sistem cagrisi ile selamlar .\n";
    size_t uzunluk = strlen(ileti);
    ssize_t yazilan = write(1, ileti, uzunluk);

    if (yazilan == -1) {
        perror("write");
        return EXIT_FAILURE;


    }
    if ((size_t)yazilan < uzunluk) {
        fprintf(stderr, "Uyari: Tum baytlar yazilamadi!\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}