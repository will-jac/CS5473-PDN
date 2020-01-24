# UDPPingerServer.py
# We will need the following module to generate randomized lost packets
import random
from socket import *

# Create a UDP socket
# Notice the use of SOCK_DGRAM for UDP packets
serverSocket = socket(AF_INET, SOCK_DGRAM)
# Assign IP address and port number to socket
serverSocket.bind(('', 12000))
print ("The server is now ready to receive packets")
while True:
    # Generate random number in the range of 1 to 4
    rand = random.randint(1, 4)
    # Receive the client packet along with the address it is coming from
    message, address = serverSocket.recvfrom(1024)
    # If rand is 1, assumed that the packet is lost and server does not respond
    if rand == 1:
        continue
    # If rand is 2, transmit error is assumed and server responds with an error message
    elif rand == 2:
        message = "Incorrect sum".encode()
        serverSocket.sendto(message, address)
        continue
    # Otherwise, the server calculates the product and responds with the result
    message_split = message.split()
    result = int(message_split[1]) * int(message_split[2])
    message = str(result).encode()
    serverSocket.sendto(message, address)
