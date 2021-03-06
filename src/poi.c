/**
@file poi.c
@author jang berikoetnja
@brief berisi implementasi inerface ke fuse dan main program

*/

#include <fuse.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "allocation-table.h"
#include "file-manager.h"
#include "directory-entry.h"
#include "volume-information.h"
#include "data-pool-block-manager.h"

//NLOG bila tidak mencatat ke berkas log. LOG bila mencatat.
//
#define NLOG 

#ifdef LOG
FILE * logfile;
#endif

int replaceEntry(directory_entry e, uint16_t dataBlockIdx, uint32_t offset){
	poi_file_block blk=poi_data_pool_read_block(dataBlockIdx);
	memcpy(blk.data+offset,e.bytearr,32);
	int ret = poi_data_pool_write_block(blk,dataBlockIdx);
	return (ret<0)?ret:0;
}
int getEntryAndBlockOffset(const char * path, directory_entry r, directory_entry * getout, uint16_t * dataBlockIdx, uint32_t * offset){
	char buf[22];

	if (!strcmp(path,"/")){*getout = r; return 0;}
	int name_length = 0;
	char name[22];
	while (path[name_length+1]!='/' && path[name_length+1]!='\0'){
		if (name_length>=21) return -ENOENT;
		name[name_length]=path[name_length+1];
		name_length++;
	}
	name[name_length]='\0';
	
	if (name_length==0 || path[0]=='\0') {*getout = r; return 0;}

	*dataBlockIdx = getFirstDataBlockIdx(r);
	uint32_t rsize = getFileSize(r);

	uint32_t totalread = 0;
	*offset = 0;
	char namaEntri[21];
	directory_entry tmp;

	poi_file_block blk = poi_data_pool_read_block(*dataBlockIdx);

	while (totalread < rsize){
		memcpy(tmp.bytearr,&blk.data[*offset],32);
		if (!strncmp(name,getNama(namaEntri,tmp),21))
			return getEntryAndBlockOffset(path+name_length+1,tmp,getout,dataBlockIdx,offset);
		*offset += 32;
		totalread += 32;
		if (*offset>=POI_BLOCK_SIZE){
			*offset=0;
			*dataBlockIdx=getNextBlock(*dataBlockIdx);
			blk = poi_data_pool_read_block(*dataBlockIdx);
		}
	}
	return -ENOENT;
}

int getEntryRecursive(const char * path, directory_entry r, directory_entry * getout){
	char buf[22];

	if (!strcmp(path,"/")){*getout = r; return 0;}
	int name_length = 0;
	char name[22];
	while (path[name_length+1]!='/' && path[name_length+1]!='\0'){
		if (name_length>=21) return -ENOENT;
		name[name_length]=path[name_length+1];
		name_length++;
	}
	name[name_length]='\0';
	
	if (name_length==0 || path[0]=='\0') {*getout = r; return 0;}

	uint16_t dataBlockIdx = getFirstDataBlockIdx(r);
	uint32_t rsize = getFileSize(r);

	uint32_t totalread = 0;
	uint32_t offset = 0;
	char namaEntri[21];
	directory_entry tmp;

	poi_file_block blk = poi_data_pool_read_block(dataBlockIdx);

	while (totalread < rsize){
		memcpy(tmp.bytearr,&blk.data[offset],32);
		if (!strncmp(name,getNama(namaEntri,tmp),21))
			return getEntryRecursive(path+name_length+1,tmp,getout);
		offset += 32;
		totalread += 32;
		if (offset>=POI_BLOCK_SIZE){
			offset=0;
			dataBlockIdx=getNextBlock(dataBlockIdx);
			blk = poi_data_pool_read_block(dataBlockIdx);
		}
	}

	return -ENOENT;
	//WOLOLO
	
}

