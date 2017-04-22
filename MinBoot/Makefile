all:
	aarch64-elf-as -o baremetal.elf baremetal.S
	aarch64-elf-objcopy -O binary baremetal.elf baremetal.bin

clean:
	rm -f baremetal.elf
	rm -f baremetal.bin
