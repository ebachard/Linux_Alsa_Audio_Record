/* 
 * file alsa_record.cpp, belongs to audio_record project
 * Copyright Eric Bachard 2020 August 23th   17:55:00   CEST
 * License : GPL v3
 * See: http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef __ALSA_RECORD_HPP__
#define __ALSA_RECORD_HPP__


#define OPEN_ERROR        1
#define MALLOC_ERROR      2
#define ANY_ERROR         3
#define ACCESS_ERROR      4
#define FORMAT_ERROR      5
#define RATE_ERROR        6
#define CHANNELS_ERROR    7
#define PARAMS_ERROR      8
#define PREPARE_ERROR     9
#define FOPEN_ERROR       10
#define FCLOSE_ERROR      11
#define SNDREAD_ERROR     12
#define START_ERROR       13


struct wav_header // Wav file header structure
{
    uint8_t ChunkID[4];
    uint32_t ChunkSize;
    uint8_t Format[4];
    uint8_t Subchunk1ID[4];
    uint32_t Subchunk1Size;
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
    uint8_t Subchunk2ID[4];
    uint32_t Subchunk2Size;
};


class AlsaRecord
{
    public:
        // Ctor()
        AlsaRecord();

        // Dtor()
        ~AlsaRecord();

        void init(void);

        struct wav_header wav_h;

        snd_pcm_t* capture_handle;
        snd_pcm_hw_params_t* hw_params;
        snd_pcm_info_t* s_info;

        unsigned int srate = 44100;
        unsigned int nchan = 2;

        char * wav_name;
        char * fname;
        FILE * fwav;

        int init_soundcard(void);
        int close_soundcard(void);
        int init_wav_header(void);
        int init_wav_file( char *);
        int close_wav_file(void);

        void set_sound_device(const char * aDeviceName) { snd_device = aDeviceName; };
        int do_record();

        inline const char * getSoundDevice(void) { return snd_device; }

    private:
        const char * snd_device;
};


#endif /* __ALSA_RECORD_HPP__ */