int directory_entry_to_struct_stat(directory_entry e, struct stat * statbuf){
        memset(statbuf, 0, sizeof(struct stat));
	statbuf->st_mode=0;
	poi_attr_t attr=getattr(e);
	if (attr.x)
		statbuf->st_mode = statbuf->st_mode | S_IEXEC | S_IXGRP | S_IXOTH;
	if (attr.r)
		statbuf->st_mode = statbuf->st_mode | S_IREAD | S_IRGRP ;//| S_IROTH;
	if (attr.w)
		statbuf->st_mode = statbuf->st_mode | S_IWRITE | S_IWGRP ;//| S_IWOTH;
	if (attr.d)
		statbuf->st_mode = statbuf->st_mode | S_IFDIR;
	else	statbuf->st_mode = statbuf->st_mode | S_IFREG;
	statbuf->st_blocks = (getFileSize(e)==0)?1:(((unsigned long)getFileSize(e)+POI_BLOCK_SIZE-1)/POI_BLOCK_SIZE);
	statbuf->st_blksize = POI_BLOCK_SIZE;
	statbuf->st_uid = getuid();
	statbuf->st_gid = getgid();
	statbuf->st_size = getFileSize(e);
	statbuf->st_mtime = getLastModifTimetime_t(e);
	statbuf->st_atime = getLastModifTimetime_t(e);
	statbuf->st_ctime = getLastModifTimetime_t(e);
	return 0;
}
///////////////////////////////////////////////////////////
//
// Prototypes for all these functions, and the C-style comments,
// come indirectly from /usr/include/fuse.h
//
/** Get file attributes.
 *
 * Similar to stat().  The 'st_dev' and 'st_blksize' fields are
 * ignored.  The 'st_ino' field is ignored except if the 'use_ino'
 * mount option is given.
 */
int poi_getattr(const char *path, struct stat *statbuf)
{
	#ifdef LOG
	fprintf(logfile,"getattr('%s')\n",path);
	#endif
        memset(statbuf, 0, sizeof(struct stat));
	FILE * f = fopen("test","r+");
	directory_entry e;
	int a;
	if (!strcmp(path,"/")){
		e = getRootDirEntry();
	}
	else{
		a = getEntryRecursive(path,getRootDirEntry(),&e);
		if (a!=0) return a;
		//cek lagi nanti
	}
	char buf[22];
	return directory_entry_to_struct_stat(e,statbuf);//implementasi
}

/** Read directory
 *
 * This supersedes the old getdir() interface.  New applications
 * should use this.
 *
 * The filesystem may choose between two modes of operation:
 *
 * 1) The readdir implementation ignores the offset parameter, and
 * passes zero to the filler function's offset.  The filler
 * function will not return '1' (unless an error happens), so the
 * whole directory is read in a single readdir operation.  This
 * works just like the old getdir() method.
 *
 * 2) The readdir implementation keeps track of the offsets of the
 * directory entries.  It uses the offset parameter and always
 * passes non-zero offset to the filler function.  When the buffer
 * is full (or an error happens) the filler function will return
 * '1'.
 *
 * Introduced in version 2.3
 */
int poi_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	#ifdef LOG
(logfile,"readdir('%s')\n",path);
	#endif
	directory_entry e;
	int a=getEntryRecursive(path,getRootDirEntry(),&e);
	if (a!=0) return a;

	uint16_t dataBlockIdx = getFirstDataBlockIdx(e);
	uint32_t rsize = getFileSize(e);

	uint32_t totalread = 0;
	uint32_t offsetread = 0;
	char namaEntri[21];
	directory_entry tmp;

	poi_file_block blk = poi_data_pool_read_block(dataBlockIdx);


	while (totalread < rsize){
		memcpy(tmp.bytearr,&blk.data[offsetread],32);
		filler(buf,getNama(namaEntri,tmp),NULL,0);
		offsetread += 32;
		totalread += 32;
		if (offsetread>=POI_BLOCK_SIZE){
			
			offsetread=0;
			dataBlockIdx=getNextBlock(dataBlockIdx);
			blk = poi_data_pool_read_block(dataBlockIdx);

		}
	}
	return a;//implementasi
}

