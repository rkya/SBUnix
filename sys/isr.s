.text

#ISRs:

.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31

# IRQs

.global irq0
.global irq1
.global irq2
.global irq3
.global irq4
.global irq5
.global irq6
.global irq7
.global irq8
.global irq9
.global irq10
.global irq11
.global irq12
.global irq13
.global irq14
.global irq15
.global irq128


	# 0: Divide by Zero Exception
isr0:
	cli
	pushq $0 		# dummy error code
	pushq $0 		# isr number
	jmp isr_common_stub

	# 1: Debug Exception
isr1:
	cli
	pushq $0 		# dummy error code
	pushq $1 		# isr number
	jmp isr_common_stub

	# 2: NMI Exception
isr2:
	cli
	pushq $0 		# dummy error code
	pushq $2 		# isr number
	jmp isr_common_stub

	# 3: Breakpoint Exception
isr3:
	cli
	pushq $0 		# dummy error code
	pushq $3 		# isr number
	jmp isr_common_stub

	# 4: Into Detected Overflow Exception
isr4:
	cli
	pushq $0 		# dummy error code
	pushq $4 		# isr number
	jmp isr_common_stub

	# 5: Out of bounds Exception
isr5:
	cli
	pushq $0 		# dummy error code
	pushq $5 		# isr number
	jmp isr_common_stub

	# 6: Invalid Opcode Exception
isr6:
	cli
	pushq $0 		# dummy error code
	pushq $6 		# isr number
	jmp isr_common_stub

	# 7: No Coprocessor Exception
isr7:
	cli
	pushq $0 		# dummy error code
	pushq $7 		# isr number
	jmp isr_common_stub

	# 8: Double Fault Exception
isr8:
	cli
	pushq $8 		# isr number
	jmp isr_common_stub

	# 9: Coprocessor Segment Overrun Exception
isr9:
	cli
	pushq $0 		# dummy error code
	pushq $9 		# isr number
	jmp isr_common_stub

	# 10: Bad TSS Exception
isr10:
	cli
	pushq $10 		# isr number
	jmp isr_common_stub

	# 11: Segment Not Present Exception
isr11:
	cli
	pushq $11 		# isr number
	jmp isr_common_stub

	# 12: Stack Fault Exception
isr12:
	cli
	pushq $12 		# isr number
	jmp isr_common_stub

	# 13: General Protection Fault Exception
isr13:
	cli
	pushq $13 		# isr number
	jmp isr_common_stub

	# 14: Page Fault Exception
isr14:
	cli
	pushq $14 		# isr number
	jmp isr_common_stub

	# 15: Unknown Interrupt Exception
isr15:
	cli
	pushq $0 		# dummy error code
	pushq $15 		# isr number
	jmp isr_common_stub

	# 16: Coprocessor Fault Exception
isr16:
	cli
	pushq $0 		# dummy error code
	pushq $16 		# isr number
	jmp isr_common_stub

	# 17: Alignment Check Exception (486+)
isr17:
	cli
	pushq $0 		# dummy error code
	pushq $17 		# isr number
	jmp isr_common_stub

	# 18: Machine Check Exception (Pentium/586+)
isr18:
	cli
	pushq $0 		# dummy error code
	pushq $18 		# isr number
	jmp isr_common_stub

	# 19: Reserved Exceptions
isr19:
	cli
	pushq $0 		# dummy error code
	pushq $19 		# isr number
	jmp isr_common_stub

	# 20: Reserved Exceptions
isr20:
	cli
	pushq $0 		# dummy error code
	pushq $20 		# isr number
	jmp isr_common_stub

	# 21: Reserved Exceptions
isr21:
	cli
	pushq $0 		# dummy error code
	pushq $21 		# isr number
	jmp isr_common_stub

	# 22: Reserved Exceptions
isr22:
	cli
	pushq $0 		# dummy error code
	pushq $22 		# isr number
	jmp isr_common_stub

	# 23: Reserved Exceptions
isr23:
	cli
	pushq $0 		# dummy error code
	pushq $23 		# isr number
	jmp isr_common_stub

	# 24: Reserved Exceptions
isr24:
	cli
	pushq $0 		# dummy error code
	pushq $24 		# isr number
	jmp isr_common_stub

	# 25: Reserved Exceptions
isr25:
	cli
	pushq $0 		# dummy error code
	pushq $25 		# isr number
	jmp isr_common_stub

	# 26: Reserved Exceptions
isr26:
	cli
	pushq $0 		# dummy error code
	pushq $26 		# isr number
	jmp isr_common_stub

	# 27: Reserved Exceptions
isr27:
	cli
	pushq $0 		# dummy error code
	pushq $27 		# isr number
	jmp isr_common_stub

	# 28: Reserved Exceptions
isr28:
	cli
	pushq $0 		# dummy error code
	pushq $28 		# isr number
	jmp isr_common_stub

	# 29: Reserved Exceptions
isr29:
	cli
	pushq $0 		# dummy error code
	pushq $29 		# isr number
	jmp isr_common_stub

	# 30: Reserved Exceptions
isr30:
	cli
	pushq $0 		# dummy error code
	pushq $30 		# isr number
	jmp isr_common_stub

	# 31: Reserved Exceptions
isr31:
	cli
	pushq $0 		# dummy error code
	pushq $31 		# isr number
	jmp isr_common_stub


