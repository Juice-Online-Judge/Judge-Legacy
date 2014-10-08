import platform

debug = ARGUMENTS.get("debug", 0)
build32 = ARGUMENTS.get("build32", int(platform.architecture()[0] == "32bit"))
debug = int(debug)
Export("debug build32")

SConscript("src/SConscript", variant_dir = "build", duplicate = 0)
