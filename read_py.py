import zmq
import numpy as np  # noqa
import time


def unpack_tensor_msg(data):
    """Unpack tensor_msg send by c++ counterpart

    Args:
        data: string containing plain bytes send over socked

    Returns:
        received list of tensors as NumPy arrays with correct shape and dtype

    """
    import struct
    import msgpack
    data = msgpack.unpackb(data)

    rsl = []

    for dp in data:
        shape = dp[1]
        if dp[0] == 0:
            # int32
            fmt = 'i' * np.prod(shape)
            typ = np.int32
        elif dp[0] == 1:
            # float32
            fmt = 'f' * np.prod(shape)
            typ = np.float32
        elif dp[0] == 2:
            # double (float64)
            fmt = 'd' * np.prod(shape)
            typ = np.float64

        arr = struct.unpack(fmt, dp[2])
        arr = np.reshape(np.asarray(arr, dtype=typ), shape)
        rsl.append(arr)

    return rsl


try:
    ctx = zmq.Context()
    socket = ctx.socket(zmq.PULL)
    socket.bind('ipc:///tmp/ipc-socket-0')
    while True:
        data = socket.recv(copy=False).bytes
        print(unpack_tensor_msg(data))
        time.sleep(1)

finally:
    ctx.destroy(linger=0)
