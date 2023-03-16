@echo off
cls
set PS3SDK=/c/PSDK3v2
set WIN_PS3SDK=C:/PSDK3v2
set PS3DEV=%PS3SDK%/ps3dev2
set PATH=%WIN_PS3SDK%/mingw/msys/1.0/bin;%WIN_PS3SDK%/mingw/bin;%PS3DEV%/ppu/bin;
set CYGWIN=C:\PSDK3v2\MinGW\msys\1.0\bin

mv *CEX_490_DEBUG PS3HEN_GEN
cd henplugin
%CYGWIN%\bash -i -c 'make debug; rm -r objs; rm henplugin.prx; rm henplugin.sym; mv henplugin.sprx ../PS3HEN_GEN;'
cd ../stage0_file
rm -f *.o *.elf *.self *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
make -f Makefile_490C
rm -f *.o *.elf *.self ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
cd ../payload
rm -f *.o *.elf *.self *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
make -f Makefile_490C_debug
rm -f *.o *.elf *.self ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
cd ..
mv  *490C PS3HEN_GEN
cd PS3HEN_GEN
gcc main.c -o HEN_GEN

HEN_GEN PS3HEN.BIN_CEX_490_DEBUG stage2.bin_debug_490C stage0.bin_490C HENplugin.sprx
rm stage* *.sprx *.exe
mv PS3HEN* ../
::cp ../PS3HEN.BIN_CEX_490_DEBUG ../HENPKG312/dev_rewrite/hen/PS3HEN.BIN
::cp ../PS3HEN.BIN_CEX_490_DEBUG ../host/4.90/PS3HEN.p3t
::cd.. 
::pkg_custom.exe -c EP0001-HENINSDAT_00-0000000000000000 HENPKG312
::mv EP0001-HENINSDAT_00-0000000000000000.pkg host/Latest_HEN_Installer_signed.pkg
pause