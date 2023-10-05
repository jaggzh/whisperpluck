#!/usr/bin/env python
from flask import Flask, abort, request
import whisper
from tempfile import NamedTemporaryFile

# Load the Whisper model:
model = whisper.load_model('small')

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
        # The location of the temporary file is available in `temp.name`.
        temp = NamedTemporaryFile()
        # Write the user's uploaded file to the temporary file.
        # The file will get deleted when it drops out of scope.
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
    #return {'results': results}
    return '. '.join(strings)

# set noet ts=4
