PROJECT=ghostos
BUILD=build
ISO_DIR=iso
BOOT=boot/boot.asm
KERNEL=kernel/kernel.asm
IMG=$(BUILD)/$(PROJECT).img
ISO=$(BUILD)/$(PROJECT).iso

.PHONY: all clean run run-iso iso img check

all: iso

$(BUILD):
	mkdir -p $(BUILD)

check:
	@command -v nasm >/dev/null || (echo "Missing nasm. Install with: sudo apt install nasm" && exit 1)
	@command -v xorriso >/dev/null || (echo "Missing xorriso. Install with: sudo apt install xorriso" && exit 1)

img: $(BUILD) check
	nasm -f bin $(BOOT) -o $(BUILD)/boot.bin
	nasm -f bin $(KERNEL) -o $(BUILD)/kernel.bin
	dd if=/dev/zero of=$(IMG) bs=512 count=2880 status=none
	dd if=$(BUILD)/boot.bin of=$(IMG) bs=512 count=1 conv=notrunc status=none
	dd if=$(BUILD)/kernel.bin of=$(IMG) bs=512 seek=1 conv=notrunc status=none
	@echo "Created $(IMG)"

iso: img
	mkdir -p $(ISO_DIR)
	cp $(IMG) $(ISO_DIR)/$(PROJECT).img
	xorriso -as mkisofs -quiet -o $(ISO) -b $(PROJECT).img $(ISO_DIR)
	@echo "Created $(ISO)"

run: img
	qemu-system-i386 -fda $(IMG)

run-iso: iso
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(BUILD) $(ISO_DIR)
