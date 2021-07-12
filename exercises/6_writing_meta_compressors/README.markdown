# Writing a meta compressor

The goal of this exercise is to learn about how to write meta-compressors and what facilities that libpressio provides to make it easier for you.
We will accomplish this by writing a meta-commpressor called `type_switch` which selects between two compressors depending on if the data passed in is
integral or floating point.  This might be useful because traditional lossless compressors can perform very well with lower overhead on integral datasets than
error bounded lossy compressors.  A more general version of this appears in LibPressio's standard meta-compressors under the name switch.

There are a few files in this directory:


+ `test_pressio_base.cc` automated tests for the plugin that you are developing
+ `pressio_typeswitch_base.cc` where you will implement your plugin 
+ `pressio_typeswitch_solution.cc` a solution for this exercise

## Initializing Meta Components

For a single meta-components, the initialization looks like this:

```cpp
pressio_compressor integral = compressor_plugins().build("noop");
std::string integral_id = "noop";
```

This provides a place to store the id for the active compressor, and uses the `pressio_compressor` class which handles cloning the plugin if the meta-plugin is cloned.

## Configuring Options for meta compressors

Well behaved meta compressors follow the following rules for `set_options_impl`, `get_options_impl`, `get_configuration_impl`, `get_documentation_impl`, and `get_metrics_results_impl`.

1. Options for delegated to compressors are always done before the meta-compressor itself
2. Each function should call the corresponding function in the delegated to compressor either directly with the results copied into the current options structure or through one of the helpers `get_meta`, `set_meta`, or `set_meta_docs` for `get_documentation_impl` which does this implicitly.

Following these rules enables users to configure the delegated to compressors completely.

There are also `get_meta_many`, `set_meta_many`, `set_meta_many_docs` versions that operate when a variable list of plugins can be used.

## Naming

Configurable things in LibPressio have "names" associated with them.  By default, the name is the empty string, but if a name is provided, the names get changed to a name that reflects the current hierarchy that is being used.  This allows each entry to be configured separately if needed.

A meta-compressor should provide a `set_name_impl` function to set the name for the delegated to compressors.  By convention the name `name + '/' + child->prefix()` is used where it makes sense.

## Exercises

1. Complete the `pressio_typeswitch_base.cc` implementation
2. Think about an implement a meta compressor that is useful for you

