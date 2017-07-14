/*
** hello.c in /home/dlacreme/Projects/waat/test
**
** Made by Mathieu Delacroix
** mathieu.delacroix.fr@gmail.com
**
** Started on  T6 Th06 09 18:45:22 2017 Creme
** Last update T7 Th07 08 23:24:49 2017 Creme
*/

#include "mic_reader.h"


/* BASE */

cmd_ln_t*		set_config()
{
    return cmd_ln_init(NULL, ps_args(), TRUE,
                 "-hmm", MODELDIR "/en-us/en-us",
                 "-lm", MODELDIR "/en-us/en-us.lm.bin",
                 "-dict", MODELDIR "/en-us/cmudict-en-us.dict",
                 NULL);
}

ps_decoder_t*		create_decoder(cmd_ln_t* config)
{
	return ps_init(config);
}

int	clean(cmd_ln_t* config, ps_decoder_t *ps)
{
	if (ps != NULL)
		ps_free(ps);
	if (config != NULL)
		cmd_ln_free_r(config);
	return (0);
}

int		main(int argc, char *argv[])
{
    cmd_ln_t* config = NULL;
    ps_decoder_t* ps = NULL;
	char				quit = 0;

	// Init
	if ((config = set_config()) == NULL)
		E_FATAL("Fail while setting the config");

	// Decoder
	if ((ps = create_decoder(config)) == NULL)
		E_FATAL("Fail while creating the decoder");

	// Hello sir,
	printf("READY TO GO SIR\n");

	recognize_from_microphone(config, ps);

	// We clean
    return (clean(config, ps));
}

/*
 * Main utterance processing loop:
 *     for (;;) {
 *        start utterance and wait for speech to process
 *        decoding till end-of-utterance silence will be detected
 *        print utterance result;
 *     }
 */
static void*    recognize_from_microphone(cmd_ln_t* config, ps_decoder_t* ps)
{
    void*           final_data;
    ps_lattice_t*   data;
    ad_rec_t *ad;
    int16 adbuf[2048];
    uint8 utt_started, in_speech;
    int32 k;
    char const *hyp;


    if ((ad = ad_open_sps((int) cmd_ln_float32_r(config, "-samprate"))) == NULL)
        E_FATAL("Failed to open audio device\n");
    if (ad_start_rec(ad) < 0)
        E_FATAL("Failed to start recording\n");
    if (ps_start_utt(ps) < 0)
        E_FATAL("Failed to start utterance\n");
    utt_started = FALSE;

    E_INFO("Ready....\n");

    for (;;) {

        // We try to read audio
        if ((k = ad_read(ad, adbuf, 2048)) < 0)
            E_FATAL("Failed to read audio\n");

        // Switch from []int16 to []char
        if (ps_process_raw(ps, adbuf, k, FALSE, FALSE) < 0)
            E_FATAL("FAIL TO READ AUDIO");

        // Do we have speech?
        in_speech = ps_get_in_speech(ps);

        // someone is CURRENTLY talking. So we keep listening using UTT
        if (in_speech && !utt_started) {
            utt_started = TRUE;
            E_INFO("Listening...\n");
        }

        // No one is talking anymore. If UTT is set to true, then we have to do process
        if (!in_speech && utt_started) {
            // Stop UTT
            ps_end_utt(ps);

            // We get the data without any process (use ps_get_hyp to get best hypotesis)
            if ((data = ps_get_lattice(ps)) == NULL)
                E_FATAL("Failed to get data");
            // Did we call our BOT?
            if ((final_data = get_garde_a_vous(data)) != NULL)
                return (final_data);

            // WE RESET UTT
            if (ps_start_utt(ps) < 0)
                E_FATAL("Failed to start utterance\n");
            utt_started = FALSE;
            E_INFO("Ready....\n");
        }
        usleep(10000);
    }
    ad_close(ad);
}