#include <tools/logger/Stdout.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <semaphore.h>

#include <opengl/Screen.h>

extern "C" {
#include <audio/broadcom/ilclient.h>
}

using namespace tools::logger;

short Sinewave[] = {
   0,    201,    402,    603,    804,   1005,   1206,   1406,
   1607,   1808,   2009,   2209,   2410,   2610,   2811,   3011,
   3211,   3411,   3611,   3811,   4011,   4210,   4409,   4608,
   4807,   5006,   5205,   5403,   5601,   5799,   5997,   6195,
   6392,   6589,   6786,   6982,   7179,   7375,   7571,   7766,
   7961,   8156,   8351,   8545,   8739,   8932,   9126,   9319,
   9511,   9703,   9895,  10087,  10278,  10469,  10659,  10849,
   11038,  11227,  11416,  11604,  11792,  11980,  12166,  12353,
   12539,  12724,  12909,  13094,  13278,  13462,  13645,  13827,
   14009,  14191,  14372,  14552,  14732,  14911,  15090,  15268,
   15446,  15623,  15799,  15975,  16150,  16325,  16499,  16672,
   16845,  17017,  17189,  17360,  17530,  17699,  17868,  18036,
   18204,  18371,  18537,  18702,  18867,  19031,  19194,  19357,
   19519,  19680,  19840,  20000,  20159,  20317,  20474,  20631,
   20787,  20942,  21096,  21249,  21402,  21554,  21705,  21855,
   22004,  22153,  22301,  22448,  22594,  22739,  22883,  23027,
   23169,  23311,  23452,  23592,  23731,  23869,  24006,  24143,
   24278,  24413,  24546,  24679,  24811,  24942,  25072,  25201,
   25329,  25456,  25582,  25707,  25831,  25954,  26077,  26198,
   26318,  26437,  26556,  26673,  26789,  26905,  27019,  27132,
   27244,  27355,  27466,  27575,  27683,  27790,  27896,  28001,
   28105,  28208,  28309,  28410,  28510,  28608,  28706,  28802,
   28897,  28992,  29085,  29177,  29268,  29358,  29446,  29534,
   29621,  29706,  29790,  29873,  29955,  30036,  30116,  30195,
   30272,  30349,  30424,  30498,  30571,  30643,  30713,  30783,
   30851,  30918,  30984,  31049,
   31113,  31175,  31236,  31297,
   31356,  31413,  31470,  31525,  31580,  31633,  31684,  31735,
   31785,  31833,  31880,  31926,  31970,  32014,  32056,  32097,
   32137,  32176,  32213,  32249,  32284,  32318,  32350,  32382,
   32412,  32441,  32468,  32495,  32520,  32544,  32567,  32588,
   32609,  32628,  32646,  32662,  32678,  32692,  32705,  32717,
   32727,  32736,  32744,  32751,  32757,  32761,  32764,  32766,
   32767,  32766,  32764,  32761,  32757,  32751,  32744,  32736,
   32727,  32717,  32705,  32692,  32678,  32662,  32646,  32628,
   32609,  32588,  32567,  32544,  32520,  32495,  32468,  32441,
   32412,  32382,  32350,  32318,  32284,  32249,  32213,  32176,
   32137,  32097,  32056,  32014,  31970,  31926,  31880,  31833,
   31785,  31735,  31684,  31633,  31580,  31525,  31470,  31413,
   31356,  31297,  31236,  31175,  31113,  31049,  30984,  30918,
   30851,  30783,  30713,  30643,  30571,  30498,  30424,  30349,
   30272,  30195,  30116,  30036,  29955,  29873,  29790,  29706,
   29621,  29534,  29446,  29358,  29268,  29177,  29085,  28992,
   28897,  28802,  28706,  28608,  28510,  28410,  28309,  28208,
   28105,  28001,  27896,  27790,  27683,  27575,  27466,  27355,
   27244,  27132,  27019,  26905,  26789,  26673,  26556,  26437,
   26318,  26198,  26077,  25954,  25831,  25707,  25582,  25456,
   25329,  25201,  25072,  24942,  24811,  24679,  24546,  24413,
   24278,  24143,  24006,  23869,  23731,  23592,  23452,  23311,
   23169,  23027,  22883,  22739,  22594,  22448,  22301,  22153,
   22004,  21855,  21705,  21554,  21402,  21249,  21096,  20942,
   20787,  20631,  20474,  20317,  20159,  20000,  19840,  19680,
   19519,  19357,  19194,  19031,  18867,  18702,  18537,  18371,
   18204,  18036,  17868,  17699,  17530,  17360,  17189,  17017,
   16845,  16672,  16499,  16325,  16150,  15975,  15799,  15623,
   15446,  15268,  15090,  14911,  14732,  14552,  14372,  14191,
   14009,  13827,  13645,  13462,  13278,  13094,  12909,  12724,
   12539,  12353,  12166,  11980,  11792,  11604,  11416,  11227,
   11038,  10849,  10659,  10469,  10278,  10087,   9895,   9703,
   9511,   9319,   9126,   8932,   8739,   8545,   8351,   8156,
   7961,   7766,   7571,   7375,   7179,   6982,   6786,   6589,
   6392,   6195,   5997,   5799,   5601,   5403,   5205,   5006,
   4807,   4608,   4409,   4210,   4011,   3811,   3611,   3411,
   3211,   3011,   2811,   2610,   2410,   2209,   2009,   1808,
   1607,   1406,   1206,   1005,    804,    603,    402,    201,
   0,   -201,   -402,   -603,   -804,  -1005,  -1206,  -1406,
   -1607,  -1808,  -2009,  -2209,  -2410,  -2610,  -2811,  -3011,
   -3211,  -3411,  -3611,  -3811,  -4011,  -4210,  -4409,  -4608,
   -4807,  -5006,  -5205,  -5403,  -5601,  -5799,  -5997,  -6195,
   -6392,  -6589,  -6786,  -6982,  -7179,  -7375,  -7571,  -7766,
   -7961,  -8156,  -8351,  -8545,  -8739,  -8932,  -9126,  -9319,
   -9511,  -9703,  -9895, -10087, -10278, -10469, -10659, -10849,
   -11038, -11227, -11416, -11604, -11792, -11980, -12166, -12353,
   -12539, -12724, -12909, -13094, -13278, -13462, -13645, -13827,
   -14009, -14191, -14372, -14552, -14732, -14911, -15090, -15268,
   -15446, -15623, -15799, -15975, -16150, -16325, -16499, -16672,
   -16845, -17017, -17189, -17360, -17530, -17699, -17868, -18036,
   -18204, -18371, -18537, -18702, -18867, -19031, -19194, -19357,
   -19519, -19680, -19840, -20000, -20159, -20317, -20474, -20631,
   -20787, -20942, -21096, -21249, -21402, -21554, -21705, -21855,
   -22004, -22153, -22301, -22448, -22594, -22739, -22883, -23027,
   -23169, -23311, -23452, -23592, -23731, -23869, -24006, -24143,
   -24278, -24413, -24546, -24679, -24811, -24942, -25072, -25201,
   -25329, -25456, -25582, -25707, -25831, -25954, -26077, -26198,
   -26318, -26437, -26556, -26673, -26789, -26905, -27019, -27132,
   -27244, -27355, -27466, -27575, -27683, -27790, -27896, -28001,
   -28105, -28208, -28309, -28410, -28510, -28608, -28706, -28802,
   -28897, -28992, -29085, -29177, -29268, -29358, -29446, -29534,
   -29621, -29706, -29790, -29873, -29955, -30036, -30116, -30195,
   -30272, -30349, -30424, -30498, -30571, -30643, -30713, -30783,
   -30851, -30918, -30984, -31049, -31113, -31175, -31236, -31297,
   -31356, -31413, -31470, -31525, -31580, -31633, -31684, -31735,
   -31785, -31833, -31880, -31926, -31970, -32014, -32056, -32097,
   -32137, -32176, -32213, -32249, -32284, -32318, -32350, -32382,
   -32412, -32441, -32468, -32495, -32520, -32544, -32567, -32588,
   -32609, -32628, -32646, -32662, -32678, -32692, -32705, -32717,
   -32727, -32736, -32744, -32751, -32757, -32761, -32764, -32766,
   -32767, -32766, -32764, -32761, -32757, -32751, -32744, -32736,
   -32727, -32717, -32705, -32692, -32678, -32662, -32646, -32628,
   -32609, -32588, -32567, -32544, -32520, -32495, -32468, -32441,
   -32412, -32382, -32350, -32318, -32284, -32249, -32213, -32176,
   -32137, -32097, -32056, -32014, -31970, -31926, -31880, -31833,
   -31785, -31735, -31684, -31633, -31580, -31525, -31470, -31413,
   -31356, -31297, -31236, -31175, -31113, -31049, -30984, -30918,
   -30851, -30783, -30713, -30643, -30571, -30498, -30424, -30349,
   -30272, -30195, -30116, -30036, -29955, -29873, -29790, -29706,
   -29621, -29534, -29446, -29358, -29268, -29177, -29085, -28992,
   -28897, -28802, -28706, -28608, -28510, -28410, -28309, -28208,
   -28105, -28001, -27896, -27790, -27683, -27575, -27466, -27355,
   -27244, -27132, -27019, -26905, -26789, -26673, -26556, -26437,
   -26318, -26198, -26077, -25954, -25831, -25707, -25582, -25456,
   -25329, -25201, -25072, -24942, -24811, -24679, -24546, -24413,
   -24278, -24143, -24006, -23869, -23731, -23592, -23452, -23311,
   -23169, -23027, -22883, -22739, -22594, -22448, -22301, -22153,
   -22004, -21855, -21705, -21554, -21402, -21249, -21096, -20942,
   -20787, -20631, -20474, -20317, -20159, -20000, -19840, -19680,
   -19519, -19357, -19194, -19031, -18867, -18702, -18537, -18371,
   -18204, -18036, -17868, -17699, -17530, -17360, -17189, -17017,
   -16845, -16672, -16499, -16325, -16150, -15975, -15799, -15623,
   -15446, -15268, -15090, -14911, -14732, -14552, -14372, -14191,
   -14009, -13827, -13645, -13462, -13278, -13094, -12909, -12724,
   -12539, -12353, -12166, -11980, -11792, -11604, -11416, -11227,
   -11038, -10849, -10659, -10469, -10278, -10087,  -9895,  -9703,
   -9511,  -9319,  -9126,  -8932,  -8739,  -8545,  -8351,  -8156,
   -7961,  -7766,  -7571,  -7375,  -7179,  -6982,  -6786,  -6589,
   -6392,  -6195,  -5997,  -5799,  -5601,  -5403,  -5205,  -5006,
   -4807,  -4608,  -4409,  -4210,  -4011,  -3811,  -3611,  -3411,
   -3211,  -3011,  -2811,  -2610,  -2410,  -2209,  -2009,  -1808,
   -1607,  -1406,  -1206,  -1005,   -804,   -603,   -402,   -201,
};

