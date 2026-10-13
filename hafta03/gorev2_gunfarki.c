/*
 * Ders    : Sistem Programlama
 * Hafta   : 03
 * Amac    : Gorev 2 - Iki Tarih Arasindaki Gun Farki
 */

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    // 1. Arguman kontrolu (1 veya 2 tarih verilmeli)
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Kullanim: %s GG.AA.YYYY [GG.AA.YYYY]\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct tm tm1 = {0};
    struct tm tm2 = {0};

    // 1. Tarihi oku (strptime)
    char *kalinti = strptime(argv[1], "%d.%m.%Y", &tm1);
    if (kalinti == NULL || *kalinti != '\0') {
        fprintf(stderr, "\"%s\": GG.AA.YYYY bicimine uymuyor\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Guvenlik icin saat dilimi ve yaz saati ayarini sabitleyelim
    tm1.tm_hour = 12;
    tm1.tm_min = 0;
    tm1.tm_sec = 0;
    tm1.tm_isdst = -1;

    // 2. Tarih verilmis mi kontrol et, verilmemisse bugunu al
    if (argc == 3) {
        kalinti = strptime(argv[2], "%d.%m.%Y", &tm2);
        if (kalinti == NULL || *kalinti != '\0') {
            fprintf(stderr, "\"%s\": GG.AA.YYYY bicimine uymuyor\n", argv[2]);
            return EXIT_FAILURE;
        }
    } else {
        time_t simdi = time(NULL);
        if (simdi == (time_t)-1) {
            perror("time");
            return EXIT_FAILURE;
        }
        if (localtime_r(&simdi, &tm2) == NULL) {
            perror("localtime_r");
            return EXIT_FAILURE;
        }
    }

    tm2.tm_hour = 12;
    tm2.tm_min = 0;
    tm2.tm_sec = 0;
    tm2.tm_isdst = -1;

    // struct tm yapilarini time_t'ye cevir (mktime)
    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    if (t1 == (time_t)-1 || t2 == (time_t)-1) {
        perror("mktime");
        return EXIT_FAILURE;
    }

    // Tarihleri bicimli yazdirmak icin strftime kullanimi
    char buf1[100];
    char buf2[100];

    if (strftime(buf1, sizeof(buf1), "%A, %d %B %Y", &tm1) == 0 ||
        strftime(buf2, sizeof(buf2), "%A, %d %B %Y", &tm2) == 0) {
        fprintf(stderr, "strftime hatasi\n");
        return EXIT_FAILURE;
    }

    printf("1. tarih : %s\n", buf1);
    printf("2. tarih : %s\n", buf2);

    // difftime ile saniye farkini al ve gune cevir
    double saniye_farki = difftime(t2, t1);
    long gun_farki = (long)(saniye_farki / (60 * 60 * 24));

    if (gun_farki < 0) {
        gun_farki = -gun_farki;
    }

    printf("Fark     : %ld gun (%.0f saniye)\n", gun_farki, saniye_farki);

    // Yon belirleme
    if (t1 < t2) {
        printf("Yon      : 2. tarih daha SONRA\n");
    } else if (t1 > t2) {
        printf("Yon      : 1. tarih daha SONRA\n");
    } else {
        printf("Yon      : ayni gun\n");
    }

    return EXIT_SUCCESS;
}