/*
 * Ders    : Sistem Programlama
 * Hafta   : 03
 * Amac    : Gorev 1 - Zaman Pusulasi
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

int main(void) {
    // şuanki zamanı alıyor
    time_t simdiki_zaman = time(NULL);
    if (simdiki_zaman == (time_t)-1) {//time başarılı mı değil mi
        perror("time");
        return EXIT_FAILURE;
    }
    printf("[1] Epoch saniyesi : %11d\n", (int)simdiki_zaman);//saniye cinsinden yazdırır

    //okunabiir paraçalara ayırıyoruz
    struct tm yerel_tm; //türkiyedeki
    struct tm utc_tm; //utc 

    if (localtime_r(&simdiki_zaman, &yerel_tm) == NULL) {//localtime_r =Epoch zamanını bilgisayarın yerel saatine dönüştürmek
        perror("localtime_r");
        return EXIT_FAILURE;
    }
    //Epoch zamanını UTC'ye çevirir.
    if (gmtime_r(&simdiki_zaman, &utc_tm) == NULL) {
        perror("gmtime_r");
        return EXIT_FAILURE;
    }

    // tm_year 1900'den baslar, tm_mon 0'dan baslar (ocak = 0)
    int gercek_yil = yerel_tm.tm_year + 1900;
    int gercek_ay = yerel_tm.tm_mon + 1;
    int gun = yerel_tm.tm_mday;
    int yil_gunu = yerel_tm.tm_yday + 1; // 1 Ocak icin 1 olmasi adina +1

    printf("[2] Yil / Ay / Gun : %d/%d/%d\n", gercek_yil, gercek_ay, gun);
    printf("    Yilin kacinci gunu: %d\n", yil_gunu);
    printf("    Yaz saati (isdst): %d\n", yerel_tm.tm_isdst);

    // 3. strftime ile bicimlendirme (zamanı metin ile saklar)
    char buf1[100];
    char buf2[100];
    char buf3[100];

    //strftime(hedef,hedefin boyutu, format, zaman)
    if (strftime(buf1, sizeof(buf1), "%Y-%m-%d %H:%M:%S %Z", &yerel_tm) == 0) {
        fprintf(stderr, "strftime hatasi (yerel)\n");
        return EXIT_FAILURE;
    }

    if (strftime(buf2, sizeof(buf2), "%Y-%m-%d %H:%M:%S UTC", &utc_tm) == 0) {
        fprintf(stderr, "strftime hatasi (utc)\n");
        return EXIT_FAILURE;
    }

    if (strftime(buf3, sizeof(buf3), "%A, %d %B %Y", &yerel_tm) == 0) {
        fprintf(stderr, "strftime hatasi (uzun bicim)\n");
        return EXIT_FAILURE;
    }

    printf("[3] Yerel saat      : %s\n", buf1);
    printf("    UTC             : %s\n", buf2);
    printf("    Uzun bicim      : %s\n", buf3);

    return EXIT_SUCCESS;
}