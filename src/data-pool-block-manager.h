#ifndef data_pool_block_manager_h
#define data_pool_block_manager_h

#include "file-manager.h"

#define DATA_POOL_BLOCK_MIN_IDX 0
#define DATA_POOL_BLOCK_MAX_IDX POI_DATA_POOL_BLOCKS_NUM-1
#define DATA_POOL_BLOCK_MIN_NUM POI_VOLUME_INFORMATION_BLOCKS_NUM + POI_ALLOCATION_TABLE_BLOCKS_NUM
#define DATA_POOL_BLOCK_MAX_NUM DATA_POOL_BLOCK_MIN_NUM+DATA_POOL_BLOCK_MAX_IDX

typedef uint16_t poi_data_pool_block_idx_t;

//mengambil blok data pool dengan indeks n dari file dotpoi_mounted_file
//prekondisi: DATA_POOL_BLOCK_MIN_IDX <= n <= DATA_POOL_BLOCK_MAX_IDX
poi_file_block poi_data_pool_read_block(poi_data_pool_block_idx_t n);

//menulis b.data ke blok data pool dengan indeks n dari file dotpoi_mounted_file
//prekondisi: DATA_POOL_BLOCK_MIN_IDX <= n <= DATA_POOL_BLOCK_MAX_IDX
int poi_data_pool_write_block(poi_file_block b, poi_data_pool_block_idx_t n);

poi_data_pool_block_idx_t poi_data_pool_get_next_empty(poi_data_pool_block_idx_t n);

#endif
