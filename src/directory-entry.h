/**
@file directory-entry.h
@author jang berikoetnja
@brief struktur data entri direktori

Ini adalah struktur data entri direktori yang sudah dalam bentuk array of byte. Untuk menulis, tinggal disalin ke array of byte lain lalu ditulis.
penyalinan array of byte dapat menggunakan memcpy langsung ke directory_entry.bytearr

*/

#ifndef directory_entry_h
#define directory_entry_h

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "ADT Jam + Tanggal/waktu.h"

typedef struct{
	uint8_t bytearr[32];
} directory_entry;

//prekondisi: dst berkapasitas 21 byte
char* getNama(char * dst, directory_entry e);

typedef struct {
	bool x;
	bool w;
	bool r;
	bool d;
} poi_attr_t;

poi_attr_t getattr(directory_entry e);

JAM getLastModifTime(directory_entry e);

TANGGAL getLastModifDate(directory_entry e);

WAKTU getLastModifDateTime(directory_entry e);

time_t getLastModifTimetime_t(directory_entry e);

uint16_t getFirstDataBlockIdx(directory_entry e);

uint32_t getFileSize(directory_entry e);

void setNama(directory_entry* e, const char * namabaru);

void setattr(directory_entry* e, poi_attr_t attr);

void setLastModifTime(directory_entry* e, JAM J);

void setLastModifDate(directory_entry* e, TANGGAL T);

void setLastModifDateTime(directory_entry* e, WAKTU W);

void setFirstDataBlockIdx(directory_entry* e, uint16_t idx);

void setFileSize(directory_entry* e, uint32_t size);

directory_entry makeEntry(const char* nama, poi_attr_t atribut, JAM waktu, TANGGAL tanggal, uint16_t idx, uint32_t size);

#endif
