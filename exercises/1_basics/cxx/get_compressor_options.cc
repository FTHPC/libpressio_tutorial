#include <iostream>
#include <libpressio_ext/cpp/libpressio.h>
int main(int argc, char *argv[])
{
    pressio library;
    if(argc != 2) {
        std::cerr << "usage: ./get_compressor_options $id" << std::endl;
        exit(1);
    }

    pressio_compressor compressor = library.get_compressor(argv[1]);
    if(!compressor) {
        std::cerr << library.err_msg() << std::endl;
        exit(library.err_code());
    }

    std::cout << compressor->get_options() << std::endl;

    return 0;
}
