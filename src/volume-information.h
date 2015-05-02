/**

@file volume-information.h
@author jang berikoetnja
@brief mengatur informasi volume

Modul ini bertugas untuk membaca dan menulis informasi volume pada blok pertama berkas .poi. Modul ini memiliki cache, sehingga setiap akhir manipulasi, savePoiVolinfoCache() harus dipanggil.

*/

#ifndef volume_information_h
#define volume_information_h

#include "file-manager.h"
#include "directory-entry.h"

void loadPoiVolinfoCache();

//harus dipanggil setiap selesai urut-urutan set
void savePoiVolinfoCache();

//mengembalikan nama volume
const char* getNamaVolume();

uint32_t getTotalSize();

uint32_t getNumFreeBlocks();

uint32_t getFirstFreeBlockIdx();

directory_entry getRootDirEntry();

//menetapkan nama volume
void setNamaVolume(const char * nama_baru);

void setTotalSize(uint32_t);

void setNumFreeBlocks(uint32_t);

void setFirstFreeBlockIdx(uint32_t);

void setRootDirEntry(directory_entry);

#endif
