#include <libpressio.h>
#include <pressio_version.h>
#include <stdio.h>
#include <stdlib.h>

void print_options(struct pressio_options* options) {
  //produce a human readable string of an options structure
  //there are better ways to get specific values out that we'll cover later
  char* options_str = pressio_options_to_string(options);
  fprintf(stdout, "%s\n\n", options_str);
  free(options_str);
}

int main(int argc, char *argv[])
{
  if(argc != 2) {
    fprintf(stderr, "usage: ./get_compressor_options compressor_id\n");
    return 1;
  }

  //get an instance of a compressor
  struct pressio* library = pressio_instance();
  struct pressio_compressor* comp = pressio_get_compressor(library, argv[1]);
  if(comp == NULL) {
    fprintf(stderr, "%s\n", pressio_error_msg(library));
    return pressio_error_code(library);
  }

  //get the runtime configuration
  struct pressio_options* options = pressio_compressor_get_options(comp);
  printf("runtime configuration\n");
  print_options(options);
  pressio_options_free(options);


  //get the compile-time configuration
  struct pressio_options* configuration = pressio_compressor_get_configuration(comp);
  printf("compile_time configuration\n");
  print_options(configuration);
  pressio_options_free(configuration);

#if LIBPRESSIO_MAJOR_VERSION > 1 || LIBPRESSIO_MINOR_VERSION >= 65
  //newer versions of libpressio also support getting documentation
  struct pressio_options* documentation = pressio_compressor_get_documentation(comp);
  printf("documentation\n");
  print_options(documentation);
  pressio_options_free(documentation);
#endif

  
  pressio_compressor_release(comp);
  pressio_release(library);
  return 0;
}
