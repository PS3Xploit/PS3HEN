#include <lv2/lv2.h>
#include <lv2/libc.h>
#include <lv2/memory.h>
#include <lv2/patch.h>
#include <lv2/syscall.h>
#include <lv2/io.h>
#include <lv2/error.h>
#include <lv2/security.h>
#include <lv2/synchronization.h>
#include <stdbool.h>
#include <stdio.h>
#include "common.h"
#include "mappath.h"
#include "modulespatch.h"
#include "syscall8.h"
#include "ps3mapi_core.h"
#include "homebrew_blocker.h"
#include "make_rif.h"

char ntfs_iso_path[256];

typedef struct MapEntry {
	char *oldpath;
	char *newpath;
	size_t newpath_len;
	size_t oldpath_len;
	uint32_t flags;
	struct MapEntry *next;
} MapEntry_t;

// typedef struct CellFsMountInformation {
	// char p_name[0x20];
	// char p_type[0x20];
	// char p_systype[0x20];
	// char padding[0x33];
	// unsigned char p_writable;
// } CellFsMountInformation_t;

uint8_t photo_gui = 1;
static MapEntry_t *head = NULL;
static MapEntry_t *found = NULL;
static mutex_t map_mtx = 0;
static mutex_t oph_mtx = 0;

#ifdef DEBUG
	mutex_t pgui_mtx = 0;
#endif

/*
// These are declared in mappath.h for external use
#ifdef DEBUG
	static mutex_t pgui_mtx = 0;
#endif
//static int init_mtx(mutex_t* mtx, uint32_t attr_protocol, uint32_t attr_recursive);
//static int lock_mtx(mutex_t* mtx);
//static int unlock_mtx(mutex_t* mtx);
*/

//int destroy_mtx(mutex_t* mtx);
//int trylock_mtx(mutex_t* mtx);
//int addMapping(const char *opath, const char *npath, uint32_t flags);
//bool isMapTableEmpty();
static bool deleteAllMappings(uint32_t flags);
static int deleteMapping(const char *opath);
static MapEntry_t* findMapping(const char *opath);
static MapEntry_t* getMapping(const char *opath);
static uint32_t mpcount = 0;
static uint64_t mapTableByteSize = 0;
static MapEntry_t* mcount[0x20]; // to keep track of mappings already applied to the path so they cannot be reapllied even when they fit, a dept of 32 is probably overkill, 16 might actually be sufficient for this, 32 max???
static size_t count = 0; // to keep track of mcount length
//display the list

#ifdef DEBUG
static void printMapTableList() {
		DPRINTF("printMappingList: Mappings Count 0x%X : \n", getMapTableLength());
		uint32_t idx=0;
		for(MapEntry_t* ptr = head; ptr != NULL; ptr = ptr->next,idx++) {
			DPRINTF("mapping index %u: %s len: 0x%X byte(s) to %s len: 0x%X byte(s) with flags: %X\n", idx, ptr->oldpath, (unsigned int)ptr->oldpath_len, ptr->newpath, (unsigned int)ptr->newpath_len, ptr->flags);
		}
}
#endif

void printMappingList() {
	#ifdef DEBUG
		lock_mtx(&map_mtx);
		printMapTableList();
		unlock_mtx(&map_mtx);
	#endif
}

//bool isMapTableEmpty() {
//	return head == NULL;
//}

uint32_t getMapTableLength() {
	uint32_t length = 0;
	for(MapEntry_t *curr = head; curr != NULL; curr = curr->next,length++) {}
	return length;
}

uint64_t getMapTableByteSize() {
	return mapTableByteSize;
}

