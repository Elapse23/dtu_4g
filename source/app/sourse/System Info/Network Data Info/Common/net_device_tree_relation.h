#ifndef _NET_DEVICE_TREE_RELASTION_H_
#define _NET_DEVICE_TREE_RELASTION_H_

#include <stdint.h>

#define cb_device_tree_element_subset_max_number    20
#define cb_net_device_tree_info_system_index		0
#define cb_net_device_tree_info_master_index		1

typedef struct
{
    uint32_t *pdw_master_device_id;
    struct subset_device_id
    {
        uint8_t b_number;
        uint32_t *pdw_subset_id_list[cb_device_tree_element_subset_max_number];
    }st_subset_device_id;
}net_device_tree_element_info;

typedef struct
{
    uint8_t b_number;
    net_device_tree_element_info *pst_device_tree_element_info;
}net_device_tree_element_info_list;

#endif
