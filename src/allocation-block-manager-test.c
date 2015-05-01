#include "allocation-block-manager.h"
#include <assert.h>

int main(){
	poi_file_create_new("testfile.poi");
	poi_file_open("testfile.poi");	
	int i;
	printf("\n");
	poi_file_block blk = poi_file_read_block(1);
	for (i=0;i<POI_BLOCK_WORD_NUM;i++){
		printf("%x ", get_poi_file_block_word_little_endian(&blk,i));
	}
	printf("\n");
	for (i=0;i<POI_BLOCK_WORD_NUM;i++){
		printf("%x ", getNextBlock(i));
	}
	printf("\n");
	printf("a %x sd%x\n",POI_BLOCK_WORD_NUM,((unsigned long)POI_BLOCK_WORD_NUM)/((unsigned long)POI_BLOCK_WORD_NUM));
	for (i=0;i<20;i++){
		printf("check %d: 0x%x\n",i,getNextBlock(i));
		setNextBlock(i,0xffff);
		printf("\tdiganti jadi 0xffff: 0x%x\n",getNextBlock(i));
		fflush(stdout);
		//assert(isEmpty(i));
	}
	savePoiAllocationCache();
	blk = poi_file_read_block(1);
	for (i=0;i<POI_BLOCK_WORD_NUM;i++){
		printf("%x ", get_poi_file_block_word_little_endian(&blk,i));
	}
	for (i=0;i<20;i++){
		printf("check %d: 0x%x\n",i,getNextBlock(i));
		setNextBlock(i,0xffff);
		printf("\tdiganti jadi 0xffff: 0x%x\n",getNextBlock(i));
		fflush(stdout);
		//assert(isEmpty(i));
	}
}
