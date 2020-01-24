from socket import *
serverName = '156.110.247.18'
serverPort = 80
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName, serverPort))
sentence = 'GET / HTTP/1.1\r\nHost: 156.110.247.18\r\n\r\n'
clientSocket.send(sentence.encode())
modifiedSentence = clientSocket.recv(1024)
print('From Server: ', modifiedSentence.decode())
clientSocket.close()
