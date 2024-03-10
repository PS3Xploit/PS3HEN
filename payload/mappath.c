#include <lv2/lv2.h>
#include <lv2/libc.h>
#include <lv2/memory.h>
#include <lv2/patch.h>
#include <lv2/syscall.h>
#include <lv2/io.h>
#include <lv2/error.h>
#include "common.h"
#include "mappath.h"
#include "modulespatch.h"
#include "syscall8.h"
#include "ps3mapi_core.h"
#include <lv2/security.h>
#include <lv2/synchronization.h>
#include <stdbool.h>
#include <stdio.h>

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
#ifdef DEBUG
static mutex_t pgui_mtx = 0;
#endif
static int init_mtx(mutex_t* mtx, uint32_t attr_protocol, uint32_t attr_recursive);
static int lock_mtx(mutex_t* mtx);
static int unlock_mtx(mutex_t* mtx);
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
					#ifdef DEBUG
						//DPRINTF("addMapping: existing mapping path found, new_path updated : %s\n",opath);
					#endif
					return 0;
				}
				else{
					#ifdef DEBUG
						DPRINTF("addMapping: deleting found mapping : %s\n",opath);
					#endif
					return deleteMapping(opath);
				}
			}
			else if(npath) {
				size_t sz = flags & FLAG_COPY ? olen + 1 : 0;
				sz += sizeof(MapEntry_t) + MAX_PATH;
				if(mapTableByteSize + sz <= MAX_TABLE_SIZE) {
					if(nlen>=MAX_PATH) {
						#ifdef DEBUG
							DPRINTF("addMapping: Mapping NOT added, new_path could not be created: too long %s\n",opath);
						#endif
						return EINVAL;
					}
					//create a link
					MapEntry_t *link = (MapEntry_t*) alloc(sizeof(MapEntry_t),0x27);
					mapTableByteSize += sizeof(MapEntry_t);
					if(!link) {
						#ifdef DEBUG
							DPRINTF("addMapping: Mapping NOT added, MapEntry_t could not be created: %s\n",opath);
						#endif
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
							#ifdef DEBUG
								DPRINTF("addMapping: Mapping NOT added, old_path could not be created: %s\n",opath);
							#endif
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
						#ifdef DEBUG
							DPRINTF("addMapping:  Mapping NOT added, new_path could not be created: %s\n",opath);
						#endif
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
						DPRINTF("addMapping: Mapping Added: %s - Mapping Count: %u - Table Mapping Size: 0x%lx bytes\n", opath, mpcount, mapTableByteSize);
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
		#ifdef DEBUG
			// DPRINTF("addMapping: Mapping NOT added: Mapping Table already full\n");
		#endif
		return EAGAIN;
	}
	#ifdef DEBUG
		 DPRINTF("addMapping: Mapping NOT added: bad argument\n");
	#endif
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
	#ifdef DEBUG
		if(opath && dst)
			DPRINTF("patchAllMappingStartingWith=: %s -> %s\n", opath, dst);
		else
			DPRINTF("patchAllMappingStartingWith=: null argument\n");
	#endif
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
			#ifdef DEBUG
				DPRINTF("deleteAllMappings=: deleting %s\n",curr->oldpath);
			#endif
			if(strcmp(curr->oldpath, "/dev_bdvd") == 0) {
				condition_apphome = false;
				#ifdef DEBUG
					DPRINTF("deleteAllMappings: removed %s condition_apphome set to false\n",curr->oldpath);
				#endif
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
		#ifdef DEBUG
		if(opath)
			DPRINTF("deleteMapping=: %s\n",opath);
		else
			DPRINTF("deleteMapping=: null argument\n");
	#endif
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

static int init_mtx(mutex_t* mtx, uint32_t attr_protocol, uint32_t attr_recursive) {
	int ret=mtx ? 0 : EINVAL;
	if(mtx && !*mtx) {
		ret = mutex_create(mtx, attr_protocol, attr_recursive);
		#ifdef DEBUG
			if(ret)
			{
				DPRINTF("init_mtx=: mutex creation error %x\n",ret);
			}
			else
			{
				DPRINTF("init_mtx=: mutex 0x%8lx created\n", (uint64_t)*mtx);
			}
		#endif
	}
	#ifdef DEBUG
		DPRINTF("init_mtx=: return %x \n", ret);
	#endif
	return ret;
}

static int lock_mtx(mutex_t* mtx) {
	int ret = EINVAL;
	if(mtx) {
		ret = 0;
		if(!*mtx) {
			//ret = mtx==&pgui_mtx ? init_mtx(mtx,SYNC_PRIORITY, SYNC_RECURSIVE) : init_mtx(mtx,SYNC_PRIORITY, SYNC_NOT_RECURSIVE);
			ret = init_mtx(mtx,SYNC_PRIORITY, SYNC_NOT_RECURSIVE);
		}
		if(!ret) {
			ret = mutex_lock(*mtx, 0);
			#ifdef DEBUG
				if(ret)
				{
					//DPRINTF("lock_mtx=: mutex 0x%8lx lock error %x\n", (uint64_t)*mtx, ret);
				}
				else
				{
					//DPRINTF("lock_mtx=: mutex 0x%8lx locked\n", (uint64_t)*mtx);
				}
			#endif
		}
	}
	#ifdef DEBUG
		//DPRINTF("lock_mtx=: return %x \n", ret);
	#endif
	return ret;
}

static int unlock_mtx(mutex_t* mtx) {
	int ret= mtx ? !(*mtx) ? ESRCH : 0 : EINVAL;
	if(!ret) {
		ret = mutex_unlock(*mtx);
		#ifdef DEBUG
			if(ret)
			{
				//DPRINTF("unlock_mtx=: mutex 0x%8lx unlock error %x\n", (uint64_t)*mtx, ret);
			}
			else
			{
				//DPRINTF("unlock_mtx=: mutex 0x%8lx unlocked\n", (uint64_t)map_mtx);
			}
		#endif
	}
	#ifdef DEBUG
		//DPRINTF("unlock_mtx=: return %x \n", ret);
	#endif
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
				DPRINTF("map_path: condition_apphome set to %s\n",condition_apphome ? "true":"false" );
			#endif
		}
		lock_mtx(&map_mtx);
		ret = addMapping(oldpath, newpath, flags);
		unlock_mtx(&map_mtx);
		#ifdef DEBUG

			if(ret==0) {
				DPRINTF("map_path: mapped path: %s -> %s flags %x\n", oldpath, newpath, flags);
			}
			else{
				DPRINTF("map_path: add mapping error %X for path: %s -> %s flags %x\n",ret, oldpath, newpath, flags);
			}
		#endif
	}
	return ret;
}

int map_path_user2(char *oldpath, char *newpath, uint32_t flags)
{
	char *oldp, *newp;

	#ifdef DEBUG
		//DPRINTF("map_path_user, called by process %s: %s -> %s\n", get_process_name(get_current_process_critical()), oldpath, newpath);
	#endif

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
			#ifdef DEBUG
				//DPRINTF("get_map_path: slot: 0x%x oldpath: %s -> newpath: %s\n", num, path, new_path);
			#endif
		}
		else{
			ret=ESRCH;
			#ifdef DEBUG
				//DPRINTF("get_map_path slot %u not found\n", num);
			#endif
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
//////////////////////////////////////////////////////////////////////////////////////
// KW - HOMEBREW BLOCKER SUPPORT CODE TO USE IN open_path_hook()
//
// Functions, global vars and directives are here to improve code readability
// Le funzionalità, le variabili globali e le direttive sono qui per migliorare la lettura del codice
//
// declaration for read_text_line() which is defined in modulespatch.c after removal of the "static" declaration.
// la dechiarazione per read_text_line() viene definita in modulepatch.c soltanto dopo aver rimosso la dichiarazione "static"
int read_text_line(int fd, char *line, unsigned int size, int *eof);

#define BLACKLIST_FILENAME "/dev_hdd0/tmp/blacklist.cfg"
#define WHITELIST_FILENAME "/dev_hdd0/tmp/whitelist.cfg"
#define MAX_LIST_ENTRIES 30 // Maximum elements for noth the custom blacklist and whitelist.

static int __initialized_lists = 0; // Are the lists initialized ?
static int __blacklist_entries = 0; // Module global var to hold the current blacklist entries.
static char *__blacklist;
static int __whitelist_entries = 0; // Module global var to hold the current whitelist entries.
static char *__whitelist;


//
// init_list()
//
// inits a list.
// returns the number of elements read from file
char line[0x10];

static int init_list(char *list, char *path, int maxentries)
{
	int loaded, f;
	// uncomment this to avoid hook recursivity & remappings if appropriate
	//lock_mtx(&pgui_mtx);
	if (cellFsOpen(path, CELL_FS_O_RDONLY, &f, 0, NULL, 0) != 0)
		return 0; // failed to open

	loaded = 0;

	while (loaded < maxentries)
	{
		int eof;
		if (read_text_line(f, line, sizeof(line), &eof) > 0)
		if (strlen(line) >=9) // avoid copying empty lines
		{
			strncpy(list + (9*loaded), line, 9); // copy only the first 9 chars - if it has less than 9, it will fail future checks. should correct in file.
			loaded++;
		}

		if (eof)
			break;
	}
	cellFsClose(f);
	return loaded;
}


//
// listed()
//
// tests if a char gameid[9] is in the blacklist or whitelist
// initialize the both lists, if not yet initialized;
// receives the list to test blacklist (1) or whitelist (0), and the gameid
// to initialize the lists, tries to read them from file BLACKLIST_FILENAME and WHITELIST_FILENAME
//
// prova se un gameid[9] è in blacklist o whitelist
// inizializza entrambi gli elenchi, se non ancora inizializzati;
// riceve l'elenco per testare la blacklist (1) o la whitelist (0) e il gameid
// per inizializzare gli elenchi, prova a leggerli dal file BLACKLIST_FILENAME e WHITELIST_FILENAME

static int listed(int blacklist, char *gameid)
	{
		char *list;
		int i, elements;
		if (!__initialized_lists)
		{
			__blacklist=(char*)alloc(9*MAX_LIST_ENTRIES,0x27);
			__whitelist=(char*)alloc(9*MAX_LIST_ENTRIES,0x27);
			__blacklist_entries = init_list(__blacklist, BLACKLIST_FILENAME, MAX_LIST_ENTRIES);
			__whitelist_entries = init_list(__whitelist, WHITELIST_FILENAME, MAX_LIST_ENTRIES);
			__initialized_lists = 1;
		}

		if (blacklist)
		{
			list = __blacklist;
			elements = __blacklist_entries;
		}
		else
		{
			list = __whitelist;
			elements = __whitelist_entries;
		}

		for (i = 0; i < elements; i++)
		if (!strncmp(list+(9*i),gameid, 9))
			return 1; // gameid is in the list

		// if it got here, it is not in the list. return 0
		return 0;
	}

#define IDPS_KEYBITS 128
#define ACT_DAT_KEYBITS 128
#define RIF_KEYBITS 128
#define RAP_KEYBITS 128

static uint8_t libft2d_access = 0;

// BEGIN KW & AV block access to homebrews when syscalls are disabled
// After the core tests it will test first if the gameid is in whitelist.cfg (superseeds previous tests)
// In the it will test if the gameid is in blacklist.cfg (superseeds all previous tests)
// ** WARNING ** This syscall disablement test assumes that the syscall table entry 6 (peek) was replaced by the original value (equals syscall 0 entry) as done by PSNPatch
// ** WARNING ** If only a partial disablement was made, this assumption WILL FAIL !!!
//
// Iniziato da KW & AV blocca l'accesso a homebrews quando le syscalls sono disabilitati
// Dopo i test di base, verificherà innanzitutto se il gameid è in whitelist.cfg (test superiori precedenti)
// Nel verificherà se il gameid è in blacklist.cfg (sostituisce tutti i test precedenti)
// ** AVVERTENZA ** Questo test di disabilitazione delle syscall, presuppone che la voce di tabella syscall 6 (peek) sia stata sostituita dal valore originale (equivale alla voce syscall 0) come fatto da PSNPatch
// ** AVVERTENZA ** Se è stata effettuata solo una disattivazione parziale, questa assunzione AVRA' ESITO NEGATIVO !!!

void aescbc128_decrypt(unsigned char *key, unsigned char *iv, unsigned char *in, unsigned char *out, int len)
{
	aescbccfb_dec(out,in,len,key,128,iv);
	// Reset the IV.
	memset(iv, 0, 0x10);
}

static unsigned char RAP_KEY[] = {0x86, 0x9F, 0x77, 0x45, 0xC1, 0x3F, 0xD8, 0x90, 0xCC, 0xF2, 0x91, 0x88, 0xE3, 0xCC, 0x3E, 0xDF};
static unsigned char RAP_PBOX[] = {0x0C, 0x03, 0x06, 0x04, 0x01, 0x0B, 0x0F, 0x08, 0x02, 0x07, 0x00, 0x05, 0x0A, 0x0E, 0x0D, 0x09};
static unsigned char RAP_E1[] = {0xA9, 0x3E, 0x1F, 0xD6, 0x7C, 0x55, 0xA3, 0x29, 0xB7, 0x5F, 0xDD, 0xA6, 0x2A, 0x95, 0xC7, 0xA5};
static unsigned char RAP_E2[] = {0x67, 0xD4, 0x5D, 0xA3, 0x29, 0x6D, 0x00, 0x6A, 0x4E, 0x7C, 0x53, 0x7B, 0xF5, 0x53, 0x8C, 0x74};
static unsigned char no_exists[] = {"/fail"};

static void get_rif_key(unsigned char* rap, unsigned char* rif)
{
	int i;
	int round;

	unsigned char key[0x10];
	unsigned char iv[0x10];
	memset(key, 0, 0x10);
	memset(iv, 0, 0x10);

	// Initial decrypt.
	aescbc128_decrypt(RAP_KEY, iv, rap, key, 0x10);

	// rap2rifkey round.
	for (round = 0; round < 5; ++round)
	{
		for (i = 0; i < 16; ++i)
		{
			int p = RAP_PBOX[i];
			key[p] ^= RAP_E1[p];
		}
		for (i = 15; i >= 1; --i)
		{
			int p = RAP_PBOX[i];
			int pp = RAP_PBOX[i - 1];
			key[p] ^= key[pp];
		}
		int o = 0;
		for (i = 0; i < 16; ++i)
		{
			int p = RAP_PBOX[i];
			unsigned char kc = key[p] - o;
			unsigned char ec2 = RAP_E2[p];
			if (o != 1 || kc != 0xFF)
			{
				o = kc < ec2 ? 1 : 0;
				key[p] = kc - ec2;
			}
			else if (kc == 0xFF)
			{
				key[p] = kc - ec2;
			}
			else
			{
				key[p] = kc;
			}
		}
	}
	memcpy(rif, key, 0x10);
}

static int read_act_dat_and_make_rif(uint8_t *idps,uint8_t *rap, uint8_t *act_dat, char *content_id, char *out)
{
	uint8_t idps_const[0x10]={0x5E,0x06,0xE0,0x4F,0xD9,0x4A,0x71,0xBF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};
	uint8_t rif_key_const[0x10]={0xDA,0x7D,0x4B,0x5E,0x49,0x9A,0x4F,0x53,0xB1,0xC1,0xA1,0x4A,0x74,0x84,0x44,0x3B};

	uint8_t act_dat_key[0x10];
	uint64_t account_id;
	//uint8_t rifkey[0x10];

	memcpy(&account_id, act_dat+0x8,0x8);
	memcpy(act_dat_key,act_dat+0x10,0x10);

	uint8_t *rif;
	page_allocate_auto(NULL, 0x98, 0x2F, (void *)&rif);

	memset(rif,0,0x98);

	get_rif_key(rap, rif+0x50); //convert rap to rifkey(klicensee)
	uint8_t iv[16];
	memset(iv,0,0x10);

	aescbccfb_enc(idps_const,idps_const,0x10,idps,IDPS_KEYBITS,iv);
	memset(iv,0,0x10);

	aescbccfb_dec(act_dat_key,act_dat_key,0x10,idps_const,IDPS_KEYBITS,iv);
	memset(iv,0,0x10);

	aescbccfb_enc(rif+0x50,rif+0x50,0x10,act_dat_key,ACT_DAT_KEYBITS,iv);
	memset(iv,0,0x10);

	aescbccfb_enc(rif+0x40,rif+0x40,0x10,rif_key_const,RIF_KEYBITS,iv);
	memset(iv,0,0x10);

	uint64_t timestamp=0x000001619BF6DDCA;
	uint32_t version_number=1;
	uint32_t license_type=0x00010002;
	uint64_t expiration_time=0;

	memcpy(rif, &version_number,4);
	memcpy(rif+4, &license_type,4);
	memcpy(rif+8,&account_id,8);
	memcpy(rif+0x10, content_id, 0x24);
	memcpy(rif+0x60, &timestamp, 8);
	memcpy(rif+0x68, &expiration_time,8);
	memset(rif+0x70,1,0x28); //compatible with old cfw patches too

	int fd;
	uint64_t size;
	// uncomment this to avoid hook recursivity & remappings if appropriate
	// lock_mtx(&pgui_mtx);
	if(cellFsOpen(out, CELL_FS_O_WRONLY|CELL_FS_O_CREAT|CELL_FS_O_TRUNC, &fd, 0666, NULL, 0) == 0)
	{
		cellFsWrite(fd, rif, 0x98, &size);
		cellFsClose(fd);
	}
	page_free(NULL, (void *)rif, 0x2F);
		return 0;
}

extern char umd_file;
static uint8_t block_psp_launcher = 0;

LV2_HOOKED_FUNCTION_POSTCALL_2(int, open_path_hook, (char *path0, char *path1))
{
	//extend_kstack(0);
	if(path0) {
		CellFsStat stat;
		#ifdef DEBUG
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
			}
		#endif

		if (block_psp_launcher && !umd_file && !strncmp(path0, "/dev_flash/pspemu", 17))
		{
			block_psp_launcher = 0;
			set_patched_func_param(1, (uint64_t)no_exists);
			return 0;
		}

		if (!strncmp(path0, "/dev_hdd0/game/", 15))
		{
			char *gameid = path0 + 15;

			// block PSP Launchers if PSP umd was not set
			if (!umd_file && (!strncmp(gameid, "PSPC66820/USRDIR", 16) || !strncmp(gameid, "PSPM66820/USRDIR", 16)))
			{
				block_psp_launcher = 1;
			}

			// syscalls are disabled and an EBOOT.BIN is being called from hdd. Let's test it.
			int syscalls_disabled = ((*(uint64_t *)MKA(syscall_table_symbol + 8 * 6)) == (*(uint64_t *)MKA(syscall_table_symbol)));

			if (syscalls_disabled && strstr(path0 + 15, "/EBOOT.BIN"))
			{
				// flag "whitelist" id's
				int allow =
					!strncmp(gameid, "NP", 2) ||
					!strncmp(gameid, "BL", 2) ||
					!strncmp(gameid, "BC", 2) ||
					!strncmp(gameid, "KOEI3", 5) ||
					!strncmp(gameid, "KTGS3", 5) ||
					!strncmp(gameid, "MRTC0", 5) ||
					!strncmp(gameid, "ASIA0", 5) ||
					!strncmp(gameid, "_DEL_", 5) || // Fix data corruption if you uninstall game/game update/homebrew with syscall disabled # Alexander's
					!strncmp(gameid, "_INST_", 6) || // 80010006 error fix when trying to install a game update with syscall disabled. # Joonie's, Alexander's, Aldo's
					!strncmp(gameid, "GUST0", 5) ;

				// flag some "blacklist" id's
				if (
					!strncmp(gameid, "BLES806", 7) || // Multiman and assorted tools are in the format BLES806**
					!strncmp(gameid, "BLJS10018", 9) || // PSNPatch Stealth (older versions were already detected as non-NP/BC/BL)
					!strncmp(gameid, "BLES08890", 9) || // PSNope by user
					!strncmp(gameid, "BLES13408", 9) || // FCEU NES Emulator
					!strncmp(gameid, "BLES01337", 9) || // Awesome File Manager
					!strncmp(gameid, "BLND00001", 9) || // dev_blind
					!strncmp(gameid, "NPEA90124", 9) || // SEN Enabler
					!strncmp(gameid, "NP0", 3)			// NP0APOLLO / NP00PKGI3
					) allow = 0;

				// test whitelist.cfg and blacklist.cfg
				if (listed(0, gameid)) // whitelist.cfg test
					allow = 1;
				if (listed(1, gameid)) // blacklist.cfg test
					allow = 0;

				// let's now block homebrews if the "allow" flag is false
				if (!allow)
				{
					set_patched_func_param(1, (uint64_t)no_exists);
					return 0;
				}
			}
		}
		else if((!strncmp(path0,"/dev_hdd0/home/", 14)) && (strstr(path0,".rif")))
		{
			char content_id[0x24];
			strncpy(content_id, strrchr(path0,'/')+1, 0x24);
			char *buf;
			page_allocate_auto(NULL, 0x60, 0x2F, (void *)&buf);
			memset(buf,0,0x60);

			// // Template code to detect usb/sd cards mount points
			// // cannot use the syscalls directly though as they expect user space pointers, not kernel memory offsets
			// // we need to reverse engineer the 2 syscalls...
			// f_desc_t f;
			// f.addr=(void*)MKA(get_syscall_address(0x349));
			// f.toc=(void*)MKA(TOC);
			// int (*sys_fs_get_mount_info_size)(uint64_t* out_length) =(void*)&f;
			// f_desc_t f2;
			// f2.addr=(void*)MKA(get_syscall_address(0x34A));
			// f2.toc=(void*)MKA(TOC);
			// int (*sys_fs_get_mount_info)(CellFsMountInformation_t* info, uint64_t buffer_length, uint64_t* written_length) =(void*)&f2;
			// uint64_t minfo_size = 0;
			// uint64_t mcount = 0;
			// int path_chk = 1;
			// int misret = sys_fs_get_mount_info_size(&minfo_size);
			// if(misret==0) {
				// CellFsMountInformation_t *minfo = alloc(minfo_size*sizeof(CellFsMountInformation_t),0x27);
				// CellFsMountInformation_t *iterator;
				// int miret = sys_fs_get_mount_info(minfo,minfo_size,&mcount);
				// if(miret==0) {
					// for(int i=1;i<mcount;i++) {
						// iterator=minfo+i;
						// if(strcmp(iterator->p_type,"CELL_FS_FAT") == 0 && strcmp(iterator->p_systype,"CELL_FS_IOS:USB_MASS_STORAGE000") == 0 && iterator->p_writable==0) {
							// sprintf(buf,"%s/exdata/%.36s.rap",iterator->p_name,content_id);
							// path_chk=cellFsStat(buf,&stat);
							// if(path_chk==0) {
								// break;
							// }
						// }

					// }
				// }
				// iterator=NULL;
				// dealloc(minfo,0x27);
			// }

			// Support for rap and RAP extension (By aldostools)
			int path_chk;
			const char *ext = "rap";
			for(uint8_t retry = 0; retry < 2; retry++)
			{

				// hard coded usb path 000 & 001 should be replaced with usb mount points detection!!!
				sprintf(buf,"/dev_usb000/exdata/%.36s.%s",content_id, ext);
				// uncomment this to avoid hook recursivity & remappings if appropriate
				//lock_mtx(&pgui_mtx);
				path_chk = cellFsStat(buf,&stat);
				if(path_chk)
				{
					sprintf(buf,"/dev_usb001/exdata/%.36s.%s",content_id, ext);
					// uncomment this to avoid hook recursivity & remappings if appropriate
					//lock_mtx(&pgui_mtx);
					path_chk=cellFsStat(buf,&stat);
				}
				if(path_chk)
				{
					sprintf(buf,"/dev_hdd0/exdata/%.36s.%s",content_id, ext);
					// uncomment this to avoid hook recursivity & remappings if appropriate
					//lock_mtx(&pgui_mtx);
					path_chk=cellFsStat(buf,&stat);
				}
				if(path_chk)
					ext = "RAP";
				else
					break;
			}

			uint8_t is_ps2_classic = !strncmp(content_id, "2P0001-PS2U10000_00-0000111122223333", 0x24);
			uint8_t is_psp_launcher_c = !strncmp(content_id, "UP0001-PSPC66820_00-0000111122223333", 0x24);
			uint8_t is_psp_launcher_m = !strncmp(content_id, "UP0001-PSPM66820_00-0000111122223333", 0x24);
			if(path_chk == 0 || is_ps2_classic || is_psp_launcher_c || is_psp_launcher_m)
			{
				uint8_t rap[0x10] = {0xF5, 0xDE, 0xCA, 0xBB, 0x09, 0x88, 0x4F, 0xF4, 0x02, 0xD4, 0x12, 0x3C, 0x25, 0x01, 0x71, 0xD9};
				uint8_t idps[0x10];
				char *act_path;
				page_allocate_auto(NULL, 0x60, 0x2F, (void *)&act_path);
				memset(act_path,0,0x60);

				char act_dat_name[0x10]={"/act.dat"};
				strncpy(act_path, path0, strrchr(path0,'/')-path0);
				strcpy(act_path+strlen(act_path),act_dat_name);
				char output[0x60];
				sprintf(output, "/%s",path0);

				#ifdef DEBUG
					//DPRINTF("act_path:%s content_id:%s output:%s rap_path:%s\n",act_path,content_id,output,buf);
				#endif

				uint8_t *act_dat;
				page_allocate_auto(NULL, 0x1038, 0x2F, (void *)&act_dat);
				int fd;
				uint64_t nread;

				if(!is_ps2_classic || is_psp_launcher_c || is_psp_launcher_m)					
				{
					// uncomment this to avoid hook recursivity & remappings if appropriate
					//lock_mtx(&pgui_mtx);
					if(cellFsOpen(buf, CELL_FS_O_RDONLY, &fd, 0, NULL, 0) == 0)
					{
						cellFsRead(fd, rap, 0x10, &nread);
						cellFsClose(fd);
					}
				}
				// uncomment this to avoid hook recursivity & remappings if appropriate
				//lock_mtx(&pgui_mtx);
				if(cellFsOpen(act_path, CELL_FS_O_RDONLY, &fd, 0, NULL, 0) == 0)
				{
					cellFsRead(fd, act_dat, 0x1038, &nread);
					cellFsClose(fd);
				}

				page_free(NULL, (void *)act_path, 0x2F);
				page_free(NULL, (void *)buf, 0x2F);

				memcpy(idps,(void*)PS3MAPI_IDPS_2,0x10);
				if(nread == 0x1038)
				{
					read_act_dat_and_make_rif(idps,rap,act_dat,content_id,output);
				}
				page_free(NULL, (void *)act_dat, 0x2F);
			}
			else
			{
				page_free(NULL, (void *)buf, 0x2F);
			}
		}
		#ifdef DEBUG
			//DPRINTF("open_path_hook=: processing path [%s]\n", path0);
		#endif

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
		if(path && (strncmp(path,"/dev_",5) == 0 || strncmp(path,"/app_",5) == 0 || strncmp(path,"/host_",6) == 0)) {
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
							//lock_mtx(&pgui_mtx)
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
						DPRINTF("open_path_hook:= found matching entry for %s in Map Table oldpath: [%s] \nMap Table newpath: [%s] \nMap Table newpath_len: [0x%x]\n",path,curr->oldpath,curr->newpath,(unsigned int)curr->newpath_len);
					#endif
				}
				else{
					#ifdef DEBUG
						//DPRINTF("open_path_hook=: no mapping found for path [%s]\n", path);
					#endif
				}
				unlock_mtx(&map_mtx);
			}
		}
	}
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
