/* configsys.c - configure the system parameters */
#include <conf.h>
#include <kernel.h>
#include <openprom.h>
#include <idprom.h>
#include <mem.h>
#include <frame.h>
#include <vmem.h>

/*#define DEBUG*/
#define SYS_INFO

	/* We need as least this much for XINU kernel */
#define MIN_MEM_SIZE	(2 * 1024 * 1024)
	/* properity name max length */
#define MAX_PNAME_LEN	80

int dev_root;		/* device property root handle 		*/
int mmu_nctx;		/* the number of MMU context 		*/
int mmu_npmeg;		/* the number MMU PMEGs			*/
int vac_line_size;	/* Virtual Address Cache line size	*/
int vac_size;		/* Virtual Address Cache size		*/
unsigned mem_size;	/* available memory size		*/
int num_mem_banks;	/* the number of memory banks		*/
struct physmemdesc physmembanks[MAX_MEM_BANKS];/* list of phys mem banks*/
struct idprom idprom;	/* ID prom structure			*/

/*-------------------------------------------------------------------------
 * config_system - use the configuration tree built by the PROMs to
 *		   determine system parameters.
 *-------------------------------------------------------------------------
 */
int config_system()
{
    int len;
    
    if (romp->v_magic != ROMVEC_MAGIC)
	panic("config_system: unable to configure the system.");
    
    dev_root = (*romp->v_config_ops->devr_next)(0);

    (void) (*romp->v_config_ops->devr_getprop)(dev_root, "idprom", &idprom);
    
    /* find out the number of MMU context */
    mmu_nctx = get_mmu_context();
    
    /* find out the number of MMU PMEGs */
    mmu_npmeg = get_mmu_pmeg();
    
    /* find out the VAC line size */
    vac_line_size = get_vac_line_size();
    
    /* find out the VAC size */
    vac_size = get_vac_size();

    /* find out the physical memory size and build the memory list */
    get_mem_size();

#ifdef SYS_INFO
    kprintf("System configuration:\n");
    kprintf("---------------------------------------------------------\n");
    kprintf("	Monitor version       : %d\n", romp->v_romvec_version);
    kprintf("	Monitor plugin version: %d\n", romp->v_plugin_version);
    kprintf("	Monitor firmware id   : 0x%x\n", romp->v_mon_id);
    kprintf("	IDPROM machine type   : 0x%x\n", idprom.id_machine);
    kprintf("	Number of MMU context : %d\n", mmu_nctx);
    kprintf("	Number of MMU PMEG    : %d\n", mmu_npmeg);
    kprintf("	VAC line size         : %d\n", vac_line_size);
    kprintf("	Available memory size : 0x%x\n", mem_size);
    kprintf("	Number of memory banks: %d\n", num_mem_banks);
    kprintf("---------------------------------------------------------\n");
#endif
}

/*-------------------------------------------------------------------------
 * get_mem_size - get the available memory size
 *-------------------------------------------------------------------------
 */
LOCAL get_mem_size()
{
    struct memlist *physmem_list;
    int len, vmnode, blocks;
    struct dev_reg regs[16];

    physmem_list = (*(romp->v_availmemory));

    if (physmem_list) {
	physmeminit(physmem_list);
	return;
    }

    /* use properity tree */
    if (physmem_list == 0) {
	vmnode = search_property_tree(dev_root, "memory");
	if (vmnode == 0)
	    panic("get_mem_size: unable to find the 'memory' node");
	
	len = (*romp->v_config_ops->devr_getproplen)(vmnode, "available");
	if (len == -1)
	    panic("get_mem_size: unable to find the 'memory' node");
	
	blocks = len / sizeof(struct dev_reg);
	(void) (*romp->v_config_ops->devr_getprop)(vmnode, "available", regs);
	build_memory_list(regs, blocks);
    }
}

/*-------------------------------------------------------------------------
 * get_vac_size - get the VAC size
 *-------------------------------------------------------------------------
 */
LOCAL get_vac_size()
{
    int len, n;

    len = (*romp->v_config_ops->devr_getproplen)(dev_root, "vac-size");
    if (len == -1) {
	switch(idprom.id_machine) {
	  case 0x51:
	  case 0x52:
	  case 0x53:
	  case 0x54:
	  case 0x55:
	  case 0x56:
	  case 0x57:
	    return(65536);
	  default:
	    panic("get_vac_size: unable to find VAC size");
	}
    }
    
    (void) (*romp->v_config_ops->devr_getprop)(dev_root, "vac-size", &n);
    return(n);
}

/*-------------------------------------------------------------------------
 * get_vac_line_size - get the VAC line size
 *-------------------------------------------------------------------------
 */
LOCAL get_vac_line_size()
{
    int len, n;

    len = (*romp->v_config_ops->devr_getproplen)(dev_root, "vac-linesize");
    if (len == -1) {
	switch(idprom.id_machine) {
	  case 0x51:
	  case 0x52:
	  case 0x53:
	  case 0x54:
	    return(16);
	  case 0x56:
	  case 0x55:
	  case 0x57:
	    return(32);
	  default:
	    panic("get_vac_line_size: unable to find VAC line size");
	}
    }
    
    (void) (*romp->v_config_ops->devr_getprop)(dev_root, "vac-linesize", &n);
    return(n);
}

/*-------------------------------------------------------------------------
 * get_mmu_pmeg - get the number of MMU PMEGs
 *-------------------------------------------------------------------------
 */
