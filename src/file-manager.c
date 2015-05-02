#ifndef file_manager_c
#define file_manager_c

#include "file-manager.h"
#include "directory-entry.h"
#include <string.h>
#include <assert.h>

/* ------------------------------------------------ */
/* --------STRUKTUR DATA BLOK PEMBACAAN BERKAS----- */

//spesifikasi getter dan setter jelas
//offset dihitung berdasarkan ukuran data yang diambil.
//misal isi data	: a b c d e f g h ....
//maka	offset byte	: 0 1 2 3 4 5 6 7 ....
//	offset word	: 0   1   2   3   ....
//	offset dword	: 0       1       ....

uint8_t get_poi_file_block_byte(const poi_file_block * fb, uint32_t offset){
	assert(0<=offset && offset<POI_BLOCK_SIZE);
	return fb->data[offset];
}

uint16_t get_poi_file_block_word_terurut(const poi_file_block * fb, uint32_t offset){
	assert(0<=offset && offset<POI_BLOCK_SIZE/2);
	uint8_t bytes[2];
	bytes[0]=fb->data[offset*2];
	bytes[1]=fb->data[offset*2+1];
	return (((uint16_t)bytes[0])<<8) | bytes[1];
}

uint16_t get_poi_file_block_word_little_endian(const poi_file_block * fb, uint32_t offset){
	assert(0<=offset && offset<POI_BLOCK_SIZE/2);
	uint8_t bytes[2];
	bytes[0]=fb->data[offset*2];
	bytes[1]=fb->data[offset*2+1];
	return (((uint16_t)bytes[1])<<8) | bytes[0];
}

uint32_t get_poi_file_block_dword_terurut(const poi_file_block * fb, uint32_t offset){
	assert(0<=offset && offset<POI_BLOCK_SIZE/4);
	uint8_t bytes[4];
	int i;
	for (i=0;i<4;i++){
		bytes[i]=fb->data[offset*4+i];
	}
	uint32_t retval = 0;
	for (i=0;i<4;i++){
		retval = retval | (bytes[i]<<((3-i)*8));
	}
	return retval;
}

uint32_t get_poi_file_block_dword_little_endian(const poi_file_block * fb, uint32_t offset){
	assert(0<=offset && offset<POI_BLOCK_SIZE/4);
	uint8_t bytes[4];
	int i;
	for (i=0;i<4;i++){
		bytes[i]=fb->data[offset*4+i];
	}
	uint32_t retval = 0;
	for (i=0;i<4;i++){
		retval = retval | (bytes[i]<<(i*8));
	}
	return retval;
}

uint8_t* get_pointer_to_data(poi_file_block * fb){
	return (fb->data);
}


void set_poi_file_block_byte(poi_file_block * fb, uint32_t offset, uint8_t data){
	assert(0<=offset && offset<POI_BLOCK_SIZE);
	fb->data[offset]=data;
	return;
}

void set_poi_file_block_word_terurut(poi_file_block * fb, uint32_t offset, uint16_t data){
	assert(0<=offset && offset<POI_BLOCK_SIZE/2);
	uint8_t bytes[2];
	fb->data[offset*2]=(data&0xff00)>>8;
	fb->data[offset*2+1]=(data&0x00ff);
	return;
}

void set_poi_file_block_word_little_endian(poi_file_block * fb, uint32_t offset, uint16_t data){
	assert(0<=offset && offset<POI_BLOCK_SIZE/2);

	fb->data[offset*2]=(data&0x00ff);
	fb->data[offset*2+1]=(data&0xff00)>>8;
	return;
}

void set_poi_file_block_dword_terurut(poi_file_block * fb, uint32_t offset, uint32_t data){
	assert(0<=offset && offset<POI_BLOCK_SIZE/4);
	int i;
	for (i=3;i>=0;i--){
		fb->data[offset*4+i]=data&0xff;
		data=data>>8;
	}
	return;
}

void set_poi_file_block_dword_little_endian(poi_file_block * fb, uint32_t offset, uint32_t data){
	assert(0<=offset && offset<POI_BLOCK_SIZE/4);
	int i;
	for (i=0;i<4;i++){
		fb->data[offset*4+i]=data&0xff;
		data=data>>8;
	}
	return;
}


/* ------------------------------------------------ */




