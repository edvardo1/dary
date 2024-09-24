this is a header only library for dynamic arrays in C.
tries its best not to use macros, but includes two for convenience's sake

MEANT FOR PROTOTYPES ONLY (aborts on failed malloc and realoc).

the user can provide their own custom alloc, free, abort and choose not
to use the stdlib implementations.

tests and examples provided in test.c