int poi_insertentry(directory_entry * dst, directory_entry val){
	char buf1[22];
	char buf2[22];
	poi_attr_t attr = getattr(*dst);
	if (!(attr.w)) return -EPERM;
	if (!(attr.d)) return -ENOTDIR;
		
	int op_status;

	uint16_t dataBlockIdx = getFirstDataBlockIdx(*dst);
	uint16_t prev;
	uint32_t rsize = getFileSize(*dst);

	uint32_t totalread = 0;
	uint32_t offset = 0;

	poi_file_block blk = poi_data_pool_read_block(dataBlockIdx);

	while (totalread < rsize){
		offset += 32;
		totalread += 32;
		if (offset>=POI_BLOCK_SIZE){
			offset=0;
			prev=dataBlockIdx;
			dataBlockIdx=getNextBlock(dataBlockIdx);
			blk = poi_data_pool_read_block(dataBlockIdx);
		}
	}

	if (dataBlockIdx==0xffff){
		if (getNumFreeBlocks()<1) return -ENOSPC;

		allocateAfter(prev);
		dataBlockIdx=getNextBlock(prev);

		blk = poi_data_pool_read_block(dataBlockIdx);
		memcpy(blk.data,val.bytearr,32);
		op_status = poi_data_pool_write_block(blk,dataBlockIdx);
		if (op_status<0) return op_status;
		setFileSize(dst,getFileSize(*dst)+32);
		saveAllocTableCaches();
		
	}else{
		blk=poi_data_pool_read_block(dataBlockIdx);
		memcpy(blk.data+offset,val.bytearr,32);
		op_status = poi_data_pool_write_block(blk,dataBlockIdx);
		setFileSize(dst,getFileSize(*dst)+32);
		if (op_status<0) return op_status;
	}
	setLastModifDateTime(dst,GetCurrentTime());
	return 0;

}

poi_attr_t mode_t_to_poi_attr_t(mode_t mode){
	poi_attr_t attr;
	if (mode & (S_IEXEC | S_IXGRP | S_IXOTH))
		attr.x = true;
	else	attr.x = false;
	if (mode & (S_IREAD | S_IRGRP | S_IROTH))
		attr.r = true;
	else	attr.r = false;
	if (mode & (S_IWRITE | S_IWGRP | S_IWOTH))
		attr.w = true;
	else	attr.w = false;
	if (mode & S_IFDIR)
		attr.d = true;
	else	attr.d = false;
	return attr;
}

/** Create a directory 
 *
 * Note that the mode argument may not have the type specification
 * bits set, i.e. S_ISDIR(mode) can be false.  To obtain the
 * correct directory type bits use  mode|S_IFDIR
 * */

int poi_mkdir (const char * path, mode_t mode){
	#ifdef LOG
(logfile,"mkdir('%s')\n",path);
	#endif
	directory_entry yangdimasuki;
	if (getEntryRecursive(path,getRootDirEntry(),&yangdimasuki)==0) return -EEXIST;
	mode = mode|S_IFDIR;
	int path_length=strlen(path);
	int n = path_length;
	while (path[n]!='/') n--;

	int status_op;

	poi_attr_t attr = mode_t_to_poi_attr_t(mode);
	WAKTU waktu_buat = GetCurrentTime();
	if (strlen(path+n+1)>21) return -ENAMETOOLONG;
	poi_data_pool_block_idx_t firstblock;
	if (status_op = newList(&firstblock)< 0) return status_op;
	directory_entry yangdimasukkan = makeEntry(path+n+1, attr, GetJam(waktu_buat), GetTanggal(waktu_buat), firstblock, 0);
	char * parentDir;

	uint32_t offset;
	poi_data_pool_block_idx_t dataBlockIdx;

	poi_file_block blk;

	if (n==0){ //root
		yangdimasuki=getRootDirEntry();
		status_op=poi_insertentry(&yangdimasuki,yangdimasukkan);
		if (status_op!=0) return status_op;
		setRootDirEntry(yangdimasuki);
		savePoiVolinfoCache();
		return 0;
	}else{
		parentDir = malloc(sizeof(char)*n);
		if (parentDir==NULL) return -ENAMETOOLONG;
		memcpy(parentDir,path,sizeof(char)*n); parentDir[n]='\0';
		status_op = getEntryAndBlockOffset(parentDir,getRootDirEntry(),&yangdimasuki,&dataBlockIdx,&offset);
		if (status_op!=0) return status_op;
		status_op = poi_insertentry(&yangdimasuki,yangdimasukkan);
		if (status_op!=0) return status_op;

		//memperbarui entri direktori induknya
		blk=poi_data_pool_read_block(dataBlockIdx);
		memcpy(blk.data+offset,yangdimasuki.bytearr,32);
		poi_data_pool_write_block(blk,dataBlockIdx);
		free(parentDir);
		return 0;
	}
	return -EPERM;//implementasi
}



