#ifndef _NET_ATTRIBUTE_TREE_BUILD_H_
#define _NET_ATTRIBUTE_TREE_BUILD_H_

#include <stdint.h>
#include "system_info_general_macro.h"
#include "net_system_info.h"
#include "application_general_func.h"

typedef enum
{
    cb_net_attribute_tree_type_none = 0,
    cb_net_attribute_tree_type_offline,
    cb_net_attribute_tree_type_set_modify,
}net_attribute_tree_type;

uint8_t net_attribute_tree_build(uint16_t *pw_data_out_len,
                                 uint8_t *pb_data_out,
                                 uint16_t w_data_out_buffer_max_len,
                                 net_system_info_list *pst_info_list,
                                 uint8_t b_attribute_tree_type);

#endif
