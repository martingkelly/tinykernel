/*
 * MODULE:
 *
 *  lpc2378.h
 * 
 * DESCRIPTION:
 *
 *  LPC2378 standard definitions file
 * 
 * OS:
 *
 *  N/A
 *
 * PLATFORM:
 *
 *  Olimex LPC2378-STK
 *
 * ORIGINATOR:
 *
 *  Andrew N. Sloss
 *
 * HISTORY:
 *
 *  - started creating the header file  
 *
 * NOTES:
 *
 *  n/a
 *
 */
 
#ifndef LPC2378_STANDARD_DEFINITIONS
#define LPC2378_STANDARD_DEFINITIONS

/*
 * STANDARD DEFINITIONS 
 */

#define BIT(a)  (1<<a)

#define U32 unsigned long
#define U16 unsigned short
#define U8  unsigned char 

#define S32 long 
#define S16 short    
#define S8 char  

#define INT8U  U8
#define INT16U U16
#define INT32U U32
#define BOOLEAN U8
#define FALSE 0
#define TRUE 1

#define VOLATILE32(addr) (*(volatile U32 *)(addr)) 
#define VOLATILE16(addr) (*(volatile U16 *)(addr)) 
#define VOLATILE8(addr)  (*(volatile U8 *)(addr))  

/*
 *  read/write registers
 */
#define READREG32(pReg) (*(volatile U32 *)(pReg))
#define READREG16(pReg) (*(volatile U16 *)(pReg))
#define READREG8(pReg) (*(volatile U8 *)(pReg))
#define WRITEREG32(pReg, data) ((*(volatile U32 *)(pReg)) = data)
#define WRITEREG16(pReg, data) ((*(volatile U16 *)(pReg)) = data)
#define WRITEREG8(pReg, data) ((*(volatile U8 *)(pReg))  = data)
 
#define SCB_READREG32(pReg) (P_SCB_REGS->pReg)
#define SCB_READREG16(pReg) (P_SCB_REGS->pReg)
#define SCB_READREG8(pReg)  (P_SCB_REGS->pReg)
#define SCB_WRITEREG32(pReg, data) (((P_SCB_REGS->pReg)) = data)
#define SCB_WRITEREG16(pReg, data) (((P_SCB_REGS->pReg)) = data)
#define SCB_WRITEREG8(pReg, data)  (((P_SCB_REGS->pReg))  = data)


/*
 * REGISTER MAPPINGS
 */ 

/* 
 * Fast GPIO Registers 
 */

#define FIO0DIR        0x3FFFC000
#define FIO0MASK       0x3FFFC010
#define FIO0PIN        0x3FFFC014
#define FIO0SET        0x3FFFC018
#define FIO0CLR        0x3FFFC01C
#define FIO1DIR        0x3FFFC020
#define FIO1MASK       0x3FFFC030
#define FIO1PIN        0x3FFFC034
#define FIO1SET        0x3FFFC038
#define FIO1CLR        0x3FFFC03C
#define FIO2DIR        0x3FFFC040
#define FIO2MASK       0x3FFFC050
#define FIO2PIN        0x3FFFC054
#define FIO2SET        0x3FFFC058
#define FIO2CLR        0x3FFFC05C
#define FIO3DIR        0x3FFFC060
#define FIO3MASK       0x3FFFC070
#define FIO3PIN        0x3FFFC074
#define FIO3SET        0x3FFFC078
#define FIO3CLR        0x3FFFC07C
#define FIO4DIR        0x3FFFC080
#define FIO4MASK       0x3FFFC090
#define FIO4PIN        0x3FFFC094
#define FIO4SET        0x3FFFC098
#define FIO4CLR        0x3FFFC09C

/*
 * UART0
 */ 

#define U0RBR          0xE000C000
#define U0THR          0xE000C000
#define U0DLL          0xE000C000
#define U0DLM          0xE000C004
#define U0IER          0xE000C004
#define U0IIR          0xE000C008
#define U0FCR          0xE000C008
#define U0LCR          0xE000C00C
#define U0LSR          0xE000C014
#define U0SCR          0xE000C01C
#define U0ACR          0xE000C020
#define U0ICR          0xE000C024
#define U0FDR          0xE000C028
#define U0TER          0xE000C030

#define PWM1IR         0xE0018000
#define PWM1TCR        0xE0018004
#define PWM1TC         0xE0018008
#define PWM1PR         0xE001800C
#define PWM1PC         0xE0018010
#define PWM1MCR        0xE0018014
#define PWM1MR0        0xE0018018
#define PWM1MR1        0xE001801C
#define PWM1MR2        0xE0018020
#define PWM1MR3        0xE0018024
#define PWM1CCR        0xE0018028
#define PWM1CR0        0xE001802C
#define PWM1CR1        0xE0018030
#define PWM1CR2        0xE0018034
#define PWM1CR3        0xE0018038
#define PWM1EMR        0xE001803C
#define PWM1MR4        0xE0018040
#define PWM1MR5        0xE0018044
#define PWM1MR6        0xE0018048
#define PWM1PCR        0xE001804C
#define PWM1LER        0xE0018050
#define PWM1CTCR       0xE0018070
#define IO0DIR         0xE0028008
#define IO0CLR         0xE002800C
#define IO1PIN         0xE0028010
#define IO1SET         0xE0028014
#define IO1DIR         0xE0028018
#define IO1CLR         0xE002801C

