#include "libpressio.h"
#include "assert.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

// normally a macro expands prior to invoking the next level of a macro, but the
// # operator prevents expansion, so use two levels of macros to concatenate
// these values
#define CONCAT(a, b, c) CONCAT_INNER(a, b, c)
#define CONCAT_INNER(a,b,c) #a "." #b "." #c

const size_t dims[3] = {100, 100, 100};

struct pressio_data* make_input_data() {
  size_t n_dims = sizeof(dims)/sizeof(dims[0]);
  struct pressio_data* data = pressio_data_new_owning(pressio_float_dtype, n_dims, dims);
  auto f = static_cast<float*>(pressio_data_ptr(data, nullptr));
  for (int i = 0; i < dims[0]; ++i) {
    for (int j = 0; j < dims[1]; ++j) {
      for (int k = 0; k < dims[2]; ++k) {
        if(i == 0 || j == 0 || k == 0 || i == (dims[0] -1) || j == (dims[1] - 1) || k == (dims[2] -1)) {
          *f = 0;
        } 
        else {
          *f = 1;
        }
        ++f;
      }
    }
  }
  return data;
}

void assert_data_same(pressio_data const* d1, pressio_data const* d2) {
  size_t n_dims = 0;
  assert(pressio_data_dtype(d1) == pressio_data_dtype(d2));
  assert((n_dims = pressio_data_num_dimensions(d1)) == pressio_data_num_dimensions(d2));
  for (int i = 0; i < n_dims; ++i) {
    assert(pressio_data_get_dimension(d1, i) == pressio_data_get_dimension(d2, i));
  }

  size_t n_elms = pressio_data_num_elements(d1);
  auto d1_bytes  = static_cast<const float*>(pressio_data_ptr(d1, nullptr));
  auto d2_bytes = static_cast<const float*>(pressio_data_ptr(d2, nullptr));

  for (size_t i = 0; i < n_elms; ++i) {
    assert(d1_bytes[i] == d2_bytes[i]);
  }
}
void assert_type_is(struct pressio_options* options, const char* key, pressio_option_type type) {
  assert(pressio_options_exists(options, key) == pressio_options_key_set);
  struct pressio_option* option = pressio_options_get(options, key);
  assert(pressio_option_get_type(option) == type);
  pressio_option_free(option);
}

int main(int argc, char *argv[])
{
  struct pressio* library = pressio_instance();
  struct pressio_compressor* compressor = pressio_get_compressor(library, "typeswitch");
  assert(compressor && "compressor must be registered");

  const char* prefix = pressio_compressor_get_prefix(compressor);
  assert(strcmp(prefix, "typeswitch") == 0);

  struct pressio_options* options = pressio_compressor_get_options(compressor);
  assert(options);
  assert_type_is(options, "typeswitch:integral", pressio_option_charptr_type);
  assert_type_is(options, "typeswitch:floating", pressio_option_charptr_type);

  struct pressio_options* configuration = pressio_compressor_get_configuration(compressor);
  assert(configuration);
  assert_type_is(configuration, "pressio:thread_safe", pressio_option_threadsafety_type);
  assert_type_is(configuration, "pressio:stability", pressio_option_charptr_type);

  struct pressio_options* documentation = pressio_compressor_get_documentation(compressor);
  assert_type_is(documentation, "pressio:description", pressio_option_charptr_type);
  assert_type_is(options, "typeswitch:integral", pressio_option_charptr_type);
  assert_type_is(options, "typeswitch:floating", pressio_option_charptr_type);
  assert(documentation);

  struct pressio_data* input = make_input_data();
  struct pressio_data* compressed = pressio_data_new_empty(pressio_byte_dtype, 0, nullptr);
  struct pressio_data* decompressed = pressio_data_new_owning(
      pressio_data_dtype(input),
      pressio_data_num_dimensions(input),
      dims
      );

  pressio_options_set_string(options, "typeswitch:integral", "blosc");
  pressio_options_set_string(options, "typeswitch:floating", "sz");
  pressio_options_set_string(options, "sz:error_bound_mode_str", "abs");
  pressio_options_set_string(options, "blosc:compressor", "zstd");
  pressio_options_set_double(options, "sz:abs_err_bound", 1e-6);
  pressio_options_set_integer(options, "blosc:clevel", 9);
  if(pressio_compressor_set_options(compressor, options)) {
    printf("%s\n", pressio_compressor_error_msg(compressor));
    exit(pressio_compressor_error_code(compressor));
  }
  if(pressio_compressor_compress(compressor, input, compressed)) {
    printf("%s\n", pressio_compressor_error_msg(compressor));
    exit(pressio_compressor_error_code(compressor));
  }

  assert(pressio_data_dtype(compressed) == pressio_byte_dtype);
  assert(pressio_data_get_bytes(input) > pressio_data_get_bytes(compressed));
  assert(pressio_data_get_bytes(compressed) != 0);

  if(pressio_compressor_decompress(compressor, compressed, decompressed)) {
    printf("%s\n", pressio_compressor_error_msg(compressor));
    exit(pressio_compressor_error_code(compressor));
  }

  pressio_data_free(input);
  pressio_data_free(compressed);
  pressio_data_free(decompressed);
  pressio_options_free(options);
  pressio_options_free(configuration);
  pressio_options_free(documentation);
  pressio_compressor_release(compressor);
  pressio_release(library);
  printf("passed!\n");
  return 0;
}
