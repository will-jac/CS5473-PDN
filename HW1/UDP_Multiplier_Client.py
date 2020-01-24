from socket import *
from datetime import datetime

# sends the message
# - wait up to one second for a reply
# - - resend if timeout
# - resend on error
# print what happens on each attempt to std_out
def send_message(socket, message, server):
    time_start = datetime.now()
    try:
        # this blocks until success, throws timeout on error
        socket.sendto(message.encode(), server)
        # get the response
        response, server_address = socket.recvfrom(2048)
        response = response.decode()
        if response == 'Incorrect sum':
            print('Server Error, ' + \
                'RTT = ' + str(datetime.now() - time_start))
            send_message(socket, message, server)    # try again
        else:
            print('Result = ' + str(response) + ' '\
                'RTT = ' + str(datetime.now() - time_start))
            return(response)
    except timeout:
        print('Request timed out')
        send_message(socket, message, server)        # try again


# init UDP object to send
client_socket = socket(AF_INET, SOCK_DGRAM)

# init servername, port, numbers from user
server_name = input('Input server name: ')
server_port = input('Input server port: ')
num_a       = input('Input first number to multiply: ')
num_b       = input('Input second number to multiply: ')

message = 'Multiply ' \
    + str(num_a) + ' ' + str(num_b) + ' ' + str(num_a+num_b) + ' ' \
    + str(datetime.now())

client_socket.settimeout(1.0)

send_message(client_socket, message, (server_name, int(server_port)))


client_socket.close()
