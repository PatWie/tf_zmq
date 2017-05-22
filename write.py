import zmq
import numpy as np  # noqa
import tensor_msg

try:
    ctx = zmq.Context()
    socket = ctx.socket(zmq.PUSH)
    socket.connect('ipc:///tmp/ipc-socket-0')

    image_data = np.reshape(np.arange(16, dtype=np.float32), (4, 4))
    label_data = np.asarray([44], dtype=np.int32)

    msg = tensor_msg.pack([image_data, label_data])

    while True:
        socket.send(msg, copy=False)

finally:
    ctx.destroy(linger=0)
