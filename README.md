# liblzhl

The SAGE engine games support LZHL compression from [An Algorithm for Online Data Compression](https://web.archive.org/web/20180127002629/http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/1998/9810/ignatche/ignatche.htm)
as a possible algorithm for map compression in place of Refpack although it goes unused. It appears to be part of an
EA compression library that bundles together several compression algorithms and thus is likely used in some EA game or another.

This library version is mostly the original code modified only where required to compile correctly and to add functions
that were added to the EA build of it along with a CMake build system. It is provided here in the hope someone finds it
useful to have the same implementation as used by EA.