int addMapping(const char *opath, const char *npath, uint32_t flags) {
	bool mtable_full = false;
	size_t nlen = npath ? strlen(npath):0;
	size_t olen =0;
	if(nlen>3 && (npath[0] == '/') && (npath[1] == '.') && (npath[2] == '/') && !(flags&FLAG_PROTECT)) {
		#ifdef DEBUG
			DPRINTF("addMapping: /./ token found : %s\n",npath);
		#endif
		flags|=FLAG_PROTECT;
		npath+=2;
	}
	if(opath) {
		olen=strlen(opath);
		if(olen > 6) {
			const char* store = opath+olen;
			while(opath[0]!='/' || opath[1] == '/' || opath[1] == '.') {
				opath++;
				if(opath >= store) {
					opath=NULL;
					break;
				}
			}
		}
	}
	if(opath)
	{
		if((strncmp(opath,"/dev_",5) == 0 || strncmp(opath,"/app_",5) == 0 || strncmp(opath,"/host_",6) == 0)) {
			MapEntry_t *curr = getMapping(opath);
			if(curr!=NULL) {
				if (nlen>6 && nlen<MAX_PATH)
				{
					strncpy(curr->newpath, npath, nlen+1);
					curr->newpath[nlen] = 0;
					curr->newpath_len = nlen;
					curr->flags = (curr->flags&FLAG_COPY) | (flags&(~FLAG_COPY));
					/* #ifdef DEBUG
						DPRINTF("addMapping: existing mapping path found, new_path updated : %s\n",opath);
					#endif */
					return 0;
				}
				else{
					/* #ifdef DEBUG
						DPRINTF("addMapping: deleting found mapping : %s\n",opath);
					#endif */
					return deleteMapping(opath);
				}
			}
			else if(npath) {
				size_t sz = flags & FLAG_COPY ? olen + 1 : 0;
				sz += sizeof(MapEntry_t) + MAX_PATH;
				if(mapTableByteSize + sz <= MAX_TABLE_SIZE) {
					if(nlen>=MAX_PATH) {
						/* #ifdef DEBUG
							DPRINTF("addMapping: Mapping NOT added, new_path could not be created: too long %s\n",opath);
						#endif */
						return EINVAL;
					}
					//create a link
					MapEntry_t *link = (MapEntry_t*) alloc(sizeof(MapEntry_t),0x27);
					mapTableByteSize += sizeof(MapEntry_t);
					if(!link) {
						/* #ifdef DEBUG
							DPRINTF("addMapping: Mapping NOT added, MapEntry_t could not be created: %s\n",opath);
						#endif */
						return ENOMEM;
					}
					link->flags = flags;
					link->oldpath_len = olen;
					if (flags & FLAG_COPY) {
						link->oldpath = (char*)alloc(olen + 1, 0x27);
						mapTableByteSize += olen + 1;
						if(!link->oldpath) {
							dealloc(link, 0x27);
							mapTableByteSize-=sizeof(MapEntry_t);
							/* #ifdef DEBUG
								DPRINTF("addMapping: Mapping NOT added, old_path could not be created: %s\n",opath);
							#endif */
							return ENOMEM;
						}
						strncpy(link->oldpath, opath, olen + 1);
						link->oldpath[olen] = 0;
					}
					else{
						link->oldpath = (char*)opath;
					}
					link->newpath_len = nlen;
					link->newpath_len = link->newpath_len < MAX_PATH ? link->newpath_len : MAX_PATH-1;
					link->newpath = (char*)alloc(MAX_PATH, 0x27);
					mapTableByteSize+=MAX_PATH;
					if(!link->newpath) {
						if(flags & FLAG_COPY) {
							dealloc(link->oldpath, 0x27);
							mapTableByteSize-=olen;
						}
						dealloc(link, 0x27);
						mapTableByteSize -= sizeof(MapEntry_t);
						/* #ifdef DEBUG
							DPRINTF("addMapping:  Mapping NOT added, new_path could not be created: %s\n",opath);
						#endif */
						return ENOMEM;
					}
					strncpy(link->newpath, npath, link->newpath_len+1);
					link->newpath[link->newpath_len] = 0;
					for(curr = head; curr != NULL; curr = curr->next) {
						if(curr->next== NULL)
							break;
					}
					if(curr) {
						curr->next = link;
						link->next = NULL;
					}
					else{
						link->next = head;
						head = link;
					}
					mpcount++;
					#ifdef DEBUG
						//DPRINTF("addMapping: Mapping Added: %s - Mapping Count: %u - Table Mapping Size: 0x%lx bytes\n", opath, mpcount, mapTableByteSize);
					#endif
					return 0;
				}
				else{
					mtable_full = true;
				}
			}

		}

	}
	if(!npath) {
		return 0;
	}
	if(mtable_full) {
		/* #ifdef DEBUG
			DPRINTF("addMapping: Mapping NOT added: Mapping Table already full\n");
		#endif */
		return EAGAIN;
	}
	/* #ifdef DEBUG
		 DPRINTF("addMapping: Mapping NOT added: bad argument\n");
	#endif */
	return EINVAL;
}

