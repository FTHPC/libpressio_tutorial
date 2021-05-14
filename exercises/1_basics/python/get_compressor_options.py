#!/usr/bin/env python

import libpressio
import argparse
import pprint

parser = argparse.ArgumentParser()
parser.add_argument("compressor_id")
args = parser.parse_args()

compressor = libpressio.PressioCompressor.from_config({"compressor_id": args.compressor_id})

print("runtime settings")
pprint.pprint(compressor.get_config()['compressor_config'])
print("compile-time settings")
pprint.pprint(compressor.get_compile_config())
