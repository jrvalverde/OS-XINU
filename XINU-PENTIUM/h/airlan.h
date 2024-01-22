/* 
 * airlan.h - For the AirLAN Ethernet card
 * 
 * Author:	John C. Lin
 * 		Dept. of Computer Science
 * 		Purdue University
 *		(Based on the Linux WaveLAN driver by Bruce Janson.)
 * Date:	Mon Feb 20 22:40:35 1995
 */

#ifdef WIRELESS

#include <i82586.h>

/*
 * Manufacture's Ethernet prefix (for checking purpose)
 */
#define AL_MAC0		0x08
#define AL_MAC1		0x00
#define AL_MAC2		0x0e

/*
 * program I/O mode register values
 */
#define ALPIO_STATIC		0	/* Mode 1: static mode		*/
#define ALPIO_AUTOINCR		1  	/* Mode 2: auto increment mode	*/
#define ALPIO_AUTODECR		2       /* Mode 3: auto decrement mode 	*/
#define ALPIO_PARAM_ACCESS	3       /* Mode 4: LAN parameter access mode */
#define ALPIO_MASK		3       /* register mask 		*/

#define ALPIOM(cmd, piono)	((unsigned short)(cmd) << 10 << ((piono) << 1))

/*
 * AirLan register offsets. (Offsets are from base I/O address.)
 */
#define AL_CS		0	/* command and status register		*/
#define AL_MMCR		2	/* modem management ctrl register	*/
#define AL_PIO_R0	4	/* program I/O address register port 0	*/
#define AL_PIO_P0	6	/* program I/O port 0			*/
#define AL_PIO_R1	8	/* program I/O address register port 1	*/
#define AL_PIO_P1      10	/* program I/O port 1			*/
#define AL_PIO_R2      12	/* program I/O address register port 2	*/
#define AL_PIO_P2      14	/* program I/O port 2			*/

/*
 * commands to the command register
 */
#define ALC_RESET	0x0001  /* Reset board 				*/
#define ALC_CA		0x0002  /* Set Channel Attention for 82586	*/
#define ALC_16BITS	0x0004  /* 16 bits operation (0 => 8bits) 	*/
#define ALC_OUT0	0x0008  /* General purpose output pin 0  	*/
                                       /* not used - must be 1 		*/
#define ALC_OUT1        0x0010  /* General purpose output pin 1 	*/
                                       /* not used - must be 1 		*/
#define ALC_82586_INTEN 0x0020  /* Enable 82586 interrupts 		*/
#define ALC_MMC_INTEN   0x0040  /* Enable MMC interrupts 		*/
#define ALC_INTR_CLREN  0x0080  /* Enable interrupt status read/clear 	*/
#define ALC_PIO_AUTOINC	0x1000 	/* auto-increment mode (???)		*/
#define ALC_PIO_PARAM_ACC 0xc000/* PIO parameter access			*/ 

#define ALC_DEFAULT 	(ALC_OUT0 | ALC_OUT1 | ALC_16BITS | ALC_PIO_AUTOINC) 
#define ALC_INTRON 	(ALC_82586_INTEN | ALC_MMC_INTEN | ALC_INTR_CLREN)

/*
 * signals to i82586
 */
#define AL_82586_INT_ON	(ALC_DEFAULT | ALC_INTRON)
#define AL_82586_CA	(ALC_DEFAULT | ALC_CA) 
    
/*
 * status values of the status register
 */
#define ALS_82586_INTR	0x0001  /* Interrupt request from 82586 	*/
#define ALS_MMC_INTR	0x0002  /* Interrupt request from MMC 		*/
#define ALS_MMC_BUSY	0x0004  /* MMC busy indication 			*/
#define ALS_PSA_BUSY	0x0008  /* LAN parameter storage area busy	*/

/*
 * Parameter Storage Area (PSA).
 */
