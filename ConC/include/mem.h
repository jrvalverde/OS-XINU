/* mem.h - getmem, freemem, freestk */

/*----------------------------------------------------------------------
 *  getmem  --  allocate storage, returning lowest integer address
 *----------------------------------------------------------------------
 */
#define getmem(nbytes)	malloc(nbytes)

/*----------------------------------------------------------------------
 *  getstk  --  allocate stack memory, returning address of topmost int.
 *		Implemented as a C routine.
 *----------------------------------------------------------------------
 */

/*----------------------------------------------------------------------
 *  freemem  --  free a memory block
 *----------------------------------------------------------------------
 */
#define freemem(p,len)	free(p)

/*----------------------------------------------------------------------
 *  freestk  --  free stack memory allocated by getstk
 *----------------------------------------------------------------------
 */
#define freestk(p,len)	free((p) - (len) + sizeof(char))
