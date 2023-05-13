# whisperpluck

Some scripts for convenient voice transcription

Usable with hotkeys or desktop icons, for now.

## **IMPORTANT**

This was initially designed for the desktop icons. It updates/renames the icons to give status (like WH-Ready -> WH-Rec), but this is not updating [the desktop icons] in KDE Plasma, and that's something I'm working on now.

Note that this also means the hotkeys method of usage will still b trying to access the desktop icons, and I've not tested that.

`whisper-auto`

# Usage:
    ```
    whisper-auto      # Begins recording
    whisper-kill-rec  # Ends recording
    ```

Upon ending recording, `whisper-auto` continues to then handle the transcription.  It uses an existing python `venv` called `whisper`, automatically changing to the pyenv.  (It checks the current env to reduce overhead of changing repeatedly if not necessary).

# Installation - Hotkeys
    1. Assign two hotkeys, one to -auto and one to -kill-rec

# Installation - Desktop icons
    1. Drag both to your desktop, or otherwise add them.

*Note: They should probably be links or .desktop entries, not copies, so updates will be handled when you `git pull` updates.*


