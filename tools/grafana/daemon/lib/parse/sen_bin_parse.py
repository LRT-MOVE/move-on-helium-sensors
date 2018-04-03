import logging
from subprocess import Popen, PIPE, STDOUT
from lib.parse.sen_ascii_parse import SenAsciiParse


class SenBinParse:
    def __init__(self):
        self.ascii_parser = SenAsciiParse()

    def parse_packet(self, packet):
        if len(packet) < 10:
            return

        length, status = packet[:2]
        data = packet[2:-1]
        invalid_chunks = packet[-1]
        logging.debug("BIN IN: CHK=%d DATA%s" % (invalid_chunks, packet))

        parser = Popen(['moveon-sen-parser'], stdout=PIPE, stdin=PIPE, stderr=STDOUT)
        stdout = parser.communicate(input=data)[0]

        logging.debug("BIN OUT: %s" % stdout.strip().decode())

        for line in stdout.decode().splitlines():
            for data in self.ascii_parser.parse_packet(line, "com"):
                yield data