# 32: Stack Fault Exception
irq0:
  cli
  pushq $0 		# dummy error code
  pushq $32 		# irq number
  jmp irq_common_stub

# 33: General Protection Fault Exception
irq1:
  cli
  pushq $0 		# dummy error code
  pushq $33 		# irq number
  jmp irq_common_stub

# 34: Page Fault Exception
irq2:
  cli
  pushq $0 		# dummy error code
  pushq $34 		# irq number
  jmp irq_common_stub

# 35: Unknown Interrupt Exception
irq3:
  cli
  pushq $0 		# dummy error code
  pushq $35 		# irq number
  jmp irq_common_stub

# 36: Coprocessor Fault Exception
irq4:
  cli
  pushq $0 		# dummy error code
  pushq $36 		# irq number
  jmp irq_common_stub

# 37: Alignment Check Exception (486+)
irq5:
  cli
  pushq $0 		# dummy error code
  pushq $37 		# irq number
  jmp irq_common_stub

# 38: Machine Check Exception (Pentium/586+)
irq6:
  cli
  pushq $0 		# dummy error code
  pushq $38 		# irq number
  jmp irq_common_stub

# 39: Reserved Exceptions
irq7:
  cli
  pushq $0 		# dummy error code
  pushq $39 		# irq number
  jmp irq_common_stub

# 40: Reserved Exceptions
irq8:
  cli
  pushq $0 		# dummy error code
  pushq $40 		# irq number
  jmp irq_common_stub

# 41: Reserved Exceptions
irq9:
  cli
  pushq $0 		# dummy error code
  pushq $41 		# irq number
  jmp irq_common_stub

# 42: Reserved Exceptions
irq10:
  cli
  pushq $0 		# dummy error code
  pushq $42 		# irq number
  jmp irq_common_stub

# 43: Reserved Exceptions
irq11:
  cli
  pushq $0 		# dummy error code
  pushq $43 		# irq number
  jmp irq_common_stub

# 44: Reserved Exceptions
irq12:
  cli
  pushq $0 		# dummy error code
  pushq $44 		# irq number
  jmp irq_common_stub

# 45: Reserved Exceptions
irq13:
  cli
  pushq $0 		# dummy error code
  pushq $45 		# irq number
  jmp irq_common_stub

# 46: Reserved Exceptions
irq14:
  cli
  pushq $0 		# dummy error code
  pushq $46 		# irq number
  jmp irq_common_stub

# 47: Reserved Exceptions
irq15:
  cli
  pushq $0 		# dummy error code
  pushq $47 		# irq number
  jmp irq_common_stub

# 128: Reserved Exceptions
irq128:
  cli
  pushq $0 		# dummy error code
  pushq $128 		# irq number
  jmp irq_common_stub


# ISR common stub: saves the processor state, sets up kernel mode segments
# calls C-level fault handler and restores the stack frame

/*from http://forum.osdev.org/viewtopic.php?f=1&t=17306*/

.extern fault_handler

isr_common_stub:
  #iretq
  # pusha
  pushq %rax
  pushq %rcx
  pushq %rdx
  pushq %rbx
  pushq %rsp
  pushq %rbp
  pushq %rsi
  pushq %rdi

  movq %ds, %rax
  pushq %rax

  movq $0x10, %rax	# load the kernel data segment descriptor
  movq %rax, %ds
  movq %rax, %es
  movq %rax, %fs
  movq %rax, %gs

  # mov %eax, fault_handler
  # call %eax
  movq %rsp, %rdi
  callq fault_handler

  popq %rax
  movq %rax, %ds
  movq %rax, %es
  movq %rax, %fs
  movq %rax, %gs

  popq %rdi
  popq %rsi
  popq %rbp
  popq %rsp
  popq %rbx
  popq %rdx
  popq %rcx
  popq %rax

  add $0x10, %rsp
  sti
  iretq


.extern irq_handler
# IRQ common stub. It saves processor state, sets up for kernel mode segments
# calls the C-level fault handler, and finally restores the stack frame.

irq_common_stub:
  # pusha
  pushq %rax
  pushq %rcx
  pushq %rdx
  pushq %rbx
  pushq %rsp
  pushq %rbp
  pushq %rsi
  pushq %rdi

  movq %ds, %rax
  pushq %rax

  movq $0x10, %rax
  movq %rax, %ds
  movq %rax, %es
  movq %rax, %fs
  movq %rax, %gs

	movq %rsp, %rdi
  callq irq_handler

  popq %rbx
  movq %rbx, %ds
  movq %rbx, %es
  movq %rbx, %fs
  movq %rbx, %gs

  # popa
  popq %rdi
  popq %rsi
  popq %rbp
  popq %rsp
  popq %rbx
  popq %rdx
  popq %rcx
  popq %rax


  add $0x10, %rsp
  sti
  iretq




#irq_common_stub:
#  # pusha
#  pushq %rdi
#  pushq %rax
#  pushq %rbx
#  pushq %rcx
#  pushq %rdx
#  pushq %rbp
#  pushq %rsi
#  pushq %r8
#  pushq %r9
#  movq %rsp, %rdi
#
#  callq irq_handler
#
#  popq %r9
#  popq %r8
#  popq %rsi
#  popq %rbp
#  popq %rdx
#  popq %rcx
#  popq %rbx
#  popq %rax
#  popq %rdi
#  add $16, %rsp
#  iretq
