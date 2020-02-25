#!/usr/bin/python3

import mido
import os
import json
import argparse
import time

class MidiMenu:
    def __init__(self, midiPort, config):
        self.config = config
        self.midiPort = midiPort

    def run(self):
        midi = mido.open_input(self.midiPort)


        while(True):
            msg = midi.receive()

            if msg.type == 'note_on':
                if str(msg.note) in self.config:
                    os.system(self.config[str(msg.note)])

                    # flush midi messages
                    while midi.poll():
                        pass

        while(True):
            time.sleep(.1)

        self.midi.close_port()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('config')
    args = parser.parse_args()

    config = json.load(open(args.config))

    midiport = 'SparkFun Pro Micro:SparkFun Pro Micro MIDI 1 20:0'

    menu = MidiMenu(midiport, config)
    menu.run()

if __name__ == "__main__":
    main()