//find a link with given key
static MapEntry_t* getMapping(const char *opath) {
	MapEntry_t *ret = NULL;
	MapEntry_t *curr = NULL;
	if(head && opath) {
		size_t len = strlen(opath);
		if(len > 6) {// length of /dev_cf == minimum mount point length
			//start from the first link, slot order matters
			for(curr = head; curr != NULL; curr = curr->next) {
				if(curr->oldpath_len == len && strncmp(curr->oldpath, opath, curr->oldpath_len) == 0) {
					//if data found, return the current Link
					ret = curr;
					break;
				}
			}
		}
	}
	return ret;
}

static MapEntry_t* findMapping(const char *opath) {
	MapEntry_t *ret = NULL;
	MapEntry_t *curr = NULL;
	//char tmp_mapping[MAX_PATH]="";

	if(head && opath) {
		size_t len = strlen(opath);
		if(len>6) { // length of /dev_cf == minimum mount point length
			//start from the first link, slot order matters
			for(curr = head; curr != NULL; curr = curr->next) {
				if(curr->flags&FLAG_MAX_PRIORITY && curr->oldpath_len <= len && strncmp(curr->oldpath, opath, curr->oldpath_len) == 0) {
					//if data found, return the current Link
					//if(curr->flags&FLAG_FOLDER && strncmp(curr->newpath, opath, curr->newpath_len) == 0)
					if(strncmp(curr->newpath, opath, curr->newpath_len) == 0)
						//ignore mapping where opath argument == curr->newpath
						continue;
					bool mcount_found = false;
					for(size_t i = 0; i < count; i++) {
						if(mcount[i] == curr) {
							mcount_found = true;
							// mapping found in mcount
							break;
						}
					}
					if(mcount_found) {
						// mapping already applied, continue looping
						continue;
					}
					ret = curr;
					//add  mapping to mcount array
					mcount[count] = ret;
					count++;
					break;
				}
			}
			if(!ret) { // if no item with
				//start from the first link, slot order matters
				for(curr = head; curr != NULL; curr = curr->next) {
					if(curr->oldpath_len <= len && strncmp(curr->oldpath, opath, curr->oldpath_len) == 0) {
						//if(curr->flags&FLAG_FOLDER && strncmp(curr->newpath, opath, curr->newpath_len) == 0)
						if(strncmp(curr->newpath, opath, curr->newpath_len) == 0)
							//ignore mapping where opath argument == curr->newpath
							continue;
						//if data found, use the current Link
						bool mcount_found = false;
						for(size_t j = 0; j < count; j++) {
							if(mcount[j] == curr) {
								mcount_found = true;
								// mapping found in mcount
								break;
							}
						}
						if(mcount_found) {
							// mapping already applied, continue looping
							continue;
						}
						ret=curr;
						//add  mapping to mcount array
						mcount[count] = ret;
						count++;
						break;
					}
				}
			}
		}
		if(ret) {
			char* tmp_mapping = alloc(MAX_PATH,0x27);
			if(tmp_mapping) {
				tmp_mapping[0]=0;
				if(ret->newpath_len)
					strncpy(tmp_mapping, ret->newpath,ret->newpath_len);
				tmp_mapping[ret->newpath_len]=0;
				len = strlen(opath+ret->oldpath_len);
				if(ret->newpath_len + len < MAX_PATH) {
					if(len) {
						strncpy(tmp_mapping+ret->newpath_len, opath+ret->oldpath_len, len);
					}
					tmp_mapping[ret->newpath_len+len]=0;
				}
				if(found!=ret) {
					found = ret;
					ret = findMapping(tmp_mapping);
				}
				else{
					if(len) {
						strncpy(ret->newpath+ret->newpath_len, opath+ret->oldpath_len,len);
					}
					ret->newpath[ret->newpath_len+len]=0;
					found=NULL;
					// set count to 0 to reset mcount
					count = 0;
				}
				dealloc(tmp_mapping,0x27);
			}
			else{
				ret = NULL;
			}
		}
		else if(found) {
			ret=found;
			len = strlen(opath + ret->newpath_len);
			if(len)
				strncpy(ret->newpath+ret->newpath_len, opath+ret->newpath_len, len);
			ret->newpath[ret->newpath_len+len]=0;
			found = NULL;
			// set count to 0 to reset mcount
			count = 0;
		}
	}
	return ret;
}
//find a link with given key
static bool patchAllMappingStartingWith(const char *opath, char* dst) {
	/*#ifdef DEBUG
		if(opath && dst)
			DPRINTF("patchAllMappingStartingWith=: %s -> %s\n", opath, dst);
		else
			DPRINTF("patchAllMappingStartingWith=: null argument\n");
	#endif*/
	//if list is empty
	if(head == NULL || opath == NULL) {
		return false;
	}
	//start from the first link
	 MapEntry_t* curr = head;
	size_t len = strlen(opath),j = 1;
	//navigate through list
	while(curr && len>8) {
		if(curr->oldpath_len==len && strncmp(opath,curr->oldpath,curr->oldpath_len) == 0) {
			for (j=1;j < curr->newpath_len; j++) {
				dst[j] = curr->newpath[j];
				if (dst[j] == 0)
					break;
				if (dst[j] == '/') {
					dst[j] = 0;
					break;
				}
			}
		}
		 curr = curr->next;
	}
	return true;
}
//find a link with given key
static bool deleteAllMappings(uint32_t flags) {
	#ifdef DEBUG
		DPRINTF("deleteAllMappings=: flag %x\n",flags);
	#endif
	//if list is empty
	if(head == NULL) {
	  return true;
	}
	//start from the first link
	MapEntry_t* curr = head;
	MapEntry_t* next = NULL;
	MapEntry_t* prev = NULL;
	#ifdef DEBUG
		printMapTableList();
	#endif
	//navigate through list
	while(curr) {
		//store curr->next
		next = curr->next;
		if((!flags || curr->flags & flags) && !(curr->flags & FLAG_PROTECT)) {
			/* #ifdef DEBUG
				DPRINTF("deleteAllMappings=: deleting %s\n",curr->oldpath);
			#endif */
			if(strcmp(curr->oldpath, "/dev_bdvd") == 0) {
				condition_apphome = false;
				/* #ifdef DEBUG
					DPRINTF("deleteAllMappings: removed %s condition_apphome set to false\n",curr->oldpath);
				#endif */
			}
			if(curr == head) {
				//change first to point to next link
				head = next;
			}
			else if(prev) {
				//bypass the curr link
				prev->next = next;
			}
			if(curr->flags & FLAG_COPY)
				dealloc(curr->oldpath, 0x27);
			dealloc(curr->newpath, 0x27);
			dealloc(curr, 0x27);
			if(mpcount)mpcount--;
		}
		else{
			prev=curr;
		}
		//if it is last node
		if(next == NULL) {
			#ifdef DEBUG
				printMapTableList();
			#endif
			return true;
		}
		else {
			//go to next link
			curr = next;
		}
	}
	#ifdef DEBUG
		printMapTableList();
	#endif
	return false;
}
//delete a link with given key
static int deleteMapping(const char *opath) {
	/* #ifdef DEBUG
		if(opath)
			DPRINTF("deleteMapping=: %s\n",opath);
		else
			DPRINTF("deleteMapping=: null argument\n");
	#endif */
	//if list is empty
	if(head && opath && (strncmp(opath,"/dev_",5) == 0 || strncmp(opath,"/app_",5) == 0 || strncmp(opath,"/host_",6) == 0))
	{

		#ifdef DEBUG
			printMapTableList();
		#endif
		//start from the first link
		MapEntry_t* curr = head;
		MapEntry_t* previous = NULL;
		size_t len = strlen(opath);
		//navigate through list
		while(curr->oldpath_len != len || strncmp(curr->oldpath, opath, len)!=0) {
			//if it is last node
			if(curr->next == NULL) {
				return ESRCH;
			}
			else {
				//store reference to curr link
				previous = curr;
				//move to next link
				curr = curr->next;
			}
		}
		if(!(curr->flags & FLAG_PROTECT)) {
			//found a match, update the link
			if(curr == head) {
			  //change first to point to next link
			  head = head->next;
			} else {
			  //bypass the curr link
			  previous->next = curr->next;
			}
			if (curr->flags & FLAG_COPY)
				dealloc(curr->oldpath, 0x27);
			dealloc(curr->newpath, 0x27);
			dealloc(curr, 0x27);
			if(mpcount)
				mpcount--;
			#ifdef DEBUG
				printMapTableList();
			#endif

			return 0;
		}
		else
			return EPERM;
	}
	return EINVAL;
}

