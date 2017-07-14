#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/select.h>
#include <unistd.h>

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <pocketsphinx.h>


// Mic reader.c
static void* recognize_from_microphone();

// Mic process.c
void*   get_garde_a_vous(ps_lattice_t* data);