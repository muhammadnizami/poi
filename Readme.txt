

       #      #       #    #                  #       ######   
      ####### #       #   ########       ##########            
     #     #         #   #    #              #    # ########## 
    # #   #         #   #    #     #####     #    #      #     
       ###        ##        #               #     #      #     
      ##        ##         #               #   # #      #      
    ##        ##          #               #     #     ##       



 ===============================================================
(								}
|	 _______  _______ _________     _______  _______	}
|	(  ____ )(  ___  )\__   __/    (  ____ \(  ____ \	}
|	| (    )|| (   ) |   ) (       | (    \/| (    \/	}
|	| (____)|| |   | |   | | _____ | (__    | (_____ 	}
|	|  _____)| |   | |   | |(_____)|  __)   (_____  )	}
|	| (      | |   | |   | |       | (            ) |	}
|	| )      | (___) |___) (___    | )      /\____) |	}
|	|/       (_______)\_______/    |/       \_______)	}
|								}
(								}
 ===============================================================

			      oleh
			jang berikoetnja
			    13512501
			    13513051
			    13513061

			       ||
			       ||
			       ||
			       ||
			       ||
			       ||


/////////////////////////////////////////////////////////////////
////////////////////////////README///////////////////////////////
/////////////////////////////////////////////////////////////////

////I. STRUKTUR DIREKTORI////////////////////////////////////////
 -----------------------------------
|dir	isi			    |
 -----------------------------------
|/bin	eksekutabel		    |
|/doc	dokumentasi dan laporan	    |
|/obj	berkas objek hasil kompilasi|
|/src	berkas sumber		    |
 -----------------------------------

////II. PEMBANGUNAN//////////////////////////////////////////////

Berikut ini adalah langkah-langkah untuk membangun poi-fs:
1. masuk ke direktori poi
2. pastikan ada direktori bin. bila tidak ada, buat direktori
bin dengan cara berikut:
	mkdir bin
3. lakukan perintah berikut:
	make

setelah langkah-langkah ini, poi-fs ada di bin/poifs



////III. PENGGUNAAN//////////////////////////////////////////////

Menunggangi berkas .poi yang sudah ada
	./poifs TITIK-TUNGGANG BERKASPOI
	contoh:
	./poifs a a.poi

Membuat dan menunggangi berkas .poi
	./poifs TITIK-TUNGGANG BERKASPOI -new
	contoh:
	./poifs a a.poi


Sebelum menunggangi, pastikan TITIK-TUNGGANG ada dan kosong.
untuk membuat TITIK-TUNGGANG:
	mkdir TITIK-TUNGGANG
untuk mengosongkan TITIK-TUNGGANG: (berbahaya)
	rm TITIK-TUNGGANG/* -r


Berhenti menunggangi
	fusermount -u TITIK-TUNGGANG
	contoh:
	fusermount -u a

////IV. PENGUJIAN////////////////////////////////////////////////
1. getattr
	stat NAMA-BERKAS/NAMA-DIREKTORI
2. readdir
	ls
3. mkdir
	mkdir	
4. mknod
	echo "" > NAMA-BERKAS
5. read
	cat NAMA-BERKAS
6. rmdir
	rmdir NAMA-DIREKTORI
7. unlink
	rm NAMA-BERKAS
8. rename
	mv JALUR JALUR-BARU
9. write
	echo ISI > NAMA-BERKAS
	echo ISI-TAMBAHAN >> NAMA-BERKAS
10. truncate
	truncate -s UKURAN-BARU NAMA-BERKAS
11. chmod
	chmod ug[+/-][wrx]
