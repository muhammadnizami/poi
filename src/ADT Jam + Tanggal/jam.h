#ifndef jam_h
#define jam_h

#include "stdbool.h"

/* *** Definisi TYPE JAM <HH:MM:SS> *** */
typedef struct {
	unsigned char HH; /* int [0..23] */
	unsigned char MM; /* int [0..59] */
	unsigned char SS; /* int [0..59] */
} JAM;
/* ******************************************************************/
/* DEFINISI PRIMITIF*/
/* ******************************************************************/
/* KELOMPOK VALIDASI TERHADAP TYPE*/
/* ******************************************************************/
bool IsJAMValid (int H, int M, int S);
/* Mengirim true jika H,M,S dapat membentuk J yang valid */
/* dipakai untuk mentest SEBELUM membentuk sebuah Jam */


/* *** Konstruktor: Membentuk sebuah JAM dari komponen-komponennya *** */
JAM MakeJAM (int HH, int MM, int SS);
/* Membentuk sebuah JAM dari komponen-komponennya yang valid */
/* Prekondisi : HH, MM, SS valid untuk membentuk JAM */
/* *** Operasi terhadap komponen : selektor Get dan Set *** */
/* *** Selektor *** */
int GetHour (JAM J);
/* Mengirimkan bagian jam (HH) dari JAM */
int GetMinute (JAM J);
/* Mengirimkan bagian menit (MM) dari JAM */
int GetSecond (JAM J);
/* Mengirimkan bagian detik (SS) dari JAM */
/* *** Pengubah nilai Komponen *** */
/* prekondisi pengubah nilai komponen: nilai baru valid */
void SetHH (JAM *  J, int newHH);
/* Mengubah nilai komponen jam (HH) dari J dengan newHH */
void SetMM (JAM *  J, int newMM);
/* Mengubah nilai komponen menit (MM) dari J dengan newMM */
void SetSS (JAM *  J, int newSS);
/* Mengubah nilai komponen detik (SS) dari J dengan newSS */
/* ***************************************************************** */
/* KELOMPOK BACA/TULIS
*/
/* ***************************************************************** */
void TulisJam (JAM J);
/* I.S. : J sembarang */
/* F.S. :
Nilai J ditulis dg format HH:MM:SS */
/* Proses : menulis nilai setiap komponen J ke layar */
/* ***************************************************************** */

#endif
