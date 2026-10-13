#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

extern char **environ; //programın ortam değişkenlerini tutan diziyi gösterir

static void kullanim(const char *ad) {
    fprintf(stderr, "kullanim: %s [-h] [-c] [-p ONEK]\n", ad);
    fprintf(stderr, "  -c        : yalnizca eslesen sayiyi yazar\n");
    fprintf(stderr, "  -p ONEK   : yalnizca verilen onekle baslayanlari listeler\n");
    fprintf(stderr, "  -h        : bu iletiyi goster\n");
}

int main(int argc, char *argv[]) {
    int c;
    int sadece_sayi = 0;
    char *onek = NULL;
    int eslesme_sayisi = 0;
    char **p;

    opterr = 0; // Kendi hata iletilerimizi yazmak icin

    while ((c = getopt(argc, argv, ":chp:")) != -1) {// getopt ile komut satiri argumanlarini isliyoruz
        switch (c) {
            case 'h':
                kullanim(argv[0]);// Yardim secenegi verildiyse kullanim fonksiyonunu cagiriyoruz
                return 0;
            case 'c':// -c secenegi verildiyse sadece_sayi degiskenini 1 yapiyoruz
                sadece_sayi = 1;
                break;
            case 'p':
                onek = optarg; //optarg: getopt fonksiyonu tarafından işlenen argümanları tutan global bir değişkendir. -p seçeneği ile birlikte gelen argümanı onek değişkenine atıyoruz
                break;
            case ':':
                fprintf(stderr, "-%c secenegi bir arguman bekliyor\n", optopt);
                kullanim(argv[0]);
                return 1;
            case '?':
                fprintf(stderr, "bilinmeyen secenek: -%c\n", optopt);
                kullanim(argv[0]);
                return 1;
            default:
                return 1;
        }
    }

    // environ dizisini dolasarak ortam degiskenlerini inceleyelim
    for (p = environ; *p != NULL; p++) {
        int yazdir = 0;

        if (onek != NULL) {
            // Eger onek belirtilmisse, degisken o onekle basliyor mu kontrol et
            size_t onek_boyu = strlen(onek);
            if (strncmp(*p, onek, onek_boyu) == 0) {
                // Onekten sonra '=' gelmeli veya tam eslesmeli
                if ((*p)[onek_boyu] == '=' || (*p)[onek_boyu] == '\0') {
                    yazdir = 1;
                }
            }
        } else {
            yazdir = 1; // Onek yoksa hepsi gecerli
        }

        if (yazdir) {
            eslesme_sayisi++;
            if (!sadece_sayi) {
                printf("%s\n", *p);
            }
        }
    }

    // -c secenegi verilmis mi kontrol et
    if (sadece_sayi) {
        printf("%d\n", eslesme_sayisi);
    } else if (onek != NULL) {
        printf("%d degisken eslesti\n", eslesme_sayisi);
    }

    // En az bir eslesme bulursa 0, hic bulamazsa 1 dondur
    if (eslesme_sayisi > 0) {
        return 0;
    } else {
        return 1;
    }
}