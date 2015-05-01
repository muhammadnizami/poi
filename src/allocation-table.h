#ifndef allocation_table_h
#define allocation_table_h

#include "allocation-block-manager.h"
#include "volume-information.h"
#include "data-pool-block-manager.h"

/////////////////////////////////////////////////////////////////
///////////////////ALLOCATION-TABLE//////////////////////////////
/////////////////////////////////////////////////////////////////

//panggil saveAllocTableCaches() setiap kali selesai memanipulasi
//bila tidak dipanggil undefined behavior. filesystem dapat rusak

//menyimpan semua cache yang digunakan allocation-table
void saveAllocTableCaches();

//membuat list baru berukuran 1
//mengembalikan -errno bila tidak bisa. misal -ENOSPC bila penuh
//mengembalikan 0 bila berhasil
//indeks blok disimpan di *n
//perhatikan numfreeblocks dari volinfo
int32_t newList(poi_data_pool_block_idx_t *n);

//mengalokasikan satu blok dan mengkaitkan n kepadanya
//mengembalikan -errno bila tidak bisa. misal -ENOSPC bila penuh
//mengembalikan 0 bila berhasil
//perhatikan numfreeblocks dari volinfo
//F.S. getNext(n) sudah dialokasi
int32_t allocateAfter(poi_data_pool_block_idx_t n);

////////////////////insertAfter BELUM LULUS UJI///////////////////
//mengalokasikan satu blok dan mengkaitkan n kepadanya, kemudian
//	menuliskan blk ke blok tersebut
//mengembalikan -errno bila tidak bisa. misal -ENOSPC bila penuh
//mengembalikan 0 bila berhasil
//perhatikan numfreeblocks dari volinfo
//F.S. getNext(n) sudah dialokasi
int32_t insertAfter(poi_data_pool_block_idx_t n, poi_file_block blk);
//////////////////////////////////////////////////////////////////

//menghapus list of blok teralokasi (pointernya diset 0x00)
//bila ditemukan blok dengan indeks kurang dari getFirstFreeBlock()
//	maka setFirstFreeBlock(blok tersebut)
//perhatikan numfreeblocks dari volinfo
void deleteListOfBlock(poi_data_pool_block_idx_t n);



#endif
