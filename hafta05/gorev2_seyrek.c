
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

/* Seyrek dosya olusturan fonksiyon */
static int seyrek_dosya_uret(const char *dosya_adi, off_t delik_boyutu) {
    
    int fd = open(dosya_adi, O_WRONLY | O_CREAT | O_EXCL, 0644);
    if (fd == -1) {
        perror("open (dosya olusturulamadi)");
        return EXIT_FAILURE;
    }

    const char baslangic[] = "BASLANGIC\n";
    const char bitis[] = "BITIS\n";
    size_t bas_uzunluk = strlen(baslangic);
    size_t bitis_uzunluk = strlen(bitis);

    /* Dosyanin basina veri yaz */
    if (write(fd, baslangic, bas_uzunluk) != (ssize_t)bas_uzunluk) {
        perror("write baslangic");
        close(fd);
        return EXIT_FAILURE;
    }

    /*lseek ile delik boyutu kadar ileri zipla */
    if (lseek(fd, delik_boyutu, SEEK_END) == -1) {
        perror("lseek hatasi");
        close(fd);
        return EXIT_FAILURE;
    }

    /*  Zipladigimiz yeni yere yazinca aradaki delik resmiyet kazanir */
    if (write(fd, bitis, bitis_uzunluk) != (ssize_t)bitis_uzunluk) {
        perror("write bitis");
        close(fd);
        return EXIT_FAILURE;
    }

    if (close(fd) == -1) {
        perror("close hatasi");
        return EXIT_FAILURE;
    }

    printf("Seyrek dosya basariyla uretildi: %s (Delik: %ld bayt)\n", dosya_adi, (long)delik_boyutu);
    return EXIT_SUCCESS;
}

/* Dosyanın bilgilerini st içine doldurur*/
static void seyreklik_olc(const char *dosya_adi) {
    struct stat st;
    if (stat(dosya_adi, &st) == -1) {
        perror(dosya_adi);
        return;
    }

    /* Siradan bir dosya degilse atla */
    if (!S_ISREG(st.st_mode)) {
        printf("%-20s: Siradan bir dosya degil, atlaniyor.\n", dosya_adi);
        return;
    }

    long long mantiksal_boyut = (long long)st.st_size;
    /* st_blocks POSIX standardi geregi daima 512 baytlik blok sayisidir */
    long long gercek_disk_alani = (long long)st.st_blocks * 512;

   
    const char *karar = (gercek_disk_alani < mantiksal_boyut) ? "SEYREK" : "dolu";

    printf("%-20s: Mantiksal: %10lld B | Gercek Disk: %10lld B -> [%s]\n",
           dosya_adi, mantiksal_boyut, gercek_disk_alani, karar);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Kullanim:\n  %s <dosya1> [dosya2 ...]\n  %s -o <yeni_dosya> <delik_boyu>\n", argv[0], argv[0]);
        return EXIT_FAILURE;
    }

    /* -o secenegi ile dosya uretme modu */
    if (strcmp(argv[1], "-o") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Eksik parametre! Kullanim: %s -o <yeni_dosya> <delik_boyu>\n", argv[0]);
            return EXIT_FAILURE;
        }
        off_t delik = (off_t)atoll(argv[3]);
        return seyrek_dosya_uret(argv[2], delik);
    }

   
    for (int i = 1; i < argc; i++) {
        seyreklik_olc(argv[i]);
    }

    return EXIT_SUCCESS;
}