#define N_WAVE          1024    /* dimension of Sinewave[] */
#define PI (1<<16>>1)
#define SIN(x) Sinewave[((x)>>6) & (N_WAVE-1)]
#define COS(x) SIN((x)+(PI>>1))
#define OUT_CHANNELS(num_channels) ((num_channels) > 4 ? 8: (num_channels) > 2 ? 4: (num_channels))

#ifndef countof
   #define countof(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

#define BUFFER_SIZE_SAMPLES 1024

typedef int int32_t;

typedef struct {
   sem_t sema;
   ILCLIENT_T *client;
   COMPONENT_T *audio_render;
   COMPONENT_T *list[2];
   OMX_BUFFERHEADERTYPE *user_buffer_list; // buffers owned by the client
   uint32_t num_buffers;
   uint32_t bytes_per_sample;
} AUDIOPLAY_STATE_T;

static void input_buffer_callback(void *data, COMPONENT_T *comp)
{
   // do nothing - could add a callback to the user
   // to indicate more buffers may be available.
}

int32_t audioplay_create(AUDIOPLAY_STATE_T **handle,
                         uint32_t sample_rate,
                         uint32_t num_channels,
                         uint32_t bit_depth,
                         uint32_t num_buffers,
                         uint32_t buffer_size)
{
   uint32_t bytes_per_sample = (bit_depth * OUT_CHANNELS(num_channels)) >> 3;
   int32_t ret = -1;

   *handle = NULL;

   // basic sanity check on arguments
   if(sample_rate >= 8000 && sample_rate <= 192000 &&
      (num_channels >= 1 && num_channels <= 8) &&
      (bit_depth == 16 || bit_depth == 32) &&
      num_buffers > 0 &&
      buffer_size >= bytes_per_sample)
   {
      // buffer lengths must be 16 byte aligned for VCHI
      int size = (buffer_size + 15) & ~15;
      AUDIOPLAY_STATE_T *st;

      // buffer offsets must also be 16 byte aligned for VCHI
      st = (AUDIOPLAY_STATE_T*) calloc(1, sizeof(AUDIOPLAY_STATE_T));

      if(st)
      {
         OMX_ERRORTYPE error;
         OMX_PARAM_PORTDEFINITIONTYPE param;
         OMX_AUDIO_PARAM_PCMMODETYPE pcm;
         int32_t s;

         ret = 0;
         *handle = st;

         // create and start up everything
         s = sem_init(&st->sema, 0, 1);
         assert(s == 0);

         st->bytes_per_sample = bytes_per_sample;
         st->num_buffers = num_buffers;

         st->client = ilclient_init();
         assert(st->client != NULL);

         ilclient_set_empty_buffer_done_callback(st->client, input_buffer_callback, st);

         error = OMX_Init();
         assert(error == OMX_ErrorNone);

		 char * audio_render_str = "audio_render";
         ilclient_create_component(st->client, &st->audio_render, audio_render_str, static_cast<ILCLIENT_CREATE_FLAGS_T>( ILCLIENT_ENABLE_INPUT_BUFFERS | ILCLIENT_DISABLE_ALL_PORTS ) );
         assert(st->audio_render != NULL);

         st->list[0] = st->audio_render;

         // set up the number/size of buffers
         memset(&param, 0, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
         param.nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
         param.nVersion.nVersion = OMX_VERSION;
         param.nPortIndex = 100;

         error = OMX_GetParameter(ILC_GET_HANDLE(st->audio_render), OMX_IndexParamPortDefinition, &param);
         assert(error == OMX_ErrorNone);

         param.nBufferSize = size;
         param.nBufferCountActual = num_buffers;

         error = OMX_SetParameter(ILC_GET_HANDLE(st->audio_render), OMX_IndexParamPortDefinition, &param);
         assert(error == OMX_ErrorNone);

         // set the pcm parameters
         memset(&pcm, 0, sizeof(OMX_AUDIO_PARAM_PCMMODETYPE));
         pcm.nSize = sizeof(OMX_AUDIO_PARAM_PCMMODETYPE);
         pcm.nVersion.nVersion = OMX_VERSION;
         pcm.nPortIndex = 100;
         pcm.nChannels = OUT_CHANNELS(num_channels);
         pcm.eNumData = OMX_NumericalDataSigned;
         pcm.eEndian = OMX_EndianLittle;
         pcm.nSamplingRate = sample_rate;
         pcm.bInterleaved = OMX_TRUE;
         pcm.nBitPerSample = bit_depth;
         pcm.ePCMMode = OMX_AUDIO_PCMModeLinear;

         switch(num_channels) {
         case 1:
            pcm.eChannelMapping[0] = OMX_AUDIO_ChannelCF;
            break;
         case 3:
            pcm.eChannelMapping[2] = OMX_AUDIO_ChannelCF;
            pcm.eChannelMapping[1] = OMX_AUDIO_ChannelRF;
            pcm.eChannelMapping[0] = OMX_AUDIO_ChannelLF;
            break;
         case 8:
            pcm.eChannelMapping[7] = OMX_AUDIO_ChannelRS;
         case 7:
            pcm.eChannelMapping[6] = OMX_AUDIO_ChannelLS;
         case 6:
            pcm.eChannelMapping[5] = OMX_AUDIO_ChannelRR;
         case 5:
            pcm.eChannelMapping[4] = OMX_AUDIO_ChannelLR;
         case 4:
            pcm.eChannelMapping[3] = OMX_AUDIO_ChannelLFE;
            pcm.eChannelMapping[2] = OMX_AUDIO_ChannelCF;
         case 2:
            pcm.eChannelMapping[1] = OMX_AUDIO_ChannelRF;
            pcm.eChannelMapping[0] = OMX_AUDIO_ChannelLF;
            break;
         }

         error = OMX_SetParameter(ILC_GET_HANDLE(st->audio_render), OMX_IndexParamAudioPcm, &pcm);
         assert(error == OMX_ErrorNone);

         ilclient_change_component_state(st->audio_render, OMX_StateIdle);
         if(ilclient_enable_port_buffers(st->audio_render, 100, NULL, NULL, NULL) < 0)
         {
            // error
            ilclient_change_component_state(st->audio_render, OMX_StateLoaded);
            ilclient_cleanup_components(st->list);

            error = OMX_Deinit();
            assert(error == OMX_ErrorNone);

            ilclient_destroy(st->client);

            sem_destroy(&st->sema);
            free(st);
            *handle = NULL;
            return -1;
         }

         ilclient_change_component_state(st->audio_render, OMX_StateExecuting);
      }
   }

   return ret;
}

int32_t audioplay_delete(AUDIOPLAY_STATE_T *st)
{
   OMX_ERRORTYPE error;

   ilclient_change_component_state(st->audio_render, OMX_StateIdle);

   error = OMX_SendCommand(ILC_GET_HANDLE(st->audio_render), OMX_CommandStateSet, OMX_StateLoaded, NULL);
   assert(error == OMX_ErrorNone);

   ilclient_disable_port_buffers(st->audio_render, 100, st->user_buffer_list, NULL, NULL);
   ilclient_change_component_state(st->audio_render, OMX_StateLoaded);
   ilclient_cleanup_components(st->list);

   error = OMX_Deinit();
   assert(error == OMX_ErrorNone);

   ilclient_destroy(st->client);

   sem_destroy(&st->sema);
   free(st);

   return 0;
}

uint8_t *audioplay_get_buffer(AUDIOPLAY_STATE_T *st)
{
   OMX_BUFFERHEADERTYPE *hdr = NULL;

   hdr = ilclient_get_input_buffer(st->audio_render, 100, 0);

   if(hdr)
   {
      // put on the user list
      sem_wait(&st->sema);

      hdr->pAppPrivate = st->user_buffer_list;
      st->user_buffer_list = hdr;

      sem_post(&st->sema);
   }

   return hdr ? hdr->pBuffer : NULL;
}

int32_t audioplay_play_buffer(AUDIOPLAY_STATE_T *st,
                              uint8_t *buffer,
                              uint32_t length)
{
   OMX_BUFFERHEADERTYPE *hdr = NULL, *prev = NULL;
   int32_t ret = -1;

   if(length % st->bytes_per_sample)
      return ret;

   sem_wait(&st->sema);

   // search through user list for the right buffer header
   hdr = st->user_buffer_list;
   while(hdr != NULL && hdr->pBuffer != buffer && hdr->nAllocLen < length)
   {
      prev = hdr;
      hdr = (OMX_BUFFERHEADERTYPE*) hdr->pAppPrivate;
   }

   if(hdr) // we found it, remove from list
   {
      ret = 0;
      if(prev)
         prev->pAppPrivate = hdr->pAppPrivate;
      else
         st->user_buffer_list = (OMX_BUFFERHEADERTYPE*) hdr->pAppPrivate;
   }

   sem_post(&st->sema);

   if(hdr)
   {
      OMX_ERRORTYPE error;

      hdr->pAppPrivate = NULL;
      hdr->nOffset = 0;
      hdr->nFilledLen = length;

      error = OMX_EmptyThisBuffer(ILC_GET_HANDLE(st->audio_render), hdr);
      assert(error == OMX_ErrorNone);
   }

   return ret;
}

int32_t audioplay_set_dest(AUDIOPLAY_STATE_T *st, const char *name)
{
   int32_t success = -1;
   OMX_CONFIG_BRCMAUDIODESTINATIONTYPE ar_dest;

   if (name && strlen(name) < sizeof(ar_dest.sName))
   {
      OMX_ERRORTYPE error;
      memset(&ar_dest, 0, sizeof(ar_dest));
      ar_dest.nSize = sizeof(OMX_CONFIG_BRCMAUDIODESTINATIONTYPE);
      ar_dest.nVersion.nVersion = OMX_VERSION;
      strcpy((char *)ar_dest.sName, name);

      error = OMX_SetConfig(ILC_GET_HANDLE(st->audio_render), OMX_IndexConfigBrcmAudioDestination, &ar_dest);
      assert(error == OMX_ErrorNone);
      success = 0;
   }

   return success;
}


uint32_t audioplay_get_latency(AUDIOPLAY_STATE_T *st)
{
   OMX_PARAM_U32TYPE param;
   OMX_ERRORTYPE error;

   memset(&param, 0, sizeof(OMX_PARAM_U32TYPE));
   param.nSize = sizeof(OMX_PARAM_U32TYPE);
   param.nVersion.nVersion = OMX_VERSION;
   param.nPortIndex = 100;

   error = OMX_GetConfig(ILC_GET_HANDLE(st->audio_render), OMX_IndexConfigAudioRenderingLatency, &param);
   assert(error == OMX_ErrorNone);

   return param.nU32;
}


#define CTTW_SLEEP_TIME 10
#define MIN_LATENCY_TIME 20

static const char *audio_dest[] = {"local", "hdmi"};
int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	Logger::get() << "Ok." << Logger::endl;
	
	// Not needed when Screen is used.
	bcm_host_init();
	
	// Variables
	int dest = 0;
	int samplerate = 48000;
	int nchannels = 2;
	int bitdepth = 16;
	
	AUDIOPLAY_STATE_T *st;
	int32_t ret;
	unsigned int i, j, n;
	int phase = 0;
	int inc = 256<<16;
	int dinc = 0;
	int buffer_size = (BUFFER_SIZE_SAMPLES * bitdepth * OUT_CHANNELS(nchannels))>>3;

	assert(dest == 0 || dest == 1);

	ret = audioplay_create(&st, samplerate, nchannels, bitdepth, 10, buffer_size);
	assert(ret == 0);

	ret = audioplay_set_dest(st, audio_dest[dest]);
	assert(ret == 0);

	// iterate for 5 seconds worth of packets
	for (n=0; n<((samplerate * 1000)/ BUFFER_SIZE_SAMPLES); n++)
	{
		uint8_t *buf;
		int16_t *p;
		uint32_t latency;

		while((buf = audioplay_get_buffer(st)) == NULL)
		 usleep(10*1000);

		p = (int16_t *) buf;

		// fill the buffer
		for (i=0; i<BUFFER_SIZE_SAMPLES; i++)
		{
		 int16_t val = SIN(phase);
		 phase += inc>>16;
		 inc += dinc;
		 if (inc>>16 < 512)
			dinc++;
		 else
			dinc--;

		 for(j=0; j<OUT_CHANNELS(nchannels); j++)
		 {
			if (bitdepth == 32)
			   *p++ = 0;
			*p++ = val;
		 }
		}

		// try and wait for a minimum latency time (in ms) before
		// sending the next packet
		while((latency = audioplay_get_latency(st)) > (samplerate * (MIN_LATENCY_TIME + CTTW_SLEEP_TIME) / 1000))
		 usleep(CTTW_SLEEP_TIME*1000);

		ret = audioplay_play_buffer(st, buf, buffer_size);
		assert(ret == 0);
	}

	audioplay_delete(st);
	
	
	/* audioplay_create
	 * init semaphore
	 * ilclient_set_empty_buffer_done_callback
	 * OMX_SetParameter ...
	 * ilclient_change_component_state
	 * ilclient_enable_port_buffers
	 * ilclient_change_component_state
	 */
	 
	/* audioplay_set_dest
	 * OMX_SetConfig
	 */
	 
	/* audioplay_get_buffer
	 * ilclient_get_input_buffer
	 */
	
	/* audioplay_get_latency
	 * OMX_GetConfig
	 */
	
	/* audioplay_play_buffer
	 * OMX_EmptyThisBuffer
	 */
	 
	/* audioplay_delete
	 * ilclient_change_component_state
	 * ilclient_disable_port_buffers
	 * ilclient_change_component_state
	 * ilclient_cleanup_components
	 * OMX_Deinit
	 * ilclient_destroy
	 */
	 
	Logger::destroy();
	
	return 0;
}

