/**
@file volume-information-test.c
@author jang berikoetnja
@brief poros penggerak uji untuk volume-information

*/


#include "volume-information.h"
#include "file-manager.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(){
	poi_file_create_new("testfile.poi");
	assert(poi_file_validasi("testfile.poi"));
	poi_file_open("testfile.poi");
	int i;

	assert(!strcmp(getNamaVolume(),"POI!"));

	printf("asersi %x == %x\n",getTotalSize(),POI_DATA_POOL_BLOCKS_NUM);
	assert(getTotalSize()==POI_DATA_POOL_BLOCKS_NUM);

	printf("asersi %x == %x\n",getNumFreeBlocks(),POI_DATA_POOL_BLOCKS_NUM-1);
	assert(getNumFreeBlocks()==POI_DATA_POOL_BLOCKS_NUM-1);

	printf("asersi %x == %x\n",getFirstFreeBlockIdx(),0x0001);
	assert(getFirstFreeBlockIdx()==0x0001);

	directory_entry r = getRootDirEntry();
	TulisWAKTU(getLastModifDateTime(r));
	putchar('\n');

	setNamaVolume("TEST");

	savePoiVolinfoCache();

	poi_file_close();

	poi_file_open("testfile.poi");

	assert(!strcmp(getNamaVolume(),"TEST"));
}
