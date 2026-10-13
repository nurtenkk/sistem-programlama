# Sistem Programlama - Hafta 03 Raporu
Ogrenci Adi : Nurten Kaval
Ogrenci No  : 445879



##  Gorev 1
1. 
   tm_yda alanı 0'dan başlar (0, yılın ilk günüdür). eklemeseydik 1 Ocak için 0 yazardı.
2. 
   Evet, turkcelesir. Cunku setlocale(LC_TIME, "") cagrisi programin sistemin yerel ayarlarini okumasini saglar ve strftime ay/gun isimlerini bu ayarlara gore basar.
   
3. 
   %Z belirteci o anki saat dilimi kisaltmasini yazdirdi. Bu bilgi isletim sisteminin saat dilimi veritabanindan veya ortam degiskeninden gelir.

# Çıktı
aval@DESKTOP-4FC10L4:~/sp/hafta03$ gcc -std=c17 -Wall -Wextra -o gorev1_zamanpusulasi gorev1_zamanpusulasi.c
./gorev1_zamanpusulasi
[1] Epoch saniyesi :  1791901201
[2] Yil / Ay / Gun : 2026/10/13
    Yilin kacinci gunu: 286
    Yaz saati (isdst): 0
[3] Yerel saat      : 2026-10-13 17:20:01 +03
    UTC             : 2026-10-13 14:20:01 UTC
    Uzun bicim      : Tuesday, 13 October 2026
---

## 2. Gorev 2 
1.
   Subat ayi 31 cekmedigi icin strptime veya mktime bu tarihi gecersiz kabul eder ve hata dondurur. Ancak mktime bazi tasarlanmis durumlarda aylari normalize edebilir; yine de hatali girdi durumunda programimiz strptime kalintisindan dolayi hata mesaji uretir.
2. 
   struct tm yapisinin icindeki bazi alanlar cop degerlerle baslayabilir bu da mktime fonksiyonunun yanlis hesap yapmasina veya beklenmeyen sonuclar vermesine yol acar.
3.
   time_t veri tipinin ic yapisi standartlar tarafindan kesin olarak belirtilmemistir. Dogrudan cikarma bazi sistemlerde derleme veya tip uyusmazligi sorunlari yaratabilir; difftime() ise bunu platformdan bagimsiz guvenli sekilde yapar.

# Çıktı
gcc -std=c17 -Wall -Wextra -o gorev2_gunfarki gorev2_gunfarki.c
./gorev2_gunfarki 01.01.2026 15.08.2026
1. tarih : Thursday, 01 January 2026
2. tarih : Saturday, 15 August 2026
Fark     : 226 gun (19526400 saniye)
Yon      : 2. tarih daha SONRA

---

## 3. Gorev 3 
1. 
   Tokyo saat dilimi (JST), UTC'ye gore ileride oldugundan (UTC+9) ve diger bazi bati merkezli saat dilimlerine gore gun sinirini gectiginden tarih bir sonraki gune gecmistir.
2. 
   Her bolgenin yaz saati uygulamasi ve yerel kisaltma kurallari farklidir. Bu bilgi sistemdeki tzdata veritabanindan okunur.

3. 
   Kodumuz icinde dongunun basinda her adimda setenv("TZ", dilim, 1) cagirdigimiz icin disaridan verilen TZ degeri ilk adimda ezilir, bu yuzden program ici ciktilar degismez.
   # Çıktı:
   gcc -std=c17 -Wall -Wextra -o gorev3_dunyasaati gorev3_dunyasaati.c
./gorev3_dunyasaati
Ayni an (1791900162) farkli saat dilimlerinde:
Europe/Istanbul       Tue 13 Oct 2026 17:02:42 +03
UTC                   Tue 13 Oct 2026 14:02:42 UTC
Europe/London         Tue 13 Oct 2026 15:02:42 BST
America/New_York      Tue 13 Oct 2026 10:02:42 EDT
Asia/Tokyo            Tue 13 Oct 2026 23:02:42 JST
kaval@DESKTOP-4FC10L4:~/sp/hafta03$ ./gorev3_dunyasaati Europe/Paris Australia/Sydney
Ayni an (1791900178) farkli saat dilimlerinde:
Europe/Paris          Tue 13 Oct 2026 16:02:58 CEST
Australia/Sydney      Wed 14 Oct 2026 01:02:58 AEDT 