/** Create a file node
 *
 * This is called for creation of all non-directory, non-symlink
 * nodes.  If the filesystem defines a create() method, then for
 * regular files that will be called instead.
 */
int poi_mknod (const char *path, mode_t mode, dev_t dev){
	#ifdef LOG
(logfile,"mknod('%s')\n",path);
	#endif
	directory_entry yangdimasuki;
	if (getEntryRecursive(path,getRootDirEntry(),&yangdimasuki)==0) return -EEXIST;
	int path_length=strlen(path);
	int n = path_length;
	while (path[n]!='/') n--;

	int status_op;

	poi_attr_t attr = mode_t_to_poi_attr_t(mode);
	WAKTU waktu_buat = GetCurrentTime();
	if (strlen(path+n+1)>21) return -ENAMETOOLONG;
	poi_data_pool_block_idx_t firstblock;
	if (status_op = newList(&firstblock)< 0) return status_op;
	directory_entry yangdimasukkan = makeEntry(path+n+1, attr, GetJam(waktu_buat), GetTanggal(waktu_buat), firstblock, 0);
	char * parentDir;

	uint32_t offset;
	poi_data_pool_block_idx_t dataBlockIdx;

	poi_file_block blk;

	if (n==0){
		yangdimasuki=getRootDirEntry();
		status_op=poi_insertentry(&yangdimasuki,yangdimasukkan);
		if (status_op!=0) return status_op;
		setRootDirEntry(yangdimasuki);
		savePoiVolinfoCache();
		return 0;
	}else{
		parentDir = malloc(sizeof(char)*n);
		if (parentDir==NULL) return -ENAMETOOLONG;
		memcpy(parentDir,path,sizeof(char)*n); parentDir[n]='\0';
		status_op = getEntryAndBlockOffset(parentDir,getRootDirEntry(),&yangdimasuki,&dataBlockIdx,&offset);
		if (status_op!=0) return status_op;
		status_op = poi_insertentry(&yangdimasuki,yangdimasukkan);
		if (status_op!=0) return status_op;

		//memperbarui entri direktori induknya
		blk=poi_data_pool_read_block(dataBlockIdx);
		memcpy(blk.data+offset,yangdimasuki.bytearr,32);
		poi_data_pool_write_block(blk,dataBlockIdx);
		free(parentDir);
		return 0;
	}
	return -EPERM;//implementasi
		//dev_t
}

/** Read data from an open file
 *
 * Read should return exactly the number of bytes requested except
 * on EOF or error, otherwise the rest of the data will be
 * substituted with zeroes.	 An exception to this is when the
 * 'direct_io' mount option is specified, in which case the return
 * value of the read system call will reflect the return value of
 * this operation.
 *
 * Changed in version 2.2
 */
 poi_read (const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	#ifdef LOG
(logfile,"read('%s')\n",path);
	#endif

	directory_entry e;
	int opstat = getEntryRecursive(path,getRootDirEntry(),&e);
	if (opstat!=0) return opstat;

	uint16_t dataBlockIdx = getFirstDataBlockIdx(e);
	uint16_t prev;
	uint32_t fsize = getFileSize(e);

	uint32_t totalskipped = 0;


	while (totalskipped < (long)offset-(long)POI_BLOCK_SIZE && totalskipped < fsize){
		totalskipped += POI_BLOCK_SIZE;
		dataBlockIdx=getNextBlock(dataBlockIdx);
	}

	uint32_t in_block_offset = offset-totalskipped;
	totalskipped = offset;

	uint32_t totalread=0;

	poi_file_block blk;
	
	uint32_t mustbereadsize = (fsize-totalskipped<size)?fsize-totalskipped:size;
	while (totalread<mustbereadsize){
		blk = poi_data_pool_read_block(dataBlockIdx);

		if (in_block_offset+(mustbereadsize-totalread)>POI_BLOCK_SIZE){
			memcpy(buf+totalread,blk.data+in_block_offset,POI_BLOCK_SIZE-in_block_offset);
			totalread+=POI_BLOCK_SIZE-in_block_offset;

		}else{
			memcpy(buf+totalread,blk.data+in_block_offset,mustbereadsize-totalread);
			totalread=mustbereadsize;

		}
		dataBlockIdx=getNextBlock(dataBlockIdx);
		in_block_offset=0;
	}

	if (totalread<size) memset(buf+totalread,0x00,size-totalread);
	return totalread;
	}

