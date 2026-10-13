/*
 * Ders    : Sistem Programlama
 * Hafta   : 01
 * Amac    : Gorev 3 - Tamponlama Deneyi
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Kismi yazmalara (partial write) karsi dongu kullanan yardimci fonksiyon
static void guvenli_yaz(const char *s) {
    size_t toplam = strlen(s);
    size_t yazilan_toplam = 0;

    while (yazilan_toplam < toplam) {
        ssize_t n = write(1, s + yazilan_toplam, toplam - yazilan_toplam);
        if (n == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        yazilan_toplam += (size_t)n;
    }
}

int main(void) {
    // A - write()
    guvenli_yaz("A - write()\n");

    // B - printf()
    printf("B - printf()\n");

    // C - write()
    guvenli_yaz("C - write()\n");

    // D - printf()
    printf("D - printf()\n");

    // fflush çagrisi (E satirasindan once tamponu bosaltmak icin)
    fflush(stdout);

    // E - write() (fflush sonrasi)
    guvenli_yaz("E - write() (fflush sonrasi)\n");

    return EXIT_SUCCESS;
}