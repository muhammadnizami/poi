/**
@file allocation-table-test.c
@author jang berikoetnja
@brief berkas penggerak uji untuk allocation-table-test.c

*/

#include "allocation-table.h"
#include <stdio.h>

void saveAllocTableCaches();

//test fungsi newList, seharusnya newList:
//membuat list baru berukuran 1
//mengembalikan -errno bila tidak bisa. misal -ENOSPC bila penuh
//mengembalikan 0 bila berhasil
//indeks blok disimpan di *n
//perhatikan numfreeblocks dari volinfo
	poi_data_pool_block_idx_t n,m;
void newListTest(){
	printf("////newListTest////\n");
	int i;
	newList(&n);
	printf("new list 0x%x. next: 0x%x\n",n,getNextBlock(n));
	printf("ffb: 0x%x,\tnfb: 0x%x\n",getFirstFreeBlockIdx(),getNumFreeBlocks());
	for (i=0;i<0xf;i++) printf("next of 0x%x: 0x%x\n",i,getNextBlock(i));
	newList(&m);
	printf("new list 0x%x. next: 0x%x\n",m,getNextBlock(m));
	printf("ffb: 0x%x,\tnfb: 0x%x\n",getFirstFreeBlockIdx(),getNumFreeBlocks());
	for (i=0;i<0xf;i++) printf("next of 0x%x: 0x%x\n",i,getNextBlock(i));

	
}

//test fungsi allocateAfter, seharusnya allocateAfter:
//mengalokasikan satu blok dan mengkaitkan n kepadanya
//mengembalikan -errno bila tidak bisa. misal -ENOSPC bila penuh
//mengembalikan 0 bila berhasil
//perhatikan numfreeblocks dari volinfo
//F.S. getNext(n) sudah dialokasi
void allocateAfterTest(){
	printf("////allocateAfterTest////\n");
	int i;
	allocateAfter(n);
	printf("allocate after 0x%x: 0x%x\n",n,getNextBlock(n));
	printf("ffb: 0x%x,\tnfb: 0x%x\n",getFirstFreeBlockIdx(),getNumFreeBlocks());
	for (i=0;i<0xf;i++) printf("next of 0x%x: 0x%x\n",i,getNextBlock(i));

	allocateAfter(m);
	printf("allocate after 0x%x: 0x%x\n",m,getNextBlock(m));
	printf("ffb: 0x%x,\tnfb: 0x%x\n",getFirstFreeBlockIdx(),getNumFreeBlocks());


	for (i=0;i<0xf;i++) printf("next of 0x%x: 0x%x\n",i,getNextBlock(i));
}

//test fungsi insertAfter, seharusnya insertAfter:
//mengalokasikan satu blok dan mengkaitkan n kepadanya, kemudian
//	menuliskan blk ke blok tersebut
//mengembalikan -errno bila tidak bisa. misal -ENOSPC bila penuh
//mengembalikan 0 bila berhasil
//perhatikan numfreeblocks dari volinfo
//F.S. getNext(n) sudah dialokasi
#include <string.h>
#include <unistd.h>
int32_t insertAfterTest(){
	poi_file_block blk;
	memset(blk.data,0x43,512);
	int ret = insertAfter(getNextBlock(n),blk);
	int i;
	printf("\tawal isi blk: ");
	for (i=0;i<20;i++) printf("%x ",blk.data[i]);
	printf("\n");
	uint32_t written = getNextBlock(getNextBlock(n));

	poi_data_pool_write_block(blk,written);

	poi_file_block blk2 = poi_data_pool_read_block(written);
	printf("insert after 0x%x: 0x%x, ret: %x\n",getNextBlock(n),written,ret);
	printf("\tawal isi blk2: ");
	for (i=0;i<20;i++) printf("%x ",blk2.data[i]);
	printf("\n");
	printf("ffb: 0x%x,\tnfb: 0x%x\n",getFirstFreeBlockIdx(),getNumFreeBlocks());
}

//test fungsi deleteList, seharusnya deleteList:
//menghapus list of blok teralokasi Test(pointernya diset 0x00)
//bila ditemukan blok dengan indeks kurang dari getFirstFreeBlock()
//	maka setFirstFreeBlock(blok tersebut)
//perhatikan numfreeblocks dari volinfo
void deleteListOfBlockTest(){
	deleteListOfBlock(n);
	printf("delete list 0x%x. next: 0x%x\n",n,getNextBlock(n));
	printf("ffb: 0x%x,\tnfb: 0x%x\n",getFirstFreeBlockIdx(),getNumFreeBlocks());

}

void allocateAfterBanyakTest(){
	uint32_t i;
	for (i=0;i<0xfff;i++){
		if (!isEmpty(i)){printf("delete 0x%x\n",i); deleteListOfBlock(i);} //else printf("0x%x empty. next: 0x%x\n",i);
	}
	newList(&n);
	printf("new list 0x%x. next: 0x%x\n",n,getNextBlock(n));
	printf("ffb: 0x%x,\tnfb: 0x%x\n",getFirstFreeBlockIdx(),getNumFreeBlocks());
	for (i=0;i<0xfff;i++){
		allocateAfter(n);
		printf("allocate after 0x%x: 0x%x\n",n,getNextBlock(n));
		printf("ffb: 0x%x,\tnfb: 0x%x\n",getFirstFreeBlockIdx(),getNumFreeBlocks());
		n=getNextBlock(n);
	}
	for (i=0x00;i<0x10;i++)
		printf("check next 0x%x: 0x%x\n",i,getNextBlock(i));
	for (i=0x100;i<0x110;i++)
		printf("check next 0x%x: 0x%x\n",i,getNextBlock(i));
	for (i=0x3200;i<0x3210;i++)
		printf("check next 0x%x: 0x%x\n",i,getNextBlock(i));

}

int main(){
	poi_file_create_new("testfile.poi");
	poi_file_open("testfile.poi");
	newListTest();
	allocateAfterTest();
	insertAfterTest();
	deleteListOfBlockTest();
	allocateAfterBanyakTest();
}