#define PINSEL0        0xE002C000
#define PINSEL1        0xE002C004
#define PINSEL2        0xE002C008
#define PINSEL3        0xE002C00C
#define PINSEL4        0xE002C010
#define PINSEL5        0xE002C014
#define PINSEL6        0xE002C018
#define PINSEL7        0xE002C01C
#define PINSEL8        0xE002C020
#define PINSEL9        0xE002C024
#define PINSEL10       0xE002C028
#define PINSEL10_MASK  0x00000008   /* mask reserved bits */

/* PIN SELECT - Chapter 8 of the LPC2378 User Manual */
#define PINMODE0       0xE002C040
#define PINMODE1       0xE002C044
#define PINMODE2       0xE002C048
#define PINMODE3       0xE002C04C
#define PINMODE4       0xE002C050
#define PINMODE5       0xE002C054
#define PINMODE6       0xE002C058
#define PINMODE7       0xE002C05C
#define PINMODE8       0xE002C060
#define PINMODE9       0xE002C064
/* ADC */
/* A/D Intrrupt Enable Register */
typedef struct _ADCR_BITS{
    U32 SEL     : 8;
    U32 CLKDIV  : 8;
    U32 BURST   : 1;
    U32 CLKS    : 3;
    U32         : 1;
    U32 PDN     : 1;
    U32         : 2;
    U32 START   : 3;
    U32 EDGE    : 1;
    U32         : 4;
} ADCR_BITS, *PADCR_BITS;

/* A/D Global Data Register */
typedef struct  _ADGDR_BITS{
    U32         : 6;
    U32 RESULT  :10;
    U32         : 8;
    U32 CHN     : 3;
    U32         : 3;
    U32 OVERUN  : 1;
    U32 DONE    : 1;
} ADGDR_BITS, *PADGDR_BITS;

/* A/D Status Register */
typedef struct _ADSTAT_BITS{
    U32 DONE0     : 1;
    U32 DONE1     : 1;
    U32 DONE2     : 1;
    U32 DONE3     : 1;
    U32 DONE4     : 1;
    U32 DONE5     : 1;
    U32 DONE6     : 1;
    U32 DONE7     : 1;
    U32 OVERRUN0  : 1;
    U32 OVERRUN1  : 1;
    U32 OVERRUN2  : 1;
    U32 OVERRUN3  : 1;
    U32 OVERRUN4  : 1;
    U32 OVERRUN5  : 1;
    U32 OVERRUN6  : 1;
    U32 OVERRUN7  : 1;
    U32 ADINT     : 1;
    U32           :15;
} ADSTAT_BITS, *PADSTAT_BITS;

/* A/D Intrrupt Enable Register */
typedef struct{
    U32 ADINTEN0  : 1;
    U32 ADINTEN1  : 1;
    U32 ADINTEN2  : 1;
    U32 ADINTEN3  : 1;
    U32 ADINTEN4  : 1;
    U32 ADINTEN5  : 1;
    U32 ADINTEN6  : 1;
    U32 ADINTEN7  : 1;
    U32 ADGINTEN  : 1;
    U32           :23;
} __adinten_bits;

/* A/D Data Register */
typedef struct{
    U32         : 6;
    U32 RESULT  :10;
    U32         :14;
    U32 OVERUN  : 1;
    U32 DONE    : 1;
} __addr_bits;

/* D/A Converter Register */
typedef struct{
    U32        : 6;
    U32 VALUE  :10;
    U32 BIAS   : 1;
    U32        :15;
} __dacr_bits;

typedef struct _ADINTEN_BITS{
    U32 ADINTEN0  : 1;
    U32 ADINTEN1  : 1;
    U32 ADINTEN2  : 1;
    U32 ADINTEN3  : 1;
    U32 ADINTEN4  : 1;
    U32 ADINTEN5  : 1;
    U32 ADINTEN6  : 1;
    U32 ADINTEN7  : 1;
    U32 ADGINTEN  : 1;
    U32           :23;
} ADINTEN_BITS, *PADINTEN_BITS;

#define AD0CR          0xE0034000
#define AD0GDR         0xE0034004
#define ADINTEN        0xE003400C
#define ADDR0          0xE0034010
#define ADDR1          0xE0034014
#define ADDR2          0xE0034018
#define ADDR3          0xE003401C
#define ADDR4          0xE0034020
#define ADDR5          0xE0034024
#define ADDR6          0xE0034028
#define ADDR7          0xE003402C
/*
 * TIMER0
 */
#define T0IR            0xE0004000
#define T0TCR           0xE0004004
#define T0TC            0xE0004008
#define T0PR            0xE000400C
#define T0PC            0xE0004010
#define T0MCR           0xE0004014
#define T0MR0           0xE0004018
#define T0MR1           0xE000401C
#define T0MR2           0xE0004020
#define T0MR3           0xE0004024
#define T0CCR           0xE0004028
#define T0CR0           0xE000402C
#define T0CR1           0xE0004030
#define T0CR2           0xE0004034
#define T0CR3           0xE0004038
#define T0EMR           0xE000403C
#define T0CTCR          0xE0004070
/*
 * TIMER1
 */
#define T1IR            0xE0008000
#define T1TCR           0xE0008004
#define T1TC            0xE0008008
#define T1PR            0xE000800C
#define T1PC            0xE0008010
#define T1MCR           0xE0008014
#define T1MR0           0xE0008018
#define T1MR1           0xE000801C
#define T1MR2           0xE0008020
#define T1MR3           0xE0008024
#define T1CCR           0xE0008028
#define T1CR0           0xE000802C
#define T1CR1           0xE0008030
#define T1CR2           0xE0008034
#define T1CR3           0xE0008038
#define T1EMR           0xE000803C
#define T1CTCR          0xE0008070

