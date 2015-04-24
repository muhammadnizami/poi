#ifndef waktu_h
#define waktu_h

#include <time.h>
#include "jam.h"
#include "tanggal.h"

typedef struct {
	TANGGAL TT;
	JAM JJ;
}WAKTU;

TANGGAL GetTanggal(WAKTU W);
//mengembalikan W.TT

JAM GetJam(WAKTU W);
//mengembalikan W.JJ

WAKTU MakeWAKTU (TANGGAL T, JAM J);
/*prekondisi: JAM dan TANGGAL terdefinisi*/
/*mengirim WAKTU dengan TANGGAL T dan JAM J;*/

WAKTU GetCurrentTime();
/*mengembalikan waktu sekarang*/

void TulisWAKTU(WAKTU W);
/*I.S. W terdefinisi*/
/*F.S. W ditulis ke layar dengan format tanggal diikuti jam*/


#endif
