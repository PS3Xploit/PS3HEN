/* Keep this file language agnostic. Only preprocessor here. */

#ifndef __FIRMWARE_SYMBOLS_H_S__
#define __FIRMWARE_SYMBOLS_H_S__

#if defined(FIRMWARE_4_84)

#define TOC 											0x34FBB0

#define create_kernel_object_symbol						0x11FB0
#define destroy_kernel_object_symbol					0x11914
#define destroy_shared_kernel_object_symbol				0x11288
#define open_kernel_object_symbol						0x12600
#define open_shared_kernel_object_symbol				0x12410
#define close_kernel_object_handle_symbol				0x11A30

#define alloc_symbol									0x64824
#define dealloc_symbol									0x64C60
#define copy_to_user_symbol								0xF86C
#define copy_from_user_symbol							0xFA88
#define copy_to_process_symbol							0xF924
#define copy_from_process_symbol						0xF734
#define page_allocate_symbol							0x60394
#define page_free_symbol								0x5FDF8
#define page_export_to_proc_symbol						0x60530
#define page_unexport_from_proc_symbol					0x5FCEC
#define kernel_ea_to_lpar_addr_symbol					0x7014C
#define process_ea_to_lpar_addr_ex_symbol				0x77760
#define set_pte_symbol                      			0x5E390
#define map_process_memory_symbol						0x7726C
#define panic_symbol									0x26D5C8

#define memcpy_symbol									0x7E92C
#define memset_symbol									0x4D66C
#define memcmp_symbol									0x4C97C
#define memchr_symbol									0x4C92C
#define printf_symbol									0x2706AC
#define printfnull_symbol								0x275120
#define sprintf_symbol				    				0x4EA94
#define snprintf_symbol				    				0x4EA00
#define strcpy_symbol									0x4D818
#define strncpy_symbol									0x4D8E0
#define strlen_symbol									0x4D840
#define strcat_symbol									0x4D748
#define strcmp_symbol									0x4D7C4
#define strncmp_symbol									0x4D86C
#define strchr_symbol									0x4D780
#define strrchr_symbol									0x4D950

#define spin_lock_irqsave_ex_symbol						0x26D798
#define spin_unlock_irqrestore_ex_symbol				0x26D76C

#define create_process_common_symbol					0x26B534
#define create_process_base_symbol						0x26ABF0
#define load_process_symbol								0x5004
#define process_kill_symbol								0x26B340

#define ppu_thread_create_symbol						0x13EC8
#define ppu_thread_exit_symbol							0x13F80
#define ppu_thread_join_symbol							0x13FD4
#define ppu_thread_delay_symbol							0x287A4
#define create_kernel_thread_symbol						0x24B20
#define create_user_thread_symbol						0x2525C
#define create_user_thread2_symbol						0x25080
#define start_thread_symbol								0x23D4C
#define run_thread_symbol								0x2357C
#define register_thread_symbol							0x26794C
#define allocate_user_stack_symbol						0x268134
#define deallocate_user_stack_symbol					0x26809C

#define mutex_create_symbol								0x1363C
#define mutex_destroy_symbol							0x135D4
#define mutex_lock_symbol								0x135CC
#define mutex_lock_ex_symbol							0x1DB5C
#define mutex_trylock_symbol							0x135C8
#define mutex_unlock_symbol								0x135C4

#define cond_create_symbol								0x137A8
#define cond_destroy_symbol								0x13758
#define cond_wait_symbol								0x13750
#define cond_wait_ex_symbol								0x1F148
#define cond_signal_symbol								0x1372C
#define cond_signal_all_symbol							0x13708

#define semaphore_initialize_symbol						0x30C54
#define semaphore_wait_ex_symbol						0x3095C
#define semaphore_trywait_symbol						0x30544
#define semaphore_post_ex_symbol						0x30690

#define event_port_create_symbol						0x13078
#define event_port_destroy_symbol						0x134E0
#define event_port_connect_symbol						0x13558
#define event_port_disconnect_symbol					0x13484
#define event_port_send_symbol							0x13070
#define event_port_send_ex_symbol						0x2B204

#define event_queue_create_symbol						0x13380
#define event_queue_destroy_symbol						0x13308
#define event_queue_receive_symbol						0x1314C
#define event_queue_tryreceive_symbol					0x13218

#define cellFsOpen_symbol								0x297900
#define cellFsClose_symbol								0x297768
#define cellFsRead_symbol								0x2978A4
#define cellFsWrite_symbol								0x297810
#define cellFsLseek_symbol								0x296E98
#define cellFsStat_symbol								0x29711C
#define cellFsUtime_symbol 								0x298C08
#define cellFsUnlink_internal_symbol   		 			0x19C64C

#define cellFsUtilMount_symbol							0x296C78
#define cellFsUtilUmount_symbol 						0x296C4C
#define cellFsUtilNewfs_symbol							0x2986A0

#define pathdup_from_user_symbol						0x29C8E4
#define open_path_symbol								0x297638
#define open_fs_object_symbol							0x18A970
#define close_fs_object_symbol							0x1898AC

