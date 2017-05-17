import tensorflow as tf
import os

__all__ = ['zmq_pull']

path = os.path.join(os.path.dirname(__file__), 'zmq_pull.so')
_zmq_pull_module = tf.load_op_library(path)
zmq_pull = _zmq_pull_module.zmq_pull
