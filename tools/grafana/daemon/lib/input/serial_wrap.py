import logging
import serial


class SerialWrap(serial.Serial):
    def __init__(self, *args, **kwargs):
        self.serial_args = args
        self.serial_kwargs = kwargs
        self.serial = False
        self.try_connect()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.serial.close()

    def try_connect(self):
        try:
            self.serial = serial.Serial(*self.serial_args, **self.serial_kwargs)
            logging.info("Serial connected")
        except serial.serialutil.SerialException:
            logging.info("Serial connection failed")
            self.try_close()

    def try_close(self):
        try:
            self.serial.close()
        except AttributeError:
            pass
        self.serial = False
        return

    def try_readline(self, *args, **kwargs):
        if self.serial:
            try:
                return self.serial.readline(*args, **kwargs)
            except serial.serialutil.SerialException:
                self.try_close()
                return
        else:
            self.try_connect()
            return

    def try_readline_decode(self, binary=False, *args, **kwargs):
        line = self.try_readline(*args, **kwargs)
        if not line:
            return

        # Try to decode line
        try:
            line = line.strip().decode()
        except TypeError:
            return
        except UnicodeDecodeError:
            if binary:
                return line
            else:
                return
        return line

    def get_packets(self, binary=False):
        while 1:
            line = self.try_readline_decode(binary)
            if line:
                yield line

    def try_write(self, *args, **kwargs):
        if self.serial:
            try:
                return self.serial.write(*args, **kwargs)
            except serial.serialutil.SerialException:
                self.try_close()
                return
        else:
            self.try_connect()
            return
