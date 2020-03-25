/* Same as elf_x86_64_fbsd_efi.lds, except for OUTPUT_FORMAT below - KEEP IN SYNC */
OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)
SECTIONS
{
  . = 0;
  ImageBase = .;
  .hash : { *(.hash) }	/* this MUST come first! */
  . = ALIGN(4096);
  .text :
  {
   *(.text)
   *(.text.*)
   *(.gnu.linkonce.t.*)
  }
  . = ALIGN(4096);
  .reloc :
  {
   *(.reloc)
  }
  . = ALIGN(4096);
  .data :
  {
   *(.rodata*)
   *(.got.plt)
   *(.got)
   *(.data*)
   *(.sdata)
   /* the EFI loader doesn't seem to like a .bss section, so we stick
      it all into .data: */
   *(.sbss)
   *(.scommon)
   *(.dynbss)
   *(.bss)
   *(COMMON)
   *(.rel.local)
  }
  . = ALIGN(4096);
  .dynamic  : { *(.dynamic) }
  . = ALIGN(4096);
  .rela :
  {
    *(.rela.data*)
    *(.rela.got)
    *(.rela.stab)
  }
  . = ALIGN(4096);
  .dynsym   : { *(.dynsym) }
  . = ALIGN(4096);
  .dynstr   : { *(.dynstr) }

  . = ALIGN(4096);
  .comment 0 : { *(.comment) }

  . = ALIGN(4096);

  /* A .reloc section is required to flag the final PE32+ file as relocatable
   * Here we hardcode one block with one entry to keep the PE32+ loader happy.
   */
  _a = .;
  . = ALIGN(4096);
  .reloc :
  {
		LONG(. - _a);
    LONG(10);
    SHORT(0);
  }
}
