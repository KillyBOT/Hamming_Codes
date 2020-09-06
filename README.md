# Hamming_Codes
I watched a video by 3blue1brown that I really liked, so I decided as a little exercise in C I would program a hamming code encoder and decoder. This hopefully shouldn't take too long, since the algorithm itself is very easy. I also plan on making more complicated error correction programs later.

I initially planned on doing a (265,256) hamming code, but it turns out that (72,64) is much better because they share a gcd of 8, which is 8 bits!