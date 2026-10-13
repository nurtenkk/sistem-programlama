#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

static void kullanim(const char *ad) {
    fprintf(stderr, "kullanim: %s [-h] [-u] [-n SAYI] sozcuk [sozcuk ...]\n", ad);
    fprintf(stderr, "  -n SAYI : her sozcugun kac kez yazilacagi (1..1000)\n");
    fprintf(stderr, "  -u      : buyuk harfe cevir\n");
    fprintf(stderr, "  -h      : bu iletiyi goster\n");
}

// komut satırından gelen bir metni güvenli şekilde tam sayıya dönüştürür
static int dizeden_long(const char *s, long *sonuc) {
    char *son;
    long v;

    errno = 0;
    v = strtol(s, &son, 10); // metni sayıya çevirir

    if (son == s) return -1;       // Hic rakam yok
    if (*son != '\0') return -1;   // Sonda gecersiz karakter var /0 metnin sonunu gösterir
    if (errno == ERANGE) return -1;// sınırlar aşıldı mı
    if (errno != 0) return -1;

    *sonuc = v;
    return 0;
}

int main(int argc, char *argv[]) {
    int c;
    int buyuk_harf = 0;
    long sayi = 1;
    long parsed_sayi = 0;
    int i, j, k;

    opterr = 0; // Kendi hata iletilerimizi yazmak icin

    while ((c = getopt(argc, argv, ":hun:")) != -1) {
        switch (c) {
            case 'h':
                kullanim(argv[0]);
                return 0;
            case 'u':
                buyuk_harf = 1;
                break;
            case 'n':
                if (dizeden_long(optarg, &parsed_sayi) == -1) {
                    fprintf(stderr, "-n icin gecersiz sayi: '%s' (1..1000 olmali)\n", optarg);
                    return 1;
                }
                if (parsed_sayi < 1 || parsed_sayi > 1000) {
                    fprintf(stderr, "-n icin gecersiz sayi: '%s' (1..1000 olmali)\n", optarg);
                    return 1;
                }
                sayi = parsed_sayi;
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

    // Secenek disi arguman verilmis mi kontrol et
    if (optind >= argc) {
        fprintf(stderr, "Hata: Hic sozcuk verilmedi.\n");
        kullanim(argv[0]);
        return 1;
    }

    // Sozcukleri yazdirma dongusu
    for (i = optind; i < argc; i++) {//Her kelimeyi seçer
        for (j = 0; j < sayi; j++) {//Her kelimeyi sayi kadar tekrarlar
            if (buyuk_harf) {
                // Buyuk harfe cevirerek yazdir
                for (k = 0; argv[i][k] != '\0'; k++) {
                    putchar(toupper((unsigned char)argv[i][k]));
                }
            } else {
                printf("%s", argv[i]);
            }
            
            // Son ekleme kontrolu (bosluk veya yeni satir)
            if (j < sayi - 1 || i < argc - 1) {
                putchar(' ');
            }
        }
    }
    putchar('\n');

    return 0;
}