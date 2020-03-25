# UEFI Example
Replaces efi-example project. Still work in progress.

Headers from https://fuchsia.googlesource.com/fuchsia/+/refs/heads/master/zircon/kernel/lib/efi/ except for:
 * partition-info.h
 * block-io.h from Fuchsia lacked EFIAPI on WriteBlocks, so this has been fixed here
