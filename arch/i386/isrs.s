
#for exceptions without pushing error  
.macro ISR_NOERRCODE num
    .global isr\num
    isr\num:
        cli         
        push $0      
        push $\num    
        jmp isr_common_stub
.endm

#for exceptions with pushing error 
.macro ISR_ERRCODE num
    .global isr\num
    isr\num:
        cli       
        push $\num 
        jmp isr_common_stub
.endm

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE   17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

.extern isr_handler
isr_common_stub:
    #push all registers to stack.
    pusha

    mov %ds, %ax
    push %eax

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    #cpp called 
    push %esp
    call isr_handler
    add $4, %esp

    #restore original state
    pop %eax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    popa

    add $8, %esp

    # hardware return from interrupt
    iret

# --- HARDWARE INTERRUPTS (IRQs) ---

# IRQ 0 (Timer) lands at IDT Entry 32
.global irq0
.extern timer_handler
irq0:
    cli                   #disable interrupts
    pusha                

    call timer_handler 

    popa                 
    sti                   # allow interrupts
    iret                  

# IRQ 1 (Keyboard) lands at IDT Entry 33
.global irq1
.extern keyboard_handler
irq1:
    cli                   #disable interrupts
    pusha                

    call keyboard_handler 

    popa                 
    sti                   # allow interrupts
    iret                  
