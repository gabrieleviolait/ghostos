PROJECT=ghostos
BUILD=build
ISO_DIR=iso
BOOT=boot/boot.asm
KERNEL=kernel/kernel.asm
PYTHON?=python3
PAGES=$(wildcard pages/*.GHT)
GEX_PROGRAMS=$(BUILD)/HELLO.GEX
IMG=$(BUILD)/$(PROJECT).img
ISO=$(BUILD)/$(PROJECT).iso

.PHONY: all clean run run-iso iso img check-img check-iso

all: iso

$(BUILD):
	mkdir -p $(BUILD)

check-img:
	@command -v nasm >/dev/null || (echo "Missing nasm. Install with: sudo apt install nasm" && exit 1)
	@command -v $(PYTHON) >/dev/null || (echo "Missing $(PYTHON). Set PYTHON=python if needed." && exit 1)

check-iso:
	@command -v xorriso >/dev/null || (echo "Missing xorriso. Install with: sudo apt install xorriso" && exit 1)

$(BUILD)/HELLO.GEX: programs/hello_gex.asm | $(BUILD)
	nasm -f bin $< -o $@

img: $(BUILD) check-img $(GEX_PROGRAMS)
	nasm -f bin $(BOOT) -o $(BUILD)/boot.bin
	nasm -f bin $(KERNEL) -o $(BUILD)/kernel.bin
	$(PYTHON) tools/mkfat12.py $(IMG) $(BUILD)/boot.bin $(BUILD)/kernel.bin $(PAGES) $(GEX_PROGRAMS)
	@echo "Created $(IMG)"

iso: img check-iso
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
