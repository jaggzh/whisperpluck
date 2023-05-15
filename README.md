# whisperpluck

Some scripts for convenient voice transcription. At present, we use OpenAI-Whisper, since it's such high quality.

## Features

* Transcribes your voice, and copies it to the clipboard.

* Can run `whisper` each time you record yourself
* Can run our little whisper server (in `server-flask/` dir) for faster use (model stays loaded)

* UI buttons: In `ui/whisper-buttons` may be run for on-screen buttons
* Hotkeys: You can assign hotkeys to `whisper-auto` and `whisper-kill-rec`
* Desktop Shortcuts: Drag the scripts to your desktop (preferably as .desktop entries -- I'm not sure what they'll do as links because whisper-auto tries to find its own location)

## See the [optional] new button UI in action

[![Whisperpluck Whisper UI Buttons Video]
(https://i.ytimg.com/vi/z5kiq3F-ros/maxresdefault.jpg)]
(https://www.youtube.com/watch?v=z5kiq3F-ros "Whisperpluck Whisper UI Buttons Video")

## Please consider contributing...

* [Donate to patreon.com/jaggz](https://patreon.com/jaggz)
* Or paypal me at jaggz.h {at} gmail.com

## IMPORTANT, Maybe...

This was initially designed for the desktop icons method of execution. It updates/renames icons on the desktop to provide status changes (like, it'll rename them to WH-Ready, WH-Rec, ...).  **This doesn't work on all X11 desktops** (the renaming doesn't update the desktop entries).  I worked on this for some time but gave up and focused on the little GTK ui (`ui/whisper-buttons`).

# WHISPER USAGE METHODS

We have two methods of using whisper for our purposes:

1. *No-Server Method:* Runs whisper each time you want a transcription (model must be loaded each run)
2. *Server Method*: Run a separate little server (in `server-flask/`) which keeps the model loaded so transcriptions are faster

## Both methods require a venv, which right now is `whisper`

(A venv is a python virtual environment, to store all the whisper-related stuff in this case.)

1. Make venv:
    1. Make venv base dir: `mkdir -p ~/venv` (or wherever you want it. Adjust the following commands to match this base directory.)
    1. Create whisper venv: `python -m venv ~/venv/whisper`
1. Activate venv: `. ~/venv/whisper/bin/activate`
1. Install whisper stuff in venv:
    1. `pip install openai-whisper`
    1. `pip install flask` <--- used only if you want to run our server

## Usage without server (will work right away.. least hassle to get started)

1. Edit `whisper-auto`
1. Change "use\_server=1` to `=0`
1. If you used a venv other than `~/venv/whisper`:
    1. You'll need to change the `envact=` line to point to it
    2. and the `envpattern=` line which is used to detect if we're in the venv already.
1. Run `./whisper-auto` (it will begin recording)
    1. Hit ctrl-c to stop recording, and whisper should begin processing.
    1. Alternatively, use the `whisper-kill-rec` script to end the recording task.

## Usage with our Whisper Server

In this mode, we run a separate little python app that keeps the model loaded.

https://blog.deepgram.com/how-to-build-an-openai-whisper-api/
Implemented an ultra-easy flask server (modified in this here project)
for the task.  Thanks Adam!  Check out his blog post.

1. In a separate term, go to the `server-flask` folder
    1. `flask run` should run it fine
    1. It uses port 5000 by default. Edit the `app.py` and `whisper-auto` to set the `server\_port=` if you change it.
1. Run `./whisper-auto` (it will begin recording)
    1. Hit ctrl-c to stop recording, and whisper should begin processing.
    1. Alternatively, use the `whisper-kill-rec` script to end the recording task.

# Installation - UI
    1. Go to the ui/ folder and type `make`
    1. I've not built out a list of dependencies -- you'll have to figure it out yourself (it uses gtk).  Submit a pull-request with dependencies added here if you want :)

# Installation - Hotkeys
    1. Assign two hotkeys, one to -auto and one to -kill-rec

# Installation - Desktop icons
    1. Drag both to your desktop, or otherwise add them.

*Note: They should probably be links or .desktop entries, not copies, so updates will be handled when you `git pull` updates.*

