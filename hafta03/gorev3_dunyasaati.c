
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <errno.h>


static const char *varsayilan_dilimler[] = {
    "Europe/Istanbul",
    "UTC",
    "Europe/London",
    "America/New_York",
    "Asia/Tokyo"
};

static void saat_goster(const char *tz_adi, time_t *an) {
    struct tm tm_bilgi;

    // saat dilimini ayarlar.değişken var olduğu icin 3. parametre 1 olmali)
    if (setenv("TZ", tz_adi, 1) != 0) { // 1: mevcutsada degistir, 0: mevcutsa degistirme
        perror("setenv");
        exit(EXIT_FAILURE);
    }

    // TZ degisikliginden sonra C kutuphanesinin saat dilimini guncellemesi icin tzset() sarttir
    tzset();

    if (localtime_r(an, &tm_bilgi) == NULL) {
        perror("localtime_r");
        exit(EXIT_FAILURE);
    }

    char buf[100];
    
    if (strftime(buf, sizeof(buf), "%a %d %b %Y %H:%M:%S %Z", &tm_bilgi) == 0) {
        fprintf(stderr, "strftime hatasi\n");
        exit(EXIT_FAILURE);
    }

    printf("%-20s  %s\n", tz_adi, buf);
}

int main(int argc, char *argv[]) {

    if (setlocale(LC_TIME, "") == NULL) {
        fprintf(stderr, "setlocale uyarisi: yerel ayar ayarlanamadi\n");
    }


    time_t simdiki_zaman = time(NULL);
    if (simdiki_zaman == (time_t)-1) {
        perror("time");
        return EXIT_FAILURE;
    }

    printf("Ayni an (%ld) farkli saat dilimlerinde:\n", (long)simdiki_zaman);

    
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            saat_goster(argv[i], &simdiki_zaman);
        }
    } else {
        
        int n = sizeof(varsayilan_dilimler) / sizeof(varsayilan_dilimler[0]);
        for (int i = 0; i < n; i++) {
            saat_goster(varsayilan_dilimler[i], &simdiki_zaman);
        }
    }

    return EXIT_SUCCESS;
}