#define SSP0CR0         0xE0068000
#define SSP0CR1         0xE0068004
#define SSP0DR          0xE0068008
#define SSP0SR          0xE006800C
#define SSP0CPSR        0xE0068010
#define SSP0IMSC        0xE0068014
#define SSP0RIS         0xE0068018
#define SSP0MIS         0xE006801C
#define SSP0ICR         0xE0068020
#define SSP0DMACR       0xE0068024


//??#define MAMCR          0xE01FC000
//??#define MAMTIM         0xE01FC004
//??#define MEMMAP         0xE01FC040
//??#define PLLCON         0xE01FC080
//??#define PLLCFG         0xE01FC084
//??#define PLLSTAT        0xE01FC088
//??#define PLLFEED        0xE01FC08C

//??#define PCON           0xE01FC0C0
//??#define PCONP          0xE01FC0C4
	
//??#define SCS            0xE01FC1A0	
//??#define CCLKCFG        0xE01FC104
//??#define USBCLKCFG      0xE01FC108
//??#define CLKSRCSEL      0xE01FC10C
//??#define PCLKSEL0       0xE01FC1A8
//??#define PCLKSEL1       0xE01FC1AC

/*
 * Vectored Interrupt Controller
 */
/* VIC Interrupt registers */
typedef struct _VICINT_BITS{
    int WDT       : 1;
    int           : 1;
    int ARMCORE0  : 1;
    int ARMCORE1  : 1;
    int TIMER0    : 1;
    int TIMER1    : 1;
    int UART0     : 1;
    int UART1     : 1;
    int PWM1      : 1;
    int I2C0      : 1;
    int SPI       : 1;
    int SSP1      : 1;
    int PLL       : 1;
    int RTC       : 1;
    int EINT0     : 1;
    int EINT1     : 1;
    int EINT2     : 1;
    int EINT3     : 1;
    int AD0       : 1;
    int I2C1      : 1;
    int BOD       : 1;
    int ETHERNET  : 1;
    int USB       : 1;
    int CAN12     : 1;
    int SDMMC     : 1;
    int GPDMA     : 1;
    int TIMER2    : 1;
    int TIMER3    : 1;
    int UART2     : 1;
    int UART3     : 1;
    int I2C2      : 1;
    int I2S       : 1;
} VICINT_BITS, *PVICINT_BITS;

#define VICIRQSTATUS        0xFFFFF000
#define VICFIQSTAUS         0xFFFFF004
#define VICRAWINTR          0xFFFFF008
#define VICINTSELECT        0xFFFFF00C
#define VICINTENABLE        0xFFFFF010
#define VICINTENCLEAR       0xFFFFF014
#define VICSOFTINT          0xFFFFF018
#define VICSOFTINTCLEAR     0xFFFFF01C
#define VICPROTECTION       0xFFFFF020
#define VICSWPRIORITYMASK   0xFFFFF024
#define VICVECTADDR0        0xFFFFF100
#define VICVECTADDR1        0xFFFFF104
#define VICVECTADDR2        0xFFFFF108
#define VICVECTADDR3        0xFFFFF10C
#define VICVECTADDR4        0xFFFFF110
#define VICVECTADDR5        0xFFFFF114
#define VICVECTADDR6        0xFFFFF118
#define VICVECTADDR7        0xFFFFF11C
#define VICVECTADDR8        0xFFFFF120
#define VICVECTADDR9        0xFFFFF124
#define VICVECTADDR10       0xFFFFF128
#define VICVECTADDR11       0xFFFFF12C
#define VICVECTADDR12       0xFFFFF130
#define VICVECTADDR13       0xFFFFF134
#define VICVECTADDR14       0xFFFFF138
#define VICVECTADDR15       0xFFFFF13C
#define VICVECTADDR16       0xFFFFF140
#define VICVECTADDR17       0xFFFFF144
#define VICVECTADDR18       0xFFFFF148
#define VICVECTADDR19       0xFFFFF14C
#define VICVECTADDR20       0xFFFFF150
#define VICVECTADDR21       0xFFFFF154
#define VICVECTADDR22       0xFFFFF158
#define VICVECTADDR23       0xFFFFF15C
#define VICVECTADDR24       0xFFFFF160
#define VICVECTADDR25       0xFFFFF164
#define VICVECTADDR26       0xFFFFF168
#define VICVECTADDR27       0xFFFFF16C
#define VICVECTADDR28       0xFFFFF170
#define VICVECTADDR29       0xFFFFF174
#define VICVECTADDR30       0xFFFFF178
#define VICVECTADDR31       0xFFFFF17C
#define VICVECTPRIORITY0    0xFFFFF200
#define VICVECTPRIORITY1    0xFFFFF204
#define VICVECTPRIORITY2    0xFFFFF208
#define VICVECTPRIORITY3    0xFFFFF20C
#define VICVECTPRIORITY4    0xFFFFF210
#define VICVECTPRIORITY5    0xFFFFF214
#define VICVECTPRIORITY6    0xFFFFF218
#define VICVECTPRIORITY7    0xFFFFF21C
#define VICVECTPRIORITY8    0xFFFFF220
#define VICVECTPRIORITY9    0xFFFFF224
#define VICVECTPRIORITY10   0xFFFFF228
#define VICVECTPRIORITY11   0xFFFFF22C
#define VICVECTPRIORITY12   0xFFFFF230
#define VICVECTPRIORITY13   0xFFFFF234
#define VICVECTPRIORITY14   0xFFFFF238
#define VICVECTPRIORITY15   0xFFFFF23C
#define VICVECTPRIORITY16   0xFFFFF240
#define VICVECTPRIORITY17   0xFFFFF244
#define VICVECTPRIORITY18   0xFFFFF248
#define VICVECTPRIORITY19   0xFFFFF24C
#define VICVECTPRIORITY20   0xFFFFF250
#define VICVECTPRIORITY21   0xFFFFF254
#define VICVECTPRIORITY22   0xFFFFF258
#define VICVECTPRIORITY23   0xFFFFF25C
#define VICVECTPRIORITY24   0xFFFFF260
#define VICVECTPRIORITY25   0xFFFFF264
#define VICVECTPRIORITY26   0xFFFFF268
#define VICVECTPRIORITY27   0xFFFFF26C
#define VICVECTPRIORITY28   0xFFFFF270
#define VICVECTPRIORITY29   0xFFFFF274
#define VICVECTPRIORITY30   0xFFFFF278
#define VICVECTPRIORITY31   0xFFFFF27C
#define VICADDRESS          0xFFFFFF00
/*
 *  VIC Interrupt channels
 */
