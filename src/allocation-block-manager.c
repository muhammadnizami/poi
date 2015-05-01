#ifndef allocation_manager_c
#define allocation_manager_c

#include "allocation-block-manager.h"

#include <assert.h>
poi_file_block poi_allocation_cache;
//blok file .poi yang di-cache, dihitung dari awal
//berisi 0 bila tidak ada
int poi_allocation_cache_file_block_num = 0;

void loadPoiAllocationCache(poi_file_block_num_t n){
	assert(1<=n && n<=POI_ALLOCATION_TABLE_BLOCKS_NUM);
	if (poi_allocation_cache_file_block_num>0)
		savePoiAllocationCache();

	poi_allocation_cache = poi_file_read_block(n);
	poi_allocation_cache_file_block_num = n;
	return;
}


void savePoiAllocationCache(){
	if (poi_allocation_cache_file_block_num > 0);
	poi_file_write_block(poi_allocation_cache,poi_allocation_cache_file_block_num);
	return;
}


//true bila indeks blok data pool tersebut kosong, yaitu next = 0x00
//prekondisi: DATA_POOL_BLOCK_MIN_IDX <= n <= DATA_POOL_BLOCK_MAX_IDX
bool isEmpty(poi_data_pool_block_idx_t n){
	assert(DATA_POOL_BLOCK_MIN_IDX <=n
		&& DATA_POOL_BLOCK_MAX_IDX >=n);
	return getNextBlock(n)==0x00;
}

poi_file_block_num_t getPointerBlockLocation(poi_data_pool_block_idx_t n){

	return n/POI_BLOCK_WORD_NUM+1;
}

//mengembalikan blok selanjutnya dari blok ke-n
//yaitu word dengan indeks n dimulai dari word paling awal di blok allocation table paling awal
//prekondisi: DATA_POOL_BLOCK_MIN_IDX <= n <= DATA_POOL_BLOCK_MAX_IDX
poi_data_pool_block_idx_t getNextBlock(poi_data_pool_block_idx_t n){
	assert(DATA_POOL_BLOCK_MIN_IDX <=n
		&& DATA_POOL_BLOCK_MAX_IDX >=n);
	if ((getPointerBlockLocation(n)) != poi_allocation_cache_file_block_num)
		loadPoiAllocationCache(getPointerBlockLocation(n));
	return get_poi_file_block_word_little_endian(&poi_allocation_cache,((unsigned long)n)%((unsigned long)POI_BLOCK_WORD_NUM));
}


//menetapkan next dari current menjadi next
void setNextBlock(poi_data_pool_block_idx_t current, 
			poi_data_pool_block_idx_t next){
	assert(DATA_POOL_BLOCK_MIN_IDX <=current
		&& DATA_POOL_BLOCK_MAX_IDX >=current
		&& DATA_POOL_BLOCK_MIN_IDX <=next
		&& DATA_POOL_BLOCK_MAX_IDX >=next);
	////printf("\t\tpoi_allocation_cache_file_block_num: %x\n",poi_allocation_cache_file_block_num);
	if ((getPointerBlockLocation(current)) != poi_allocation_cache_file_block_num)
		loadPoiAllocationCache(getPointerBlockLocation(current));
	set_poi_file_block_word_little_endian(&poi_allocation_cache,((unsigned long)current)%((unsigned long)POI_BLOCK_WORD_NUM),next);
	////printf("\t%x\n",get_poi_file_block_word_little_endian(&poi_allocation_cache,((uint64_t)current)%POI_BLOCK_WORD_NUM));
	return;
}

poi_data_pool_block_idx_t getNextEmpty(poi_data_pool_block_idx_t n){
	assert(DATA_POOL_BLOCK_MIN_IDX <=n
		&& DATA_POOL_BLOCK_MAX_IDX >=n);
	poi_data_pool_block_idx_t retval = n+1;
	while (!isEmpty(retval) && retval<=DATA_POOL_BLOCK_MAX_IDX) retval++;
	return retval;
}


#endif
