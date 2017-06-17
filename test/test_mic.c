/*
** hello.c in /home/dlacreme/Projects/waat/test
**
** Made by Mathieu Delacroix
** mathieu.delacroix.fr@gmail.com
**
** Started on  T6 Th06 09 18:45:22 2017 Creme
** Last update T7 Th06 17 19:48:57 2017 Creme
*/

#include <stdlib.h>
#include <stdio.h>
#include <pocketsphinx.h>

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


/* Listen on Mic */



int		main(int argc, char *argv[])
{
    cmd_ln_t *config = NULL;
    ps_decoder_t *ps = NULL;
	char				quit = 0;

	// Init
	if ((config = set_config()) == NULL)
		panic("Fail while setting the config");

	// Decoder
	if ((ps = create_decoder(config)) == NULL)
		panic("Fail while creating the decoder");

	// utterance
	if (ps_start_utt(ps) < 0)
		panic("Fail to run utterance");

	// Hello sir,
	printf("READY TO GO SIR");

	// Loop while ctrl+c
	while(true || quit == 'q')
	{

	}
    
	// We clean
    return (clean(config, ps));
}
