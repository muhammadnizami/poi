bin/poifs: src/poi.c src/file-manager.h obj/file-manager.o src/ADT\ Jam\ +\ Tanggal/waktu.h src/ADT\ Jam\ +\ Tanggal/jam.h src/ADT\ Jam\ +\ Tanggal/tanggal.h obj/ADT\ Jam\ +\ Tanggal/waktu.o obj/ADT\ Jam\ +\ Tanggal/jam.o obj/ADT\ Jam\ +\ Tanggal/tanggal.o obj/directory-entry.o src/volume-information.h obj/volume-information.o src/data-pool-block-manager.h obj/data-pool-block-manager.o src/allocation-block-manager.h obj/allocation-block-manager.o
	gcc src/poi.c obj/file-manager.o obj/ADT\ Jam\ +\ Tanggal/jam.o obj/ADT\ Jam\ +\ Tanggal/tanggal.o obj/ADT\ Jam\ +\ Tanggal/waktu.o obj/directory-entry.o obj/volume-information.o obj/data-pool-block-manager.o obj/allocation-block-manager.o -o bin/poifs -D_FILE_OFFSET_BITS=64 -lfuse -DFUSE_USE_VERSION=22

obj/file-manager.o: obj src/file-manager.h src/file-manager.c src/ADT\ Jam\ +\ Tanggal/waktu.h src/ADT\ Jam\ +\ Tanggal/jam.h src/ADT\ Jam\ +\ Tanggal/tanggal.h src/directory-entry.h
	gcc -c src/file-manager.c -o obj/file-manager.o

obj/ADT\ Jam\ +\ Tanggal/waktu.o: obj/ADT\ Jam\ +\ Tanggal src/ADT\ Jam\ +\ Tanggal/jam.h src/ADT\ Jam\ +\ Tanggal/tanggal.h src/ADT\ Jam\ +\ Tanggal/waktu.h src/ADT\ Jam\ +\ Tanggal/waktu.c
	gcc -c src/ADT\ Jam\ +\ Tanggal/waktu.c -o obj/ADT\ Jam\ +\ Tanggal/waktu.o

obj/ADT\ Jam\ +\ Tanggal/jam.o: obj/ADT\ Jam\ +\ Tanggal src/ADT\ Jam\ +\ Tanggal/jam.h src/ADT\ Jam\ +\ Tanggal/jam.c
	gcc -c src/ADT\ Jam\ +\ Tanggal/jam.c -o obj/ADT\ Jam\ +\ Tanggal/jam.o
obj/ADT\ Jam\ +\ Tanggal/tanggal.o: obj/ADT\ Jam\ +\ Tanggal src/ADT\ Jam\ +\ Tanggal/tanggal.h src/ADT\ Jam\ +\ Tanggal/tanggal.c
	gcc -c src/ADT\ Jam\ +\ Tanggal/tanggal.c -o obj/ADT\ Jam\ +\ Tanggal/tanggal.o

obj:
	[ -d obj ] || mkdir -p obj

obj/ADT\ Jam\ +\ Tanggal: 
	[ -d obj/ADT\ Jam\ +\ Tanggal ] || mkdir -p obj/ADT\ Jam\ +\ Tanggal
	

obj/directory-entry.o: src/directory-entry.h src/directory-entry.c 
	gcc -c src/directory-entry.c -o obj/directory-entry.o

obj/volume-information.o: src/volume-information.h src/volume-information.c
	gcc -c src/volume-information.c -o obj/volume-information.o

obj/data-pool-block-manager.o: src/data-pool-block-manager.h src/data-pool-block-manager.c
	gcc -c src/data-pool-block-manager.c -o obj/data-pool-block-manager.o

obj/allocation-block-manager.o: src/allocation-block-manager.h src/allocation-block-manager.c
	gcc -c src/allocation-block-manager.c -o obj/allocation-block-manager.o