struct al_psa {
    unsigned char psa_iobase_addr_1;	/* Base address 1 ??? 		*/
    unsigned char psa_iobase_addr_2;	/* Base address 2 		*/
    unsigned char psa_iobase_addr_3;	/* Base address 3 		*/
    unsigned char psa_iobase_addr_4;	/* Base address 4 		*/
    unsigned char psa_rem_boot_addr_1;	/* Remote Boot Address 1 	*/
    unsigned char psa_rem_boot_addr_2;	/* Remote Boot Address 2 	*/
    unsigned char psa_rem_boot_addr_3;	/* Remote Boot Address 3 	*/
    unsigned char psa_holi_params;	/* HOst Lan Interface (HOLI) Param.*/
    unsigned char psa_int_req_no;	/* Interrupt Request Line 	*/
    unsigned char psa_unused0[7];	/* unused */
    unsigned char psa_univ_mac_addr[EP_ALEN];/* Universal (factory) MAC addr*/
    unsigned char psa_local_mac_addr[EP_ALEN];/* Local MAC Address 	*/
    unsigned char psa_univ_local_sel;	/* Universal Local Selection 	*/
#define PSA_UNIVERSAL   	0	/* Universal (factory) 		*/
#define PSA_LOCAL       	1	/* Local 			*/
    unsigned char   psa_comp_number;	/* Compatability Number: 	*/
#define PSA_COMP_PC_AT_915      0       /* PC-AT 915 MHz        	*/
#define PSA_COMP_PC_MC_915      1       /* PC-MC 915 MHz        	*/
#define PSA_COMP_PC_AT_2400     2       /* PC-AT 2.4 GHz        	*/
#define PSA_COMP_PC_MC_2400     3       /* PC-MC 2.4 GHz        	*/
#define PSA_COMP_PCMCIA_915     4       /* PCMCIA 915 MHz       	*/
    unsigned char psa_thr_pre_set;	/* Modem Threshold Preset 	*/
    unsigned char psa_feature_select;	/* ??? 				*/
    unsigned char psa_subband;		/* Subband      		*/
#define PSA_SUBBAND_915         0       /* 915 MHz      		*/
#define PSA_SUBBAND_2425        1       /* 2425 MHz     		*/
#define PSA_SUBBAND_2460        2       /* 2460 MHz     		*/
#define PSA_SUBBAND_2484        3       /* 2484 MHz     		*/
#define PSA_SUBBAND_2430_5      4       /* 2430.5 MHz   		*/
    unsigned char psa_quality_thr;	/* Modem Quality Threshold 	*/
    unsigned char psa_mod_delay;	/* Modem Delay ??? 		*/
    unsigned char psa_nwid[2];		/* Network ID 			*/
    unsigned char psa_undefined;	/* undefined 			*/
    unsigned char psa_encryption_select;/* Encryption On Off 		*/
    unsigned char psa_encryption_key[8];/* Encryption Key 		*/
    unsigned char psa_databus_width;	/* 8/16 bit bus width 		*/
    unsigned char psa_call_code;	/* ??? 				*/
    unsigned char psa_no_of_retries;	/* LAN Cont. No of retries 	*/
    unsigned char psa_acr;		/* LAN Cont. ACR 		*/
    unsigned char psa_dump_count;	/* number of Dump Commands in TFB*/
    unsigned char psa_unused1[4];	/* unused 			*/
    unsigned char psa_nwid_prefix;	/* ??? 				*/
    unsigned char psa_unused2[3];	/* unused 			*/
    unsigned char psa_conf_status;	/* Card Configuration Status 	*/
    unsigned char psa_crc[2];		/* CRC over PSA 		*/
    unsigned char psa_crc_status;	/* CRC Valid Flag 		*/
};

/*
 * Modem Management Controller (MMC) write structure.
 */
struct al_mmcw {
    unsigned char   mmcw_encr_key[8];	/* encryption key 		*/
    unsigned char   mmcw_encr_enable;	/* enable/disable encryption 	*/
    unsigned char   mmcw_unused0[1];	/* unused 			*/
    unsigned char   mmcw_des_io_invert;	/* ??? 				*/
    unsigned char   mmcw_unused1[5];	/* unused 			*/
    unsigned char   mmcw_loopt_sel;	/* looptest selection 		*/
#define MMCW_LOOPT_SEL_UNDEFINED 0x40	/* undefined 			*/
#define MMCW_LOOPT_SEL_INT       0x20	/* activate Attention Request	*/
#define MMCW_LOOPT_SEL_LS        0x10	/* looptest w/o collision avoidance */
#define MMCW_LOOPT_SEL_LT3A      0x08	/* looptest 3a 			*/
#define MMCW_LOOPT_SEL_LT3B      0x04	/* looptest 3b 			*/
#define MMCW_LOOPT_SEL_LT3C      0x02	/* looptest 3c 			*/
#define MMCW_LOOPT_SEL_LT3D      0x01	/* looptest 3d 			*/
    unsigned char   mmcw_jabber_enable;	/* jabber timer enable 		*/
    unsigned char   mmcw_freeze;	/* freeze / unfreeeze signal level */
    unsigned char   mmcw_anten_sel;	/* antenna selection 		*/
#define MMCW_ANTEN_SEL_SEL       0x01	/* direct antenna selection 	*/
#define MMCW_ANTEN_SEL_ALG_EN    0x02	/* antenna selection algorithm enable*/
    unsigned char   mmcw_ifs;		/* inter frame spacing 		*/
    unsigned char   mmcw_mod_delay;	/* modem delay 			*/
    unsigned char   mmcw_jam_time;	/* jamming time 		*/
    unsigned char   mmcw_unused2[1];	/* unused 			*/
    unsigned char   mmcw_thr_pre_set;	/* level threshold preset 	*/
    unsigned char   mmcw_decay_prm;	/* decay parameters 		*/
    unsigned char   mmcw_decay_updat_prm;/* decay update parameterz 	*/
    unsigned char   mmcw_quality_thr;	/* quality (z-quotient) threshold */
    unsigned char   mmcw_netw_id_l;	/* NWID low order byte 		*/
    unsigned char   mmcw_netw_id_h; 	/* NWID high order byte 	*/
};

