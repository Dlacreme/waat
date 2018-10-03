/*
** hello.c in /home/dlacreme/Projects/waat/test
**
** Made by Mathieu Delacroix
** mathieu.delacroix.fr@gmail.com
**
** Started on  T6 Th06 09 18:45:22 2017 Creme
** Last update T7 Th07 08 23:24:49 2017 Creme
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/select.h>
#include <unistd.h>

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <pocketsphinx.h>


static void recognize_from_microphone();

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

void panic(char* str)
{
	fprintf(stderr, "%s\n", str);
	exit(-42);
}


int		main(int argc, char *argv[])
{
    cmd_ln_t* config = NULL;
    ps_decoder_t* ps = NULL;
	char				quit = 0;

	// Init
	if ((config = set_config()) == NULL)
		panic("Fail while setting the config");

	// Decoder
	if ((ps = create_decoder(config)) == NULL)
		panic("Fail while creating the decoder");

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
static void
recognize_from_microphone(cmd_ln_t* config, ps_decoder_t* ps)
{
    ad_rec_t *ad;
    int16 adbuf[2048];
    uint8 utt_started, in_speech;
    int32 k;
    char const *hyp;

    /*if ((ad = ad_open_dev("/dev/input/event14",
                          (int) cmd_ln_float32_r(config,
                                                 "-samprate"))) == NULL)*/
    if ((ad = ad_open_sps((int) cmd_ln_float32_r(config, "-samprate"))) == NULL)
        E_FATAL("Failed to open audio device\n");
    if (ad_start_rec(ad) < 0)
        E_FATAL("Failed to start recording\n");

    if (ps_start_utt(ps) < 0)
        E_FATAL("Failed to start utterance\n");
    utt_started = FALSE;
    E_INFO("Ready....\n");

    for (;;) {
        E_INFO("START LOOP");
        if ((k = ad_read(ad, adbuf, 2048)) < 0)
            E_FATAL("Failed to read audio\n");
        ps_process_raw(ps, adbuf, k, FALSE, FALSE);
        in_speech = ps_get_in_speech(ps);
        if (in_speech && !utt_started) {
            utt_started = TRUE;
            E_INFO("Listening...\n");
        }
        else if (!in_speech && utt_started) {
            /* speech -> silence transition, time to start new utterance  */
            E_INFO("I GOT THE POWER.");
            ps_end_utt(ps);
            hyp = ps_get_hyp(ps, NULL );
            if (hyp != NULL) {
                printf("%s\n", hyp);
                fflush(stdout);
            }

            if (ps_start_utt(ps) < 0)
                E_FATAL("Failed to start utterance\n");
            utt_started = FALSE;
            E_INFO("Ready....\n");
        } else {
            E_INFO("Nothing to do.");
        }
        E_INFO("END OF LOOP");
        usleep(10000);
    }
    ad_close(ad);
}