#ifndef file_manager_h
#define file_manager_h

#include <stdio.h>
#include <stdint.h>

#define POI_BLOCK_SIZE 512
#define POI_BLOCK_NUM_MAX 65752
#define POI_BLOCK_NUM_MIN 0

#define POI_FILE_SIZE (POI_BLOCK_NUM_MAX-POI_BLOCK_NUM_MIN+1)*POI_BLOCK_SIZE

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


typedef uint32_t poi_file_block_num;

FILE * dotpoi_mounted_file;

/* ------------------------------------------------ */
/* PROSEDUR DAN FUNGSI UNTUK MEMULAI DAN MENGAKHIRI */

int poi_file_create_new ( const char * path);

int poi_file_verify (const char * path);

int poi_file_open (const char * path);

int poi_file_close();

/* ------------------------------------------------ */





/* ------------------------------------------------ */
/* ------- PROSEDUR PEMBACAAN DAN PENULISAN ------- */

poi_file_block poi_file_read_block(poi_file_block_num n);

int poi_file_write_block(poi_file_block b, poi_file_block_num n);

/* ------------------------------------------------ */

#endif