//static int init_mtx(mutex_t* mtx, uint32_t attr_protocol, uint32_t attr_recursive) {
int init_mtx(mutex_t* mtx, uint32_t attr_protocol, uint32_t attr_recursive) {
	int ret=mtx ? 0 : EINVAL;
	if(mtx && !*mtx) {
		ret = mutex_create(mtx, attr_protocol, attr_recursive);
		/* #ifdef DEBUG
			if(ret)
			{
				DPRINTF("init_mtx=: mutex creation error %x\n",ret);
			}
			else
			{
				DPRINTF("init_mtx=: mutex 0x%8lx created\n", (uint64_t)*mtx);
			}
		#endif */
	}
	/* #ifdef DEBUG
		DPRINTF("init_mtx=: return %x \n", ret);
	#endif */
	return ret;
}

//static int lock_mtx(mutex_t* mtx) {
int lock_mtx(mutex_t* mtx) {
	int ret = EINVAL;
	if(mtx) {
		ret = 0;
		if(!*mtx) {
			//ret = mtx==&pgui_mtx ? init_mtx(mtx,SYNC_PRIORITY, SYNC_RECURSIVE) : init_mtx(mtx,SYNC_PRIORITY, SYNC_NOT_RECURSIVE);
			ret = init_mtx(mtx,SYNC_PRIORITY, SYNC_NOT_RECURSIVE);
		}
		if(!ret) {
			ret = mutex_lock(*mtx, 0);
			/* #ifdef DEBUG
				if(ret)
				{
					//DPRINTF("lock_mtx=: mutex 0x%8lx lock error %x\n", (uint64_t)*mtx, ret);
				}
				else
				{
					//DPRINTF("lock_mtx=: mutex 0x%8lx locked\n", (uint64_t)*mtx);
				}
			#endif */
		}
	}
	/* #ifdef DEBUG
		//DPRINTF("lock_mtx=: return %x \n", ret);
	#endif */
	return ret;
}

