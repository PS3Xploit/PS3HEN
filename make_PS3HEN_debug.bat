@echo off
cls
set PS3SDK=/c/PSDK3v2
set WIN_PS3SDK=C:/PSDK3v2
set PS3DEV=%PS3SDK%/ps3dev2
set PATH=%WIN_PS3SDK%/mingw/msys/1.0/bin;%WIN_PS3SDK%/mingw/bin;%PS3DEV%/ppu/bin;
set CYGWIN=C:\PSDK3v2\MinGW\msys\1.0\bin

mv *DEX_482_DEBUG *DEX_484_DEBUG *CEX_480_DEBUG *CEX_481_DEBUG *CEX_482_DEBUG *CEX_483_DEBUG *CEX_484_DEBUG *CEX_485_DEBUG *CEX_486_DEBUG *CEX_487_DEBUG *CEX_488_DEBUG *CEX_489_DEBUG *CEX_490_DEBUG PS3HEN_GEN
cd henplugin
%CYGWIN%\bash -i -c 'make debug; rm -r objs; rm henplugin.prx; rm henplugin.sym; mv henplugin.sprx ../PS3HEN_GEN;'
cd ../stage0_file
rm -f *.o *.elf *.self *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
make -f Makefile all
rm -f *.o *.elf *.self ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
cd ../payload
rm -f *.o *.elf *.self *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
make -f Makefile debug
rm -f *.o *.elf *.self ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
cd ..
mv *482D *484D *480C *481C *482C *483C *484C *485C *486C *487C *488C *489C *490C PS3HEN_GEN
cd PS3HEN_GEN
gcc main.c -o HEN_GEN
HEN_GEN PS3HEN.BIN_CEX_480_DEBUG stage2.bin_debug_480C stage0.bin_480C HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_481_DEBUG stage2.bin_debug_481C stage0.bin_481C HENplugin.sprx
HEN_GEN PS3HEN.BIN_DEX_482_DEBUG stage2.bin_debug_482D stage0.bin_482D HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_482_DEBUG stage2.bin_debug_482C stage0.bin_482C HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_483_DEBUG stage2.bin_debug_483C stage0.bin_483C HENplugin.sprx
HEN_GEN PS3HEN.BIN_DEX_484_DEBUG stage2.bin_debug_484D stage0.bin_484D HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_484_DEBUG stage2.bin_debug_484C stage0.bin_484C HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_485_DEBUG stage2.bin_debug_485C stage0.bin_485C HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_486_DEBUG stage2.bin_debug_486C stage0.bin_486C HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_487_DEBUG stage2.bin_debug_487C stage0.bin_487C HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_488_DEBUG stage2.bin_debug_488C stage0.bin_488C HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_489_DEBUG stage2.bin_debug_489C stage0.bin_489C HENplugin.sprx
HEN_GEN PS3HEN.BIN_CEX_490_DEBUG stage2.bin_debug_490C stage0.bin_490C HENplugin.sprx
rm stage* *.sprx *.exe
mv PS3HEN* ../
pause