/* key to types of symbol table */
#define TUNDEF   00     /* undefined                            */
#define TABS     01     /* absolute                             */
#define TTEXT    02     /* text                                 */
#define TDATA    03     /* data                                 */
#define TBSS     04     /* bss                                  */
#define TFLDST   05     /* flop freg,dst (movfo, = stcfd)       */
#define TBR      06     /* branch                               */
#define TJSR     07     /* jsr                                  */
#define TRTS    010     /* rts                                  */
#define TSYS    011     /* sys                                  */
#define TMOVF   012     /* movf (=ldf,stf)                      */
#define TDBLOP  013     /* double operand (mov)                 */
#define TFLREG  014     /* flop fsrc,freg (addf)                */
#define TSINGLE 015     /* single operand (clr)                 */
#define TCBYTE  016     /* .byte                                */
#define TCSTR   017     /* string (.ascii, """)                 */
#define TCEVEN  020     /* .even                                */
#define TCIF    021     /* .if                                  */
#define TCENDIF 022     /* .endif                               */
#define TCGLOBL 023     /* .globl                               */
#define TREG    024     /* register                             */
#define TCTEXT  025     /* .text (The next three must follow one another) */
#define TCDATA  026     /* .data                                */
#define TCBSS   027     /* .bss                                 */
#define TMUL    030     /* mul,div, etc                         */
#define TSOB    031     /* sob                                  */
#define TCCOMM  032     /* .comm                                */
#define TETEXT  033     /* estimated text                       */
#define TEDATA  034     /* estimated data                       */
#define TJBR    035     /* jbr                                  */
#define TJCOND  036     /* jeq, jne, etc                        */

#define TGLOBAL 040     /* Ored into type when global           */
