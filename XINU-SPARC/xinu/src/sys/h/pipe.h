/* pipe.h file -- greatly simplified from the 503 project       sjm	*/

#define PIPESIZE 4097		/* num chars + 1 for queue wrap */

/* modes for pipe */
#define PIPE_FREE	7
#define PIPE_INUSE	8

struct pipe {
	char mode;
	char buffer[PIPESIZE];
	int i_nextout, i_nextin;
	int i_readsem, i_writesem;
}; 

extern struct pipe pipe[];

#define pipe_full(pp) (((pp)->i_nextin + 1) % PIPESIZE == (pp)->i_nextout)
#define pipe_empty(pp) ((pp)->i_nextin == (pp)->i_nextout)
#define pipe_count(pp) ((((pp)->i_nextin + PIPESIZE) - (pp)->i_nextout) % PIPESIZE)
#define pipe_space(pp) (PIPESIZE - pipe_count(pp) -1)
#define pipe_bump(i) (i) = ((i) + 1) % PIPESIZE
#define pipe_min(x,y) ((x) >= (y) ? (y) : (x))

