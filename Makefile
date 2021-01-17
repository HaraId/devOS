GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS  = --32

LDPARAMS = -melf_i386

objects = obj/loader.o \
		  obj/common/stdlib.o \
		  obj/tm/stdio.o \
		  obj/hardwarecommunication/gdt.o \
		  obj/hardwarecommunication/port.o \
		  obj/hardwarecommunication/interruptstubs.o \
		  obj/hardwarecommunication/interrupts.o \
		  obj/drivers/driver.o\
		  obj/drivers/PS2Keyboard.o \
		  obj/drivers/PS2Mouse.o \
		  obj/kernel.o

bin-files = mykernel.bin mykernel.iso

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	gcc $(GPPPARAMS) -c -o $@  $<
	
obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<
	
mykernel.bin: src/linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)
	
install: mykernel.bin 
	sudo cp $< /boot/mykernel.bin
	

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Os sys" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso
	
	
run: mykernel.iso
	qemu-system-x86_64 -hda bin/dharma.qcow -boot d -cdrom mykernel.iso -m 640
	#VirtualBox --startvm "DaramOsi" &

	
	
.PHONY: clean
clean:
	rm -f $(objects) mykernel.bin mykernel.iso
