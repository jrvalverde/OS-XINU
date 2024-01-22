#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <tftp.h>

short dest_port;
int device;
int block;
int mempos;
char *memloc;
IPaddr server_addr;
char buff[1000];
req_ack ack;
req_data *rd= (req_data *)(buff + 8);


/*-------------------------------------------------------------------------
 * readpacket - 
 *-------------------------------------------------------------------------
 */
int readpacket()
{
    int i;
    do {
	i = read(device, &buff, 1000);
	if (debug_level("tftp")>1) {
	    kprintf("TFTP : read returned %d.\n",i);
	    if (debug_level("tftp")>3)
		PrintTftp(rd);
	}
	
	if (i == TIMEOUT) {
	    if (debug_level("tftp"))
		kprintf("TFTP: TIMEOUT on receive\n");
	    return TIMEOUT;
	}
	
 	if ((i <= (sizeof(req_data)) ) && (i >= 4)) {
	    rd->type = net2hs(rd->type);
	    rd->block = net2hs(rd->block);
	    if (rd->type == 5) {
		kprintf("Received error packet\n");
		return SYSERR;
	    }
	    dest_port = ((struct xgram *)buff)->xg_fport;
	    if (debug_level("tftp") > 2)
		kprintf("TFTP: Destination port set to %d\n", dest_port);
	    return i;
	}
    } while (1);
}


/*-------------------------------------------------------------------------
 * senddata - 
 *-------------------------------------------------------------------------
 */
int senddata(void *data, int len)
{
    struct xgram msg;
    if (debug_level("tftp") > 1) {
	kprintf("TFTP: Sending Packet   block = %d  \n", block );
	if (debug_level("tftp") > 3)
	    PrintTftp(data);
    }
    
    if (debug_level("tftp") > 3)
	kprintf("TFTP: Sending to port %d.\n", dest_port);
    
    msg.xg_fip = server_addr;
    msg.xg_fport = dest_port;
    msg.xg_lport = 3000;
    blkcopy(msg.xg_data, data, len);	
    return write(device, &msg, len);
}


/*-------------------------------------------------------------------------
 * tftp - 
 *-------------------------------------------------------------------------
 */
int tftp(char *Addr, char *filename)	/* server Addr & boot file name */ 
{
    char serviceport[100];
    char str[80];
    int count, c, len;
    req_init rw;	/* bootp request */

    mempos = 0;
    server_addr = dot2ip(Addr);	
    dest_port = 69;
    
    memloc = (void *)BOOTPLOC;
    
    if ((device = open (UDP, ANYFPORT, 3000)) == SYSERR) {		
	kprintf("TFTP: open failed. \n");
	if (debug_level("tftp") > 1)
	    kprintf("TFTP: server address = %s\n", Addr);
	return SYSERR;
    }
    
    if (debug_level("timeout")) {
	c = control(device, DG_SETMODE, DG_TMODE | DG_NMODE);
	if (debug_level("tftp") > 1)
	    kprintf("TFTP: control returned %d\n", c);
    }

    /*
     * for a bootp request and send it
     */
    bzero(&rw, sizeof(req_init));
    rw.type = hs2net(1);	 
	
    strcpy(rw.data, filename);
    strcpy(&rw.data[strlen(filename) + 1], "octet");
    len = 2 + strlen(filename) + 1 + 5 +1;
    
    if (senddata(&rw, len) == SYSERR) {
	kprintf("Error Sending data.\n");
	close(device);
	return SYSERR;
    }

    count = TIMEOUTCOUNT;	
    while (((c = readpacket()) == TIMEOUT) && (--count)) {
	if (senddata (&rw, len) == SYSERR) {
	    kprintf("Error Sending data.\n");
	    close(device);
	    return SYSERR;
	}
    }
    
    if (c == SYSERR) {
	close(device);
	return SYSERR;
    }
    
    if (count <= 0) {
	kprintf("No response\n");
	close(device);
	return SYSERR;
    }
		
    ack.type = hs2net(4);
    block = 1;
	
    blkcopy(&memloc[mempos], rd->data, 512);
    mempos += 512;
	
    do {
	ack.block = hs2net(block);
	block ++;
	if (senddata(&ack, 4) == SYSERR) {
	    kprintf("TFTP: Error sending ACK\n");
	    close(device);
	    return SYSERR;
	}

	count = TIMEOUTCOUNT;	
	while (((c=readpacket() ) == TIMEOUT) && ((--count)>0))
	    if (senddata (&ack, 4) == SYSERR) {
		kprintf("TFTP: Error sending ACK\n");
		close(device);
		return SYSERR;
	    }
	if (c == SYSERR) return SYSERR;
	if (count <= 0) {
	    kprintf("No response\n");
	    close(device);
	    return SYSERR;
	}

	blkcopy(&memloc[mempos], rd->data, 512);
	mempos += 512;
		
    } while (c == 516);
	
    ack.block = hs2net(block);
    if (senddata(&ack,4) == SYSERR) {
	kprintf("TFTP: Error sending ACK\n");
	close(device);
	return SYSERR;
    }
    
    if (c == SYSERR) return SYSERR;
		
    kprintf("TFTP: received %d blocks.\n", block);
    close(device);
    
    kprintf("TFTP: Jumping to new image....Good Luck\n");
    {
	STATWORD ps;
	disable(ps);
	jumptobootcode();
    }
	
}

PrintTftp(void *package)
{
    kprintf("TFTP: Type == %d\n", net2hs(((req_data *)package)->type));
    hexdump(package, 32);
    return OK;
}







