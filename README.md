# uuencoding

uuencoding is an old encoding mechanism that stands for unix-to-unix encoding.
Essentially, since all the printing characters only require six bits to express their integer value a file can be compressed substantially
by shifting the bits across boundaries, discarding the unused high bits, and only sending the six low bits.  This also is a strategy
to keep the files being transfered safe since they may pass through non-unix environments which are not 8-bit clean, and could become corrupt or modified on their way to the reciever.

This is a reimplementation of uuencoding done personally for educational reasons.
The idea for this challenge came from r/dailyprogrammer.

This was actualy quite a fun project to undertake because it was my first multi-threaded program.

## Compilation
Run ./autogen.sh in your shell, and omit the install step.
