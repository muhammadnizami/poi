#ifndef file_manager_h
#define file_manager_h

#include <stdio.h>
#include <stdint.h>

#define POI_BLOCK_SIZE 512

/* ------------------------------------------------ */
/* --------STRUKTUR DATA BLOK PEMBACAAN BERKAS----- */
typedef struct{
	uint8_t data[POI_BLOCK_SIZE];
}poi_file_block;

uint8_t get_poi_file_block_byte(const poi_file_block *, uint32_t offset);
uint16_t get_poi_file_block_word_terurut(const poi_file_block *, uint32_t offset);
uint16_t get_poi_file_block_word_little_endian(const poi_file_block *, uint32_t offset);
uint32_t get_poi_file_block_dword_terurut(const poi_file_block *, uint32_t offset);
uint32_t get_poi_file_block_dword_little_endian(const poi_file_block *, uint32_t offset);

void set_poi_file_block_byte(poi_file_block *, uint32_t offset, uint8_t data);
void set_poi_file_block_word_terurut(poi_file_block *, uint32_t offset, uint16_t data);
void set_poi_file_block_word_little_endian(poi_file_block *, uint32_t offset, uint16_t data);
void set_poi_file_block_dword_terurut(poi_file_block *, uint32_t offset, uint32_t data);
void set_poi_file_block_dword_little_endian(poi_file_block *, uint32_t offset, uint32_t data);

/* ------------------------------------------------ */




/* ------------------------------------------------ */
/* -----------FILE POI----------------------------- */
typedef uint32_t poi_file_block_num;

FILE * dotpoi_mounted_file;

#define POI_VOLUME_INFORMATION_BLOCKS_NUM 1
#define POI_ALLOCATION_TABLE_BLOCKS_NUM 256
#define POI_DATA_POOL_BLOCKS_NUM 65536

#define POI_TOTAL_BLOCKS_NUM POI_VOLUME_INFORMATION_BLOCKS_NUM+POI_ALLOCATION_TABLE_BLOCKS_NUM+POI_DATA_POOL_BLOCKS_NUM


#define POI_BLOCK_NUM_MIN 0
#define POI_BLOCK_NUM_MAX TOTAL_BLOCKS_NUM+POI_BLOCK_NUM_MIN-1


#define POI_FILE_SIZE (POI_TOTAL_BLOCKS_NUM)*POI_BLOCK_SIZE
/* ------------------------------------------------ */



/* ------------------------------------------------ */
/* PROSEDUR DAN FUNGSI UNTUK MEMULAI DAN MENGAKHIRI */

int poi_file_verify (const char * path);

int poi_file_open (const char * path);

int poi_file_close();



//membuat sebuah berkas berukuran POI_FILE_SIZE yang berisi volume poi kosong
//mengisi 4 byte pertama dengan magic string "poi!" (tanpa kutipan)
//mengisi 32 byte dari offset 0x04 dengan Sebuah null-terminated string yang menyimpan nama dari volume. Karakter yang digunakan ditulis dalam format yang dapat dibaca oleh manusia. Jika tak diisikan maka secara default akan berisi "POI!".
//mengisi 4 byte dari offset 0x24 dengan Kapasitas keseluruhan filesystem dalam blok (Dalam hal ini POI_TOTAL_BLOCKS_NUM), ditulis dalam integer 32-bit little endian. 
//mengisi 4 byte dari offset 0x28 dengan jumlah blok yang belum terpakai, ditulis dalam integer 32-bit little endian
//mengisi 4 byte dari offset 0x2C dengan indeks blok pertama yang bebas, ditulis dalam integer 32-bit little endian
//mengisi 32 byte dari offset 0x30 dengan entri blok direktori untuk direktori root
//mengisi 428 byte dari offset 0x50 dengan nilai '\0'
//mengisi 4 byte dari offset 0x1FC dengan string "!iop" (tanpa kutipan)
//keluar fungsi dalam keadaan file tertutup
//melempar nilai 1 bila sukses, melempar kode error bila tidak sukses
int poi_file_create_new ( const char * path);

/* ------------------------------------------------ */





/* ------------------------------------------------ */
/* ------- PROSEDUR PEMBACAAN DAN PENULISAN ------- */

poi_file_block poi_file_read_block(poi_file_block_num n);

int poi_file_write_block(poi_file_block b, poi_file_block_num n);

/* ------------------------------------------------ */

#endif
