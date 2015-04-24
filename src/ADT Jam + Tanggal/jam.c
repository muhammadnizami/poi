#ifndef jam_c
#define jam_c
#include <stdio.h>
#include "jam.h"
/* *** Definisi TYPE JAM <HH:MM:SS> *** */
/* ******************************************************************/
/* DEFINISI PRIMITIF*/
/* ******************************************************************/
/* KELOMPOK VALIDASI TERHADAP TYPE*/
/* ******************************************************************/
bool IsJAMValid (int H, int M, int S){
/* Mengirim true jika H,M,S dapat membentuk J yang valid */
/* dipakai untuk mentest SEBELUM membentuk sebuah Jam */
	return 0<=H && H<= 23 && M<=0 && M<=59 && S<=0 && M<=59;
}
/* *** Konstruktor: Membentuk sebuah JAM dari komponen-komponennya *** */
JAM MakeJAM (int HH, int MM, int SS){
/* Membentuk sebuah JAM dari komponen-komponennya yang valid */
/* Prekondisi : HH, MM, SS valid untuk membentuk JAM */
	JAM J;
	J.HH=HH;
	J.MM=MM;
	J.SS=SS;
	return J;
}
/* *** Operasi terhadap komponen : selektor Get dan Set *** */
/* *** Selektor *** */
int GetHour (JAM J){
/* Mengirimkan bagian jam (HH) dari JAM */
	return J.HH;
}
int GetMinute (JAM J){
/* Mengirimkan bagian menit (MM) dari JAM */
	return J.MM;
}
int GetSecond (JAM J){
/* Mengirimkan bagian detik (SS) dari JAM */
	return J.SS;
}
/* *** Pengubah nilai Komponen *** */
/* prekondisi pengubah nilai komponen: nilai baru valid */
void SetHH (JAM *  J, int newHH){
/* Mengubah nilai komponen jam (HH) dari J dengan newHH */
		J->HH=newHH;
}
void SetMM (JAM *  J, int newMM){
/* Mengubah nilai komponen menit (MM) dari J dengan newMM */
		J->MM=newMM;
}
void SetSS (JAM *  J, int newSS){
/* Mengubah nilai komponen detik (SS) dari J dengan newSS */
		J->SS=newSS;
}
/* ***************************************************************** */
/* KELOMPOK BACA/TULIS
*/
/* ***************************************************************** */
void TulisJam (JAM J){
/* I.S. : J sembarang */
/* F.S. :
Nilai J ditulis dg format HH:MM:SS */
/* Proses : menulis nilai setiap komponen J ke layar */
	printf("%02d:%02d:%02d",GetHour(J),GetMinute(J),GetSecond(J));
}
/* ***************************************************************** */

#endif