#define VIC_WDT          0  /* Watchdog                           */
#define VIC_SW           1  /* Software interrupts                */
#define VIC_DEBUGRX      2  /* Embedded ICE, DbgCommRx            */
#define VIC_DEBUGTX      3  /* Embedded ICE, DbgCommTx            */
#define VIC_TIMER0       4  /* Timer 0 (Match 0-3 Capture 0-3)    */
#define VIC_TIMER1       5  /* Timer 1 (Match 0-3 Capture 0-3)    */
#define VIC_UART0        6  /* UART 0  (RLS, THRE, RDA, CTI)      */
#define VIC_UART1        7  /* UART 1  (RLS, THRE, RDA, CTI, MSI) */
#define VIC_PWM01        8  /* PWM 01  (Match 0-6 Capture 0-3)    */
#define VIC_I2C0         9  /* I2C 0   (SI)                       */
#define VIC_SPI         10  /* SPI 0, SSP 0                       */
#define VIC_SSP1        11  /* SSP 1                              */
#define VIC_PLL         12  /* PLL lock (PLOCK)                   */
#define VIC_RTC         13  /* RTC      (RTCCIF, RTCALF)          */
#define VIC_EINT0       14  /* External interrupt 0 (EINT0)       */
#define VIC_EINT1       15  /* External interrupt 1 (EINT1)       */
#define VIC_EINT2       16  /* External interrupt 2 (EINT2)       */
#define VIC_EINT3       17  /* External interrupt 3 (EINT3)       */
#define VIC_AD0         18  /* A/D converter 0                    */
#define VIC_I2C1        19  /* I2C 1                              */
#define VIC_BOD         20  /* Brown out detect                   */
#define VIC_ETHERNET    21  /* Ethernet                           */
#define VIC_USB         22  /* USB Low and High priority          */
#define VIC_CAN12       23  /* CAN1,2 Tx, Rx                      */
#define VIC_MMC         24  /* SD/MMC                             */
#define VIC_GP_DMA      25  /* DMA channel 0, DMA channel 1       */
#define VIC_TIMER2      26  /* Timer 2 (Match 0-3 Capture 0-3)    */
#define VIC_TIMER3      27  /* Timer 3 (Match 0-3 Capture 0-3)    */
#define VIC_UART2       28  /* UART 2  (RLS, THRE, RDA, CTI)      */
#define VIC_UART3       29  /* UART 3  (RLS, THRE, RDA, CTI, MSI) */
#define VIC_I2C2        30  /* I2C 0   (SI)                       */
#define VIC_I2S         31  /* I2S Rx, Tx                         */

/*
 * REGISTER DEFINITIONS
 */ 
#define LPC2378_SCB_BASE    0xE01FC000

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed long int32_t;
typedef signed long long int64_t;

