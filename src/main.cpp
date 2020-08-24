/* main.cpp
 * Author : Eric Bachard / dimanche 27 mars 2016, 13:24:52 (UTC+0200)
 * This document is under GPL v2 License. See : http://www.gnu.org/licenses/gpl-2.0.html
 */

int main(int argc, char *argv[])
{

    if (argc != 2)
        std::cout << "File name error!" << "\n";
    else
    {
        AlsaRecord ar;

        //ar.init();

        std::cout << "Open default soundcard" << "\n";
        init_soundcard();
        //ar.init_soundcard();

        std::cout << "Init wave file header" << "\n";
        init_wav_header();
        //ar.init_wav_header();

        std::cout << "Open and init wave file" << "\n";
        init_wav_file(argv[1]);
        //ar.init_wav_file(argv[1]);

        //std::thread first (ar.keyboard_loop);
        std::thread first (keyboard_loop);

        int anErr = do_record();
        //int anErr = ar.do_record();

        if (anErr !=  EXIT_SUCCESS)
            std::cout << "Pb with do_record() "  << "\n";

        first.join();

        std::cout << "Close wave file" << "\n";
        close_wav_file();
        //ar.close_wav_file();

        std::cout << "Close default soundcard" << "\n";
        close_soundcard();
        //ar.close_soundcard();
    }
    return EXIT_SUCCESS;
}