//static int unlock_mtx(mutex_t* mtx) {
int unlock_mtx(mutex_t* mtx) {
	int ret= mtx ? !(*mtx) ? ESRCH : 0 : EINVAL;
	if(!ret) {
		ret = mutex_unlock(*mtx);
		/* #ifdef DEBUG
			if(ret)
			{
				DPRINTF("unlock_mtx=: mutex 0x%8lx unlock error %x\n", (uint64_t)*mtx, ret);
			}
			else
			{
				DPRINTF("unlock_mtx=: mutex 0x%8lx unlocked\n", (uint64_t)map_mtx);
			}
		#endif */
	}
	/* #ifdef DEBUG
		DPRINTF("unlock_mtx=: return %x \n", ret);
	#endif */
	return ret;
}

//int destroy_mtx(mutex_t* mtx) {
//	int ret=mtx ? 0 : EINVAL;
//	if(mtx && *mtx) {
//		ret=mutex_destroy(*mtx);
//		if(ret) {
//		#ifdef DEBUG
//			//DPRINTF("destroy_mtx=: mutex 0x%8lx destroy error %x\n", (uint64_t)*mtx, ret);
//		#endif
//			if(ret==ESRCH) { //ESRCH error
//				#ifdef DEBUG
//					//DPRINTF("destroy_mtx=: mutex 0x%8lx reset to 0\n", (uint64_t)*mtx);
//				#endif
//				*mtx=0;
//			}
//		}
//		else{
//		#ifdef DEBUG
//			//DPRINTF("destroy_mtx=: mutex 0x%8lx destroyed\n", (uint64_t)*mtx);
//		#endif
//			*mtx=0;
//		}
//	}
//	#ifdef DEBUG
//		//DPRINTF("destroy_mtx=: return %x \n", ret);
//	#endif
//	return ret;
//}

