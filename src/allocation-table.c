#ifndef allocation_table_c
#define allocation_table_c

#include "allocation-table.h"

/////////////////////////////////////////////////////////////////
///////////////////ALLOCATION-TABLE//////////////////////////////
/////////////////////////////////////////////////////////////////

//panggil saveAllocTableCaches() setiap kali selesai memanipulasi
//bila tidak dipanggil undefined behavior. filesystem dapat rusak

//menyimpan semua cache yang digunakan allocation-table
void saveAllocTableCaches(){
	savePoiVolinfoCache();
	savePoiAllocationCache();
	return;//TODO
}

//membuat list baru berukuran 1
//mengembalikan -errno bila tidak bisa. misal -ENOSPC bila penuh
//mengembalikan 0 bila berhasil
//indeks blok disimpan di *n
//perhatikan numfreeblocks dari volinfo
int32_t newList(poi_data_pool_block_idx_t *n){
	if (getNumFreeBlocks()<1) return -ENOSPC;
	*n=getFirstFreeBlockIdx();
	if (*n==0XFFFF) return -ENOSPC;
	setNextBlock(*n,0xFFFF);
	setFirstFreeBlockIdx(getNextEmpty(*n));
	setNumFreeBlocks(getNumFreeBlocks()-1);
	return 0;
}

//mengalokasikan satu blok dan mengkaitkan n kepadanya
//mengembalikan -errno bila tidak bisa. misal -ENOSPC bila penuh
//mengembalikan 0 bila berhasil
//perhatikan numfreeblocks dari volinfo
//F.S. getNext(n) sudah dialokasi
int32_t allocateAfter(poi_data_pool_block_idx_t n){
	poi_data_pool_block_idx_t next;
	int32_t retval = newList(&next);
	if (retval < 0) return retval;
	setNextBlock(next,getNextBlock(n));
	setNextBlock(n,next);
	return retval;
}

//mengalokasikan satu blok dan mengkaitkan n kepadanya, kemudian
//	menuliskan blk ke blok tersebut
//mengembalikan -errno bila tidak bisa. misal -ENOSPC bila penuh
//mengembalikan 0 bila berhasil
//perhatikan numfreeblocks dari volinfo
//F.S. getNext(n) sudah dialokasi
int32_t insertAfter(poi_data_pool_block_idx_t n, poi_file_block blk){
	int32_t retval=allocateAfter(n);
	if (retval<0) return retval;
	retval = poi_data_pool_write_block(blk,getNextBlock(n));
	if (retval<0) return retval;
	return 0;
}

//menghapus list of blok teralokasi (pointernya diset 0x00)
//bila ditemukan blok dengan indeks kurang dari getFirstFreeBlock()
//	maka setFirstFreeBlock(blok tersebut)
//perhatikan numfreeblocks dari volinfo
void deleteListOfBlock(poi_data_pool_block_idx_t n){
	poi_data_pool_block_idx_t iter = n;
	uint32_t numfreed = 0;
	while (iter!=0xffff){
		//menghapus
		poi_data_pool_block_idx_t prev = iter;
		iter = getNextBlock(iter);
		setNextBlock(prev,0x0000);

		//cek getFirstFreeBlockIdx
		if (prev < getFirstFreeBlockIdx())
			setFirstFreeBlockIdx(prev);

		//track jumlah
		numfreed++;
	}
	setNumFreeBlocks(getNumFreeBlocks()-numfreed);
	
	return 0;//TODO
}


//'memotong' list: n dijadikan list yang tidak punya seterusnya, dan
//lanjutannya dihapus
void truncateList(poi_data_pool_block_idx_t n){
	deleteListOfBlock(getNextBlock(n));
	setNextBlock(n,0xffff);
}


#endif
