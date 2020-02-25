from socket import *
import pickle

worker_port_1 = 1111
worker_port_2 = 2222
worker_name = '127.0.0.1'

# open up the socket
w1_socket = socket(AF_INET, SOCK_STREAM)
w2_socket = socket(AF_INET, SOCK_STREAM)

w1_socket.connect((worker_name, int(worker_port_1)))
w2_socket.connect((worker_name, int(worker_port_2)))

A = [1,3,5,7,9,11,13,15]
B = [2,4,6,8,10,12,14,16]
k = len(A)
n = int(k/2)

# send the tasks to the workers
w1_socket.send(pickle.dumps((A[0:n], B[0:n])))
w2_socket.send(pickle.dumps((A[n:k], B[n:k])))

# get the responses
p1 = int(w1_socket.recv(1024).decode())
p2 = int(w2_socket.recv(1024).decode())

w1_socket.close()
w2_socket.close()

print(str(p1+p2))