/** Write data to an open file
 *
 * Write should return exactly the number of bytes requested
 * except on error.	 An exception to this is when the 'direct_io'
 * mount option is specified (see read operation).
 *
 * Changed in version 2.2
 */
int poi_write (const char *path, const char *buf, size_t size, off_t offset,
	     struct fuse_file_info *fi){

	#ifdef LOG
(logfile,"write('%s'). size: 0x%x\n",path,size);
	#endif

	uint16_t entryblockidx;
	uint32_t entryblockoff;
	directory_entry e;
	int opstat = getEntryAndBlockOffset(path,getRootDirEntry(),&e,&entryblockidx,&entryblockoff);
	if (opstat!=0) return opstat;

	uint16_t dataBlockIdx = getFirstDataBlockIdx(e);
	uint16_t prev;
	uint32_t fsize = getFileSize(e);

	uint32_t totalskipped = 0;


	while (totalskipped < (long)offset-(long)POI_BLOCK_SIZE && totalskipped < fsize){
		totalskipped += POI_BLOCK_SIZE;
		dataBlockIdx=getNextBlock(dataBlockIdx);
	}

	uint32_t in_block_offset = offset-totalskipped;
	totalskipped = offset;

	uint32_t totalwritten=0;

	poi_file_block blk;
	
	uint32_t mustbereplacedsize = (fsize-totalskipped<size)?fsize-totalskipped:size;
	while (totalwritten<mustbereplacedsize){

		if (in_block_offset>=POI_BLOCK_SIZE){
			dataBlockIdx=getNextBlock(dataBlockIdx);
			in_block_offset=0;
		}
		blk = poi_data_pool_read_block(dataBlockIdx);
		if (in_block_offset+(mustbereplacedsize-totalwritten)>POI_BLOCK_SIZE){
			memcpy(blk.data+in_block_offset,buf+totalwritten,POI_BLOCK_SIZE-in_block_offset);
			totalwritten+=POI_BLOCK_SIZE-in_block_offset;
			in_block_offset=POI_BLOCK_SIZE;
			
		}else{
			memcpy(blk.data+in_block_offset,buf+totalwritten,mustbereplacedsize-totalwritten);
			in_block_offset+=mustbereplacedsize-totalwritten;
			totalwritten=mustbereplacedsize;
			
		}
		opstat = poi_data_pool_write_block(blk,dataBlockIdx);
		if (opstat<0) return opstat;

	}
	while (totalwritten<size){

		if (in_block_offset>=POI_BLOCK_SIZE){
			int opstat = allocateAfter(dataBlockIdx);
			if (opstat<0) return opstat;
			dataBlockIdx=getNextBlock(dataBlockIdx);
			in_block_offset=0;
			if (dataBlockIdx==0xFFFF){
				opstat=-ENOSPC;
				break;
			}
		}
		blk = poi_data_pool_read_block(dataBlockIdx);
		if (in_block_offset+(size-totalwritten)>POI_BLOCK_SIZE){
			memcpy(blk.data+in_block_offset,buf+totalwritten,POI_BLOCK_SIZE-in_block_offset);
			totalwritten+=POI_BLOCK_SIZE-in_block_offset;
			in_block_offset=POI_BLOCK_SIZE;
			
		}else{
			memcpy(blk.data+in_block_offset,buf+totalwritten,size-totalwritten);
			in_block_offset+=size-totalwritten;
			totalwritten=size;
			
		}
		opstat = poi_data_pool_write_block(blk,dataBlockIdx);
		if (opstat<0) return opstat;		
	}

	//update file entry
	setFileSize(&e,totalskipped+totalwritten);
	setLastModifDateTime(&e,GetCurrentTime());
	replaceEntry(e,entryblockidx,entryblockoff);

	//simpan yang diubah
	savePoiVolinfoCache();
	savePoiAllocationCache();
	return totalwritten;


	return -ENOSYS;//implementasi
}

