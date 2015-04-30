#include <fuse.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "file-manager.h"
#include "directory-entry.h"
#include "volume-information.h"
#include "data-pool-block-manager.h"
FILE * logfile;
int getEntryAndBlockOffset(const char * path, directory_entry r, directory_entry * getout, uint16_t * dataBlockIdx, uint32_t * offset){
	char buf[22];
	fprintf(logfile,"getEntryAndBlockOffset path %s di entri dir %s\n",path,getNama(buf,r));
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
	fprintf(logfile,"getEntryRecursive path %s di entri dir %s\n",path,getNama(buf,r));
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
	//TODO WOLOLO
	
}

int directory_entry_to_struct_stat(directory_entry e, struct stat * statbuf){
        memset(statbuf, 0, sizeof(struct stat));
	statbuf->st_mode=0;
	poi_attr_t attr=getattr(e);
	if (attr.x)
		statbuf->st_mode = statbuf->st_mode | S_IEXEC;
	if (attr.r)
		statbuf->st_mode = statbuf->st_mode | S_IREAD;
	if (attr.w)
		statbuf->st_mode = statbuf->st_mode | S_IWRITE;
	if (attr.d)
		statbuf->st_mode = statbuf->st_mode | S_IFDIR;
	else	statbuf->st_mode = statbuf->st_mode | S_IFREG;
	statbuf->st_blocks = (getFileSize(e)==0)?1:(((unsigned long)getFileSize(e)+POI_BLOCK_SIZE-1)/POI_BLOCK_SIZE);
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
	fprintf(logfile,"getattr('%s')\n",path);
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
		//TODO cek lagi nanti
	}
	char buf[22];
	return directory_entry_to_struct_stat(e,statbuf);//TODO implementasi
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
	fprintf(logfile,"readdir('%s')\n",path);
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
	return a;//TODO implementasi
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

	if (dataBlockIdx==0xFFFF){
		if (getNumFreeBlocks()<1) return -ENOSPC;
		setNextBlock(prev,getFirstFreeBlockIdx());
		dataBlockIdx=getFirstFreeBlockIdx();
		setNextBlock(dataBlockIdx,0XFFFF);
		memcpy(blk.data,val.bytearr,32);
		op_status = poi_data_pool_write_block(blk,dataBlockIdx);
		if (op_status!=1) return op_status;
		setFileSize(dst,getFileSize(*dst)+32);
		setNumFreeBlocks(getNumFreeBlocks()-1);
		setFirstFreeBlockIdx(getNextEmpty(dataBlockIdx));
		savePoiVolinfoCache();
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
	if (mode & S_IEXEC)
		attr.x = true;
	else	attr.x = false;
	if (mode & S_IREAD)
		attr.r = true;
	else	attr.r = false;
	if (mode & S_IWRITE)
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
	fprintf(logfile,"mkdir('%s')\n",path);
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
	directory_entry yangdimasukkan = makeEntry(path+n+1, attr, GetJam(waktu_buat), GetTanggal(waktu_buat), getFirstFreeBlockIdx(), 0);
	setFirstFreeBlockIdx(getNextEmpty(getFirstFreeBlockIdx()));
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
	return -EPERM;//TODO implementasi
}



/** Create a file node
 *
 * This is called for creation of all non-directory, non-symlink
 * nodes.  If the filesystem defines a create() method, then for
 * regular files that will be called instead.
 */
int poi_mknod (const char *path, mode_t mode, dev_t dev){
	fprintf(logfile,"mkdir('%s')\n",path);
	directory_entry yangdimasuki;
	if (getEntryRecursive(path,getRootDirEntry(),&yangdimasuki)==0) return -EEXIST;
	int path_length=strlen(path);
	int n = path_length;
	while (path[n]!='/') n--;

	int status_op;

	poi_attr_t attr = mode_t_to_poi_attr_t(mode);
	WAKTU waktu_buat = GetCurrentTime();
	if (strlen(path+n+1)>21) return -ENAMETOOLONG;
	directory_entry yangdimasukkan = makeEntry(path+n+1, attr, GetJam(waktu_buat), GetTanggal(waktu_buat), getFirstFreeBlockIdx(), 0);
	setFirstFreeBlockIdx(getNextEmpty(getFirstFreeBlockIdx()));
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
	return -EPERM;//TODO implementasi
		//TODO dev_t
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
	fprintf(logfile,"read('%s')\n",path);
	return -ENOSYS;//TODO implementasi
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
	fprintf(logfile,"write('%s')\n",path);
	return -ENOSYS;//TODO implementasi
}

/** Remove a file */
int poi_unlink (const char * path){
	return ;//TODO implementasi
}

/** Remove a directory */
int poi_rmdir (const char * path){
	return ;//TODO implementasi
}

/** Rename a file */
int poi_rename (const char * path, const char * newpath){
	return ;//TODO implementasi
}

/** Change the size of a file */
int poi_truncate (const char * path, off_t offset){
	return ;//TODO implementasi
}


struct fuse_operations poi_oper = { //TODO tiap kali ada yang diimplementasi, diubah jadi bukan komentar
	.getattr = poi_getattr,
	.readdir = poi_readdir,
	.mkdir = poi_mkdir,
	.mknod = poi_mknod,
	.read = poi_read,
//	.rmdir = poi_rmdir,
//	.unlink = poi_unlink,
//	.rename = poi_rename,
	.write = poi_write,
//	.truncate = poi_truncate,
};

int main(int argc, char *argv[]){
	logfile = fopen("logfile","w");
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
	poi_attr_t  attr = getattr(getRootDirEntry());
	printf("\n%x%x%x%x\n",attr.x,attr.r,attr.w,attr.d);
	fflush(stdout);
	argc--;
	int a = fuse_main(argc,argv,&poi_oper);
}

