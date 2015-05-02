/**
@file allocation-block-manager.h
@author jang berikoetnja
@brief mengambil dan menyimpan pointer to next di allocation table.

Tidak memperhitungkan free space.
Memiliki cache. Setiap akhir manipulasi harus memanggil savePoiAllocationCache()

*/

#ifndef allocation_manager_h
#define allocation_manager_h

#include "file-manager.h"
#include "data-pool-block-manager.h"
#include <stdbool.h>

void loadPoiAllocationCache(poi_file_block_num_t n);

//harus dipanggil setiap selesai urut-urutan setNextBlock
void savePoiAllocationCache();

//true bila indeks blok data pool tersebut kosong, yaitu next = 0x00
//prekondisi: DATA_POOL_BLOCK_MIN_IDX <= n <= DATA_POOL_BLOCK_MAX_IDX
bool isEmpty(poi_data_pool_block_idx_t n);

//mengembalikan blok selanjutnya dari blok ke-n
//yaitu word dengan indeks n dimulai dari word paling awal di blok allocation table paling awal
//prekondisi: DATA_POOL_BLOCK_MIN_IDX <= n <= DATA_POOL_BLOCK_MAX_IDX
poi_data_pool_block_idx_t getNextBlock(poi_data_pool_block_idx_t n);

//menetapkan next dari current menjadi next
void setNextBlock(poi_data_pool_block_idx_t current, poi_data_pool_block_idx_t next);

poi_data_pool_block_idx_t getNextEmpty(poi_data_pool_block_idx_t n);

#endif
