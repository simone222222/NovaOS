#!/bin/bash
set -ex

clang \
  --target=x86_64-unknown-windows \
  -ffreestanding -fshort-wchar -mno-red-zone \
  -I /usr/include/efi -I /usr/include/efi/x86_64 \
  -c novabootmanager.c -o novabootmanager.o

clang \
  --target=x86_64-unknown-windows \
  -nostdlib \
  -Wl,-entry:efi_main \
  -Wl,-subsystem:efi_application \
  -fuse-ld=lld-link \
  novabootmanager.o \
  -o novabootmanager.efi     


  
mkdir -p iso_root/EFI/BOOT
mkdir -p iso_root/EFI/NovaOS
mkdir -p iso_root/boot/grub

cp novabootmanager.efi iso_root/EFI/NovaOS

cat > iso_root/boot/grub/grub.cfg <<EOF
timeout=0
default=0
menuentry "Nova Boot Manager" {
    chainloader /EFI/NovaOS/novabootmanager.efi
}
EOF

grub-mkimage -O x86_64-efi -o BOOTX64.EFI \
  -p /boot/grub \
  part_gpt fat ext2 chain search configfile \
  gfxterm gfxmenu png jpeg font video video_fb terminal

cp BOOTX64.EFI iso_root/EFI/BOOT/

grub-mkrescue -o AutumnOS.iso iso_root
