/*
 * Ders    : Sistem Programlama
 * Hafta   : 01
 * Amac    : Gorev 2 - Kimlik Karti (Süreç ve Sistem Bilgileri)
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

// Tanimsiz ortam degiskenleri icin guvenli okuma yardimcisi
static void yazdir_env(const char *isim) {
    char *deger = getenv(isim);
    if (deger == NULL) {
        printf(" | %-20s : %-31s |\n", isim, "(tanimsiz)");
    } else {
        printf(" | %-20s : %-31s |\n", isim, deger);
    }
}

int main(void) {
    printf("====================================================\n");
    printf(" | SISTEM PROGRAMLAMA - KIMLIK KARTI               |\n");
    printf("====================================================\n");

    // 1. Kullanici Adi, Ev Dizini ve Kabuk (getpwuid ve getuid denetimi)
    uid_t uid = getuid();
    struct passwd *kadir = getpwuid(uid);
    if (kadir == NULL) {
        perror("getpwuid");
        return EXIT_FAILURE;
    }

    printf(" | %-20s : %-31s |\n", "Kullanici adi", kadir->pw_name);
    printf(" | %-20s : %-31d |\n", "Kullanici no (UID)", (int)uid);
    printf(" | %-20s : %-31s |\n", "Ev dizini", kadir->pw_dir);
    printf(" | %-20s : %-31s |\n", "Oturum kabugu", kadir->pw_shell);

    // 2. Surec Bilgileri (PID ve PPID)
    printf(" | %-20s : %-31d |\n", "Surec no (PID)", (int)getpid());
    printf(" | %-20s : %-31d |\n", "Ebeveyn surec (PPID)", (int)getppid());

    // 3. Calisma Dizini (getcwd denetimi)
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return EXIT_FAILURE;
    }
    printf(" | %-20s : %-31s |\n", "Calisma dizini", cwd);

    // 4. Cekirdek Bilgileri (uname)
    struct utsname bilgi;
    if (uname(&bilgi) == -1) {
        perror("uname");
        return EXIT_FAILURE;
    }
    printf(" | %-20s : %-31s |\n", "Makine adi", bilgi.nodename);
    printf(" | %-20s : %-31s |\n", "Cekirdek surumu", bilgi.release);
    printf(" | %-20s : %-31s |\n", "Mimari", bilgi.machine);

    printf("----------------------------------------------------\n");

    // 5. Ortam Degiskenleri (SHELL, LANG, TERM)
    yazdir_env("SHELL");
    yazdir_env("LANG");
    yazdir_env("TERM");

    printf("====================================================\n");

    return EXIT_SUCCESS;
}