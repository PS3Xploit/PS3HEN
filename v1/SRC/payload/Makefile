clean:
	rm -f *~ *.*~ *.o *.elf *.cex *.dex *.release *.debug *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o

all:
	rm -f *.o *.elf *.bin *.release *.debug *.cex *.dex *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o
	make -f Makefile_release --no-print-directory
	
	rm -f *.o *.elf *.bin *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o
	make -f Makefile_debug --no-print-directory
debug:
	rm -f *.o *.elf *.bin *.release *.debug *.cex *.dex *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o
	make -f Makefile_debug --no-print-directory
	
