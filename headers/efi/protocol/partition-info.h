#ifndef EFI_PROTOCOL_PARTITION_INFO_H_
#define EFI_PROTOCOL_PARTITION_INFO_H_

#include <efi/types.h>
#include <efi/boot-services.h>
#include <efi/runtime-services.h>

#define EFI_PARTITION_INFO_PROTOCOL_GUID \
    {0x8cf2f62c, 0xbc9b, 0x4821, {0x80, 0x8d, 0xec, 0x9e, 0xc4, 0x21, 0xa1, 0xa0}}

//extern efi_guid PartitionInfoProtocol;

#define EFI_PARTITION_INFO_PROTOCOL_REVISION 0x0001000

#define PARTITION_TYPE_OTHER 0x00
#define PARTITION_TYPE_MBR 0x01
#define PARTITION_TYPE_GPT 0x02


#pragma pack(1)

typedef struct efi_partition_info_protocol efi_partition_info_protocol;
//typedef struct efi_mbr_partition efi_mbr_partition;
//typedef struct efi_gpt_partition efi_gpt_partition;


typedef struct {
    uint8_t BootIndicator;
    uint8_t StartHead;
    uint8_t StartSector;
    uint8_t StartTrack;
    uint8_t OSType;
    uint8_t EndHead;
    uint8_t EndSector;
    uint8_t EndTrack;
//TODO: Any  reason UEFI Spec don't use uint32_t for these two?
    uint8_t StartingLBA[4];
    uint8_t SizeInLBA[4];
} efi_mbr_partition;

typedef struct {
    efi_guid PartitionTypeGUID;
    efi_guid UniquePartitionGUID;
    uint64_t StartingLBA;
    uint64_t EndingLBA;
    uint64_t Attributes;
    char16_t PartitionName[36];
} efi_gpt_partition;

struct efi_partition_info_protocol {
    uint32_t Revision;
    uint32_t Type;
    uint8_t System;
    uint8_t Reserved[7];
    union {
        efi_mbr_partition Mbr;
        efi_gpt_partition Gpt;
    } Info;
};

#pragma pack()
#endif // EFI_PROTOCOL_PARTITION_INFO_H_
