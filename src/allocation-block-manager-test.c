#include "allocation-block-manager.h"
#include <assert.h>

int main(){
	poi_file_create_new("testfile.poi");
	poi_file_open("testfile.poi");	
	int i;
	printf("\n");
	poi_file_block blk = poi_file_read_block(1);
	for (i=0;i<POI_BLOCK_WORD_NUM;i++){
		printf("%d ", get_poi_file_block_word_little_endian(&blk,i));
	}
	printf("\n");
	for (i=0;i<POI_BLOCK_WORD_NUM;i++){
		printf("%d ", getNextBlock(i));
	}
	printf("\n");
	printf("a %x sd%x\n",POI_BLOCK_WORD_NUM,((unsigned long)POI_BLOCK_WORD_NUM)/((unsigned long)POI_BLOCK_WORD_NUM));
	for (i=1;i<20;i++){
		printf("check %d: 0x%x\n",i,getNextBlock(i));
		fflush(stdout);
		assert(isEmpty(i));
	}
}
