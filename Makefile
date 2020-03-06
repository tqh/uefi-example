LANG="C"

.SUFFIXES= .c .cpp .s .o .so .efi #.lds

ARCH := x86_64


%.efi: %.so
  #Mine
	objcopy -j .peheader -j .text -j .sdata -j .data -j .dynamic -j .dynsym \
      -j .rel -j .rela -j .reloc -S --target=$(FORMAT) $^ $@

%.so: platform/$(ARCH)/relocation_func.o platform/$(ARCH)/start_func.o examples/%.o
	$(LD) $(LDFLAGS) -o $@ $^ $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)

%.S: %.c
	$(CC) headers $(CFLAGS) $(CPPFLAGS) -S $< -o $@


CFLAGS = -I headers -ffreestanding -fno-exceptions -fno-gnu-unique \
		-fno-stack-protector -fshort-wchar -fpic -Wall -Werror

ifeq ($(ARCH),x86_64)
	CFLAGS += -m64 -mno-red-zone -maccumulate-outgoing-args
endif

CPPFLAGS = $(CFLAGS)

LDFLAGS = -t -T platform/$(ARCH)/linker-script.lds -Bsymbolic -shared -nostdlib -znocombreloc

FORMAT = efi-app-$(ARCH)
