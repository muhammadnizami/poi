#include "directory-entry.h"

#include <stdio.h>

int main(){
	WAKTU w = GetCurrentTime();
	poi_attr_t atribut = {
		1,
		0,
		0,
		1
	};
	directory_entry e = makeEntry("test",atribut, GetJam(w), GetTanggal(w),0x1234,0x12345678);
	char buf[21];
	poi_attr_t t = getattr(e);
	printf("name: %s\nx:%d\nw:%d\nr:%d\nd:%d\n",getNama(buf,e),t.x,t.w,t.r,t.d);
	printf("Last Modified: "); TulisWAKTU(getLastModifDateTime(e));printf("\n");
	printf("First Idx: 0x%x\nSize: 0x%x\n",getFirstDataBlockIdx(e),getFileSize(e));

}
