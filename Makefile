BUILD:=./build

HD_IMG_NAME:= "hd.img"

all: ${BUILD}/boot/boot.o ${BUILD}/boot/setup.o
	$(shell rm -rf $(HD_IMG_NAME))
	bximage -q -hd=16 -func=create -sectsize=512 -imgmode=flat $(HD_IMG_NAME)
	dd if=${BUILD}/boot/boot.o of=hd.img bs=512 seek=0 count=1 conv=notrunc
	dd if=${BUILD}/boot/setup.o of=hd.img bs=512 seek=1 count=2 conv=notrunc

${BUILD}/boot/%.o: oskernel/boot/%.asm
	$(shell mkdir -p ${BUILD}/boot)
	nasm $< -o $@

clean:
	$(shell rm -rf ${BUILD})

bochs: all
	bochs -q -f bochsrc

qemug: all
	qemu-system-x86_64 -hda hd.img -S -s

qemu: all
	qemu-system-x86_64 -hda hd.img