//int trylock_mtx(mutex_t* mtx) {
//	int ret = EINVAL;
//	if(mtx) {
//		ret = 0;
//		if(!*mtx) {
//			//ret = mtx==&pgui_mtx ? init_mtx(mtx,SYNC_PRIORITY, SYNC_RECURSIVE) : init_mtx(mtx,SYNC_PRIORITY, SYNC_NOT_RECURSIVE);
//			ret = init_mtx(mtx,SYNC_PRIORITY, SYNC_NOT_RECURSIVE);
//		}
//		if(!ret) {
//			ret = mutex_trylock(*mtx);
//			#ifdef DEBUG
//				if(ret)
//					DPRINTF("trylock_mtx=: mutex 0x%8lx lock error %x\n", (uint64_t)*mtx, ret);
//				else
//					DPRINTF("trylock_mtx=: mutex 0x%8lx locked\n", (uint64_t)*mtx);
//			#endif
//		}
//	}
//	#ifdef DEBUG
//		DPRINTF("trylock_mtx=: return %x \n", ret);
//	#endif
//	return ret;
//}

// int unmap_path(char *oldpath)
// {
	// int ret = EINVAL;
	// if (oldpath) {
		// lock_mtx(&map_mtx);
		// ret = deleteMapping(oldpath);
		// if(ret==0) {
			// if (strcmp(oldpath, "/dev_bdvd") == 0)
				// condition_apphome = false;
			// #ifdef DEBUG
				// DPRINTF("Unmapped path: %s\n", oldpath);
			// #endif
		// }
		// unlock_mtx(&map_mtx);
	// }
	// return ret;
// }

int map_path(char *oldpath, char *newpath, uint32_t flags)
{
	int ret = EINVAL;
	if (oldpath) {
		if (newpath && strcmp(oldpath, newpath) == 0)
			newpath = NULL;
		if (strcmp(oldpath, "/dev_bdvd") == 0) {
			condition_apphome = (newpath != NULL);
			#ifdef DEBUG
				//DPRINTF("map_path: condition_apphome set to %s\n",condition_apphome ? "true":"false" );
			#endif
		}
		lock_mtx(&map_mtx);
		ret = addMapping(oldpath, newpath, flags);
		unlock_mtx(&map_mtx);
		#ifdef DEBUG

			if(ret==0) {
				//DPRINTF("map_path: mapped path: %s -> %s flags %x\n", oldpath, newpath, flags);
			}
			else{
				//DPRINTF("map_path: add mapping error %X for path: %s -> %s flags %x\n",ret, oldpath, newpath, flags);
			}
		#endif
	}
	return ret;
}

int map_path_user2(char *oldpath, char *newpath, uint32_t flags)
{
	char *oldp, *newp;

	/* #ifdef DEBUG
		DPRINTF("map_path_user, called by process %s: %s -> %s\n", get_process_name(get_current_process_critical()), oldpath, newpath);
	#endif */

	if (oldpath == 0)
		return -1;

	int ret = pathdup_from_user(get_secure_user_ptr(oldpath), &oldp);
	if (ret != 0)
		return ret;

	if (newpath == 0)
		newp = NULL;
	else
	{
		ret = pathdup_from_user(get_secure_user_ptr(newpath), &newp);
		if (ret != 0)
		{
			dealloc(oldp, 0x27);
			return ret;
		}
	}

	ret = map_path(oldp, newp, flags | FLAG_COPY);

	dealloc(oldp, 0x27);
	if (newp)
		dealloc(newp, 0x27);

	return ret;
}

int map_path_user(char *oldpath, char *newpath, uint32_t flags)
{
	return !map_path_user2(oldpath, newpath, flags) ? 0 : -1;
}

int get_map_path(uint32_t num, char *path, char *new_path)
{
	int ret = EINVAL;
	if(path && new_path) {
		lock_mtx(&map_mtx);
		MapEntry_t* curr = NULL;
		uint32_t i=0;
		for(curr=head;curr != NULL && i!=num;curr=curr->next,i++) {}
		if(curr && i==num && curr->oldpath_len && curr->newpath_len && curr->newpath && curr->oldpath) {
			copy_to_user(&path, get_secure_user_ptr(curr->oldpath),curr->oldpath_len);
			copy_to_user(&new_path, get_secure_user_ptr(curr->newpath), curr->newpath_len);
			ret=0;
			/* #ifdef DEBUG
				DPRINTF("get_map_path: slot: 0x%x oldpath: %s -> newpath: %s\n", num, path, new_path);
			#endif */
		}
		else{
			ret=ESRCH;
			/* #ifdef DEBUG
				DPRINTF("get_map_path slot %u not found\n", num);
			#endif */
		}
		unlock_mtx(&map_mtx);
	}
	return !ret ? 0 : -1;
}
LV2_SYSCALL2(int, sys_map_path, (char *oldpath, char *newpath, uint32_t flags))
{
	extend_kstack(0);
	return map_path_user(oldpath, newpath, flags);
}

