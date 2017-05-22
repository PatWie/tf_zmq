#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# File: tensor_msg.py
# Author: Patrick Wieschollek <mail@patwie.com>

import msgpack
import struct
import numpy as np


def pack(tensor_list):
    """Unpack tensor_msg send by c++ counterpart

    Args:
        tensor_list: list of numpy arrays

    Returns:
        received list of tensors as NumPy arrays with correct shape and dtype

    """

    rsl = []

    typs = [np.int32, np.float32, np.float64]
    fmts = ['i', 'f', 'd']

    for dp in tensor_list:
        assert dp.dtype in [np.int32, np.float32, np.float64]
        typ = typs.index(dp.dtype)
        fmt = fmts[typ] * np.prod(dp.shape)
        byts = struct.pack(fmt, *dp.flatten())

        msg = [typ, dp.shape, byts]
        rsl.append(msg)

    return msgpack.packb(rsl, use_bin_type=True)


def unpack(data):
    """Unpack tensor_msg from binary data.

    Args:
        data: string containing plain bytes send over some pipe

    Returns:
        received list of tensors as NumPy arrays with correct shape and dtype

    """

    data = msgpack.unpackb(data)
    tensor_list = []

    allowed_types = [('i', np.int32), ('f', np.float32), ('d', np.float64)]

    for dp in data:
        shape = dp[1]

        fmt = allowed_types[dp[0]][0] * np.prod(shape)
        typ = allowed_types[dp[0]][1]

        arr = struct.unpack(fmt, dp[2])
        arr = np.reshape(np.asarray(arr, dtype=typ), shape)
        tensor_list.append(arr)

    return tensor_list
