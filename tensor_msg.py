import msgpack
import struct
import numpy as np
import ctypes


def pack(data):
    """Unpack tensor_msg send by c++ counterpart

    Args:
        data: string containing plain bytes send over socked

    Returns:
        received list of tensors as NumPy arrays with correct shape and dtype

    """

    rsl = []

    for dp in data:
        shape = dp.shape
        typ = [np.int32, np.float32, np.float64].index(dp.dtype)
        fmt = ['i', 'f', 'd']
        fmt = fmt[typ] * np.prod(shape)
        byts = struct.pack(fmt, *dp.flatten())

        msg = [typ, shape, byts]
        rsl.append(msg)

    msg = msgpack.packb(rsl, use_bin_type=True)

    return msg


def unpack(data):
    """Unpack tensor_msg send by c++ counterpart

    Args:
        data: string containing plain bytes send over socked

    Returns:
        received list of tensors as NumPy arrays with correct shape and dtype

    """
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
