#ifndef tanggal_h
#define tanggal_h

#include "stdbool.h"

/* *** DEFINISI DAN SPESIFIKASI TYPE TANGGAL *** */
typedef struct {
	unsigned char DD;/* hari */
	unsigned char MM;/* bulan */
	unsigned short YY;/* tahun */
} TANGGAL;
/* *** DEFINISI DAN SPESIFIKASI SELEKTOR *** */
/* *** SELEKTOR GET *** */
unsigned char Day (TANGGAL T);
/* Memberikan hari DD dari T yang terdiri dari <DD, MM, YY> */
unsigned char Month (TANGGAL T);
/* Memberikan bulan MM dari T yang terdiri dari <DD, MM, YY> */
unsigned short Year (TANGGAL T);
/* Memberikan tahun YY dari T yang terdiri dari <DD, MM, YY> */
/* *** SELEKTOR SET *** */
/* prekondisi pengubah nilai komponen: nilai baru valid */
void SetDay (TANGGAL * T, unsigned char newDay);
/* Mengubah nilai komponen DD dari T */
void SetMonth (TANGGAL * T, unsigned char newMonth);
/* Mengubah nilai komponen MM dari T */
void SetYear (TANGGAL * T, unsigned short newYear);
/* Mengubah nilai komponen YY dari T */
/* *** VALIDASI TERHADAP TYPE *** */
bool IsTanggalValid (unsigned char D, unsigned char M, unsigned short Y);
/* Mengirim true jika D, M, Y dapat membentuk TANGGAL yang valid */
/* dipakai untuk mengetes SEBELUM membentuk sebuah TANGGAL */
/* Perhatikan bahwa setiap bulan memiliki jumlah hari yang berbeda, dan
khusus bulan Februari bisa 28 atau 29, tergantung apakah tahunnya kabisat atau
bukan */
/* *** DEFINISI DAN SPESIFIKASI KONSTRUKTOR *** */
TANGGAL MakeTANGGAL (unsigned char h, unsigned char b, unsigned short t);
/* Membentuk TANGGAL dari tanggal h, bulan b, dan tahun t. */
/* Prekondisi : h, b, t dapat membentuk TANGGAL yang valid. */
/* *** DEFINISI DAN SPESIFIKASI OPERATOR BACA/TULIS *** */
void TulisTANGGAL (TANGGAL T);
/* I.S. T terdefinisi */
/* F.S. Nilai T ditulis ke layar dengan format DD/MM/YY */
/* Proses : Menulis nilai setiap komponen T ke layar */

#endif
