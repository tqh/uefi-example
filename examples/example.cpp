#include <efi/types.h>
#include <efi/boot-services.h>
#include <efi/runtime-services.h>
#include <efi/system-table.h>
#include <efi/protocol/simple-text-output.h>


extern "C" efi_status
efi_main(efi_handle image, efi_system_table *system)
{
	size_t index;
  //u string prefix for char16_t
  system->ConOut->OutputString(system->ConOut, u"Hello World!\r\n");
  system->BootServices->WaitForEvent(1, &system->ConIn->WaitForKey, &index);
	return EFI_SUCCESS;
}
