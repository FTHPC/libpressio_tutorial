#include <libpressio_ext/cpp/libpressio.h>
#include <std_compat/memory.h>
#include <sstream>

struct pressio_runlength : public libpressio_compressor_plugin {

  int set_options_impl(pressio_options const& opts) override {
    return 0;
  }
  pressio_options get_options_impl() const override {
    pressio_options opts;
    return opts;
  }
  pressio_options get_configuration_impl() const override {
    pressio_options opts;
    return opts;
  }
  pressio_options get_documentation_impl() const override {
    pressio_options opts;
    return opts;
  }

  int compress_impl(pressio_data const* input, pressio_data* output) override {
    return 0;
  }
  int decompress_impl(pressio_data const* input, pressio_data* output) override {
    return 0;
  }

  int major_version() const override {
    return 0;
  }
  int minor_version() const override {
    return 0;
  }
  int patch_version() const override {
    return 0;
  }

  const char* prefix() const override {
    return "runlength";
  }
  const char* version() const override {
    return nullptr;
  }

  std::shared_ptr<libpressio_compressor_plugin> clone() override {
    return nullptr;
  }

};

pressio_register pressio_runlength_register(compressor_plugins(), "runlength", []{
      return compat::make_unique<pressio_runlength>();
    });