typedef volatile struct __attribute__ ((__packed__,__aligned__(4))) _LPC23XX_SCB {
                                /* Access Offset */
    uint8_t   MAMCR;            /* RW     0x000 */
    uint8_t   __pad_001;
    uint8_t   __pad_002;
    uint8_t   __pad_003;
    uint8_t   MAMTIM;           /* RW     0x004 */
    uint8_t   __pad_005;
    uint8_t   __pad_006;
    uint8_t   __pad_007;
    uint8_t   __pad_008;
    uint8_t   __pad_009;
    uint8_t   __pad_00A;
    uint8_t   __pad_00B;
    uint8_t   __pad_00C;
    uint8_t   __pad_00D;
    uint8_t   __pad_00E;
    uint8_t   __pad_00F;
    uint8_t   __pad_010;
    uint8_t   __pad_011;
    uint8_t   __pad_012;
    uint8_t   __pad_013;
    uint8_t   __pad_014;
    uint8_t   __pad_015;
    uint8_t   __pad_016;
    uint8_t   __pad_017;
    uint8_t   __pad_018;
    uint8_t   __pad_019;
    uint8_t   __pad_01A;
    uint8_t   __pad_01B;
    uint8_t   __pad_01C;
    uint8_t   __pad_01D;
    uint8_t   __pad_01E;
    uint8_t   __pad_01F;
    uint8_t   __pad_020;
    uint8_t   __pad_021;
    uint8_t   __pad_022;
    uint8_t   __pad_023;
    uint8_t   __pad_024;
    uint8_t   __pad_025;
    uint8_t   __pad_026;
    uint8_t   __pad_027;
    uint8_t   __pad_028;
    uint8_t   __pad_029;
    uint8_t   __pad_02A;
    uint8_t   __pad_02B;
    uint8_t   __pad_02C;
    uint8_t   __pad_02D;
    uint8_t   __pad_02E;
    uint8_t   __pad_02F;
    uint8_t   __pad_030;
    uint8_t   __pad_031;
    uint8_t   __pad_032;
    uint8_t   __pad_033;
    uint8_t   __pad_034;
    uint8_t   __pad_035;
    uint8_t   __pad_036;
    uint8_t   __pad_037;
    uint8_t   __pad_038;
    uint8_t   __pad_039;
    uint8_t   __pad_03A;
    uint8_t   __pad_03B;
    uint8_t   __pad_03C;
    uint8_t   __pad_03D;
    uint8_t   __pad_03E;
    uint8_t   __pad_03F;
    uint8_t   MEMMAP;           /* RW     0x040 */
    uint8_t   __pad_041;
    uint8_t   __pad_042;
    uint8_t   __pad_043;
    uint8_t   __pad_044;
    uint8_t   __pad_045;
    uint8_t   __pad_046;
    uint8_t   __pad_047;
    uint8_t   __pad_048;
    uint8_t   __pad_049;
    uint8_t   __pad_04A;
    uint8_t   __pad_04B;
    uint8_t   __pad_04C;
    uint8_t   __pad_04D;
    uint8_t   __pad_04E;
    uint8_t   __pad_04F;
    uint8_t   __pad_050;
    uint8_t   __pad_051;
    uint8_t   __pad_052;
    uint8_t   __pad_053;
    uint8_t   __pad_054;
    uint8_t   __pad_055;
    uint8_t   __pad_056;
    uint8_t   __pad_057;
    uint8_t   __pad_058;
    uint8_t   __pad_059;
    uint8_t   __pad_05A;
    uint8_t   __pad_05B;
    uint8_t   __pad_05C;
    uint8_t   __pad_05D;
    uint8_t   __pad_05E;
    uint8_t   __pad_05F;
    uint8_t   __pad_060;
    uint8_t   __pad_061;
    uint8_t   __pad_062;
    uint8_t   __pad_063;
    uint8_t   __pad_064;
    uint8_t   __pad_065;
    uint8_t   __pad_066;
    uint8_t   __pad_067;
    uint8_t   __pad_068;
    uint8_t   __pad_069;
    uint8_t   __pad_06A;
    uint8_t   __pad_06B;
    uint8_t   __pad_06C;
    uint8_t   __pad_06D;
    uint8_t   __pad_06E;
    uint8_t   __pad_06F;
    uint8_t   __pad_070;
    uint8_t   __pad_071;
    uint8_t   __pad_072;
    uint8_t   __pad_073;
    uint8_t   __pad_074;
    uint8_t   __pad_075;
    uint8_t   __pad_076;
    uint8_t   __pad_077;
    uint8_t   __pad_078;
    uint8_t   __pad_079;
    uint8_t   __pad_07A;
    uint8_t   __pad_07B;
    uint8_t   __pad_07C;
    uint8_t   __pad_07D;
    uint8_t   __pad_07E;
    uint8_t   __pad_07F;
    uint8_t   PLLCON;           /* RW     0x080 */
    uint8_t   __pad_081;
    uint8_t   __pad_082;
    uint8_t   __pad_083;
    uint32_t  PLLCFG;           /* RW     0x084 */
    uint32_t  PLLSTAT;          /* R      0x088 */
    uint8_t   PLLFEED;          /*  W     0x08C */
    uint8_t   __pad_08D;
    uint8_t   __pad_08E;
    uint8_t   __pad_08F;
    uint8_t   __pad_090;
    uint8_t   __pad_091;
    uint8_t   __pad_092;
    uint8_t   __pad_093;
    uint8_t   __pad_094;
    uint8_t   __pad_095;
    uint8_t   __pad_096;
    uint8_t   __pad_097;
    uint8_t   __pad_098;
    uint8_t   __pad_099;
    uint8_t   __pad_09A;
    uint8_t   __pad_09B;
    uint8_t   __pad_09C;
    uint8_t   __pad_09D;
    uint8_t   __pad_09E;
    uint8_t   __pad_09F;
    uint8_t   __pad_0A0;
    uint8_t   __pad_0A1;
    uint8_t   __pad_0A2;
    uint8_t   __pad_0A3;
    uint8_t   __pad_0A4;
    uint8_t   __pad_0A5;
    uint8_t   __pad_0A6;
    uint8_t   __pad_0A7;
    uint8_t   __pad_0A8;
    uint8_t   __pad_0A9;
    uint8_t   __pad_0AA;
    uint8_t   __pad_0AB;
    uint8_t   __pad_0AC;
    uint8_t   __pad_0AD;
    uint8_t   __pad_0AE;
    uint8_t   __pad_0AF;
    uint8_t   __pad_0B0;
    uint8_t   __pad_0B1;
    uint8_t   __pad_0B2;
    uint8_t   __pad_0B3;
    uint8_t   __pad_0B4;
    uint8_t   __pad_0B5;
    uint8_t   __pad_0B6;
    uint8_t   __pad_0B7;
    uint8_t   __pad_0B8;
    uint8_t   __pad_0B9;
    uint8_t   __pad_0BA;
    uint8_t   __pad_0BB;
    uint8_t   __pad_0BC;
    uint8_t   __pad_0BD;
    uint8_t   __pad_0BE;
    uint8_t   __pad_0BF;
    uint8_t   PCON;             /* RW     0x0C0 */
    uint8_t   __pad_0C1;
    uint8_t   __pad_0C2;
    uint8_t   __pad_0C3;
    uint32_t  PCONP;            /* RW     0x0C4 */
    uint8_t   __pad_0C8;
    uint8_t   __pad_0C9;
    uint8_t   __pad_0CA;
    uint8_t   __pad_0CB;
    uint8_t   __pad_0CC;
    uint8_t   __pad_0CD;
    uint8_t   __pad_0CE;
    uint8_t   __pad_0CF;
    uint8_t   __pad_0D0;
    uint8_t   __pad_0D1;
    uint8_t   __pad_0D2;
    uint8_t   __pad_0D3;
    uint8_t   __pad_0D4;
    uint8_t   __pad_0D5;
    uint8_t   __pad_0D6;
    uint8_t   __pad_0D7;
    uint8_t   __pad_0D8;
    uint8_t   __pad_0D9;
    uint8_t   __pad_0DA;
    uint8_t   __pad_0DB;
    uint8_t   __pad_0DC;
    uint8_t   __pad_0DD;
    uint8_t   __pad_0DE;
    uint8_t   __pad_0DF;
    uint8_t   __pad_0E0;
    uint8_t   __pad_0E1;
    uint8_t   __pad_0E2;
    uint8_t   __pad_0E3;
    uint8_t   __pad_0E4;
    uint8_t   __pad_0E5;
    uint8_t   __pad_0E6;
    uint8_t   __pad_0E7;
    uint8_t   __pad_0E8;
    uint8_t   __pad_0E9;
    uint8_t   __pad_0EA;
    uint8_t   __pad_0EB;
    uint8_t   __pad_0EC;
    uint8_t   __pad_0ED;
    uint8_t   __pad_0EE;
    uint8_t   __pad_0EF;
    uint8_t   __pad_0F0;
    uint8_t   __pad_0F1;
    uint8_t   __pad_0F2;
    uint8_t   __pad_0F3;
    uint8_t   __pad_0F4;
    uint8_t   __pad_0F5;
    uint8_t   __pad_0F6;
    uint8_t   __pad_0F7;
    uint8_t   __pad_0F8;
    uint8_t   __pad_0F9;
    uint8_t   __pad_0FA;
    uint8_t   __pad_0FB;
    uint8_t   __pad_0FC;
    uint8_t   __pad_0FD;
    uint8_t   __pad_0FE;
    uint8_t   __pad_0FF;
    uint8_t   __pad_100;
    uint8_t   __pad_101;
    uint8_t   __pad_102;
    uint8_t   __pad_103;
    uint8_t   CCLKCFG;          /* RW     0x104 */
    uint8_t   __pad_105;
    uint8_t   __pad_106;
    uint8_t   __pad_107;
    uint8_t   USBCLKCFG;        /* RW     0x108 */
    uint8_t   __pad_109;
    uint8_t   __pad_10A;
    uint8_t   __pad_10B;
    uint8_t   CLKSRCSEL;        /* RW     0x10C */
    uint8_t   __pad_10D;
    uint8_t   __pad_10E;
    uint8_t   __pad_10F;
    uint8_t   __pad_110;
    uint8_t   __pad_111;
    uint8_t   __pad_112;
    uint8_t   __pad_113;
    uint8_t   __pad_114;
    uint8_t   __pad_115;
    uint8_t   __pad_116;
    uint8_t   __pad_117;
    uint8_t   __pad_118;
    uint8_t   __pad_119;
    uint8_t   __pad_11A;
    uint8_t   __pad_11B;
    uint8_t   __pad_11C;
    uint8_t   __pad_11D;
    uint8_t   __pad_11E;
    uint8_t   __pad_11F;
    uint8_t   __pad_120;
    uint8_t   __pad_121;
    uint8_t   __pad_122;
    uint8_t   __pad_123;
    uint8_t   __pad_124;
    uint8_t   __pad_125;
    uint8_t   __pad_126;
    uint8_t   __pad_127;
    uint8_t   __pad_128;
    uint8_t   __pad_129;
    uint8_t   __pad_12A;
    uint8_t   __pad_12B;
    uint8_t   __pad_12C;
    uint8_t   __pad_12D;
    uint8_t   __pad_12E;
    uint8_t   __pad_12F;
    uint8_t   __pad_130;
    uint8_t   __pad_131;
    uint8_t   __pad_132;
    uint8_t   __pad_133;
    uint8_t   __pad_134;
    uint8_t   __pad_135;
    uint8_t   __pad_136;
    uint8_t   __pad_137;
    uint8_t   __pad_138;
    uint8_t   __pad_139;
    uint8_t   __pad_13A;
    uint8_t   __pad_13B;
    uint8_t   __pad_13C;
    uint8_t   __pad_13D;
    uint8_t   __pad_13E;
    uint8_t   __pad_13F;
    uint8_t   EXTINT;           /* RW     0x140 */
    uint8_t   __pad_141;
    uint8_t   __pad_142;
    uint8_t   __pad_143;
    uint16_t  INTWAKE;          /* RW     0x144 */
    uint8_t   __pad_146;
    uint8_t   __pad_147;
    uint8_t   EXTMODE;          /* RW     0x148 */
    uint8_t   __pad_149;
    uint8_t   __pad_14A;
    uint8_t   __pad_14B;
    uint8_t   EXTPOLAR;         /* RW     0x14C */
    uint8_t   __pad_14D;
    uint8_t   __pad_14E;
    uint8_t   __pad_14F;
    uint8_t   __pad_150;
    uint8_t   __pad_151;
    uint8_t   __pad_152;
    uint8_t   __pad_153;
    uint8_t   __pad_154;
    uint8_t   __pad_155;
    uint8_t   __pad_156;
    uint8_t   __pad_157;
    uint8_t   __pad_158;
    uint8_t   __pad_159;
    uint8_t   __pad_15A;
    uint8_t   __pad_15B;
    uint8_t   __pad_15C;
    uint8_t   __pad_15D;
    uint8_t   __pad_15E;
    uint8_t   __pad_15F;
    uint8_t   __pad_160;
    uint8_t   __pad_161;
    uint8_t   __pad_162;
    uint8_t   __pad_163;
    uint8_t   __pad_164;
    uint8_t   __pad_165;
    uint8_t   __pad_166;
    uint8_t   __pad_167;
    uint8_t   __pad_168;
    uint8_t   __pad_169;
    uint8_t   __pad_16A;
    uint8_t   __pad_16B;
    uint8_t   __pad_16C;
    uint8_t   __pad_16D;
    uint8_t   __pad_16E;
    uint8_t   __pad_16F;
    uint8_t   __pad_170;
    uint8_t   __pad_171;
    uint8_t   __pad_172;
    uint8_t   __pad_173;
    uint8_t   __pad_174;
    uint8_t   __pad_175;
    uint8_t   __pad_176;
    uint8_t   __pad_177;
    uint8_t   __pad_178;
    uint8_t   __pad_179;
    uint8_t   __pad_17A;
    uint8_t   __pad_17B;
    uint8_t   __pad_17C;
    uint8_t   __pad_17D;
    uint8_t   __pad_17E;
    uint8_t   __pad_17F;
    uint8_t   RSID;             /* RW     0x180 */
    uint8_t   __pad_181;
    uint8_t   __pad_182;
    uint8_t   __pad_183;
    uint32_t  CSPR;             /* WO     0x184 */
    uint32_t  AHBCFG1;          /* RW     0x188 */
    uint32_t  AHBCFG2;          /* RW     0x18C */
    uint8_t   __pad_190;
    uint8_t   __pad_191;
    uint8_t   __pad_192;
    uint8_t   __pad_193;
    uint8_t   __pad_194;
    uint8_t   __pad_195;
    uint8_t   __pad_196;
    uint8_t   __pad_197;
    uint8_t   __pad_198;
    uint8_t   __pad_199;
    uint8_t   __pad_19A;
    uint8_t   __pad_19B;
    uint8_t   __pad_19C;
    uint8_t   __pad_19D;
    uint8_t   __pad_19E;
    uint8_t   __pad_19F;
    uint32_t  SCS;              /* RW     0x1A0 */
    uint16_t  IRCTRIM;          /* RW     0x1A4 */
    uint8_t   __pad_1A6;
    uint8_t   __pad_1A7;
    uint32_t  PCLKSEL0;         /* RW     0x1A8 */
    uint32_t  PCLKSEL1;         /* RW     0x1AC */
} LPC23XX_SCB, *P_LPC23XX_SCB;

