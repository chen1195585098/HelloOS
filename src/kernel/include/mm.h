#ifndef _KERNEL_MM_H
#define _KERNEL_MM_H

#include "types.h"
#include "list.h"

#define MEMORY_AVAILABLE              1
#define MEMORY_RESERVED               2
#define MEMORY_ACPI_RECLAIMABLE       3
#define MEMORY_NVS                    4
#define MEMORY_BADRAM                 5

#define PAGE_4K_SHIFT                 12
#define PAGE_4K_SIZE                  (1UL << PAGE_4K_SHIFT)
#define PAGE_4K_MASK                  (~(PAGE_4K_SIZE - 1))
#define PAGE_4K_ALIGN(addr)           (((uint_t)(addr) + PAGE_4K_SIZE - 1) & PAGE_4K_MASK)

typedef struct {
    uint_t      addr;
    uint_t      len;
    uint32_t    type;
}__attribute__((packed)) E820_t;

#define MF_OLKTY_INIT (0)
#define MF_OLKTY_ODER (1)
#define MF_OLKTY_BAFH (2)
#define MF_OLKTY_TOBJ (3)

#define MF_LSTTY_LIST (1)

#define MF_MOCTY_FREE (0)
#define MF_MOCTY_KRNL (1)
#define MF_MOCTY_USER (2)

#define MF_MRV1_VAL (0)

#define MF_UINDX_INIT (0)
#define MF_UINDX_MAX (0xffffff)

#define MF_MARTY_INIT (0)
#define MF_MARTY_HWD (1)
#define MF_MARTY_KRL (2)
#define MF_MARTY_PRC (3)
#define MF_MARTY_SHD (4)
/**
 * memory space address flags
 */
typedef struct {
    uint32_t    mf_olkty:2;         // 链表类型
    uint32_t    mf_lstty:1;         // 是否挂入链表
    uint32_t    mf_mocty:2;         // 分配类型，内核/应用程序/空闲
    uint32_t    mf_marty:3;         // 所属区
    uint32_t    mf_uindx:24;        // 分配计数
}__attribute__((packed)) msadrflgs_t;

/**
 * pyhsical address flags
 */
typedef struct {
    uint64_t    pf_alloc:1;         // 分配位
    uint64_t    pf_shared:1;        // 共享位
    uint64_t    pf_swap:1;          // 交换位
    uint64_t    pf_cache:1;         // 缓存位
    uint64_t    pf_kmap:1;          // 映射位
    uint64_t    pf_lock:1;          // 锁定位
    uint64_t    pf_dirty:1;         // 脏位
    uint64_t    pf_busy:1;          // 忙位
    uint64_t    pf_rv2:4;           // 保留位
    uint64_t    pf_phyadr;          // 页物理地址
}__attribute__((packed)) phyadrflgs_t;

/**
 * memory space address descriptor
 */
typedef struct {
    list_head_t     md_list;            // 链表
    msadrflgs_t     md_flgs;            // 内存空间地址描述符标志
    phyadrflgs_t    md_phyadr;          // 物理地址及标志
}__attribute__((packed)) msadrdsc_t;

/**
 * memory area
 */
typedef struct {
    list_head_t ma_list;            // 内存区链表
    uint_t      ma_stus;            // 内存区状态
    uint_t      ma_flgs;            // 内存区标志
    uint_t      ma_type;            // 内存区类型
    uint_t      ma_pgsum;           // 内存区总页数
    uint_t      ma_allocpg;         // 内存区已分配页数
    uint_t      ma_freepg;          // 内存区空闲页数
    uint_t      ma_resvpg;          // 内存区保留页数
    uint_t      ma_horizline;       // 内存区分配时的水位线
    adr_t       ma_startadr;        // 内存区开始地址
    adr_t       ma_endadr;          // 内存区结束地址
    uint_t      ma_sz;              // 内存区大小
} memarea_t;

/**
 * block alloc free head list
 */
typedef struct {
    uint32_t    af_stus;            // 状态
    uint_t      af_oder;            // 页面数的位移量
    uint_t      af_oderpgnr;        // 位移量对应的页面数，位移量为 2，则值为 1<<2=4
    uint_t      af_objnr;           // msadrdsc_t 总数
    uint_t      af_freeobjnr;       // 空闲 msadrdsc_t 数
    uint_t      af_allocindx;       // bafhlst_t 分配计数
    uint_t      af_freeindx;        // bafhlst_t 释放计数
    list_head_t af_freelst;         // 空闲 msadrdsc_t 链表
    list_head_t af_alloclst;        // 已分配 msadrdsc_t 链表
} bafhlst_t;

typedef struct {
    E820_t      e820[32];
    uint_t      e820_num;
} global_mm_descriptor_t;

global_mm_descriptor_t gmdsc;

void init_memory();

#endif