// Saved tests used in henplugin for archival purposes


//Rename Test
DPRINTF("Folder Test - sysLv2FsMkdir returned: 0x%X\n", sysLv2FsMkdir("/dev_hdd0/hen_test", CELL_FS_S_IFDIR | 0777));
DPRINTF("Folder Test - sysLv2FsRename returned: 0x%X\n", sysLv2FsRename("/dev_hdd0/hen_test", "/dev_hdd0/hen_test_user"));

int fd;
DPRINTF("File Test - cellFsOpen returned: 0x%X\n", cellFsOpen("/dev_hdd0/hen.test", CELL_FS_O_CREAT | CELL_FS_O_RDWR, &fd, NULL, 0));
DPRINTF("File Test - cellFsClose returned: 0x%X\n", cellFsClose(fd));
DPRINTF("File Test - sysLv2FsRename returned: 0x%X\n", sysLv2FsRename("/dev_hdd0/hen.test", "/dev_hdd0/hen.test_user"));