#define P_SCB_REGS ((volatile LPC23XX_SCB *)LPC2378_SCB_BASE)
#define SET_PCLK(per,div) ((div) << (per))
#define GET_PCLK_SEL(PCLKSELn,per)     ( ( (PCLKSELn) >> (per) ) & 0x00000003 )


/* 
 * MEMMAP 
 */

#define BOOTLOADERMODE     0x0               
#define USERFLASHMODE      0x1           
#define USERRAMMODE        0x2   
#define EXTERNALMEMORYMODE 0x3                           
#define MAMCR_OFF          0
#define MAMCR_PART         1
#define MAMCR_FULL         2
#define MAMTIM_CYCLES      (((CCLK)+19999999)/20000000)
/* MAM */
#define MAMCR_OFF          0
#define MAMCR_PARTIAL      1
#define MAMCR_FULL         2
//#define MAMTIM_CYCLES      (((CCLK)+19999999)/20000000)
#define MAMTIM_AUTOCFG     0
#define MAMTIM_1_CLK       1
#define MAMTIM_2_CLK       2
#define MAMTIM_3_CLK       3
#define MAMTIM_4_CLK       4
#define MAMTIM_5_CLK       5
#define MAMTIM_6_CLK       6
#define MAMTIM_7_CLK       7
#define MAMTIM_MAX_CLK     7