/*
 * Modem Management Controller (MMC) read structure.
 */
struct al_mmcr {
    unsigned char   mmcr_unused0[8];	/* unused 			*/
    unsigned char   mmcr_des_status;	/* encryption status 		*/
    unsigned char   mmcr_des_avail;	/* encryption available (0x55 read) */
    unsigned char   mmcr_des_io_invert;	/* des I/O invert register 	*/
    unsigned char   mmcr_unused1[5];	/* unused 			*/
    unsigned char   mmcr_dce_status;	/* DCE status 			*/
#define MMCR_DCE_OFFSET		    16  /* DCE offset			*/
#define MMCR_DCE_STATUS_ENERG_DET   0x01/* energy detected 		*/
#define MMCR_DCE_STATUS_LOOPT_IND   0x02/* loop test indicated 		*/
#define MMCR_DCE_STATUS_XMTITR_IND  0x04/* transmitter on 		*/
#define MMCR_DCE_STATUS_JBR_EXPIRED 0x08/* jabber timer expired 	*/
    unsigned char   mmcr_unused2[3];    /* unused 			*/
    unsigned char   mmcr_correct_nwid_l;/* no. of correct NWID's rxd (low) */
    unsigned char   mmcr_correct_nwid_h;/* no. of correct NWID's rxd (high) */
    unsigned char   mmcr_wrong_nwid_l;  /* count of wrong NWID's received (low) */
    unsigned char   mmcr_wrong_nwid_h;	/* count of wrong NWID's received (high) */
    unsigned char   mmcr_thr_pre_set;	/* level threshold preset 	*/
    unsigned char   mmcr_signal_lvl;	/* signal level 		*/
    unsigned char   mmcr_silence_lvl;	/* silence level 		*/
    unsigned char   mmcr_sgnl_qual;	/* signal quality 		*/
#define MMCR_SGNL_QUAL_0         0x01	/* signal quality 0 		*/
#define MMCR_SGNL_QUAL_1         0x02	/* signal quality 1 		*/
#define MMCR_SGNL_QUAL_2         0x04	/* signal quality 2 		*/
#define MMCR_SGNL_QUAL_3         0x08	/* signal quality 3 		*/
#define MMCR_SGNL_QUAL_S_A       0x80	/* currently selected antenna 	*/
    unsigned char   mmcr_netw_id_l;	/* NWID low order byte ??? 	*/
    unsigned char   mmcr_unused3[1];	/* unused 			*/
};

/*
 * on-board RAM size and layout of the i82586 data structures.
 * Note: all offsets from 0x00.
 */
#define AL_RAM_SIZE		0x10000		/* 64K bytes		*/
#define AL_SCB_OFFSET		I82586_SCB
#define AL_SCB_CMD_OFFSET	AL_SCB_OFFSET+2	/* scb command field	*/

#define AL_RFD_NUM		9	/* # of receive buffers		   */
#define	BIG_FRAME_SIZE	 	1520	/* big receive frame buffer size   */
#define SMALL_FRAME_SIZE  	512	/* samll receive frame buffer size */

/*
 * externs
 */
extern unsigned short	alc_curr_cmd; /* storeed the current command bits used */
extern int al_reset();
extern int al_read_psa();
extern int al_ram_blk_read();
extern int al_ram_blk_write();
extern unsigned short al_read_ram_word();
extern int al_ram_write_word();

#endif WIRELESS
