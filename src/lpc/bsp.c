
#include "lpc/lpc2378.h"
#include "lpc/bsp.h"

/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Get the CPU clock frequency.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*********************************************************************************************************
*/

INT32U  BSP_CPU_ClkFreq (void)
{
    INT32U  msel;
    INT32U  nsel;
    INT32U  fin;
    INT32U  pll_clk_feq;                                    /* When the PLL is enabled, this is Fcco       */
    INT32U  clk_div;
    INT32U  clk_freq;


    switch (SCB_READREG8(CLKSRCSEL) & 0x03) {                                 /* Determine the current clock source                       */
        case 0:
             fin = IRC_OSC_FRQ;
             break;

        case 1:
             fin = MAIN_OSC_FRQ;
             break;

        case 2:
             fin = RTC_OSC_FRQ;
             break;

        default:
             fin = IRC_OSC_FRQ;
             break;
    }

    if ((SCB_READREG32(PLLSTAT) & (1 << 25)) > 0) {                                /* If the PLL is currently enabled and connected        */
        msel        = (INT32U)(SCB_READREG32(PLLSTAT) & 0x3FFF) + 1;           /* Obtain the PLL multiplier                            */
        nsel        = (INT32U)((SCB_READREG32(PLLSTAT) >>   16) & 0x0F) + 1;   /* Obtain the PLL divider                               */
        pll_clk_feq = (2 * msel * (fin / nsel));                    /* Compute the PLL output frequency                     */
    } else {
        pll_clk_feq = (fin);                                        /* The PLL is bypassed                                  */
    }

    clk_div         = (INT32U)(SCB_READREG32(CCLKCFG) & 0xFF) + 1;             /* Obtain the CPU core clock divider                    */
    clk_freq        = (INT32U)(pll_clk_feq / clk_div);          /* Compute the ARM Core clock frequency                 */

    return (clk_freq);
}

/*
*********************************************************************************************************
*                                            BSP_CPU_PclkFreq()
*
* Description : Get the peripheral clock frequency for a specific peripheral.
*
* Argument(s) : pclk        The peripheral clock ID, one of PCLK_??? defined in bsp.h.
*
* Return(s)   : The peripheral's clock in Hz
*********************************************************************************************************
*/


INT32U  BSP_CPU_PclkFreq (INT8U  pclk)
{
    INT32U  clk_freq;
    INT32U  selection;


    clk_freq = BSP_CPU_ClkFreq();

    switch (pclk) {
        case PCLKINDX_WDT:
        case PCLKINDX_TIMER0:
        case PCLKINDX_TIMER1:
        case PCLKINDX_UART0:
        case PCLKINDX_UART1:
        case PCLKINDX_PWM0:
        case PCLKINDX_PWM1:
        case PCLKINDX_I2C0:
        case PCLKINDX_SPI:
        case PCLKINDX_RTC:
        case PCLKINDX_SSP1:
        case PCLKINDX_DAC:
        case PCLKINDX_ADC:
        case PCLKINDX_CAN1:
        case PCLKINDX_CAN2:
        case PCLKINDX_ACF:
             selection = ((SCB_READREG32(PCLKSEL0) >> (pclk * 2)) & 0x03);
             if (selection == 0) {
                 return (clk_freq / 4);
             } else if (selection == 1) {
                 return (clk_freq);
             } else if (selection == 2) {
                 return (clk_freq / 2);
             } else {
                 return (clk_freq / 8);
             }

        case PCLKINDX_BAT_RAM:
        case PCLKINDX_GPIO:
        case PCLKINDX_PCB:
        case PCLKINDX_I2C1:
        case PCLKINDX_SSP0:
        case PCLKINDX_TIMER2:
        case PCLKINDX_TIMER3:
        case PCLKINDX_UART2:
        case PCLKINDX_UART3:
        case PCLKINDX_I2C2:
        case PCLKINDX_MCI:
        case PCLKINDX_SYSCON:
             selection = ((SCB_READREG32(PCLKSEL1) >> ((pclk - 16) * 2)) & 0x03);
             if (selection == 0) {
                 return (clk_freq / 4);
             } else if (selection == 1) {
                 return (clk_freq);
             } else if (selection == 2) {
                 return (clk_freq / 2);
             } else {
                 return (clk_freq / 8);
             }

        default:
             return (0);
    }
}


/*
*********************************************************************************************************
*                                           BSP_IntDisAll()
*
* Description : Disable ALL interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  BSP_IntDisAll (void)
{
    WRITEREG32(VICINTENCLEAR, 0xFFFFFFFFL);                                /* Disable ALL interrupts                                   */
}
