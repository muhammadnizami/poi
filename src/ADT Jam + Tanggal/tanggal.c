#ifndef tanggal_c
#define tanggal_c
#include <stdio.h>
#include "tanggal.h"
/* *** DEFINISI DAN SPESIFIKASI SELEKTOR *** */
/* *** SELEKTOR GET *** */
unsigned char Day (TANGGAL T){
/* Memberikan hari DD dari T yang terdiri dari <DD, MM, YY> */
	return T.DD;
}
unsigned char Month (TANGGAL T){
/* Memberikan bulan MM dari T yang terdiri dari <DD, MM, YY> */
	return T.MM;
}
unsigned short Year (TANGGAL T){
/* Memberikan tahun YY dari T yang terdiri dari <DD, MM, YY> */
	return T.YY;
}
/* *** SELEKTOR SET *** */
/* prekondisi pengubah nilai komponen: nilai baru valid */
void SetDay (TANGGAL * T, unsigned char newDay){
/* Mengubah nilai komponen DD dari T */
	T->DD=newDay;
}
void SetMonth (TANGGAL * T, unsigned char newMonth){
/* Mengubah nilai komponen MM dari T */
	T->MM=newMonth;
}
void SetYear (TANGGAL * T, unsigned short newYear){
/* Mengubah nilai komponen YY dari T */
	T->YY=newYear;
}
/* *** VALIDASI TERHADAP TYPE *** */
bool IsTanggalValid (unsigned char D, unsigned char M, unsigned short Y){
/* Mengirim true jika D, M, Y dapat membentuk TANGGAL yang valid */
/* dipakai untuk mengetes SEBELUM membentuk sebuah TANGGAL */
/* Perhatikan bahwa setiap bulan memiliki jumlah hari yang berbeda, dan
khusus bulan Februari bisa 28 atau 29, tergantung apakah tahunnya kabisat atau
bukan */
	bool kabisat;
	if (D==0||M==0||M>12) return false;
		
	switch (M) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return D<=31;
		case 4:
		case 6:
		case 9:
		case 11:
			return D<=30;
		case 2:
			if (Y%4!=0) kabisat=false;
			else if (Y%100!=0) kabisat=true;
			else if (Y%400!=0) kabisat=false;
	
			if (kabisat) return D<=29;
			if (kabisat) return D<=28;
	}
		
}
/* *** DEFINISI DAN SPESIFIKASI KONSTRUKTOR *** */
TANGGAL MakeTANGGAL (unsigned char h, unsigned char b, unsigned short t){
/* Membentuk TANGGAL dari tanggal h, bulan b, dan tahun t. */
/* Prekondisi : h, b, t dapat membentuk TANGGAL yang valid. */
	TANGGAL T;
	SetDay(&T,h);
	SetMonth(&T,b);
	SetYear(&T,t);
	return T;
}
/* *** DEFINISI DAN SPESIFIKASI OPERATOR BACA/TULIS *** */
void TulisTANGGAL (TANGGAL T){
/* I.S. T terdefinisi */
/* F.S. Nilai T ditulis ke layar dengan format DD/MM/YY */
/* Proses : Menulis nilai setiap komponen T ke layar */
	printf("%u/%u/%u",Day(T),Month(T),Year(T));
}

#endif