int sys_map_paths(char *paths[], char *new_paths[], uint32_t num)
{
	uint32_t *u_paths = (uint32_t*)get_secure_user_ptr(paths);
	uint32_t *u_new_paths = (uint32_t*)get_secure_user_ptr(new_paths);
	int unmap = 0;
	int ret = 0;

	if (!u_paths)
		unmap = 1;
	else
	{
		if(!u_new_paths)
			return EINVAL;

		for(unsigned int i = 0; i < num; i++)
		{
			ret = map_path_user((char *)(uint64_t)u_paths[i], (char *)(uint64_t)u_new_paths[i], FLAG_TABLE);
			if (ret != 0)
			{
				unmap = 1;
				break;
			}
		}
	}

	if (unmap)
	{
		lock_mtx(&map_mtx);
		deleteAllMappings(FLAG_TABLE);
		unlock_mtx(&map_mtx);

	}
	return ret;
}

void aescbc128_decrypt(unsigned char *key, unsigned char *iv, unsigned char *in, unsigned char *out, int len)
{
	aescbccfb_dec(out,in,len,key,128,iv);
	// Reset the IV.
	memset(iv, 0, 0x10);
}

static unsigned char no_exists[] = {"/fail"};
static uint8_t libft2d_access = 0;

LV2_HOOKED_FUNCTION_POSTCALL_2(int, open_path_hook, (char *path0, char *path1))
{
	// Save path when a ISO is mounted from NTFS partition (By Evilnat)
	if(strstr(path0, "ntfs[PS3ISO]"))
		strcpy(ntfs_iso_path, path0);
	
	//extend_kstack(0);
	if(path0) {
		#ifdef DEBUG
			CellFsStat stat;
			int lretin = lock_mtx(&pgui_mtx);
			if(lretin != 0) {
				if(lretin == EDEADLK) {
					//DPRINTF("open_path_hook=: recursive: %s\n",path0);
					unlock_mtx(&pgui_mtx);// unlock mutex and exit hook
					return 0;
				}
				else{
					//if(path1)
					//	DPRINTF("open_path_hook=: path0: %s - path1 len: 0x%X offset: 0x%8lx\n",path0, (unsigned int)strlen(path1), (uint64_t)path1);
					//else
					DPRINTF("open_path_hook=: %s\n",path0);
				}
			}
			else{
				if(cellFsStat(path0,&stat)) {
					DPRINTF("open_path_hook=: [NG] %s\n",path0);
				}
				else{
					DPRINTF("open_path_hook=: [OK] %s\n",path0);
				}
				unlock_mtx(&pgui_mtx);
			}
		#endif
		
		lock_mtx(&oph_mtx);
		make_rif(path0);
		//restore_syscalls(path0);
		//check_signin(path0);

		if (block_psp_launcher && !umd_file && !strncmp(path0, "/dev_flash/pspemu", 17))
		{
			block_psp_launcher = 0;
			set_patched_func_param(1, (uint64_t)no_exists);
			unlock_mtx(&oph_mtx);
			return 0;
		}

		// Let's now block homebrews if the "allow" flag is false
		if(!block_homebrew(path0))
				{
					set_patched_func_param(1, (uint64_t)no_exists);
			unlock_mtx(&oph_mtx);
					return 0;
				}
		
		/* #ifdef DEBUG
			DPRINTF("open_path_hook=: processing path [%s]\n", path0);
		#endif */

		char *path = path0;
		if(path) {
			size_t plen = strlen(path);
			while(path[0]!='/' || path[1] == '/' || path[1] == '.') {
				path++;
				if(path >= path0+plen) {
					path=NULL;
					break;
				}
			}
		}
		if(path && (strncmp(path,"/dev_",5) == 0 || strncmp(path,"/app_",5) == 0 || strncmp(path,"/host_",6) == 0))
		{
			/*if (path && ((strcmp(path, "/dev_bdvd/PS3_UPDATE/PS3UPDAT.PUP") == 0)))  // Blocks FW update from Game discs!
			{
				char not_update[40];
				sprintf(not_update, "/dev_bdvd/PS3_NOT_UPDATE/PS3UPDAT.PUP");
				set_patched_func_param(1, (uint64_t)not_update);
				#ifdef DEBUG
					DPRINTF("Update from disc blocked!\n");
				#endif
			}
			else*/ // Disabled due to the issue with multiMAN can't copy update files from discs.

			{

				////////////////////////////////////////////////////////////////////////////////////
				// Photo_GUI integration with webMAN MOD - DeViL303 & AV                          //
				////////////////////////////////////////////////////////////////////////////////////

				if(!libft2d_access) {
					libft2d_access = photo_gui && !strcmp(path, "/dev_flash/sys/internal/libft2d.sprx");
				}
				else if(strcmp(path, "/dev_hdd0/tmp/wm_request")) {
					libft2d_access = 0;
					if(!strncmp(path, "/dev_hdd0/photo/", 16)) {
						char *photo = path + 16;
						size_t len = strlen(photo);
						if (len < 8) ;
						else if(!strcmp(photo + len -4, ".PNG") || !strcmp(photo + len -4, ".JPG") || !strcmp(photo + len -8, "_COV.JPG") || !strncasecmp(photo + len -8, ".iso.jpg", 8) || !strncasecmp(photo + len -8, ".iso.png", 8))
						{
							#ifdef DEBUG
								DPRINTF("open_path_hook:= CREATING /dev_hdd0/tmp/wm_request\n");
							#endif
							int fd;
							if(cellFsOpen("/dev_hdd0/tmp/wm_request", CELL_FS_O_CREAT | CELL_FS_O_WRONLY | CELL_FS_O_TRUNC, &fd, 0666, NULL, 0) == 0)
							{
								cellFsWrite(fd, path, (len + 16), NULL);
								cellFsClose(fd);
							}
						}
					}
				}
				////////////////////////////////////////////////////////////////////////////////////

				lock_mtx(&map_mtx);
				MapEntry_t* curr = findMapping(path);
				if(curr) {
					set_patched_func_param(1, (uint64_t)curr->newpath);
					#ifdef DEBUG
						//DPRINTF("open_path_hook:= found matching entry for %s in Map Table oldpath: [%s] \nMap Table newpath: [%s] \nMap Table newpath_len: [0x%x]\n",path,curr->oldpath,curr->newpath,(unsigned int)curr->newpath_len);
					#endif
				}
				else{
					/* #ifdef DEBUG
						DPRINTF("open_path_hook=: no mapping found for path [%s]\n", path);
					#endif */
				}
				unlock_mtx(&map_mtx);
			}
		}
	}
	unlock_mtx(&oph_mtx);
	return 0;
}

