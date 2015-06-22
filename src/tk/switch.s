.set INT_DISABLED, 0xc0 /* Disable both FIQ and IRQ. */
.set MODE_SVC, 0x13 /* Supervisor mode */
.set TIMER_INTERRUPT_CLEAR, 0x3f /* Clear interrupts */
.set T0IR, 0xe0004000 /* Timer 0 interrupt register */

.extern T0IR
.extern TKInstrumentedSwitchThread
.extern TKSwitchThread

.global TKDisableInterrupts
.global TKEnableInterrupts
.global TKFirstContextSwitch
.global TKContextSwitchISR
.global TKContextSwitchYield

.code 32

TKDisableInterrupts:
    /* Save current value of CPSR. r0 will be unmodified and r1 will be used
     * as a scratch register to disable interrupts.
     */
    mrs r0, cpsr
    mov r1, r0

    /* Disable interrupts. */
    orr r1, r1, #INT_DISABLED
    msr cpsr_c, r1

    /* Return original CPSR value in r0. */
    bx lr    

TKEnableInterrupts:
    msr cpsr_cxsf, r0
    bx lr

/*
 *  Switch to current task without saving previous context.
 *  Used to run the first task the first time.
 *
 *  Pops the initial context setup up by TKInitStack.
 *  That means this function will act like a return to the task.
 *
 */
TKFirstContextSwitch:
    /* Switch to the current task's stack; sp now points at the task's stack. */
    mov sp, r0

    /* Pop the first value from the stack into r0.
       That value was the CPSR for the task. */
    ldmfd sp!, {r0}
    /* Set SPSR to the CPSR for the task. */
    msr spsr_cxsf, r0

    /* Run task. */
    ldmfd sp!, {r0-r12, lr, pc}^

/*
 * Context switching interrupt handler.
 */
TKContextSwitchISR:
    /* IRQ mode with IRQ stack. */

    /* Adjust LR back by 4 for use later. */
    sub lr, lr, #4

    /* Push working registers R0-R2 to the IRQ stack, as
       we'll use R0-R2 as scratch registers. */
    stmfd sp!, {r0-r2}

    /* Save task's CPSR (stored as the IRQ's SPSR) to R0 */
    mrs r0, spsr

    /* Save LR (the task's PC) to R1 */
    mov r1, lr

    /* Save exception's stack pointer to R2 */
    mov r2, sp

    /* Reset exception's stack by adding 12 to it. It's saved in R2 and we'll be going to svc mode and won't come back */
    add sp, sp, #12

    /* Change to SVC mode with interrupts disabled. */
    msr cpsr_c, #(INT_DISABLED | MODE_SVC)

    /* SVC mode with SVC stack. This is the stack of the interrupted task. */
    /* Push task's PC (saved in r1) */
    /* Push task's LR */
    /* Push task's R3-R12.
       Note that we split up the PC and LR pushes because we want to push
       r1 first but r1 is a lower register than lr (r13). If we combine
       them into one line, stmfd will push them in the wrong order. */
    stmfd sp!, {r1}
    stmfd sp!, {lr}
    stmfd sp!, {r3-r12}

    /* We can't push R0-R2 because we've used them as variables. We need to get the values
     * they had from the IRQ stack where we stored them. To do that we can pop them off the
     * IRQ stack using R2 into R3-R5, which are now free to use since we already stored their
     * values to the task's stack.
     */

    /* Pop 3 values from the IRQ stack using R2 as the stack pointer and load them into R3-R5. */
    ldmfd r2, {r3-r5}

    /* Push those 3 values to the task's stack */
    stmfd sp!, {r3-r5}

    /* Push the task's CPSR, which is in R0, to the task's stack. */
    stmfd sp!, {r0}

    /* Clear the timer interrupt. */
    ldr r0, =T0IR
    mov r1, #TIMER_INTERRUPT_CLEAR
    str r1, [r0]

    /* Call the scheduler, which will pick a new CurrentThread. */
    mov r0, sp
    bl TKInstrumentedSwitchThread

    /* Increment the tick. */
    bl TKIncrementTick

    /* Set SP to the new task's SP by reading the value stored in *CurrentThread. */
    ldr r0, =CurrentThread
    ldr r0, [r0]
    ldr sp, [r0]

    /* Pop task's CPSR and restore it to SPSR.
       SPSR will be moved to CPSR when we pop the context with ldmfd */
    ldmfd sp!, {r0}
    msr spsr_cxsf, r0

    /* Pop task's context, restores registers, which sets interrupts back to
     * their previous state.
     */
    ldmfd sp!, {r0-r12, lr, pc}^

TKContextSwitchYield:
    /* Save all context, with PC = LR. */
    stmfd sp!, {lr}
    stmfd sp!, {lr}
    stmfd sp!, {r0-r12}
    mrs r0, cpsr
    stmfd sp!, {r0}

    /* Call the scheduler, which will pick a new CurrentThread. Note that we
     * purposefully do not instrument this context switch because the
     * instrumentation data is shared between this context switch and the
     * scheduler-triggered one, so we need to avoid clobbering each other.
     */
    mov r0, sp
    bl TKSwitchThread

    /* Set SP to the new task's SP by reading the value stored in *CurrentThread. */
    ldr r0, =CurrentThread
    ldr r0, [r0]
    ldr sp, [r0]

    /* Pop task's CPSR and restore it to SPSR. */
    /* SPSR will be moved to CPSR when we pop the context with ldmfd */
    ldmfd sp!, {r0}
    msr spsr_cxsf, r0

    /* Pop task's context, restores registers. */
    ldmfd sp!, {r0-r12, lr, pc}^
