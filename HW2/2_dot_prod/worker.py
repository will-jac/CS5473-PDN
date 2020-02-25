from socket import *
import sys
import pickle

# the workers act as servers that a 'master' will submit jobs to

worker_port = sys.argv[1]
# in theory, this would be run on two seperate machines, but because it needs to run on one,
# so the user needs to be able to change the ports for each process running

# open up the socket
worker_socket = socket(AF_INET, SOCK_STREAM)
worker_socket.bind(('', int(worker_port)))
worker_socket.listen(1)
while True:
    master_socket, addr = worker_socket.accept()
    (A, B) = pickle.loads(master_socket.recv(1024))
    master_socket.send(str(sum([a*b for a,b in zip(A,B)])).encode())
    master_socket.close()

