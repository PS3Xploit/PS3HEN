@echo off
cls
set PS3SDK=/c/PSDK3v2
set WIN_PS3SDK=C:/PSDK3v2
set PS3DEV=%PS3SDK%/ps3dev2
set PATH=%WIN_PS3SDK%/mingw/msys/1.0/bin;%WIN_PS3SDK%/mingw/bin;%PS3DEV%/ppu/bin;
set CYGWIN=C:\PSDK3v2\MinGW\msys\1.0\bin

mv *CEX_482 *CEX_484 *CEX_485 *CEX_486 *CEX_487 *CEX_488 *CEX_489 *CEX_490 PS3HEN_GEN
cd henplugin
%CYGWIN%\bash -i -c 'make release; rm -r objs; rm henplugin.prx; rm henplugin.sym; mv henplugin.sprx ../PS3HEN_GEN;'
cd ../stage0_file
rm -f *.o *.elf *.self *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
make -f Makefile all
rm -f *.o *.elf *.self ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
cd ../payload
rm -f *.o *.elf *.self *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
make -f Makefile release
rm -f *.o *.elf *.self ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
cd ..
mv *482C *484C *485C *486C *487C *488C *489C *490C PS3HEN_GEN
cd PS3HEN_GEN
gcc main.c -o HEN_GEN
HEN_GEN PS3HEN.BIN_CEX_482 stage2.bin_482C stage0.bin_482C HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_484 stage2.bin_484C stage0.bin_484C HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_485 stage2.bin_485C stage0.bin_485C HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_486 stage2.bin_486C stage0.bin_486C HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_487 stage2.bin_487C stage0.bin_487C HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_488 stage2.bin_488C stage0.bin_488C HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_489 stage2.bin_489C stage0.bin_489C HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_490 stage2.bin_490C stage0.bin_490C HENplugin.sprx
rm stage* *.sprx *.exe
mv PS3HEN* ../
pause