#ifndef waktu_c
#define waktu_c

#include <time.h>
#include "jam.h"
#include "tanggal.h"
#include <stdio.h>
#include "waktu.h"

TANGGAL GetTanggal(WAKTU W){
//mengembalikan W.TT
	return W.TT;
}
JAM GetJam(WAKTU W){
//mengembalikan W.JJ
	return W.JJ;
}

WAKTU MakeWAKTU (TANGGAL T, JAM J){
/*prekondisi: JAM dan TANGGAL terdefinisi*/
/*mengirim WAKTU dengan TANGGAL T dan JAM J;*/
	WAKTU W;
	W.TT=T;
	W.JJ=J;
	return W;
}

WAKTU GetCurrentTime(){
/*mengembalikan waktu sekarang*/
	time_t T_t;
	struct tm * TM_t;
	JAM J;
	TANGGAL T;
	time(&T_t);
	TM_t=localtime(&T_t);
	J=MakeJAM(TM_t->tm_hour,TM_t->tm_min,TM_t->tm_sec);
	T=MakeTANGGAL(TM_t->tm_mday,TM_t->tm_mon,TM_t->tm_year+1900);
	return MakeWAKTU(T,J);
}

void TulisWAKTU(WAKTU W){
/*I.S. W terdefinisi*/
/*F.S. W ditulis ke layar dengan format tanggal diikuti jam*/
	TulisTANGGAL(GetTanggal(W));
	printf(" ");
	TulisJam(GetJam(W));
}

time_t totime_t(WAKTU W){
//mengubah format ke time_t
	time_t T_t;
	struct tm TM_t;
	JAM J = GetJam(W);
	TANGGAL T = GetTanggal(W);

	TM_t.tm_hour=GetHour(J);
	TM_t.tm_min=GetMinute(J);
	TM_t.tm_sec=GetSecond(J);

	TM_t.tm_mday=Day(T);
	TM_t.tm_mon=Month(T);
	TM_t.tm_year=Year(T)-1900;

	return mktime(&TM_t);
}

#endif
