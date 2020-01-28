external folder structure:

external
 \_include  (external library includes)
 |  \_boost
 |  |_gtest
 |  |_...
 |_Debug
 |  \_bin   (debug: *.dll, *.pdb)
 |  |_lib   (debug: *.lib)
 |_Release
 |  \_bin   (release: *.dll, *.pdb)
 |  |_lib   (release: *.lib)
 |_sources
    \_boost
    |_gtest
    |_...
    
build_gtest.bat - create folder structure, download gtest from github,
build gtest.

build_boost.bat - boost src must be downloaded and extracted.
build boost.