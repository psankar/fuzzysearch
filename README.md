fuzzysearch
===========

A simple fuzzy search implementation in C

Search for the string needle in a fuzzy way inside the string haystack.
Resultant words are displayed where each boundaries of each word is
identified by the delimiters array.

To build and run:
    gcc -Wall -Wextra -Werror fuzzysearch.c -o fuzzysearch
    ./fuzzysearch

All the code is available in Creative Commons Zero License.
More information at: http://creativecommons.org/publicdomain/zero/1.0/

If you looking for some ideas to improve this code, you can try to make this
a library with a proper .h file and define APIs. You can also get the results
sorted based on the number_of_characters_matching. You can also use a sqlite
in-memory database to speed up and simplify a lot of operaitons. These will
be easy fixes but I am not focusing on these as the main idea behind implementing
this is to get an idea of fuzzy search and I have got that.
