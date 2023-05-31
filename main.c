#include "tools/floppy.h"
#include "tools/fileinfo.h"

int main() {
    char* boot_filepath = "/home/roya/workSpace/Project/miniOs/build/boot/boot.o";
    char* setup_filepath = "/home/roya/workSpace/Project/miniOs/build/boot/setup.o";

    Floppy* floppy = create_floppy();

    Fileinfo* boot_fileinfo = read_file(boot_filepath);
    write_bootloader(floppy, boot_fileinfo);

    Fileinfo* setup_fileinfo = read_file(setup_filepath);
    write_floppy_fileinfo(floppy, setup_fileinfo, 0, 0, 2);

    create_image("/home/roya/workSpace/Project/miniOs/a.img", floppy);
}
