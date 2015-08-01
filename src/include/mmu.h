#ifndef MMU_H
#define MMU_H
// A linear address 'la' has a three-part structure as follows:
//
// +--------10------+-------10-------+---------12----------+
// | Page Directory |   Page Table   | Offset within Page  |
// |      Index     |      Index     |                     |
// +----------------+----------------+---------------------+
//  \--- PDX(la) --/ \--- PTX(la) --/ \---- PGOFF(la) ----/
//  \---------- PGNUM(la) ----------/
//
// The PDX, PTX, PGOFF, and PGNUM macros decompose linear addresses as shown.
// To construct a linear address la from PDX(la), PTX(la), and PGOFF(la),
// use PGADDR(PDX(la), PTX(la), PGOFF(la)).

// page number field of address
#define PGNUM(la)    (((uintptr_t) (la)) >> PTXSHIFT)

// page directory index
#define PDX(la)      ((((uintptr_t) (la)) >> PDXSHIFT) & 0x3FF)

// page table index
#define PTX(la)      ((((uintptr_t) (la)) >> PTXSHIFT) & 0x3FF)

// offset in page
#define PGOFF(la)    (((uintptr_t) (la)) & 0xFFF)

// construct linear address from indexes and offset
#define PGADDR(d, t, o)  ((void*) ((d) << PDXSHIFT | (t) << PTXSHIFT | (o)))

// Page directory and page table constants.
#define NPDENTRIES   1024        // page directory entries per page directory
#define NPTENTRIES   1024        // page table entries per page table

#define PGSIZE       4096        // bytes mapped by a page
#define PGSHIFT      12      // log2(PGSIZE)

#define PTSIZE       (PGSIZE*NPTENTRIES) // bytes mapped by a page directory entry
#define PTSHIFT      22      // log2(PTSIZE)

#define PTXSHIFT 12      // offset of PTX in a linear address
#define PDXSHIFT 22      // offset of PDX in a linear address

/*
 * struct pte {
 *  uint flag:9,
 *  uint avl: 3,
 *  uint off: 20
 * }
 */
typedef uint32_t pte_t;
typedef uint32_t pde_t;
// Page table/directory entry flags.
#define PTE_P        0x001   // Present
#define PTE_W        0x002   // Writeable
#define PTE_U        0x004   // User
#define PTE_PWT      0x008   // Write-Through
#define PTE_PCD      0x010   // Cache-Disable
#define PTE_A        0x020   // Accessed
#define PTE_D        0x040   // Dirty
#define PTE_PS       0x080   // Page Size
#define PTE_G        0x100   // Global

// The PTE_AVAIL bits aren't used by the kernel or interpreted by the
// hardware, so user processes are allowed to set them arbitrarily.
#define PTE_AVAIL    0xE00   // Available for software use

// Flags in PTE_SYSCALL may be used in system calls.  (Others may not.)
#define PTE_SYSCALL  (PTE_AVAIL | PTE_P | PTE_W | PTE_U)

// Address in page table or page directory entry
#define PTE_ADDR(pte)    ((physaddr_t) (pte) & ~0xFFF)

// Page fault error codes
#define FEC_PR       0x1 // Page fault caused by protection violation
#define FEC_WR       0x2 // Page fault caused by a write
#define FEC_U        0x4 // Page fault occured while in user mode

#endif