int deleteEntry(poi_data_pool_block_idx_t dataBlockIdx, uint32_t offset){
	#ifdef LOG
(logfile,"\tdeleteEntry(0x%x,0x%x)\n",dataBlockIdx,offset);
	#endif

	poi_data_pool_block_idx_t dataBlockIdxNxt = dataBlockIdx;
	poi_file_block blk,blknxt;
	blknxt=poi_data_pool_read_block(dataBlockIdx);
	do{
		dataBlockIdx = dataBlockIdxNxt;
		blk = blknxt;
		while (offset+32<POI_BLOCK_SIZE){
			memcpy(&blk.data[offset],&blk.data[offset+32],32);
			offset += 32;
		}
		dataBlockIdxNxt=getNextBlock(dataBlockIdx);
		if (dataBlockIdxNxt!=0xffff){
			blknxt = poi_data_pool_read_block(dataBlockIdxNxt);
			memcpy(&blk.data[offset],&blknxt.data,32);
			offset=0;
		}
		poi_data_pool_write_block(blk,dataBlockIdx);

	}while(dataBlockIdx!=0xffff);
	return 0;
}




/** Change the size of a file */
int poi_truncate (const char * path, off_t offset){

	#ifdef LOG
(logfile,"truncate('%s'). offset: 0x%x\n",path,offset);
	#endif

	uint16_t entryblockidx;
	uint32_t entryblockoff;
	directory_entry e;
	int opstat = getEntryAndBlockOffset(path,getRootDirEntry(),&e,&entryblockidx,&entryblockoff);
	if (opstat!=0) return opstat;

	uint16_t dataBlockIdx = getFirstDataBlockIdx(e);
	uint16_t prev;
	uint32_t fsize = getFileSize(e);

	uint32_t totalskipped = 0;


	while (totalskipped < (long)offset-(long)POI_BLOCK_SIZE && totalskipped < fsize){
		totalskipped += POI_BLOCK_SIZE;
		dataBlockIdx=getNextBlock(dataBlockIdx);
	}

	if (totalskipped==fsize){
		uint32_t totalBlock = (offset + POI_BLOCK_SIZE - 1) / POI_BLOCK_SIZE;//pembagian roundup
		uint32_t allocatedBlock = (fsize + POI_BLOCK_SIZE - 1) / POI_BLOCK_SIZE;//pembagian roundup
		if (totalBlock-allocatedBlock > getNumFreeBlocks())
			return -ENOSPC;
		while (allocatedBlock<totalBlock){
			allocateAfter(dataBlockIdx);
			dataBlockIdx = getNextBlock(dataBlockIdx);
			allocatedBlock++;
		}
		
	}
	else{

		uint32_t in_block_offset = offset-totalskipped;
		totalskipped = offset;

		truncateList(dataBlockIdx);
	}

	//update file entry
	setFileSize(&e,offset);
	setLastModifDateTime(&e,GetCurrentTime());
	replaceEntry(e,entryblockidx,entryblockoff);

	//simpan yang diubah
	savePoiVolinfoCache();
	savePoiAllocationCache();
	return 0;

}


