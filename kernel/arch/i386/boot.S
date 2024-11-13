# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

.set MBOOT_USE_GFX, 0

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM



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

  jmp higher_half

	# Hang if kernel_main unexpectedly returns.
	cli
1:	hlt
	jmp 1b
.size _start, . - _start
.global initial_page_dir

initial_page_dir:
    .long 0b10000011  # Entry for page directory entry 0
    .rept 768-1
    .long 0            # Fill remaining 767 entries with zero
    .endr

    .long (0 << 22) | 0b10000011  # Map first page
    .long (1 << 22) | 0b10000011  # Map second page
    .long (2 << 22) | 0b10000011  # Map third page
    .long (3 << 22) | 0b10000011  # Map fourth page

    .rept 256-4
    .long 0            # Fill remaining 252 entries with zero
    .endr
.section .text
.global enable_paging
enable_paging:
    # Load the address of the initial page directory (adjusted for -0xC0000000 offset) into ecx
    movl $(initial_page_dir - 0xC0000000), %ecx
    movl %ecx, %cr3

    # Enable PSE (Page Size Extension) by modifying cr4
    movl %cr4, %ecx
    orl $0x10, %ecx
    movl %ecx, %cr4

    # Enable paging by setting the PG (Paging) bit in cr0
    movl %cr0, %ecx
    orl $0x80000000, %ecx
    movl %ecx, %cr0

    ret
higher_half:
    movl $stack_top, %esp       # Set stack pointer to the top of the stack
    pushl %ebx                  
    pushl %eax                 
    xorl %ebp, %ebp             # Clear ebp (set it to 0)
    
	  call kernel_main



