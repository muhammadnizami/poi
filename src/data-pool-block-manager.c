/**
@file data-pool-block-manager.c
@author jang berikoetnja
@brief mengatur pembacaan dan penulisan blok kantong data

*/

#ifndef data_pool_block_manager_c
#define data_pool_block_manager_c

#include "file-manager.h"
#include "data-pool-block-manager.h"
#include <assert.h>

//mengambil blok data pool dengan indeks n dari file dotpoi_mounted_file
//prekondisi: DATA_POOL_BLOCK_MIN_IDX <= n <= DATA_POOL_BLOCK_MAX_IDX
poi_file_block poi_data_pool_read_block(poi_data_pool_block_idx_t n){
	assert(DATA_POOL_BLOCK_MIN_IDX <= n && n <= DATA_POOL_BLOCK_MAX_IDX);
	return poi_file_read_block(n+POI_VOLUME_INFORMATION_BLOCKS_NUM + POI_ALLOCATION_TABLE_BLOCKS_NUM);
}

//menulis b.data ke blok data pool dengan indeks n dari file dotpoi_mounted_file
//prekondisi: DATA_POOL_BLOCK_MIN_IDX <= n <= DATA_POOL_BLOCK_MAX_IDX
int poi_data_pool_write_block(poi_file_block b, poi_data_pool_block_idx_t n){
	assert(DATA_POOL_BLOCK_MIN_IDX <= n && n <= DATA_POOL_BLOCK_MAX_IDX);
	return poi_file_write_block(b, n+POI_VOLUME_INFORMATION_BLOCKS_NUM + POI_ALLOCATION_TABLE_BLOCKS_NUM);;
}


#endif
