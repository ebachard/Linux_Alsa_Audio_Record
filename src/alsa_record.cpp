/* 
 * file alsa_record.cpp, belongs to audio_record project
 * CopyrightEric Bachard 2020 August 23th   17:55:00   CEST
 * License : GPL v3
 * See: http://www.gnu.org/licenses/gpl-3.0.html
 */


#include <iostream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <alsa/asoundlib.h>

#include "getch.h"
#include "alsa_record.hpp"

// this is the bitrate
//#define MAX_BUF_SIZE	512
#define MAX_BUF_SIZE	1024
//#define MAX_BUF_SIZE	4096
//  8s
//#define MAX_SAMPLES	256000
#define MAX_SAMPLES	512000


AlsaRecord::AlsaRecord()
{
};


AlsaRecord::~AlsaRecord()
{
};

void AlsaRecord::init()
{
    init_soundcard();
    init_wav_header();
    init_wav_file(fname);
    std::cout << "Open and init done" << "\n";
}


/// Open and init default sound card params
int AlsaRecord::init_soundcard()
{
    int err = 0;

    if ((err = snd_pcm_open(&capture_handle, snd_device, SND_PCM_STREAM_CAPTURE, 0)) < 0)

    {
        std::cerr << "cannot open audio device " << snd_device << "(" << snd_strerror(err) << ", " << err << ")" << "\n";
        return OPEN_ERROR;
    }

    if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0)
    {
        std::cerr << "cannot allocate hardware parameter structure " << "(" << snd_strerror(err) << ", " << err << ")" << "\n";
        return MALLOC_ERROR;
    }

    if ((err = snd_pcm_hw_params_any(capture_handle, hw_params)) < 0)
    {
        std::cerr << "cannot initialize hardware parameter structure " << "(" << snd_strerror(err) << ", " << err << ")" << "\n";
        return ANY_ERROR;
    }

    if ((err = snd_pcm_hw_params_set_access(capture_handle, hw_params,
                SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
    {
        std::cerr << "cannot set access type " << "(" << snd_strerror(err) << ", " << err << ")" << "\n";
        return ACCESS_ERROR;
    }

    if ((err = snd_pcm_hw_params_set_format(capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0)
    {
        std::cerr << "cannot set sample format " << "(" << snd_strerror(err) << ", " << err << ")" << "\n";
        return FORMAT_ERROR;
    }

    if ((err = snd_pcm_hw_params_set_rate_near(capture_handle, hw_params,
                &srate, 0)) < 0)
    {
        std::cerr << "cannot set sample rate " << "(" << snd_strerror(err) << ", " << err << ")" << "\n";
        return RATE_ERROR;
    }

    if ((err = snd_pcm_hw_params_set_channels(capture_handle, hw_params, nchan))< 0)
    {
        std::cerr << "cannot set channel count " << "(" << snd_strerror(err) << ", " << err << ")" << "\n";
        return CHANNELS_ERROR;
    }

    if ((err = snd_pcm_hw_params(capture_handle, hw_params)) < 0)
    {
        std::cerr << "cannot set parameters " << "(" << snd_strerror(err) << ", " << err << ")" << "\n";
        return PARAMS_ERROR;
    }

    if ((err = snd_pcm_prepare(capture_handle)) < 0)
    {
        std::cerr << "cannot prepare audio interface for use " << "(" << snd_strerror(err) << ", " << err << ")" << "\n";
        return PREPARE_ERROR;
    }

    if ((err = snd_pcm_start(capture_handle)) < 0)
    {
        std::cerr << "cannot start soundcard " << "(" << snd_strerror(err) << ", " << err << ")" << "\n";
        return START_ERROR;
    }

    return EXIT_SUCCESS;
}

int AlsaRecord::close_soundcard()
{
    return snd_pcm_close(capture_handle);
}

int AlsaRecord::init_wav_header()
{
    wav_h.ChunkID[0] = 'R';
    wav_h.ChunkID[1] = 'I';
    wav_h.ChunkID[2] = 'F';
    wav_h.ChunkID[3] = 'F';
    wav_h.Format[0] = 'W';
    wav_h.Format[1] = 'A';
    wav_h.Format[2] = 'V';
    wav_h.Format[3] = 'E';
    wav_h.Subchunk1ID[0] = 'f';
    wav_h.Subchunk1ID[1] = 'm';
    wav_h.Subchunk1ID[2] = 't';
    wav_h.Subchunk1ID[3] = ' ';
    wav_h.Subchunk2ID[0] = 'd';
    wav_h.Subchunk2ID[1] = 'a';
    wav_h.Subchunk2ID[2] = 't';
    wav_h.Subchunk2ID[3] = 'a';
    wav_h.NumChannels = nchan;
    wav_h.BitsPerSample = 16;
    wav_h.Subchunk2Size = 300 * MAX_SAMPLES * (uint32_t) wav_h.NumChannels * (uint32_t) wav_h.BitsPerSample / 8;
    //wav_h.Subchunk2Size = 0xFFFFFFFF;
    wav_h.ChunkSize = (uint32_t) wav_h.Subchunk2Size + 36;
    wav_h.Subchunk1Size = 16;
    wav_h.AudioFormat = 1;
    wav_h.SampleRate = srate;
    wav_h.ByteRate = (uint32_t) wav_h.SampleRate
                     * (uint32_t) wav_h.NumChannels
                     * (uint32_t) wav_h.BitsPerSample / 8;
    wav_h.BlockAlign = (uint32_t) wav_h.NumChannels * (uint32_t) wav_h.BitsPerSample / 8;

    return EXIT_SUCCESS;
}

int AlsaRecord::init_wav_file(char *fname)
{
    fwav = fopen(fname, "wb");

    if (fwav != NULL)
        fwrite(&wav_h, 1, sizeof(wav_h), fwav);
    else
    {
        std::cerr << "cannot open wav file to write data" << "\n";
        return FOPEN_ERROR;
    }

    return EXIT_SUCCESS;
}

int AlsaRecord::close_wav_file()
{
    if (fwav != NULL)
        fclose(fwav);
    else
    {
        std::cerr << "cannot close wav file" << "\n";
        return FCLOSE_ERROR;
    }

    return EXIT_SUCCESS;
}

static bool b_quit = false;

static void keyboard_loop()
{
    char c = 0;
    while (c != 27) // esc
    {
        c = 0;
        c=getch();
    }
    if (c == 27) // esc
        b_quit = true;
}

int AlsaRecord::do_record()
{
    uint32_t ncount = 0;
    int err = 0;

    char wav_data[MAX_BUF_SIZE * 4];

    do
    {
        if ((err = snd_pcm_readi(capture_handle, wav_data, MAX_BUF_SIZE)) != MAX_BUF_SIZE)
        {
            std::cerr << "read from audio interface failed " << "(" << snd_strerror(err) << ", " << err << ")" << "\n";

            if (err == -32) // Broken pipe
            {
                if ((err = snd_pcm_prepare(capture_handle)) < 0)
                {
                    std::cerr << "cannot prepare audio interface for use " << "(" << snd_strerror(err) << ", " << err << ")" << "\n";
                    return PREPARE_ERROR;
                }
            }
            else
                return SNDREAD_ERROR;

        }

        if (fwav != NULL)
            fwrite(wav_data, 1, MAX_BUF_SIZE * 4, fwav);
        else
        {
            std::cerr << "cannot write data to file"  << "\n";
            return FOPEN_ERROR;
        }

                ncount++;
    } while (b_quit == false); /*esc */

    std::cout << "ncount : "  <<  ncount << "\n";
    wav_h.Subchunk2Size = MAX_BUF_SIZE * ncount * (uint32_t) wav_h.NumChannels * (uint32_t) wav_h.BitsPerSample / 8;
    wav_h.ChunkSize = (uint32_t) wav_h.Subchunk2Size + 36;
    std::cout << "wav_h.Subchunk2Size : "  <<  wav_h.Subchunk2Size << "\n";
    std::cout << "wav_h.ChunkSize     : "  <<  wav_h.ChunkSize     << "\n";
    fwrite(&wav_h, 1, sizeof(wav_h), fwav);

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        std::cout << "File name error!" << "\n";
    else
    {
        AlsaRecord ar;
        ar.fname = argv[1];
        std::cout << "ar.fname : " << ar.fname << "\n";

        ar.set_sound_device("hw:1,0");
        ar.init();

        std::thread first (keyboard_loop);

        int anErr = ar.do_record();
        if (anErr !=  EXIT_SUCCESS)
            std::cout << "Pb with do_record() "  << "\n";

        first.join();

        std::cout << "Close wave file" << "\n";
        ar.close_wav_file();

        std::cout << "Close default soundcard" << "\n";
        ar.close_soundcard();
    }
    return EXIT_SUCCESS;
}
