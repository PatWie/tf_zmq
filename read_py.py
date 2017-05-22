import zmq
import numpy as np  # noqa
import time
import tensor_msg

try:
    ctx = zmq.Context()
    socket = ctx.socket(zmq.PULL)
    socket.bind('ipc:///tmp/ipc-socket-0')
    while True:
        data = socket.recv(copy=False).bytes
        print(tensor_msg.unpack(data))
        time.sleep(1)

finally:
    ctx.destroy(linger=0)
