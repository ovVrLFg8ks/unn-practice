file(REMOVE_RECURSE
  ".1.0.0"
  "libdaemonLib.pdb"
  "libdaemonLib.so"
  "libdaemonLib.so.1.0.0"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/daemonLib.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
