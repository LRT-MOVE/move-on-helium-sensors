#!/usr/bin/env python3

import logging
from lib.input.udp_server import UdpServer
from lib.parse.sen_bin_parse import SenBinParse
from lib.output.mqtt_broker import MqttBroker

logging.basicConfig(level=logging.DEBUG, format="%(asctime)-15s %(levelname)s %(message)s")

input = UdpServer(23000)
parser = SenBinParse()
output = MqttBroker()


def main():
    logging.info("udp2mqtt_sen starting")

    for packet in input.get_packets():
        logging.debug("Packet: %s" % packet)

        for (key, value) in parser.parse_packet(packet):
            logging.info("Pub %s: %s" % (key, value))
            #TODO: Disabled until verified
            #output.publish(key, value)


if __name__ == '__main__':
    main()