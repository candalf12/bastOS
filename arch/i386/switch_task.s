.global switch_task_asm

switch_task_asm:
    
    push %ebp
    push %ebx
    push %esi
    push %edi

    mov 20(%esp), %eax     # eax = pointer to old_task->esp
    mov 24(%esp), %ebx     # ebx = new_task->esp

    mov %esp, (%eax)

    mov %ebx, %esp

    pop %edi
    pop %esi
    pop %ebx
    pop %ebp

    ret