/** Rename a file */
int poi_rename (const char * path, const char * newpath){
	#ifdef LOG
(logfile,"poi_rename %s ke %s\n", path, newpath);
	#endif

	if (!strcmp(path,newpath))return 0;
	int path_length=strlen(path);
	int n = path_length;
	while (path[n]!='/') n--;

	int newpath_length=strlen(newpath);
	int newn = newpath_length;
	while (newpath[newn]!='/') newn--;

	const char * name = path+n+1;
	const char * newname = newpath+newn+1;
	if (strlen(name)>21) return -ENOENT;
	if (strlen(newname)>21) return -ENAMETOOLONG;
	directory_entry buf;
	if (getEntryRecursive(newpath,getRootDirEntry(),&buf)>=0) return -EEXIST;

	directory_entry workentr;

	//mengambil entri
	poi_data_pool_block_idx_t yangdihapusidx;
	uint32_t yangdihapusoff;
	int opstat;
	if (opstat=getEntryAndBlockOffset(path,getRootDirEntry(),&workentr,&yangdihapusidx,&yangdihapusoff)<0) return opstat;

	#ifdef LOG
(logfile,"\tcheck\n");
	#endif

	//menghapus dari tempat semula
	directory_entry yangdimasuki;
	if (n==0){//root
		yangdimasuki=getRootDirEntry();
		deleteEntry(yangdihapusidx,yangdihapusoff);
		setFileSize(&yangdimasuki,getFileSize(yangdimasuki)-32);
		if (getFileSize(yangdimasuki)%POI_BLOCK_SIZE==0 && getFileSize(yangdimasuki) !=0){
			poi_data_pool_block_idx_t iter;
			for (iter=getFirstDataBlockIdx(yangdimasuki);getNextBlock(getNextBlock(iter))!=0xFFFF;iter=getNextBlock(iter)){}
			truncateList(iter);
		}
		setRootDirEntry(yangdimasuki);
		saveAllocTableCaches();
	}else{
		char * parentDir = malloc(sizeof(char)*n);
		if (parentDir==NULL) return -ENAMETOOLONG;
		memcpy(parentDir,path,sizeof(char)*n); parentDir[n]='\0';
		uint32_t offset;
		poi_data_pool_block_idx_t dataBlockIdx;
		int status_op = getEntryAndBlockOffset(parentDir,getRootDirEntry(),&yangdimasuki,&dataBlockIdx,&offset);
		if (status_op!=0) return status_op;

		deleteEntry(yangdihapusidx,yangdihapusoff);
		setFileSize(&yangdimasuki,getFileSize(yangdimasuki)-32);
		if (getFileSize(yangdimasuki)%POI_BLOCK_SIZE==0 && getFileSize(yangdimasuki) !=0){
			poi_data_pool_block_idx_t iter;
			for (iter=getFirstDataBlockIdx(yangdimasuki);getNextBlock(getNextBlock(iter))!=0xFFFF;iter=getNextBlock(iter)){}
			truncateList(iter);
		}

		//memperbarui entri direktori induknya
		poi_file_block blk=poi_data_pool_read_block(dataBlockIdx);
		memcpy(blk.data+offset,yangdimasuki.bytearr,32);
		poi_data_pool_write_block(blk,dataBlockIdx);
		free(parentDir);
		saveAllocTableCaches();
	}

	//mengubah nama entri
	setNama(&workentr,newname);


	//memasukkan entri


	if (newn==0){
		yangdimasuki=getRootDirEntry();
		int status_op=poi_insertentry(&yangdimasuki,workentr);
		if (status_op!=0) return status_op;
		setRootDirEntry(yangdimasuki);
		savePoiVolinfoCache();
		return 0;
	}else{
		poi_data_pool_block_idx_t dataBlockIdx;
		uint32_t offset;
		char * parentDir = malloc(sizeof(char)*newn);
		if (parentDir==NULL) return -ENAMETOOLONG;
		memcpy(parentDir,newpath,sizeof(char)*newn); parentDir[newn]='\0';
		int status_op = getEntryAndBlockOffset(parentDir,getRootDirEntry(),&yangdimasuki,&dataBlockIdx,&offset);
		if (status_op!=0) return status_op;
		status_op = poi_insertentry(&yangdimasuki,workentr);
		if (status_op!=0) return status_op;

		//memperbarui entri direktori induknya
		poi_file_block blk=poi_data_pool_read_block(dataBlockIdx);
		memcpy(blk.data+offset,yangdimasuki.bytearr,32);
		poi_data_pool_write_block(blk,dataBlockIdx);
		free(parentDir);
		return 0;
	}

	return ;//implementasi
}