LOCAL get_mmu_pmeg()
{
    int len, n;

    /* get the number of MMU PMEGs */
    len = (*romp->v_config_ops->devr_getproplen)(dev_root, "mmu-npmg");
    if (len == -1) {
	switch(idprom.id_machine) {
	  case 0x51:
	  case 0x52:
	  case 0x53:
	  case 0x54:
	  case 0x56:
	    return(128);
	  case 0x55:
	  case 0x57:
	    return(256);
	  default:
	    panic("get_mmu_pmeg: unable to find the # of MMU PMEG");
	}
    }
    
    (void) (*romp->v_config_ops->devr_getprop)(dev_root, "mmu-npmg", &n);
    return(n);
}

/*-------------------------------------------------------------------------
 * get_mmu_context - get the number of MMU context
 *-------------------------------------------------------------------------
 */
LOCAL get_mmu_context()
{
    int len, n;

    /* get the number of MMU context */
    len = (*romp->v_config_ops->devr_getproplen)(dev_root, "mmu-nctx");
    if (len == -1) {
	switch(idprom.id_machine) {
	  case 0x51:
	  case 0x52:
	  case 0x53:
	  case 0x54:
	  case 0x56:
	  case 0x57:
	    return(8);
	  case 0x55:
	    return(16);
	  default:
	    panic("get_mmu_context: unable to find the # of MMU context");
	}
    }
    
    (void) (*romp->v_config_ops->devr_getprop)(dev_root, "mmu-nctx", &n);
    return(n);
}

/*-------------------------------------------------------------------------
 * search_properity_tree - search ROM monitor properity tree.
 *			   returning node handle.
 *-------------------------------------------------------------------------
 */
LOCAL search_property_tree(node, name)
     int node;
     char *name;	/* properity name string */
{
    char buf[MAX_PNAME_LEN];
    int name_len, phandle;

    if (node == 0)
	return (0);	/* not found */

    name_len = (*romp->v_config_ops->devr_getproplen)(node, "name");

    (void)(*romp->v_config_ops->devr_getprop)(node, "name", buf);
    if(strcmp(name, buf) == 0) {
	/* found it */
	return(node);
    }
    
    phandle = search_property_tree((*romp->v_config_ops->devr_next)(node),
				   name);
    if (phandle != 0) {
	return(phandle);
    }
    
    return(search_property_tree((*romp->v_config_ops->devr_child)(node),
			       name));
}

/*-------------------------------------------------------------------------
 * physmeminit - use the SPARC prom to determine the size of physical memory.
 *-------------------------------------------------------------------------
 */
LOCAL physmeminit(physmem_list)
    struct memlist *physmem_list;
{
    mem_size = 0;
    physmem_list = (*(romp->v_availmemory));	

    /* Note: the physical memory space may not be contiguous */
    num_mem_banks = 0;
    while (physmem_list) {
	if (num_mem_banks == MAX_MEM_BANKS)
	    panic("physmeminit: need to increase the MAX_MEM_BANKS constant");
	
	physmembanks[num_mem_banks].addr = physmem_list->address;
	physmembanks[num_mem_banks].size = physmem_list->size;
	mem_size += physmem_list->size;
	physmem_list = physmem_list->next;
	num_mem_banks++;
    }

    if (mem_size < MIN_MEM_SIZE)
	panic("Not enough memory");
}


/*-------------------------------------------------------------------------
 * build_memory_list - Use ROM properity tree info to determine memory size
 *-------------------------------------------------------------------------
 */
LOCAL build_memory_list(regs, blocks)
     struct dev_reg *regs;
     int blocks;
{
    struct dev_reg tmp;
    int i, j;
    
    /*
     * First, sort the array in order of increasing starting block address.
     */
    for (i = 0; i < blocks - 1; i++) {
	for(j = i + 1; j < blocks; j++) {
	    if (regs[i].reg_addr > regs[j].reg_addr) {
		tmp = regs[i];
		regs[i] = regs[j];
		regs[j] = tmp;
	    }
	}
    }
    
    /*
     * Now, run through and coalesce adjacent blocks.
     */
    for (i = 0; i < blocks - 1; i++) {
#ifdef DEBUG
	kprintf("regs[%d] size = 0x%x\n", i, regs[i].reg_size);
	kprintf("regs[%d] addr = 0x%x\n", i, regs[i].reg_addr);
#endif
	if (regs[i].reg_size == 0)
	    continue;

	for (j = i + 1; j < blocks; j++) {
#ifdef DEBUG	    
	kprintf("regs[%d] size = 0x%x\n", j, regs[j].reg_size);
	kprintf("regs[%d] addr = 0x%x\n", j, regs[j].reg_addr);
#endif
	    if ((regs[i].reg_addr + regs[i].reg_size) == regs[j].reg_addr) {
		regs[i].reg_size += regs[j].reg_size;
		regs[j].reg_size = 0;
	    }
	}
    }

    /*
     * Finally, construct the list from what is left.
     */
    

    /* Note: the physical memory space may not be contiguous */
    num_mem_banks = 0;
    mem_size = 0;
  
    for (i = 0; i < blocks; i++) {
	if (num_mem_banks == MAX_MEM_BANKS)
	    panic("physmeminit: need to increase the MAX_MEM_BANKS constant");
	
	if (regs[i].reg_size == 0)
	    break;
	
	physmembanks[num_mem_banks].addr = (unsigned) regs[i].reg_addr;
	physmembanks[num_mem_banks].size = regs[i].reg_size;
#ifdef DEBUG
	kprintf("mlist address = 0x%x\n", regs[i].reg_addr);
	kprintf("mlist size = 0x%x\n", regs[i].reg_size);
#endif
	mem_size += regs[i].reg_size;
	num_mem_banks++;
    }

    if (mem_size < MIN_MEM_SIZE)
	panic("Not enough memory");
}
