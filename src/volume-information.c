#ifndef volume_information_c
#define volume_information_c

#include "volume-information.h"
#include <stdbool.h>
#include <assert.h>
#include <string.h>

poi_file_block poi_volinfo_cache;
//blok file .poi yang di-cache, dihitung dari awal
//berisi 0 bila tidak ada
bool poi_volinfo_cache_file_block_loaded = false;

void loadPoiVolinfoCache(){
	poi_volinfo_cache = poi_file_read_block(0);
	poi_volinfo_cache_file_block_loaded = true;
	return;
}


void savePoiVolinfoCache(){
	if (poi_volinfo_cache_file_block_loaded);
	poi_file_write_block(poi_volinfo_cache,0);
	return;
}


//mengembalikan nama volume
const char* getNamaVolume(){
	if (!poi_volinfo_cache_file_block_loaded)
		loadPoiVolinfoCache();
	return poi_volinfo_cache.data+0x04;
}


uint32_t getTotalSize(){
	if (!poi_volinfo_cache_file_block_loaded)
		loadPoiVolinfoCache();
	return get_poi_file_block_dword_little_endian(&poi_volinfo_cache,9);
}


uint32_t getNumFreeBlocks(){
	if (!poi_volinfo_cache_file_block_loaded)
		loadPoiVolinfoCache();
	int i;
	for (i=0x28;i<0x2C;i++)
		printf("%x\n",poi_volinfo_cache.data[i]);
	return get_poi_file_block_dword_little_endian(&poi_volinfo_cache,10);
}


uint32_t getFirstFreeBlockIdx(){
	if (!poi_volinfo_cache_file_block_loaded)
		loadPoiVolinfoCache();
	return get_poi_file_block_dword_little_endian(&poi_volinfo_cache,11);
}


directory_entry getRootDirEntry(){
	if (!poi_volinfo_cache_file_block_loaded)
		loadPoiVolinfoCache();
	directory_entry retval;
	memcpy(retval.bytearr,poi_volinfo_cache.data+0x30,32);
	return retval;
}


//menetapkan nama volume
void setNamaVolume(const char * nama_baru){
	if (!poi_volinfo_cache_file_block_loaded)
		loadPoiVolinfoCache();
	strncpy(poi_volinfo_cache.data+0x04,nama_baru,32);
	return;
}


void setTotalSize(uint32_t s){
	if (!poi_volinfo_cache_file_block_loaded)
		loadPoiVolinfoCache();
	set_poi_file_block_dword_little_endian(&poi_volinfo_cache,9,s);
	return;
}


void setNumFreeBlocks(uint32_t n){
	if (!poi_volinfo_cache_file_block_loaded)
		loadPoiVolinfoCache();
	set_poi_file_block_dword_little_endian(&poi_volinfo_cache,10,n);
	return;
}


void setFirstFreeBlockIdx(uint32_t i){
	if (!poi_volinfo_cache_file_block_loaded)
		loadPoiVolinfoCache();
	set_poi_file_block_dword_little_endian(&poi_volinfo_cache,11,i);
	return;
}


void setRootDirEntry(directory_entry e){
	if (!poi_volinfo_cache_file_block_loaded)
		loadPoiVolinfoCache();
	memcpy(poi_volinfo_cache.data+0x30,e.bytearr,32);
	return;
}



#endif
