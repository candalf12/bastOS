.global load_page_directory
.global enable_paging

#writes the physical address to cr3
load_page_directory:
    push %ebp
    mov %esp, %ebp
    mov 8(%ebp), %eax     
    mov %eax, %cr3        
    mov %ebp, %esp
    pop %ebp
    ret

#makes the 31th bit 1 in cr0. 
enable_paging:
    push %ebp
    mov %esp, %ebp
    mov %cr0, %eax         #reads the cr0 value
    or $0x80000000, %eax   
    mov %eax, %cr0         #rewrite the value to cr0 again. paging.
    mov %ebp, %esp
    pop %ebp
    ret
