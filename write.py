import zmq
import numpy as np  # noqa


def pack_tensor_msg(data):
    """Unpack tensor_msg send by c++ counterpart

    Args:
        data: string containing plain bytes send over socked

    Returns:
        received list of tensors as NumPy arrays with correct shape and dtype

    """

    import msgpack

    rsl = []

    for dp in data:
        shape = dp.shape
        typ = [np.int32, np.float32, np.float64].index(dp.dtype)
        msg = [typ, shape, dp.tobytes()]
        rsl.append(msg)

    msg = msgpack.packb(rsl)

    return msg


try:
    ctx = zmq.Context()
    socket = ctx.socket(zmq.PUSH)
    socket.connect('ipc:///tmp/ipc-socket-0')

    image_data = np.reshape(np.arange(16, dtype=np.float32), (4, 4))
    label_data = np.asarray([44], dtype=np.int32)

    msg = pack_tensor_msg([image_data, label_data])

    while True:
        socket.send(msg, copy=False)

finally:
    ctx.destroy(linger=0)
