# Import socket module
from socket import *
import sys # In order to terminate the program




class note():
    	message = "Hello World"
	coords = [-1,-1]
	dimensions = [0,0]
	color = ""

def createNote(messageIn,coordsIn,dimensionsIn, colorIn):
	stickyNote = note()

	stickyNote.message = messageIn
	stickyNote.coords = coordsIn
	stickyNote.dimensions = dimensionsIn
	stickyNote.color = colorIn

	return stickyNote

# Create a TCP server socket
#(AF_INET is used for IPv4 protocols)
#(SOCK_STREAM is used for TCP)
server_start = " "

#used to get all info from command line - board size , color  etc
if(len(sys.argv) > 4):

	serverPort = int(sys.argv[1])
	board_width = int(sys.argv[2])
	board_height = int(sys.argv[3])
	color = sys.argv[4:]

	server_start = sys.argv[1] + " "

		
else:

	print("not enough info provided")
	exit(0)

#server socket initialization
serverSocket = socket(AF_INET, SOCK_STREAM)

# Bind the socket to server address and server port
serverSocket.bind(("", serverPort))

# Listen to at most 1 connection at a time
serverSocket.listen(1)

print ('The server is ready to receive')

# Server should be up and running and listening to the incoming connections

#while server is on keep intaking client sockets
while True:
	print('The server is ready to receive')

	# Set up a new connection from the client
	connectionSocket, addr = serverSocket.accept()

	#convert list to string to send info to client
	formatted_message = ' '.join(str(e) for e in sys.argv[1:])

	#send in info client needs - size of board,color etc
	connectionSocket.send(formatted_message.encode())

	#getting message from client
	message = connectionSocket.recv(1024).decode()
	option = message[0]
	print(option)
	
	#if option was 1 then get note
	if(option == 1):
		note = ' '.join(message.split(" ")[1:])
		print(note)

	elif(option == 5):
		#closing all connections
		connectionSocket.close()


serverSocket.close()  
sys.exit()#Terminate the program after sending the corresponding data
