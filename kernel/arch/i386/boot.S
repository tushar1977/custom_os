# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.global gdt_flush

gdt_flush:
    mov 0x4(%esp), %eax   # Load the GDT pointer (from [esp+4]) into %eax
    lgdt (%eax)           # Load the GDT descriptor

    # Set up segment registers with 0x10 (data segment selector)
    mov $0x10, %ax        # Move 0x10 into %ax (16-bit)
    mov %ax, %ds          # Load %ax into data segment register %ds
    mov %ax, %es          # Load %ax into extra segment register %es
    mov %ax, %fs          # Load %ax into fs register
    mov %ax, %gs          # Load %ax into gs register
    mov %ax, %ss          # Load %ax into stack segment register %ss

    # Far jump to reload CS with 0x08 (code segment selector)
    ljmp $0x08, $flush    # Far jump: segment 0x08, offset flush

flush:
  ret


.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
	movl $stack_top, %esp

	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli
1:	hlt
	jmp 1b
.size _start, . - _start
