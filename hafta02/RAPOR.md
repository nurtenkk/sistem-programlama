# Sistem Programlama - Hafta 02 Ödev Raporu 

# 1. strtol() ile Dört Denetim ve Görev 2 Testleri

Görev 2'de -n SAYI seçeneğini okurken atoi() yerine strtol() tercih edilmiştir çünkü atoi("abc") ile atoi("0") ifadeleri hatayı geçerli veriden ayırt edemeden aynı şekilde 0 döndürür. strtol() kullanımı sonrasında güvenli bir sayı elde edebilmek için şu dört denetim eksiksiz uygulanmalıdır:
1. errno = 0;`** çağrı öncesi temizlenir.
2. son == s** kontrolü: Hiç rakam okunamadıysa  hata döndürülür.
3. *son != \0** kontrolü: Sayının sonunda geçersiz karakter kaldıysa hata döndürülür.
4. errno == ERANGE kontrolü: Sayı LONG_MAX veya LONG_MIN sınırlarını aşarak taştıysa hata döndürülür.

# Test Çıktıları ve Çıkış Kodları 

- -n 0 denemesi:
 ./gorev2_sayac -n 0 x
  -n icin gecersiz sayi: '0' (1..1000 olmali)
  $ echo $?
  1

 - -n abc denemesi
  ./gorev2_sayac -n abc x
  -n icin gecersiz sayi: 'abc' (1..1000 olmali)
  echo $?
  1

  - -n denemesi
  ./gorev2_sayac -n
  -n secenegi bir arguman bekliyor
kullanim: ./gorev2_sayac [-h] [-u] [-n SAYI] sozcuk [sozcuk ...]
  -n SAYI : her sozcugun kac kez yazilacagi (1..1000)
  -u      : buyuk harfe cevir
  -h      : bu iletiyi goster
$ echo $?
1


# KONTROL SORULAR

## S1

errno, önceki işlemlerden kalan hata kodunu tutabilir. Başarılı sistem çağrıları errno değerini otomatik olarak sıfırlamaz. Bu nedenle her döngü adımında errno = 0; yazılarak önceki hatadan kalan değerin karışması önlenir ve oluşan hata daha doğru kontrol edilir.

## S2:

close() fonksiyonunun dönüş değerini kontrol etmek, dosyanın kapatılması sırasında bir hata oluşup oluşmadığını anlamamızı sağlar. Özellikle dosyaya yazma işlemlerinde, verilerin diske aktarılması sırasında disk alanının dolması gibi hatalar ortaya çıkabilir. Ayrıca geçersiz bir dosya betimleyicisi kullanıldığında da close() başarısız olabilir.

# S3:

Linux sistemlerinde kabuk tarafından görülen çıkış kodu 0 ile 255 arasındadır.kullanılırsa kabukta çıkış kodu 0 olarak görülebilir.

# S4:
optstring başında : bulunmazsa, gerekli argümanı verilmeyen -n seçeneği için getopt() ? döndürür ve varsayılan hata mesajını yazdırabilir. Bu durumda programın case ':' bloğu çalışmaz. Başına : koyulduğunda ise eksik argüman durumu : ile belirtilir ve kendi hata mesajımızı yazabiliriz.

# S5

atoi() fonksiyonu, "0" ve "abc" gibi farklı girdiler için aynı sonucu döndürebilir. Bu nedenle kullanıcının gerçekten sıfır mı girdiğini yoksa geçersiz bir değer mi yazdığını anlamak zorlaşır. strtol() ise endptr işaretçisiyle dönüşümün nerede bittiğini kontrol etmemizi sağlar. Ayrıca errno kontrolüyle sayı sınırlarının aşılması gibi durumlar tespit edilebilir. Böylece geçersiz girişleri daha güvenilir şekilde kontrol edebiliriz.

# S6:

./gorev3_ortam_sorgu -p ZZZ && echo bulundu komutu çalıştırıldığında önce program çalışır. Programın dönüş değeri 0 ise echo bulundu komutu da çalışır. Ancak eşleşme bulunmadığında program 1 döndürüyorsa ikinci komut çalıştırılmaz. Çünkü && operatörü, ikinci komutun çalışması için ilk komutun başarılı olmasını gerektirir.

# S7:

 POSIX tarafından standartlaştırılmış değildir ve farklı sistemlerde taşınabilirlik sorunlarına neden olabilir.

