// Saved tests used in lv2 kernel for archival purposes


//Rename Test
DPRINTF("Folder Test - cellFsMkdir returned: 0x%X\n", cellFsMkdir("/dev_hdd0/hen_test", CELL_FS_S_IFDIR | 0777));
DPRINTF("Folder Test - cellFsRename returned: 0x%X\n", cellFsRename("/dev_hdd0/hen_test", "/dev_hdd0/hen_test_kernel"));

int fd;
DPRINTF("File Test - cellFsOpen returned: 0x%X\n", cellFsOpen("/dev_hdd0/hen.test", CELL_FS_O_CREAT | CELL_FS_O_RDWR, &fd, 0777, NULL, 0));
DPRINTF("File Test - cellFsClose returned: 0x%X\n", cellFsClose(fd));
DPRINTF("File Test - cellFsRename returned: 0x%X\n", cellFsRename("/dev_hdd0/hen.test", "/dev_hdd0/hen.test_kernel"));