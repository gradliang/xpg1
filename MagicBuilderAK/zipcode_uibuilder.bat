echo off
echo This will rar *.c, *.h, *.a *.mak *.bin *.elf and makefile newer than date
echo example : 20050805

Path=C:\Program Files\WinRAR\;%PATH%
set date=
set /p date=Type the date of oldest updated file. (yyyymmddhhmmss) : 
Rar  a mb_update_%date%.rar -r -ed -ta%date% *.cpp *.hpp *.dfm *.ddp *.dpc *.dsk *.bpr *.bpg *.c *.h *.mak *.elf *.bin *.a makefile
pause
echo on