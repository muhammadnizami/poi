#include "file-manager.h"
#include <assert.h>
#include <stdlib.h>

void test_get_poi_file_block_byte(){
	poi_file_block test;
	test.data[0]=0x10;
	test.data[POI_BLOCK_SIZE-1]=0x43;
	assert(get_poi_file_block_byte(&test,0)==0x10);
	assert(get_poi_file_block_byte(&test,POI_BLOCK_SIZE-1)==0x43);	
}
void test_get_poi_file_block_word_terurut(){
	poi_file_block test;
	test.data[0]=0x10;
	test.data[1]=0x11;
	test.data[2]=0x12;
	test.data[3]=0x13;
	test.data[POI_BLOCK_SIZE-4]=0x44;
	test.data[POI_BLOCK_SIZE-3]=0x43;
	test.data[POI_BLOCK_SIZE-2]=0x42;
	test.data[POI_BLOCK_SIZE-1]=0x41;
	assert(get_poi_file_block_word_terurut(&test,0)==0x1011);
	assert(get_poi_file_block_word_terurut(&test,POI_BLOCK_WORD_NUM-1)==0x4241);
}
void test_get_poi_file_block_word_little_endian(){
	poi_file_block test;
	test.data[0]=0x10;
	test.data[1]=0x11;
	test.data[2]=0x12;
	test.data[3]=0x13;
	test.data[POI_BLOCK_SIZE-4]=0x44;
	test.data[POI_BLOCK_SIZE-3]=0x43;
	test.data[POI_BLOCK_SIZE-2]=0x42;
	test.data[POI_BLOCK_SIZE-1]=0x41;
	assert(get_poi_file_block_word_little_endian(&test,0)==0x1110);
	assert(get_poi_file_block_word_little_endian(&test,POI_BLOCK_WORD_NUM-1)==0x4142);
}
void test_get_poi_file_block_dword_terurut(){
	poi_file_block test;
	test.data[0]=0x10;
	test.data[1]=0x11;
	test.data[2]=0x12;
	test.data[3]=0x13;
	test.data[POI_BLOCK_SIZE-4]=0x44;
	test.data[POI_BLOCK_SIZE-3]=0x43;
	test.data[POI_BLOCK_SIZE-2]=0x42;
	test.data[POI_BLOCK_SIZE-1]=0x41;
	assert(get_poi_file_block_dword_terurut(&test,0)==0x10111213);
	assert(get_poi_file_block_dword_terurut(&test,POI_BLOCK_DWORD_NUM-1)==0x44434241);

}
void test_get_poi_file_block_dword_little_endian(){
	poi_file_block test;
	test.data[0]=0x10;
	test.data[1]=0x11;
	test.data[2]=0x12;
	test.data[3]=0x13;
	test.data[POI_BLOCK_SIZE-4]=0x44;
	test.data[POI_BLOCK_SIZE-3]=0x43;
	test.data[POI_BLOCK_SIZE-2]=0x42;
	test.data[POI_BLOCK_SIZE-1]=0x41;
	assert(get_poi_file_block_dword_little_endian(&test,0)==0x13121110);
	assert(get_poi_file_block_dword_little_endian(&test,POI_BLOCK_DWORD_NUM-1)==0x41424344);

}

void test_get_pointer_to_data(){
	poi_file_block test;
	test.data[0]=0x02;
	assert(get_pointer_to_data(&test)==test.data);
	assert(get_pointer_to_data(&test)[0]==0x02);
	get_pointer_to_data(&test)[14]=0x2f;
	assert(test.data[14]==0x2f);
}

