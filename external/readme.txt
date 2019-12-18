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