/** Remove a file */
int poi_unlink (const char * path){
	directory_entry yangdimasuki;

	directory_entry yangdihapus;
	uint32_t yangdihapusoff;
	poi_data_pool_block_idx_t yangdihapusidx;



	#ifdef LOG
(logfile,"unlink('%s')\n",path);
	#endif

	int opstat;
	if (opstat=getEntryAndBlockOffset(path,getRootDirEntry(),&yangdihapus,&yangdihapusidx,&yangdihapusoff)<0) return opstat;
	int path_length=strlen(path);
	int n = path_length;
	while (path[n]!='/') n--;

	int status_op;
//	if(status_op=poi_truncate(path,0)<0) return status_op;
	deleteListOfBlock(getFirstDataBlockIdx(yangdihapus));

	if (strlen(path+n+1)>21) return -ENOENT;

	const char * name = path+n+1;
	
	char * parentDir;

	if (n==0){//root
		yangdimasuki=getRootDirEntry();
		deleteEntry(yangdihapusidx,yangdihapusoff);
		setFileSize(&yangdimasuki,getFileSize(yangdimasuki)-32);
		if (getFileSize(yangdimasuki)%POI_BLOCK_SIZE==0 && getFileSize(yangdimasuki) !=0){
			poi_data_pool_block_idx_t iter;
			for (iter=getFirstDataBlockIdx(yangdimasuki);getNextBlock(getNextBlock(iter))!=0xFFFF;iter=getNextBlock(iter)){}
			truncateList(iter);
		}
		setRootDirEntry(yangdimasuki);
		saveAllocTableCaches();
		return 0;
	}else{
		parentDir = malloc(sizeof(char)*n);
		if (parentDir==NULL) return -ENAMETOOLONG;
		memcpy(parentDir,path,sizeof(char)*n); parentDir[n]='\0';
		uint32_t offset;
		poi_data_pool_block_idx_t dataBlockIdx;
		status_op = getEntryAndBlockOffset(parentDir,getRootDirEntry(),&yangdimasuki,&dataBlockIdx,&offset);
		if (status_op!=0) return status_op;



		deleteEntry(yangdihapusidx,yangdihapusoff);
		setFileSize(&yangdimasuki,getFileSize(yangdimasuki)-32);
		if (getFileSize(yangdimasuki)%POI_BLOCK_SIZE==0 && getFileSize(yangdimasuki) !=0){
			poi_data_pool_block_idx_t iter;
			for (iter=getFirstDataBlockIdx(yangdimasuki);getNextBlock(getNextBlock(iter))!=0xFFFF;iter=getNextBlock(iter)){}
			truncateList(iter);
		}

		//memperbarui entri direktori induknya
		poi_file_block blk=poi_data_pool_read_block(dataBlockIdx);
		memcpy(blk.data+offset,yangdimasuki.bytearr,32);
		poi_data_pool_write_block(blk,dataBlockIdx);
		free(parentDir);
		saveAllocTableCaches();
		return 0;
	}

	return 0;
}


/** Remove a directory */
int poi_rmdir (const char * path){
	directory_entry yangdihapus;
	int opstat;
	if (opstat=getEntryRecursive(path,getRootDirEntry(),&yangdihapus)<0) return opstat;
	if (getFileSize(yangdihapus)>0) return -ENOTEMPTY;

	return poi_unlink(path);
}

/** Change the permission bits of a file*/
int poi_chmod(const char * path, mode_t mode){
	directory_entry e;
	poi_data_pool_block_idx_t dataBlockIdx;
	uint32_t offset;
	int opstat=getEntryAndBlockOffset(path,getRootDirEntry(),&e,&dataBlockIdx,&offset);
	if (opstat < 0) return opstat;
	setattr(&e, mode_t_to_poi_attr_t(mode));
	opstat=replaceEntry(e,dataBlockIdx,offset);
	return (opstat<0)?opstat:0;
}

struct fuse_operations poi_oper = { //tiap kali ada yang diimplementasi, diubah jadi bukan komentar
	.getattr = poi_getattr,
	.readdir = poi_readdir,
	.mkdir = poi_mkdir,
	.mknod = poi_mknod,
	.read = poi_read,
	.rmdir = poi_rmdir,
	.unlink = poi_unlink,
	.rename = poi_rename,
	.write = poi_write,
	.truncate = poi_truncate,
	.chmod = poi_chmod,
};

int main(int argc, char *argv[]){
	#ifdef LOG
	logfile = fopen("logfile","w");
	#endif

	if (argc==4)
	if (!strcmp(argv[3],"-new")){
		printf("%s",argv[2]);
		int crerrno = poi_file_create_new(argv[2]);
		if (crerrno!=1)fprintf(stderr,"error creating file %d\n",crerrno);
		argc--;
	}
	if (poi_file_validasi(argv[2])!=1){
		fprintf(stderr,"ada masalah dengan berkas %s\n",argv[2]);
		return;
	}
	poi_file_open(argv[2]);
	argc--;
	int a = fuse_main(argc,argv,&poi_oper);
}

