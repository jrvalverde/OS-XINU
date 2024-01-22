/* ctxsw.s - ctxsw(&old_reg_block,&new_reg_block) */

/*-------------------------------------------------------------------------
 * ctxsw - context switch
 *-------------------------------------------------------------------------
 */
	.text
	.align	1
	.globl	_ctxsw
_ctxsw:
	.word	07777		/* mask to save registers r0 - r11	      */
	movl	4(ap),r10	/* put &old_blk into r10 (was saved in calls) */
	movl	8(ap),r11	/* put &new_blk into r11		      */
	movl	ap,(r10)	/* save old ap				      */
	movl	fp,4(r10)	/* save old fp				      */
	movl	sp,8(r10)	/* save old sp				      */
	movl	(r11),ap	/* restore new ap			      */
	movl	4(r11),fp	/* restore new fp			      */
	movl	8(r11),sp	/* restore new sp			      */
	pushl	12(r11)		/* restore new sigvec mask while retrieving   */
	calls	$1,_sys_sigsetmask	/*	old one to save in old_blk    */
	movl	r0,12(r10)	/* save old sigvec mask			      */
	ret			/* return to new process (restoring registers)*/
