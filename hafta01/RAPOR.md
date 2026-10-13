#hafta 1 raporu
Nurten KAVAL
445879

1. Tamponlama

gorev3_tampon programi dogrudan ekranda calistirildiginda cikti sirasi A, B, C, D, E korundugu halde, cikti bir dosyaya yonlendirildiginde (./gorev3_tampon > cikti.txt) sira (A, C, B, D, E seklinde) degismektedir. Bunun temel sebebi C kutuphanesinin uyguladigi tamponlama stratejileridir.

Ekrana yazdirma yapilirken satir tamponlama  devreye girer ve '\n' karakteri goruldugunde tampon bosaltilir. Ancak cikti dosyaya yonlendirildiginde tam tamponlama  aktif olur ve kutuphane verileri tampon dolana kadar veya program bitene kadar hafizada tutar. 

Kodumuzda yer alan write cagrilari sistem cagrisi oldugundan dolayi herhangi bir tampon mekanizmasina takilmaz veriyi aninda cekirdege iletir A ve C satirlari bu yuzden hemen yazilir. Buna karsilik printf() cagrilari (B ve D satirlari) C kutuphanesinin tamponunda bekletilir. strace -e trace=write izlemesi incelendiginde printf cagrilarinin arkada birleserek tek bir write() altinda toplandigi ve dosya sonuna veya fflush(stdout) cagrildigi anda (E satiri) disari aktarildigi net bir sekilde gorulur.

2. Kontrol soruları

Gorev 0:
1. #!/bin/bash bu betiği /bin/bash programı ile çalıştır der.Buna shebang denir
2. Çalıştırma izni verir

Gorev 1:
1. Her zaman değil ama printf() çıktıyı doğrudan ekrana göndermek yerine buffer içine alırsa bozulabilir.fflush(stdout), printf() ile buffera alınan çıktının hemen ekrana gönderilmesini sağlar(yani bufferı boşaltır).
2.#include <unistd.h>,man 2 write

Gorev 2:
1. getuid():Programı çalıştıran gerçek kullanıcının  UID sini verir. geteuid() : Programın sahip olduğu etkin kullanıcı  UID sini verir. Program set-user-ID (setuid) özelliğiyle çalıştırılıyorsa farklı olabilir.
2. Eğer programı getpid() gibi süreç bilgilerini de yazdırıyorsak her çalışmada yeni süreç oluşur farklı çıkar ama UID yazdırıyorsak aynı çıkar.