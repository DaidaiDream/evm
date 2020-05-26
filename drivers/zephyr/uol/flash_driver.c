/****************************************************************************
**
** Copyright (C) 2020 @scriptiot
**
**  EVM是一款通用化设计的虚拟机引擎，拥有语法解析前端接口、编译器、虚拟机和虚拟机扩展接口框架。
**  支持js、python、qml、lua等多种脚本语言，纯Ｃ开发，零依赖，内置REPL，支持主流 ROM > 40KB, RAM > 2KB的MCU;
**  自带垃圾回收（GC）先进的内存管理，采用最复杂的压缩算法，无内存碎片（大部分解释器都存在内存碎片）
**  Version	: 1.0
**  Email	: scriptiot@aliyun.com
**  Website	: https://github.com/scriptiot/evm
**            https://gitee.com/scriptiot/evm
**  Licence: Apache-2.0
****************************************************************************/

#include "nevm.h"

#ifdef CONFIG_EVM_FLASH
#include <device.h>
#include <drivers/flash.h>
#endif

//flash_config(char pin, char flags)
evm_val_t nevm_driver_flash_config(evm_t * e, evm_val_t * p, int argc, evm_val_t * v){
    (void)e;(void)p;(void)argc;(void)v;
    return NEVM_TRUE;
}

//flash_init(String name)
evm_val_t nevm_driver_flash_init(evm_t * e, evm_val_t * p, int argc, evm_val_t * v){
    (void)e;(void)p;
#ifdef CONFIG_EVM_FLASH
    if( argc > 0 && evm_is_string(v) ){
		struct device * dev = device_get_binding(evm_2_string(v));
        if( !dev ) return NEVM_FALSE;
        nevm_object_set_ext_data(p, (intptr_t)dev);
        return NEVM_TRUE;
    }
#endif
    return NEVM_FALSE;
}

//flash_deinit(char pin, char flags)
evm_val_t nevm_driver_flash_deinit(evm_t * e, evm_val_t * p, int argc, evm_val_t * v){
    (void)e;(void)p;(void)argc;(void)v;
    return EVM_VAL_UNDEFINED;
}

//flash_read(offset, data, len)
evm_val_t nevm_driver_flash_read(evm_t * e, evm_val_t * p, int argc, evm_val_t * v){
    (void)e;(void)p;(void)argc;(void)v;
#ifdef CONFIG_EVM_FLASH
    if( argc > 2 && evm_is_number(v) && evm_is_buffer(v + 1) && evm_is_number(v + 2)){
        struct device * dev = (struct device *)nevm_object_get_ext_data(p);
        if( !dev ) return NEVM_FALSE;
        int data_len = evm_buffer_len(v + 1);
        int len = evm_2_integer(v + 2);
        len = len < data_len ? len : data_len;
        int err = flash_read(dev, evm_2_integer(v), evm_buffer_addr(v + 1), len);
        if (err){
            return NEVM_FALSE;
        }
        return NEVM_TRUE;
    }
#endif
    return NEVM_FALSE;
}
//flash_write(offset, data, len)
evm_val_t nevm_driver_flash_write(evm_t * e, evm_val_t * p, int argc, evm_val_t * v){
    (void)e;(void)p;
#ifdef CONFIG_EVM_FLASH
    if( argc > 2 && evm_is_number(v) && evm_is_buffer(v + 1) && evm_is_number(v + 2)){
        struct device * dev = (struct device *)nevm_object_get_ext_data(p);
        if( !dev ) return NEVM_FALSE;
        int err = flash_write_protection_set(dev, false);
        if (err){
            return NEVM_FALSE;
        }
        int data_len = evm_buffer_len(v + 1);
        int len = evm_2_integer(v + 2);
        len = len < data_len ? len : data_len;
        
        err = flash_write(dev, evm_2_integer(v), evm_buffer_addr(v + 1) , len);
        if (err){
            return NEVM_FALSE;
        }
        return NEVM_TRUE;
    }
#endif
    return NEVM_FALSE;
}
//flash_erase(offset, size)
evm_val_t nevm_driver_flash_erase(evm_t * e, evm_val_t * p, int argc, evm_val_t * v){
    (void)e;(void)p;
#ifdef CONFIG_EVM_FLASH
    if( argc > 1 && evm_is_number(v) && evm_is_number(v + 1) ){
        struct device * dev = (struct device *)nevm_object_get_ext_data(p);
        if( !dev ) return NEVM_FALSE;
        int err = flash_write_protection_set(dev, false);
        if (err){
            return NEVM_FALSE;
        }
        err = flash_erase(dev, evm_2_integer(v), evm_2_integer(v+1));
        if (err){
            return NEVM_FALSE;
        }
        return NEVM_TRUE;
    }
#endif
    return NEVM_FALSE;
}

//flash_write_block_size()
evm_val_t nevm_driver_flash_write_block_size(evm_t * e, evm_val_t * p, int argc, evm_val_t * v){
    (void)e;(void)p;
#ifdef CONFIG_EVM_FLASH
    struct device * dev = (struct device *)nevm_object_get_ext_data(p);
    if( !dev ) return NEVM_FALSE;
    return evm_mk_number(flash_get_write_block_size(dev));
#endif
    return EVM_VAL_UNDEFINED;
}