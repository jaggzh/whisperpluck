#!/usr/bin/env python
from flask import Flask, abort, request
import whisper
from tempfile import NamedTemporaryFile

# Configuration variables
HOST = '0.0.0.0'  # Listen on all network interfaces
PORT = 5000       # Default Flask port
DEBUG = False     # Flask debug mode

# Load the Whisper model:
# Examples from my whisper -h output:
#  tiny.en,tiny,base.en,base,small.en,small,medium.en,medium,large-v1,large-v2,large
#model = whisper.load_model('large-v2')
#model = whisper.load_model('small')
model = whisper.load_model('medium.en')

app = Flask(__name__)

@app.route('/', methods=['POST'])
def handler():
    if not request.files:
        # If the user didn't submit any files, return a 400 (Bad Request) error.
        abort(400)

    # For each file, let's store the results in a list of dictionaries.
    results = []
    strings = []

    # Loop over every file that the user submitted.
    for filename, handle in request.files.items():
        # Create a temporary file.
        temp = NamedTemporaryFile()
        # Write the user's uploaded file to the temporary file.
        handle.save(temp)
        # Let's get the transcript of the temporary file.
        result = model.transcribe(temp.name)
        print(f"Transcribed -> {result['text']}")
        # Now we can store the result object for this file.
        results.append({
            'filename': filename,
            'transcript': result['text'],
        })
        strings.append(result['text'])

    # This will be automatically converted to JSON.
    return '. '.join(strings)

if __name__ == '__main__':
    app.run(host=HOST, port=PORT, debug=DEBUG)
