#include <pocketsphinx.h>
#include <sphinxbase/ad.h>

#include <windows.h>
#include <fstream>

#include "Sensor.h"
#include "SpeechRecognition.h"

// For StringCch* and such
#include <strsafe.h>

// For INT_MAX
#include <limits.h>

// For M_PI and log definitions
#define _USE_MATH_DEFINES
#include <math.h>

#include <stdio.h>
#include <sphinxbase/cmd_ln.h>
#include <sphinxbase/logmath.h>
#include <sphinxbase/fe.h>
#include <sphinxbase/feat.h>
#include <sphinxbase/fsg_model.h>
#include <sphinxbase/ngram_model.h>
#include <pocketsphinx_export.h>
#include <cmdln_macro.h>
#include <ps_lattice.h>
#include <ps_mllr.h>

#define MODELDIR "C:/Users/Cen/Desktop/sphinx/pocketsphinx/model"

int
main(int argc, char *argv[])
{
    //recognize from microphone
    uint8 utt_started, in_speech;
    char const *hyp;

    //decoder
	ps_decoder_t *ps;
	cmd_ln_t *config;


    HRESULT hr;

    // Look at each Kinect sensor

    if ( FAILED( SpeechRecognition::instance().CreateFirstConnected() ) )
	{
		return 2;
    }

	config = cmd_ln_init(NULL, ps_args(), TRUE,
			     "-hmm", MODELDIR "/en-us/en-us",
			     "-lm", MODELDIR "/en-us/en-us.lm.dmp",
			     "-dict", MODELDIR "/en-us/cmudict-en-us.dict",
                // "-rawlogdir", "C:/Users/Cen/Desktop/sphinx/pocketsphinx/recognized",
			     NULL);

	if (config == NULL)
    {
        cmd_ln_free_r(config);
		return 3;
    }

	ps = ps_init(config);
	if (ps == NULL)
    {
        cmd_ln_free_r(config);
		return 4;
    }

    if( ps_start_utt(ps) <0 )
    {
        std::cout << "Failed to start utterance\n" << std::endl;
	    return 5;
    }
    utt_started = FALSE;
    printf("READY....\n");

    //For the kinect
    int16 buffer[4096];
    size_t sBuffer=0;

    //sleep of the timer
    int ms=15;

    while ( !GetAsyncKeyState(VK_ESCAPE) ) {
        sBuffer=0;
        //read audio kinect
        SpeechRecognition::instance().processAudio(buffer,sBuffer);
        //audio data recorded with Kinect which are processed
        //if (sBuffer>0) 
          //  {
                ps_process_raw(ps, buffer, sBuffer, FALSE, FALSE);                
            //}
         
        in_speech = ps_get_in_speech(ps);
        if (in_speech && !utt_started) {
            utt_started = TRUE;
            printf("Listening...\n");
        }
        if (!in_speech && utt_started) {
            /* speech -> silence transition, time to start new utterance  */
            ps_end_utt(ps);
            hyp = ps_get_hyp(ps, NULL );
            if (hyp != NULL)
                printf("recognized : %s\n", hyp);

            if (ps_start_utt(ps) < 0)
            {
                std::cout << "Failed to start utterance\n"<<std::endl;
                break;
            }
            utt_started = FALSE;
            printf("READY....\n");
        }
        
        Sleep(ms);
    }

    ps_free(ps);
    cmd_ln_free_r(config);

    Sensor::instance().release();

	return 0;
}
