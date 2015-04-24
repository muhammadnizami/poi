#ifndef directory_entry_c
#define directory_entry_c

#include "directory-entry.h"

#include <string.h>

char* getNama(char * dst, directory_entry e){
	strcpy(dst,e.bytearr);
	return dst;
}



poi_attr_t getattr(directory_entry e){
	uint8_t attrbyte = e.bytearr[0x14];
	poi_attr_t retval;
	retval.x = attrbyte & 0x01;
	attrbyte = attrbyte >> 1;
	retval.w = attrbyte & 0x01;
	attrbyte = attrbyte >> 1;
	retval.r = attrbyte & 0x01;
	attrbyte = attrbyte >> 1;
	retval.d = attrbyte & 0x01;
	return retval;
}


JAM getLastModifTime(directory_entry e){
	//asumsi disimpan dalam big endian
	//TODO tanyakan lagi
	
	//mengambil word
	uint16_t timeword;
	timeword = e.bytearr[0x16]<<8 | e.bytearr[0x17];

	//membuat jam
	int hh = (timeword&0xf800)>>11;
	int mm = (timeword&0x07e0)>>5;
	int ss = (timeword&0x001f)*2;

	return MakeJAM(hh,mm,ss);
}


TANGGAL getLastModifDate(directory_entry e){
	//asumsi disimpan dalam big endian
	//TODO tanyakan lagi


	//mengambil word
	uint16_t dateword;
	dateword = e.bytearr[0x18]<<8 | e.bytearr[0x19];

	//membuat jam
	int yy = ((dateword&0xfe00)>>9)+2000;
	int mm = (dateword&0x01e0)>>5;
	int dd = (dateword&0x001f);
	return MakeTANGGAL(dd,mm,yy);
}


WAKTU getLastModifDateTime(directory_entry e){
	return MakeWAKTU(getLastModifDate(e),getLastModifTime(e));
}


uint16_t getFirstDataBlockIdx(directory_entry e){
	return e.bytearr[0x1A]|(e.bytearr[0x1B]<<8);
}

uint32_t getFileSize(directory_entry e){
	return e.bytearr[0x1C]
		|(e.bytearr[0x1D]<<8)
		|(e.bytearr[0x1E]<<16)
		|(e.bytearr[0x1F]<<24);
}

void setnama(directory_entry* e, const char* nama){
	strncpy(e->bytearr,nama,21);
}

void setattr(directory_entry* e, poi_attr_t attr){
	uint8_t attrbyte = 0;
	if (attr.x)
		attrbyte = attrbyte | 0x01;
	if (attr.w)
		attrbyte = attrbyte | 0x02;
	if (attr.r)
		attrbyte = attrbyte | 0x04;
	if (attr.d)
		attrbyte = attrbyte | 0x08;
	e->bytearr[0x14]=attrbyte;
	return;
}

void setLastModifTime(directory_entry* e, JAM J){
	//asumsi disimpan dalam big endian
	//TODO tanyakan lagi
	
	uint16_t word = (GetHour(J)<<11) | (GetMinute(J)<<5) | (GetSecond(J)/2);

	e->bytearr[0x16] = (word&0xff00) >> 8;
	e->bytearr[0x17] = (word&0x00ff);
	
	return;//TODO implementasi
}


void setLastModifDate(directory_entry* e, TANGGAL T){
	//asumsi disimpan dalam big endian
	//TODO tanyakan lagi

	uint16_t word = ((Year(T)-2000)<<9) | Month(T)<<5 | Day(T);
	
	e->bytearr[0x18] = (word&0xff00) >> 8;
	e->bytearr[0x19] = (word&0x00ff);

	return;//TODO implementasi
}


void setLastModifDateTime(directory_entry* e, WAKTU W){
	//asumsi disimpan dalam big endian
	//TODO tanyakan lagi

	setLastModifDate(e,GetTanggal(W));
	setLastModifTime(e,GetJam(W));
	return;
}


void setFirstDataBlockIdx(directory_entry* e, uint16_t idx){
	e->bytearr[0x1A]=idx&0x00ff;
	e->bytearr[0x1B]=(idx&0xff00)>>8;
	return;
}


void setFileSize(directory_entry* e, uint32_t size){
	e->bytearr[0x1C]=(size&0x000000ff);
	e->bytearr[0x1D]=(size&0x0000ff00)>>8;
	e->bytearr[0x1E]=(size&0x00ff0000)>>16;
	e->bytearr[0x1F]=(size&0xff000000)>>24;
	return;
}

directory_entry makeEntry(const char* nama, poi_attr_t atribut, JAM waktu, TANGGAL tanggal, uint16_t idx, uint32_t size){
	directory_entry retval;
	setnama(&retval,nama);
	setattr(&retval,atribut);
	setLastModifTime(&retval,waktu);
	setLastModifDate(&retval,tanggal);
	setFirstDataBlockIdx(&retval,idx);
	setFileSize(&retval,size);
	return retval;
}


#endif
