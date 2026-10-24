

#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utmpx.h> 
#include <time.h>   
#include <errno.h>

int main(int argc, char *argv[]) {
    int tumu_goster = 0;

    /* Komut satirindan -a secenegi gelmis mi kontrol  */
    if (argc > 1 && strcmp(argv[1], "-a") == 0) {
        tumu_goster = 1;
    }

    /* utmp dosyasinin basina saralim */
    setutxent();

    struct utmpx *k;
    int kayit_sayisi = 0;
    errno = 0;

    
    while ((k = getutxent()) != NULL) { //bir sonraki kayit gelene kadar doner
        /* Eger -a denmediyse sadece gercek kullanici oturumlarini al */
        if (!tumu_goster && k->ut_type != USER_PROCESS) {
            errno = 0;
            continue;
        }

        kayit_sayisi++;

        /*  Kullanici adi ve Terminal bilgisini guvenli kopyalama */
        char user_buf[sizeof(k->ut_user) + 1];
        char line_buf[sizeof(k->ut_line) + 1];
        memset(user_buf, 0, sizeof(user_buf));
        memset(line_buf, 0, sizeof(line_buf));
        memcpy(user_buf, k->ut_user, sizeof(k->ut_user));
        memcpy(line_buf, k->ut_line, sizeof(k->ut_line));

        /*  Mimariler arasi farktan kacınmak icin once time_t'ye ata */
        time_t giris_zamani = (time_t)k->ut_tv.tv_sec;
        struct tm *zaman_bilgisi = localtime(&giris_zamani);
        char zaman_str[64];
        if (zaman_bilgisi != NULL) {
            strftime(zaman_str, sizeof(zaman_str), "%Y-%m-%d %H:%M", zaman_bilgisi);
        } else {
            strncpy(zaman_str, "Bilinmiyor", sizeof(zaman_str));
        }

       
        char host_buf[sizeof(k->ut_host) + 1];
        memset(host_buf, 0, sizeof(host_buf));
        memcpy(host_buf, k->ut_host, sizeof(k->ut_host));

        
        printf("%-10s %-10s %s", user_buf, line_buf, zaman_str);
        if (host_buf[0] != '\0') {
            printf(" (%s)", host_buf);
        }
        printf("\n");

        errno = 0; 
    }

    
    if (errno != 0 && errno != ENOENT) {
        perror("getutxent hatasi");
        endutxent();
        return EXIT_FAILURE;
    }

    if (kayit_sayisi == 0) {
        printf("(su an acik oturum yok)\n");
    }

   
    endutxent();
    return EXIT_SUCCESS;
}