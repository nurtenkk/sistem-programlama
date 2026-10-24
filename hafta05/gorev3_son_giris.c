

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>        
#include <lastlog.h>    
#include <paths.h>      
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Kullanim: %s <kullanici_adi1> [kullanici_adi2 ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Sistem makrosuyla lastlog dosyasini sadece okuma modunda ac */
    int fd = open(_PATH_LASTLOG, O_RDONLY);
    if (fd == -1) {
        perror("lastlog dosyasi acilamadi");
        return EXIT_FAILURE;
    }

    /* Dosyanin toplam bayt boyutunu olcelim  */
    off_t dosya_boyu = lseek(fd, 0, SEEK_END);
    if (dosya_boyu == -1) {
        perror("lseek dosya sonu hatasi");
        close(fd);
        return EXIT_FAILURE;
    }

    size_t kayit_boyu = sizeof(struct lastlog);

    /* Komut satirindan girilen her kullaniciyi tek tek sorgula */
    for (int i = 1; i < argc; i++) {
        const char *kullanici_adi = argv[i];
        struct passwd *pw = getpwnam(kullanici_adi);

        if (pw == NULL) {
            printf("%-15s: Boyle bir sistem kullanicisi bulunamadi.\n", kullanici_adi);
            continue;
        }

        /* Kullanicinin kaydinin dosyadaki adresi: UID * struct_boyutu */
        off_t adres = (off_t)pw->pw_uid * (off_t)kayit_boyu;

        /* Adres dosya sinirinin otesindeyse henuz bu UID icin kayit acilmamistir */
        if (adres + (off_t)kayit_boyu > dosya_boyu) {
            printf("%-15s (UID: %5d): **Hic giris yapmamis**\n", kullanici_adi, pw->pw_uid);
            continue;
        }

        /* Dogrudan o adrese konumlaniyoruz  */
        if (lseek(fd, adres, SEEK_SET) == -1) {
            perror("lseek adres hatasi");
            continue;
        }

        struct lastlog kayit;
        ssize_t okunan = read(fd, &kayit, kayit_boyu);
        if (okunan == -1) {
            perror("read hatasi");
            continue;
        }

        if ((size_t)okunan != kayit_boyu) {
            printf("%-15s (UID: %5d): Eksik kayit okundu.\n", kullanici_adi, pw->pw_uid);
            continue;
        }

        /* ll_time degeri 0 ise kullanici tanimlidir ama hic oturum acmamistir */
        if (kayit.ll_time == 0) {
            printf("%-15s (UID: %5d): **Hic giris yapmamis**\n", kullanici_adi, pw->pw_uid);
            continue;
        }

        /* Mimariden etkilenmemek icin ll_time'i once time_t'ye aktariyoruz */
        time_t giris_zamani = (time_t)kayit.ll_time;
        struct tm *zaman_bilgisi = localtime(&giris_zamani);
        char zaman_str[64];
        if (zaman_bilgisi != NULL) {
            strftime(zaman_str, sizeof(zaman_str), "%c", zaman_bilgisi);
        } else {
            strncpy(zaman_str, "Bilinmeyen Zaman", sizeof(zaman_str));
        }

        
        char line_buf[sizeof(kayit.ll_line) + 1];
        char host_buf[sizeof(kayit.ll_host) + 1];
        memset(line_buf, 0, sizeof(line_buf));
        memset(host_buf, 0, sizeof(host_buf));
        memcpy(line_buf, kayit.ll_line, sizeof(kayit.ll_line));
        memcpy(host_buf, kayit.ll_host, sizeof(kayit.ll_host));

        printf("%-15s (UID: %5d): %-10s %-20s %s\n",
               kullanici_adi, pw->pw_uid, line_buf, host_buf, zaman_str);
    }

    if (close(fd) == -1) {
        perror("close hatasi");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}