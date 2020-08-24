

**Alsa_record** (C++) : this code, after compilation, allows to record audio on Linux in one simple command line (approx 1 hour max if I'mnot too wrong)

It should work on any recent Linux box using Pulseaudio, and the compilation depends on 
alsa (libasound2, lib libasound2-dev), pulseaudio and the pthread library.

Be carefull, this code is provided without any gar it will work on your machine, and you use it at your own risks !

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

Windows version : work in progress, but any contribution is welcome !!**


Copyright Eric Bachard / 2020 August 23rd, 13:24:52 (UTC+0200)


# License

This project is under GPL v3 License. See : http://www.gnu.org/licenses/gpl-3.0.html 
and has been inspired from https://github.com/vrxfile/trik_alsa_record_test,
with **a big thank you to Rostislav Varzar who provides the initial code, and the ideas under a free license**.

**Questions ? Please file an issue !**


## Building (Linux only for instance):

1. Open a terminal 
2. clone alsa_record
3. cd alsa_record
4. install the dependencies
5. in the root dir type "make"


## Testing 
Then connect a microphone (seen as hw:1,0) and verify it can record : arecord -l 
For further information : arecord --dump-hw-params -D hw:1,0

N.B. : if you don't own amicrophone, just connect a webcam instead

Once everything is ok, you can start recording anything typing : 

**./alsa_record  test.wav**

(type ESC to stop recording and quit)


Important:  the output file is a .wav, and you'll have to convert it using 
ffmpeg (e.g.) intp .mp3 or whatever compressed format.
````
ffmpeg -i test.wav -vn -ar 44100 -ac 2 -b:a 192k test.mp3
````

**Any constructive feedback is welcome !**
