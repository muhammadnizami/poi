/**
@file data-pool-block-manager-test.c
@author jang berikoetnja
@brief poros penggerak uji untuk data-pool-block-manager

*/

#include "data-pool-block-manager.h"
#include <assert.h>

void testConstants(){
	assert(DATA_POOL_BLOCK_MIN_IDX==0x00);
	assert(DATA_POOL_BLOCK_MAX_IDX==0xFFFF);
}

void testReadWrite(){
	poi_file_block blkin,blkout;
	int i;
	for ( i=0;i<100;i++) blkin.data[i]=i;

	poi_file_create_new("testfile.poi");
	poi_file_open("testfile.poi");
	poi_data_pool_write_block(blkin,0x05);
	blkout=poi_data_pool_read_block(0x05);
	poi_file_close();

	for ( i=0;i<100;i++) assert(blkout.data[i]==blkin.data[i]);
}

int main(){
	testConstants();
	testReadWrite();
}
