extern crate libc;

//#[link_args = "-DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\" `pkg-config --cflags --libs pocketsphinx sphinxbase`"]
extern {
    fn double_input(input: libc::c_int) -> libc::c_int;
    //// Set env.
    // Build Init

    // Set Init

    //// Process file
    // Open

    // Start sphinx

    // Read

    // Recognition

    // End

    // Close

    // Free
}