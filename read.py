import tensorflow as tf
from zmq_op import zmq_pull

image, label = zmq_pull('ipc:///tmp/ipc-socket-0', [tf.float32, tf.int32])
with tf.Session() as sess:
    print(sess.run([image, label]))
