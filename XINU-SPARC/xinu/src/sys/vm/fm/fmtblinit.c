/* fmtblinit.c - fmtblinit */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <q.h>
#include <proc.h>
#include <vmem.h>
#include <frame.h>

#define SHOWFATALERRS
/*#define DEBUG*/
/*#define SHOW_FRAME_MAPPING*/

struct frame_map {
    unsigned int start_frame_num;/* starting frame number 	*/
    int nframes;		 /* number of frames	 	*/
    int start_ft_idx;		 /* starting frame table index 	*/
};

/* ** NOTE ** physical memory space may not be contiguous */
static struct frame_map frame_map[MAX_MEM_BANKS];
extern int num_mem_banks;	/* how many memory banks we have installed */

/*-------------------------------------------------------------------------
 * frame_to_index - Use the information in frame_map[] to map a frame number
 *		    to the index into the frame table.
 *-------------------------------------------------------------------------
 */
unsigned int frame_to_index(frame_num)
     unsigned int frame_num;
{
    int idx = 0, i;

    /* find out which bank it belongs to */
    for (i = 0; i < num_mem_banks; ++i) {
	if ((frame_num >= frame_map[i].start_frame_num) &&
	    (frame_num < frame_map[i].start_frame_num + frame_map[i].nframes)){
	    break;
	}
	idx += frame_map[i].nframes;
    }
    
    if (i == num_mem_banks) {
	kprintf("Invalid frame number %d\n", frame_num);
	panic("frame_to_index");
    }
    
    frame_num -= frame_map[i].start_frame_num;/* offset in that memory bank */
    return(frame_num + idx);
}

/*-------------------------------------------------------------------------
 * get_frame_num  - Use the information in frame_map[] to obtain frame number.
 *                  Call to this routine returns the next frame number.
 *-------------------------------------------------------------------------
 */
unsigned int get_frame_num()
{
    static int curr_bank = 0;
    static unsigned int curr_frame = -1;

    if (curr_frame == -1) {
	curr_frame = frame_map[0].start_frame_num;
	return (curr_frame);
    }

    curr_frame++;
    if (curr_frame ==
	frame_map[curr_bank].start_frame_num + frame_map[curr_bank].nframes) {
	++curr_bank;
	curr_frame = frame_map[curr_bank].start_frame_num;
    }
    
    if (curr_bank > num_mem_banks) {
	kprintf("error!!!  Total # of frames computed incorrectly");
	panic("get_frame_num");
    }

    return(curr_frame);
}

/*-------------------------------------------------------------------------
 * mem_avail_init - create frame_map for each memory bank, starting past
 *		    the xinu kernel and return available memory size.
 *-------------------------------------------------------------------------
 */
unsigned int mem_avail_init()
{
    int i, remainder;
    unsigned int total_size = 0, addr, size, ft_idx = 0;

    /* assume XINU kernel is in the first memory bank */
    addr = physmembanks[0].addr + xkernsize;
    size = physmembanks[0].size - xkernsize;

    /*
     * We have to make sure the blocks of memory have a
     * reasonable starting address/length (multiples of PGSIZ)
     */
    i = 0;
    for (;;) {
#ifdef DEBUG
	kprintf("bank: %d, before: addr %d length %d\n", i, addr, size);    
#endif
	if (0 != (remainder = (addr % PGSIZ))) {
	    remainder = PGSIZ - remainder;
	    addr += remainder;
	    size -= remainder;
	}
#ifdef DEBUG
	kprintf("\tdeleting %d\n", remainder);
#endif
	
	if (0 != (remainder = (size % PGSIZ))) {
	    size -= remainder;
#ifdef DEBUG
	    kprintf("\tdeleting %d\n", remainder);
#endif
	}
#ifdef DEBUG
	kprintf("bank: %d, after: addr %d length %d\n", i, addr, size);
#endif
	frame_map[i].start_frame_num = addr >> LOG2_PGSIZ;
	frame_map[i].nframes = size >> LOG2_PGSIZ;
	frame_map[i].start_ft_idx = ft_idx;
        ft_idx += frame_map[i].nframes;
	total_size += size;
	if (++i == num_mem_banks)
	    break;
	addr = physmembanks[i].addr;
	size = physmembanks[i].size;
    }

    return(total_size);
}

/*
 *------------------------------------------------------------------------
 * fmtbl_init - initialize frame table 
 *------------------------------------------------------------------------
 */
fmtbl_init()
{
    int frames;				/* no. of frames in avail mem	*/
    int ftesize;			/* frame table entry size	*/
    int fteno;				/* frame table entry number	*/
    int *addr;				/* temp address ptr		*/
    struct fte *fteptr;			/* frame table entry pointer	*/
    unsigned int total_size;

    total_size = mem_avail_init();
    
#ifdef DEBUG
    kprintf("    physmemsize = 0x%09x\n", (unsigned)physmemsize);
    kprintf("    xkernsize = 0x%09x\n", (unsigned)xkernsize);
    kprintf("    phys-xkern = 0x%09x\n", (unsigned)physmemsize-(unsigned)xkernsize);
    kprintf("    total_size = 0x%09x\n", (unsigned)total_size);
#endif	
    
    frames = truncpg(total_size) / PGSIZ;
    ftesize = sizeof (struct fte);

    /* compute total frames avail	*/
    ftinfo.frameavail = frames;
    ftinfo.ftsize = ftinfo.frameavail + FTXTRAS;
    
    /* place frame table after end of Xinu BSS (&end) */
    ft = (struct fte *) (4 + (unsigned) &end);
    
    vmaddrsp.ftbl = (char *) ft;
    
#ifdef DEBUG
    kprintf("    ft = 0x%09x\n", (unsigned)ft);
    kprintf("    frameavail = %d\n", ftinfo.frameavail);
#endif	

    /* initialize Reclaim, Active and Modified lists */
    ftinit();
    
    ftinfo.locked = ftinfo.frameavail;
    ftinfo.alen = ftinfo.mlen = ftinfo.rlen = 0;
    
    ftfreesem = screate(ftinfo.frameavail);
    
    /* init global page replacement clock hands */
    init_hands();

   /* move all the available frames to the reclaim list */
    for (fteno = 0; fteno < ftinfo.frameavail; fteno++) {
	int frame_num;
	
	fteptr = &(ft[fteno]);
	frame_num = get_frame_num();
	fteinit(fteptr, frame_num);
	ftinbf(fteno, FTRTL);		/* add to reclaim list */
	fteptr->bits.st.used = FALSE;
    }
    
    /* reset the min heap address after the frame table */
    vmaddrsp.xheapmin = (char *) &ft[ftinfo.ftsize];
    
#ifdef DEBUG
    kprintf("    xheapmin = 0x%09x\n", (unsigned) vmaddrsp.xheapmin);
#endif	
    
    /* framebase is the physical frame number of first frame	*/
    /* represented by the first frame in the frame table	*/
    ftinfo.framebase = toframenum(0);
    
#ifdef DEBUG
    kprintf("    ftinfo.framebase = 0x%08x\n", (unsigned) ftinfo.framebase);
#endif	
}

/*-------------------------------------------------------------------------
 * fteinit - Called when we initialize the frame table.
 *           it clears the frame table entry and sets the frame_num field.
 *-------------------------------------------------------------------------
 */
LOCAL fteinit(fteptr, frame_num)
	struct fte *fteptr;
	unsigned int frame_num;
{
    fteclear(fteptr);
    fteptr->frame_num = frame_num;
}