/* ------------------------------------------------ */
/* -----------FILE POI----------------------------- */
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
int poi_file_validasi (const char * path){
	FILE * f = fopen(path,"r");

	if (f==NULL)
		return -errno;

	if (fgetc(f)!='p')
		return 0;
	if (fgetc(f)!='o')
		return 0;
	if (fgetc(f)!='i')
		return 0;
	if (fgetc(f)!='!')
		return 0;

	fseek(f, 0L, SEEK_END);


	if (ftell(f)+1<POI_FILE_SIZE)
		return 0;

	fclose(f);


	return 1;
}




//membuka file poi dan menyimpan pointer to file di dotpoi_mounted_file
//prekondisi: poi_file_validasi(path)==1
//mengembalikan -errno bila terjadi error pembacaan file
//mengembalikan 1 bila file berhasil dibuka
int poi_file_open (const char * path){
	assert(poi_file_validasi(path)==1);
	dotpoi_mounted_file = fopen(path,"r+");
	if (dotpoi_mounted_file==NULL)
		return errno;
	//TODO bila Volume Information hendak dimuat di sini juga
	return 1;
}




//menutup dotpoi_mounted_file
int poi_file_close(){
	fclose(dotpoi_mounted_file);
	return 1;
}




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
//melempar nilai 1 bila sukses, melempar -errno bila tidak sukses
int poi_file_create_new ( const char * path){	
	poi_file_block volinfo;
	memcpy(volinfo.data,"poi!",4);
	strcpy(volinfo.data+0X04,"POI!");
	set_poi_file_block_dword_little_endian(&volinfo,9,POI_DATA_POOL_BLOCKS_NUM);
	set_poi_file_block_dword_little_endian(&volinfo,10,POI_DATA_POOL_BLOCKS_NUM-1);
	set_poi_file_block_dword_little_endian(&volinfo,11,0x0001);
	poi_attr_t root_attr = {.x=0,.w=1,.r=1,.d=1};
	WAKTU curTime = GetCurrentTime();
	directory_entry e = makeEntry("",root_attr,GetJam(curTime),GetTanggal(curTime),0,0x0000);
	memcpy(volinfo.data+0x30,e.bytearr,32);
	memset(volinfo.data+0x50,'\0',428);
	memcpy(volinfo.data+0x1FC,"!iop",4);
	dotpoi_mounted_file = fopen(path,"w");
	if (dotpoi_mounted_file==NULL) return -errno;
	poi_file_write_block(volinfo,0);
	
	poi_file_block zeroarr;
	memset(zeroarr.data,0x00,POI_BLOCK_SIZE);
	poi_file_block firstallocationblock = zeroarr;
	set_poi_file_block_word_little_endian(&firstallocationblock,0,0xffff);
	poi_file_write_block(firstallocationblock,1);
	int i;
	for (i=2;i<=POI_ALLOCATION_TABLE_BLOCKS_NUM;i++)
		poi_file_write_block(zeroarr,i);
	for (;i<POI_TOTAL_BLOCKS_NUM;i++)
		poi_file_write_block(zeroarr,i);
	fflush(dotpoi_mounted_file);
	fclose(dotpoi_mounted_file);
	return 1;
}


/* ------------------------------------------------ */





/* ------------------------------------------------ */
/* ------- PROSEDUR PEMBACAAN DAN PENULISAN ------- */

//mengambil blok ke-n dari file dotpoi_mounted_file
poi_file_block poi_file_read_block(poi_file_block_num_t n){
	fseek(dotpoi_mounted_file,n*POI_BLOCK_SIZE,SEEK_SET);
	poi_file_block retval;
	fread(retval.data,sizeof(uint8_t),POI_BLOCK_SIZE,dotpoi_mounted_file);
	return retval;
}


//menulis b.data ke blok ke-n dari file dotpoi_mounted_file
int poi_file_write_block(poi_file_block b, poi_file_block_num_t n){
	fseek(dotpoi_mounted_file,n*POI_BLOCK_SIZE,SEEK_SET);
	fwrite(b.data,sizeof(uint8_t),POI_BLOCK_SIZE,dotpoi_mounted_file);
	fflush(dotpoi_mounted_file);
	return 0;
}


/* ------------------------------------------------ */

#endif
