.global gdt_flush
.type gdt_flush, @function

gdt_flush:
    mov 4(%esp), %eax
    
    lgdt (%eax)

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    ljmp $0x08, $flush

.global tss_flush
.type tss_flush, @function
tss_flush:
    mov $0x2B, %ax      # 5*8 and + ring 3 
    ltr %ax             # Load Task Register
    ret

.global enter_user_mode
.type enter_user_mode, @function
enter_user_mode:
    cli               
    mov $0x23, %ax     
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    
    #pushing for iret 
    push $0x23          # Push User SS
    push %esp           # Push User ESP
    pushf               # Push EFLAGS
    
    # force IF to 1 for interrupts to occur in ring 3 
    pop %eax
    or $0x200, %eax
    push %eax

    push $0x1B          # Push User CS (Entry 3 * 8 bytes + 3)
    push $user_function # Push the Instruction Pointer to jump to

    iret                #fake interrupt to go to ring3 

user_function:
    # A normal user program would run here.  
1:  jmp 1b
flush:
    ret