#define FOSC_INT_RC        (4000000)

#define CLKSRC_INT_RC      (0x00)
#define CLKSRC_MAIN_OSC    (0x01)
#define CLKSRC_MIN_RTC     (0x02)
#define CLKSRCSEL_MASK     (0x03)


#define CCLK_DIV_1         (1)
#define CCLK_DIV_2         (2)
#define CCLK_DIV_4         (4)
#define CCLK_DIV_6         (6)
#define CCLK_DIV_8         (8)
#define CCLK_DIV_10        (10)
#define CCLK_DIV_12        (12)
#define CCLK_DIV_14        (14)
#define CCLK_DIV_16        (16)
#define CCLK_DIV_18        (18)
#define CCLK_DIV_20        (20)
#define CCLK_DIV_22        (22)
#define CCLK_DIV_24        (24)
#define CCLK_DIV_26        (26)
#define CCLK_DIV_28        (28)


#define CLKSRC_INT_RC      (0x00)
#define CLKSRC_MAIN_OSC    (0x01)
#define CLKSRC_MIN_RTC     (0x02)

#define SCS_GPIOM          (1<<0)
#define SCS_EMC_RST_DIS    (1<<1)
#define SCS_MCIPWR         (1<<3)
#define SCS_OSCRANGE       (1<<4)
#define SCS_OSCEN          (1<<5)
#define SCS_OSCSTAT        (1<<6)

