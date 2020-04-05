#ifndef _PAGING_H
#define _PAGING_H

#include "types.h"
#include "x86_desc.h"

#define TABLE_SIZE          0x400
#define PAGE_SIZE           0x1000
#define VIDEO_LOC           0xB8000
#define KERNEL_LOC          0x400000

#define PDE_P               0x1
#define PDE_RW              0x2
#define PDE_US              0x4
#define PDE_PWT             0x8
#define PDE_PCD             0x10
#define PDE_A               0x20
#define PDE_4MP_D           0x40
#define PDE_PS              0x80
#define PDE_G               0x100
#define PDE_4KPT_Avail      0x200
#define PDE_4MP_PAT         0x200
#define PDE_4KPT_PBA_OFFSET 12
#define PDE_4MP_PBA_OFFSET  22

#define PTE_P               0x1
#define PTE_RW              0x2
#define PTE_US              0x4
#define PTE_PWT             0x8
#define PTE_PCD             0x10
#define PTE_A               0x20
#define PTE_D               0x40
#define PTE_PAT             0x80
#define PTE_G               0x100
#define PTE_Avail           0x200
#define PTE_PBA_OFFSET      12

#define DEFAULT_PTE         PTE_RW | PTE_P

#define PROCESS_LOC(n)      (n+2) * PAGE_SIZE

#define USER_PROCESS_INDEX  32
void init_paging();

#endif
