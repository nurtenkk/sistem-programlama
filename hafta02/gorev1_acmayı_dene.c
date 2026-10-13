#define _POSIX_C_SOURCE 200809L //open bildirimi için

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int i; //döngüde sıra numarasını tutar
    int fd; //dosya betimleyicisi
    int basarisiz_sayisi = 0; 
    int toplam_dosya = 0;

    if (argc < 2) { //argc 2den küçükse yalnızca programın adı verilmiştir dosya yolu verilmemiştir
        fprintf(stderr, "kullanim: %s <dosya-yolu> [dosya-yolu ...]\n", argv[0]); //<dosya-yolu> en az bir dosya verilmesi gerektiğini, [dosya-yolu ...] ise başka dosyaların da isteğe bağlı olarak eklenebileceğini anlatır
        return 1;
    }

    toplam_dosya = argc - 1; //argc programın kendisini de saydığı için bir çıkarıyoruz

    for (i = 1; i < argc; i++) {
        errno = 0; // her döngü adımında errno sıfırlanır çünkü errno bazı sistem ve kütüphane fonksiyonları başarısız olduğunda hatanın nedenini belirten hata kodunu tutar
        fd = open(argv[i], O_RDONLY); //sadece okumak için açar

        if (fd == -1) { //open başarısız olursa -1 döndürür
            fprintf(stderr, "%s: acilamadi: %s (errno = %d)\n", argv[i], strerror(errno), errno);
            basarisiz_sayisi++;
        } else {
            printf("%s: acildi, betimleyici = %d\n", argv[i], fd);
            if (close(fd) == -1) {//close() başarısız olursa -1 döndürür
                perror("close");
            }
        }
    }

    printf("Toplam %d dosyadan %d tanesi acilamadi.\n", toplam_dosya, basarisiz_sayisi);

    // çıkış kodu 255ten büyük olamaz
    if (basarisiz_sayisi > 255) {
        return 255;
    }

    return basarisiz_sayisi; 
}