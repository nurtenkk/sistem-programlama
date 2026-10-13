#!/bin/bash
echo -n "10) tr_TR.UTF-8 kurulu mu?: "
if locale -a 2>/dev/null | grep -qi "tr_TR"; then #hata çıkarsa kullanıcı görmesin . Çıktıyıda grepe gönderir. Tr varmı diye bakar
    echo "evet"
else
    echo "HAYIR -> sudo locale-gen tr_TR.UTF-8 && sudo update-locale"
fi #if bloğunu bittiğini gösterir

echo -n "11) man 3 strftime var mi?: "
if man -w 3 strftime > /dev/null 2>&1; then
    echo "evet"
else
    echo "HAYIR -> sudo apt install manpages-dev manpages-posix-dev"
fi