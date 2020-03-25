#include <efi/types.h>
#include <efi/boot-services.h>
#include <efi/runtime-services.h>
#include <efi/system-table.h>
#include <efi/protocol/block-io.h>
#include <efi/protocol/partition-info.h>
#include <efi/protocol/simple-text-output.h>


static efi_guid PartitionInfoGUID = EFI_PARTITION_INFO_PROTOCOL_GUID;

static efi_simple_text_output_protocol* out;
static efi_boot_services* bs;

extern void (*__ctor_list)(void);
extern void (*__ctor_end)(void);

static void
call_ctors(void)
{
	void (**f)(void);

	for (f = &__ctor_list; f < &__ctor_end; f++)
		(**f)();
}

static void print(uint32_t value) {
	char16_t output[32];
	char16_t *ptr = output;
	if (value == 0) {
		out->OutputString(out, u"0");
		return;
	}

	ptr += 31;
	*--ptr = 0;
	uint32_t tmp = value;

	while (tmp) {
		*--ptr = '0' + tmp % 10;
		tmp /= 10;
	}
	//if (value < 0) *--ptr = '-';
	out->OutputString(out, ptr);
}

static efi_status fail(efi_status status, const char16_t*  msg) {
	switch(status) {
		case EFI_BUFFER_TOO_SMALL:
			out->OutputString(out, u"BUFFER TOO SMALL"); break;
		case EFI_UNSUPPORTED:
			out->OutputString(out, u"UNSUPPORTED"); break;
		default:
			print(status); break;
	}

	out->OutputString(out, u": ");
	out->OutputString(out, msg);
	out->OutputString(out, u"\n\r");
	return status;
}

static void printGPT(efi_partition_info_protocol* partitionInfo, efi_block_io_protocol* blockIO) {
	out->OutputString(out, u"GPT Partition Record: Name: ");
	out->OutputString(out, partitionInfo->Info.Gpt.PartitionName);
	out->OutputString(out, u"\n\r");
}

static void printMBR(efi_partition_info_protocol* partitionInfo, efi_block_io_protocol* blockIO) {
	out->OutputString(out, u"MBR Partition Record: OSType: ");
	print(partitionInfo->Info.Mbr.OSType);
	out->OutputString(out, u"\n\r");
}

static void printOther(efi_partition_info_protocol* partitionInfo, efi_block_io_protocol* blockIO) {
	out->OutputString(out, u"Other\r\n");
}


extern "C" efi_status
efi_main(efi_handle image, efi_system_table *system) {
	call_ctors();

	bs = system->BootServices;
	out = system->ConOut;

	efi_block_io_protocol *blockIo = NULL;
	efi_partition_info_protocol *partitionInfo;

	size_t memSize = 0;
	efi_status status;

	// Read to zero sized buffer to get memory needed for handles
	status = bs->LocateHandle(ByProtocol, &PartitionInfoGUID, 0, &memSize, 0);
	if (status != EFI_BUFFER_TOO_SMALL)
		return fail(status, u"Cannot read size of partition  handles!");

	size_t noOfHandles = memSize / sizeof(efi_handle);
	print(noOfHandles);
	out->OutputString(out, u" handles found!\r\n");
	if (noOfHandles == 0)
		return fail(EFI_UNSUPPORTED, u"No handles\r\n");
	
	efi_handle handles[noOfHandles];
	status = bs->LocateHandle(ByProtocol, &PartitionInfoGUID, 0, &memSize, handles);
	if (status != EFI_SUCCESS)
		return fail(status, u"Failed to locate partition info!");

	for (size_t n = 0; n < noOfHandles; n++) {
		status = bs->HandleProtocol(handles[n], &PartitionInfoGUID, (void**)&partitionInfo);
		if (status != EFI_SUCCESS)
			return fail(status, u"Cannot get partition info handle!");

		switch (partitionInfo->Type) {
			case PARTITION_TYPE_GPT:
				printGPT(partitionInfo, blockIo);
				break;
			case PARTITION_TYPE_MBR:
				printMBR(partitionInfo, blockIo);
				break;
			case PARTITION_TYPE_OTHER:
				printOther(partitionInfo, blockIo);
				break;
			default:
				return fail(EFI_UNSUPPORTED, u"Unknown partition type!");
		}
	}
	return EFI_SUCCESS;
}