#define storage_get_device_info_symbol					0x279B00
#define storage_get_device_config_symbol				0x278F90
#define storage_open_symbol								0x279510
#define storage_close_symbol							0x279300
#define storage_read_symbol								0x278870
#define storage_write_symbol							0x278740
#define storage_send_device_command_symbol				0x2783FC
#define storage_map_io_memory_symbol					0x2799BC
#define storage_unmap_io_memory_symbol		    		0x279888
#define new_medium_listener_object_symbol				0x974C4
#define delete_medium_listener_object_symbol			0x98CFC
#define set_medium_event_callbacks_symbol				0x99060

#define cellUsbdRegisterLdd_symbol						0x26027C
#define cellUsbdUnregisterLdd_symbol					0x26022C
#define cellUsbdScanStaticDescriptor_symbol				0x26147C
#define cellUsbdOpenPipe_symbol			        		0x26152C
#define cellUsbdClosePipe_symbol						0x2614DC
#define cellUsbdControlTransfer_symbol					0x2613E0
#define cellUsbdBulkTransfer_symbol						0x261360

#define decrypt_func_symbol								0x34970
#define lv1_call_99_wrapper_symbol						0x4EEA8
#define modules_verification_symbol						0x58AB8
#define authenticate_program_segment_symbol				0x5A910

#define prx_load_module_symbol							0x88C04
#define prx_start_module_symbol							0x878D0
#define prx_stop_module_symbol							0x88CA8
#define prx_unload_module_symbol						0x87604
#define prx_get_module_info_symbol						0x8708C
#define prx_get_module_id_by_address_symbol				0x86F9C
#define prx_get_module_id_by_name_symbol				0x86FEC
#define prx_get_module_list_symbol						0x8710C
#define load_module_by_fd_symbol						0x88234
#define parse_sprx_symbol								0x85FB8
#define open_prx_object_symbol							0x7F4F8
#define close_prx_object_symbol							0x7FE08
#define lock_prx_mutex_symbol							0x86F44
#define unlock_prx_mutex_symbol							0x86F50

#define extend_kstack_symbol							0x700A4

#define get_pseudo_random_number_symbol					0x228180
#define md5_reset_symbol								0x161E20
#define md5_update_symbol								0x1628C0
#define md5_final_symbol								0x162A40
#define ss_get_open_psid_symbol							0x22A668
#define update_mgr_read_eeprom_symbol					0x223A78
#define update_mgr_write_eeprom_symbol					0x2239C0

#define ss_params_get_update_status_symbol				0x50AB4

#define syscall_table_symbol							0x363BE0
#define syscall_call_offset								0x275C04

#define read_bdvd0_symbol								0x1A461C
#define read_bdvd1_symbol								0x1A6248
#define read_bdvd2_symbol								0x1B3400

#define storage_internal_get_device_object_symbol		0x277EB4

#define hid_mgr_read_usb_symbol	    					0x102444
#define hid_mgr_read_bt_symbol							0xFC31C

#define bt_set_controller_info_internal_symbol			0xF0388

/* Calls, jumps */
#define device_event_port_send_call						0x28206C

#define ss_pid_call_1									0x21329C

#define process_map_caller_call							0x4D24

#define read_module_header_call							0x7F3EC
#define read_module_body_call							0x671C
#define load_module_by_fd_call1							0x88AA0

#define shutdown_copy_params_call						0xAACC

#define fsloop_open_call								0x297A98
#define fsloop_close_call								0x297AE8
#define fsloop_read_call								0x297B28

/* Patches */
#define shutdown_patch_offset							0xAAB8
#define module_sdk_version_patch_offset					0x26717C
#define module_add_parameter_to_parse_sprxpatch_offset	0x88310

#define user_thread_prio_patch							0x203FC
#define user_thread_prio_patch2							0x20408

#define lic_patch										0x56230
#define ode_patch										0x2275F4

/* Rtoc entries */

#define io_rtoc_entry_1									0x25B8
#define io_sub_rtoc_entry_1								-0x7EA0
#define decrypt_rtoc_entry_2 							-0x66A0
#define decrypter_data_entry							-0x7F10

#define storage_rtoc_entry_1							0x1DD8

#define device_event_rtoc_entry_1						0x2048

#define time_rtoc_entry_1								-0x7640
#define time_rtoc_entry_2								-0x7648

#define thread_rtoc_entry_1								-0x76C0

#define process_rtoc_entry_1							-0x7800

#define bt_rtoc_entry_1									-0x3680

/* Permissions */
#define permissions_func_symbol							0x3560
#define permissions_exception1							0x25100
#define permissions_exception2							0xC3298
#define permissions_exception3							0x204D0

/* Legacy patches with no names yet */
/* Kernel offsets */
#define patch_data1_offset								0x3E2E40
#define patch_func2_offset 								0x59DB0
#define patch_func8_offset1 							0x564B0
#define patch_func8_offset2 							0x56614
#define patch_func9_offset 								0x5A6F8
#define mem_base2										0x3D90

/* vars */
#define thread_info_symbol								0x37DE30

#endif /* FIRMWARE */

#endif /* __FIRMWARE_SYMBOLS_H_S__ */
