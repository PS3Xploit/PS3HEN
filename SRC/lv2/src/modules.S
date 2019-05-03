#include <lv2/macros.h.S>
#include <lv2/symbols.h>

#ifdef decrypt_func_symbol
LV2_FUNCTION(decrypt_func, decrypt_func_symbol)
#endif

LV2_FUNCTION(authenticate_program_segment, authenticate_program_segment_symbol)

#ifdef prx_load_module_symbol
LV2_FUNCTION(prx_load_module, prx_load_module_symbol)
#endif

#ifdef prx_start_module_symbol
LV2_FUNCTION(prx_start_module, prx_start_module_symbol)
#endif

#ifdef prx_stop_module_symbol
LV2_FUNCTION(prx_stop_module, prx_stop_module_symbol)
#endif

#ifdef prx_unload_module_symbol
LV2_FUNCTION(prx_unload_module, prx_unload_module_symbol)
#endif

#ifdef prx_get_module_info_symbol
LV2_FUNCTION(prx_get_module_info, prx_get_module_info_symbol)
#endif

#ifdef prx_get_module_id_by_address_symbol
LV2_FUNCTION(prx_get_module_id_by_address, prx_get_module_id_by_address_symbol)
#endif

#ifdef prx_get_module_id_by_name_symbol
LV2_FUNCTION(prx_get_module_id_by_name, prx_get_module_id_by_name_symbol)
#endif

#ifdef prx_get_module_list_symbol
LV2_FUNCTION(prx_get_module_list, prx_get_module_list_symbol)
#endif

#ifdef open_prx_object_symbol
LV2_FUNCTION(open_prx_object, open_prx_object_symbol)
#endif

#ifdef close_prx_object_symbol
LV2_FUNCTION(close_prx_object, close_prx_object_symbol)
#endif

#ifdef lock_prx_mutex_symbol
LV2_FUNCTION(lock_prx_mutex, lock_prx_mutex_symbol)
LV2_FUNCTION(unlock_prx_mutex, unlock_prx_mutex_symbol)
#endif






