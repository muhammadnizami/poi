/**
@file file-manager.h
@author jang berikoetnja
@brief manajer berkas dan blok

Modul ini mengatur seluruh masukan-luaran (i/o) langsung kepada berkas .poi yang ditunggangi. Seluruh modul lain HARUS menggunakan modul ini untuk masukan-luaran ke berkas .poi yang ditunggangi

*/

#ifndef file_manager_h
#define file_manager_h

#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#define POI_BLOCK_SIZE 512
#define POI_BLOCK_BYTE_NUM POI_BLOCK_SIZE
#define POI_BLOCK_WORD_NUM (POI_BLOCK_SIZE/2)
#define POI_BLOCK_DWORD_NUM (POI_BLOCK_SIZE/4)

/* ------------------------------------------------ */
/* --------STRUKTUR DATA BLOK PEMBACAAN BERKAS----- */
typedef struct{
	uint8_t data[POI_BLOCK_SIZE];
}poi_file_block;

//spesifikasi getter dan setter jelas
//offset dihitung berdasarkan ukuran data yang diambil.
//misal isi data	: a b c d e f g h ....
//maka	offset byte	: 0 1 2 3 4 5 6 7 ....
//	offset word	: 0   1   2   3   ....
//	offset dword	: 0       1       ....

uint8_t get_poi_file_block_byte(const poi_file_block *, uint32_t offset);
uint16_t get_poi_file_block_word_terurut(const poi_file_block *, uint32_t offset);
uint16_t get_poi_file_block_word_little_endian(const poi_file_block *, uint32_t offset);
uint32_t get_poi_file_block_dword_terurut(const poi_file_block *, uint32_t offset);
uint32_t get_poi_file_block_dword_little_endian(const poi_file_block *, uint32_t offset);

uint8_t* get_pointer_to_data(poi_file_block *);

void set_poi_file_block_byte(poi_file_block *, uint32_t offset, uint8_t data);
void set_poi_file_block_word_terurut(poi_file_block *, uint32_t offset, uint16_t data);
void set_poi_file_block_word_little_endian(poi_file_block *, uint32_t offset, uint16_t data);
void set_poi_file_block_dword_terurut(poi_file_block *, uint32_t offset, uint32_t data);
void set_poi_file_block_dword_little_endian(poi_file_block *, uint32_t offset, uint32_t data);

/* ------------------------------------------------ */




/* ------------------------------------------------ */
/* -----------FILE POI----------------------------- */
typedef uint32_t poi_file_block_num_t;

FILE * dotpoi_mounted_file;

#define POI_VOLUME_INFORMATION_BLOCKS_NUM 1
#define POI_ALLOCATION_TABLE_BLOCKS_NUM 256
#define POI_DATA_POOL_BLOCKS_NUM 65536

#define POI_TOTAL_BLOCKS_NUM (POI_VOLUME_INFORMATION_BLOCKS_NUM+POI_ALLOCATION_TABLE_BLOCKS_NUM+POI_DATA_POOL_BLOCKS_NUM)


#define POI_BLOCK_NUM_MIN 0
#define POI_BLOCK_NUM_MAX (TOTAL_BLOCKS_NUM+POI_BLOCK_NUM_MIN-1)


#define POI_FILE_SIZE ((POI_TOTAL_BLOCKS_NUM)*POI_BLOCK_SIZE)
/* ------------------------------------------------ */



/* ------------------------------------------------ */
/* PROSEDUR DAN FUNGSI UNTUK MEMULAI DAN MENGAKHIRI */



//memeriksa apakah path menunjuk kepada berkas .poi yang valid
//memeriksa ukuran berkas apakah sama dengan POI_FILE_SIZE
//memeriksa apakah 4 byte pertama merupakan magic string "poi!"
//keluar fungsi dalam keadaan file tertutup
//mengembalikan 1 bila semua kondisi di atas terpenuhi
//mengembalikan -errno bila terjadi error pembacaan file
//mengembalikan 0 bila file berhasil dibaca dan buka file poi
int poi_file_validasi (const char * path);



//membuka file poi dan menyimpan pointer to file di dotpoi_mounted_file
//prekondisi: poi_file_validasi(path)==1
//mengembalikan -errno bila terjadi error pembacaan file
//mengembalikan 1 bila file berhasil dibuka
int poi_file_open (const char * path);



//menutup dotpoi_mounted_file
int poi_file_close();



//membuat sebuah berkas berukuran POI_FILE_SIZE yang berisi volume poi kosong
//mengisi 4 byte pertama dengan magic string "poi!" (tanpa kutipan)
//mengisi 32 byte dari offset 0x04 dengan Sebuah null-terminated string yang menyimpan nama dari volume. Karakter yang digunakan ditulis dalam format yang dapat dibaca oleh manusia. Jika tak diisikan maka secara default akan berisi "POI!".
//mengisi 4 byte dari offset 0x24 dengan Kapasitas keseluruhan filesystem dalam blok (Dalam hal ini POI_DATA_POOL_BLOCKS_NUM), ditulis dalam integer 32-bit little endian. 
//mengisi 4 byte dari offset 0x28 dengan jumlah blok yang belum terpakai, ditulis dalam integer 32-bit little endian
//mengisi 4 byte dari offset 0x2C dengan indeks blok pertama yang bebas, ditulis dalam integer 32-bit little endian
//mengisi 32 byte dari offset 0x30 dengan entri blok direktori untuk direktori root
//mengisi 428 byte dari offset 0x50 dengan nilai '\0'
//mengisi 4 byte dari offset 0x1FC dengan string "!iop" (tanpa kutipan)
//keluar fungsi dalam keadaan file tertutup
//melempar nilai 1 bila sukses, melempar melempar -errno bila tidak sukses
int poi_file_create_new ( const char * path);

/* ------------------------------------------------ */





/* ------------------------------------------------ */
/* ------- PROSEDUR PEMBACAAN DAN PENULISAN ------- */

//mengambil blok ke-n dari file dotpoi_mounted_file
poi_file_block poi_file_read_block(poi_file_block_num_t n);

//menulis b.data ke blok ke-n dari file dotpoi_mounted_file
int poi_file_write_block(poi_file_block b, poi_file_block_num_t n);

/* ------------------------------------------------ */

#endif
