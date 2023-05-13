# whisperpluck

Some scripts for convenient voice transcription

Usable with hotkeys or desktop icons, for now.

## **IMPORTANT**

This was initially designed for the desktop icons. It updates/renames the icons to give status (like WH-Ready -> WH-Rec), but this is not updating [the desktop icons] in KDE Plasma, and that's something I'm working on now.

Note that this also means the hotkeys method of usage will still b trying to access the desktop icons, and I've not tested that.

# Requirements
* jq: Needed if using the server (to parse the json response)

# USAGE METHODS (server mode or running whisper each transcription)

We have two methods:

1. Run whisper each time you want a transcription (model must be loaded each run)
2. Run a separate little server (in server-flask/) which keeps the model loaded so transcriptions are faster

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
1. Change "use_server=1` to `=0`
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
    1. It uses port 5000 by default. Edit the `app.py` and `whisper-auto` to set the `server_port=` if you change it.
1. Run `./whisper-auto` (it will begin recording)
    1. Hit ctrl-c to stop recording, and whisper should begin processing.
    1. Alternatively, use the `whisper-kill-rec` script to end the recording task.

# Installation - Hotkeys
    1. Assign two hotkeys, one to -auto and one to -kill-rec

# Installation - Desktop icons
    1. Drag both to your desktop, or otherwise add them.

*Note: They should probably be links or .desktop entries, not copies, so updates will be handled when you `git pull` updates.*

# I'm working on a ui in `ui/`