void test_set_poi_file_block_byte(){
	poi_file_block test;
	set_poi_file_block_byte(&test,0,0x02);
	set_poi_file_block_byte(&test,POI_BLOCK_SIZE-1,0x01);
	assert(test.data[0]==0x02);
	assert(test.data[POI_BLOCK_SIZE-1]==0x01);
}
void test_set_poi_file_block_word_terurut(){
	poi_file_block test;
	set_poi_file_block_word_terurut(&test,0,0x0102);
	set_poi_file_block_word_terurut(&test,POI_BLOCK_WORD_NUM-1,0x1112);
	assert(test.data[0]==0x01);
	assert(test.data[1]==0x02);
	assert(test.data[POI_BLOCK_SIZE-2]==0x11);
	assert(test.data[POI_BLOCK_SIZE-1]==0x12);
}
void test_set_poi_file_block_word_little_endian(){
	poi_file_block test;
	set_poi_file_block_word_little_endian(&test,0,0x0102);
	set_poi_file_block_word_little_endian(&test,POI_BLOCK_WORD_NUM-1,0x1112);
	assert(test.data[1]==0x01);
	assert(test.data[0]==0x02);
	assert(test.data[POI_BLOCK_SIZE-1]==0x11);
	assert(test.data[POI_BLOCK_SIZE-2]==0x12);
}
void test_set_poi_file_block_dword_terurut(){
	poi_file_block test;
	set_poi_file_block_dword_terurut(&test,0,0x01020304);
	set_poi_file_block_dword_terurut(&test,POI_BLOCK_DWORD_NUM-1,0x11121314);
	assert(test.data[0]==0x01);
	assert(test.data[1]==0x02);
	assert(test.data[2]==0x03);
	assert(test.data[3]==0x04);
	assert(test.data[POI_BLOCK_SIZE-4]==0x11);
	assert(test.data[POI_BLOCK_SIZE-3]==0x12);
	assert(test.data[POI_BLOCK_SIZE-2]==0x13);
	assert(test.data[POI_BLOCK_SIZE-1]==0x14);
}
void test_set_poi_file_block_dword_little_endian(){
	poi_file_block test;
	set_poi_file_block_dword_little_endian(&test,0,0x01020304);
	set_poi_file_block_dword_little_endian(&test,POI_BLOCK_DWORD_NUM-1,0x11121314);
	assert(test.data[3]==0x01);
	assert(test.data[2]==0x02);
	assert(test.data[1]==0x03);
	assert(test.data[0]==0x04);
	assert(test.data[POI_BLOCK_SIZE-1]==0x11);
	assert(test.data[POI_BLOCK_SIZE-2]==0x12);
	assert(test.data[POI_BLOCK_SIZE-3]==0x13);
	assert(test.data[POI_BLOCK_SIZE-4]==0x14);
}

void test_poi_file_validasi(){
	FILE * f = fopen("testfile.poi","w");
	fputc('p',f);
	fputc('o',f);
	fputc('i',f);
	fputc('!',f);
	int i;
	for (i=0; i<POI_FILE_SIZE-4;i++)
		fputc('\0',f);
	fclose(f);
	
	assert(poi_file_validasi("testfile.poi")==1);
}

void test_poi_file_create_new(){
	int i,j;
	poi_file_block pfb;
	system("rm testfile.poi");
	poi_file_create_new("testfile.poi");
	poi_file_open("testfile.poi");
	assert(poi_file_validasi("testfile.poi")==1);
}

void test_poi_file_read_write_block(){
	FILE * f = fopen("testfile.poi","w");
	fputc('p',f);
	fputc('o',f);
	fputc('i',f);
	fputc('!',f);
	int i;
	for (i=0; i<POI_FILE_SIZE-4;i++)
		fputc('\0',f);
	fclose(f);

	poi_file_open ("testfile.poi");

	poi_file_block awal = poi_file_read_block(0);
	assert(memcmp("poi!",awal.data,4)==0);
	poi_file_close();
}


int main(){
	test_get_poi_file_block_byte();
	test_get_poi_file_block_word_terurut();
	test_get_poi_file_block_word_little_endian();
	test_get_poi_file_block_dword_terurut();
	test_get_poi_file_block_dword_little_endian();
	test_get_pointer_to_data();
	test_set_poi_file_block_byte();
	test_set_poi_file_block_word_terurut();
	test_set_poi_file_block_word_little_endian();
	test_set_poi_file_block_dword_terurut();
	test_set_poi_file_block_dword_little_endian();

	test_poi_file_validasi();
	test_poi_file_create_new();
	test_poi_file_read_write_block();
}
