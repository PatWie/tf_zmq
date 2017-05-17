import tensorflow as tf
import zmq_op

image, label = zmq_op.pull('ipc:///tmp/ipc-socket-0', [tf.float32, tf.int32])
with tf.Session() as sess:
    print(sess.run([image, label]))
