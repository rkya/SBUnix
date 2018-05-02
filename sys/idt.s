.text

# References: http://www.osdever.net/bkerndev/Docs/idt.htm
.global _x86_64_asm_lidt
_x86_64_asm_lidt:
  lidt (%rdi)
  retq


