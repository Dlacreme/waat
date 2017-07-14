#include "mic_reader.h"

// Struct def: http://www.speech.cs.cmu.edu/sphinx/doc/doxygen/pocketsphinx/structps__lattice__s.html

void*   get_garde_a_vous(ps_lattice_t* data)
{
    latlink_list_t* seg_list = data.q_head;

    while (seg_list != NULL)
    {

        seg_list = seg_list.next;
    }

    char* c = strdup("Hello les copains");
    return c;
}