#define FCCO               (FOSC*PLL_MUL*2/PLL_DIV) 
#define PLLCON_PLLE        (1<<0)
#define PLLCON_PLLC        (1<<1)
#define PLLSTAT_PLLE       (1<<24)
#define PLLSTAT_PLLC       (1<<25)
#define PLLSTAT_PLOCK      (1<<26)
#define PLLSTAT_MSEL_MASK      (0x00007FFF)
#define PLLSTAT_NSEL_MASK      (0x00FF0000)


#define LED1_SET           FIO0SET
#define LED1_DIR           FIO0DIR
#define LED1_CLR           FIO0CLR
#define LED1_PIN           FIO0PIN
#define LED1_BIT           BIT(21)                                         

#define U0_TX_PINSEL_REG   PINSEL0
#define U0_TX_PINSEL       (1UL<<4)              
#define U0_TX_PINMASK      (3UL<<4)              
#define U0_RX_PINSEL_REG   PINSEL0
#define U0_RX_PINSEL       (1UL<<6)              
#define U0_RX_PINMASK      (3UL<<6)              

#define PDIV_1             0x1
#define PDIV_2             0x2
#define PDIV_4             0x0
#define PDIV_6             0x3
#define PDIV_8             0x3

#define SET_PCLK(per,div) ((div) << (per))
//??add indx
#define  PCLKINDX_WDT                         0
#define  PCLKINDX_TIMER0                      1
#define  PCLKINDX_TIMER1                      2
#define  PCLKINDX_UART0                       3
#define  PCLKINDX_UART1                       4
#define  PCLKINDX_PWM0                        5
#define  PCLKINDX_PWM1                        6
#define  PCLKINDX_I2C0                        7
#define  PCLKINDX_SPI                         8
#define  PCLKINDX_RTC                         9
#define  PCLKINDX_SSP1                       10
#define  PCLKINDX_DAC                        11
#define  PCLKINDX_ADC                        12
#define  PCLKINDX_CAN1                       13
#define  PCLKINDX_CAN2                       14
#define  PCLKINDX_ACF                        15
#define  PCLKINDX_BAT_RAM                    16
#define  PCLKINDX_GPIO                       17
#define  PCLKINDX_PCB                        18
#define  PCLKINDX_I2C1                       19
#define  PCLKINDX_SSP0                       21
#define  PCLKINDX_TIMER2                     22
#define  PCLKINDX_TIMER3                     23
#define  PCLKINDX_UART2                      24
#define  PCLKINDX_UART3                      25
#define  PCLKINDX_I2C2                       26
#define  PCLKINDX_MCI                        27
#define  PCLKINDX_SYSCON                     29

//????

#define PCLK_WDT           0
#define PCLK_TIMER0        2
#define PCLK_TIMER1        4
#define PCLK_UART0         6
#define PCLK_UART1         8
#define PCLK_PWM0          10
#define PCLK_PWM1          12
#define PCLK_I2C0          14
#define PCLK_SPI           16
#define PCLK_RTC           18         
#define PCLK_SSP1          20
#define PCLK_DAC           22
#define PCLK_ADC           24
#define PCLK_CAN1          26
#define PCLK_CAN2          28
#define PCLK_ACF           30

#define PCLK_BAT_RAM       0
#define PCLK_GPIO          2
#define PCLK_PCB           4
#define PCLK_I2C1          6
#define PCLK_SSP0          10
#define PCLK_TIMER2        12
#define PCLK_TIMER3        14
#define PCLK_UART2         16
#define PCLK_UART3         18
#define PCLK_I2C2          20
#define PCLK_I2S           22
#define PCLK_MCI           24
#define PCLK_SYSCON        28

//??
#define PLLCFG_N           1
#define PLLCFG_M           12
#define PLLCFG_VALUE       (((PLLCFG_N - 1) << 16) + ((PLLCFG_M - 1) << 0))
#define MAMTIM_3_CLK       3
#define PLLSTAT_PLOCK      (1<<26)
#define PLLSTAT_PLLE       (1<<24)
#define PLLSTAT_PLLC       (1<<25)

/*
 * OLD CODE:
 * 
 * #define PINSEL0_TX0        PINSEL_MASK (PINSEL_FUNC_ALT_1, 4)
 * #define PINSEL0_RX0        PINSEL_MASK (PINSEL_FUNC_ALT_1, 6)
 * #define PINSEL0_TX1        PINSEL_MASK (PINSEL_FUNC_ALT_1, 30)
 * #define PINSEL1_RX1        PINSEL_MASK (PINSEL_FUNC_ALT_1, 0)
 *
 */

#define ULCR_DLAB_ENABLE   (1<<7)   
#define ULSR_THRE          (1<<5)   
#define UIER_THRE          (1<<1)
#define UART_BAUD(baud)    (U16)((PCLK/((baud)*16.0))+0.5)
#define HOST_BAUD_U0       (38400)

#define FOSC               (12000000)          
#define PLL_MUL            (12)              
#define PLL_DIV            (1)         	     
//??#define CCLK_DIV           (5)        		 
#define CCLK_DIV           (7)        		 
#define CCLK               (FCCO/CCLK_DIV)   
#define CCLKCFG_VALUE      (CCLK_DIV-1)
#define PBSD               1                 
#define PCLK               (CCLK / PBSD)     

#define SD_LED_SET          FIO0SET
#define SD_LED_DIR          FIO0DIR
#define SD_LED_CLR          FIO0CLR
#define SD_LED_PIN          FIO0PIN
#define SD_LED_BIT          BIT(21)                                         

#define USBC_LED_BIT        BIT(14)
#define USB_LINK_LED_BIT    BIT(13)

#define LCD_BL_BIT          BIT(26)



#endif