int sys_aio_copy_root(char *src, char *dst)
{
	src = (char*)get_secure_user_ptr(src);
	dst = (char*)get_secure_user_ptr(dst);

	// Begin original function implementation
	if (!src)
		return EFAULT;

	size_t len = strlen(src);

	if (len >= MAX_PATH || len <= 1 || src[0] != '/')
		return EINVAL;

	strcpy(dst, src);

	for (int i = 1; i < len; i++)
	{
		if (dst[i] == 0)
			break;

		if (dst[i] == '/')
		{
			dst[i] = 0;
			break;
		}
	}
	if (strlen(dst) >= 0x20)
		return EINVAL;

	// Here begins custom part of the implementation
	if (strcmp(dst, "/dev_bdvd") == 0 && condition_apphome) // if dev_bdvd and jb game mounted
	{
		lock_mtx(&map_mtx);
		// find /dev_bdvd
		patchAllMappingStartingWith("/dev_bdvd", dst);
		unlock_mtx(&map_mtx);
	}

	return 0;
}

void unhook_all_map_path(void)
{
	suspend_intr();
	unhook_function_with_postcall(open_path_symbol, open_path_hook, 2);
	resume_intr();
}

void map_path_patches(int syscall)
{
	hook_function_with_postcall(open_path_symbol, open_path_hook, 2);
	if (syscall) {
		create_syscall2(SYS_MAP_PATH, sys_map_path);
	}
}
