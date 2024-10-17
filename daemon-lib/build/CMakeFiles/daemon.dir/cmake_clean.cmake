file(REMOVE_RECURSE
  ".1.0.0"
  "libdaemon.pdb"
  "libdaemon.so"
  "libdaemon.so.1.0.0"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/daemon.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
