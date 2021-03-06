/* === ADD START p3q1 ===*/

#ifndef VM_PAGE_H
#define VM_PAGE_H

#include <hash.h>

typedef enum _pme_type {
    PME_EXEC = 1,   // loading from executable
    PME_MMAP = 2,   // memory-mapped file
    PME_SWAP = 3,   // swap page
    PME_NULL = 4    // every other types, we basically don't care
} pme_type;

// NOTE : pme stands for pagemap entry
struct pme {
  void *vaddr;            // Must be multiples of 4K
                          // i.e. pg_round_down must be applied
  bool load_status;       // (true) if loaded to physical memory
                          // (false) otherwise
  bool write_permission;  // (true) if writable, (false) otherwise

  pme_type type;          // { PME_EXEC, PME_MMAP, PME_SWAP, PME_NULL }

  // PME_EXEC related
  struct file* pme_exec_file;
  int          pme_exec_read_offset;
  size_t       pme_exec_read_bytes;
  size_t       pme_exec_zero_bytes;

  // PME_MMAP related
  struct file* pme_mmap_file;
  int          pme_mmap_read_offset;
  size_t       pme_mmap_read_bytes;
  size_t       pme_mmap_zero_bytes;

  // PME_SWAP related
  size_t       pme_swap_index;

  struct hash_elem elem;          // used to insert to struct thread.pmap
  /* === ADD START p3q3 ===*/
  struct list_elem mmap_elem;     // used to insert to struct mmap_meta.pme_list
  /* === ADD END p3q3 ===*/
};

// NOTE : pmap stands for pagemap,
//        which is the Supplementary Page Table
struct pme* create_pme ();

void pmap_init (struct hash*);

struct pme* pmap_get_pme (struct hash*, void* vaddr);

bool pmap_set_pme (struct hash*, struct pme*);
bool pmap_clear_pme (struct hash*, struct pme*);
bool pmap_flush_pme_data ( struct pme*, const void* );
bool pmap_writeback_pme_data (struct pme*, const void* );

static struct pme* lookup_pme (struct hash*, void*);

void pmap_destroy (struct hash*);

bool load_segment_on_demand ( struct pme*, void* );

#endif /* vm/page.h */

/* === ADD END p3q1 ===*/

