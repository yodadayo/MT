/* This is a sample program using CSockServer.
 * It is neccesary to use test_client1 in order to see how this works
 * 
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "CSockServer.h"
using namespace std;

int main()
{
  char test[100];
  
  // create an instance
  CSockServer testserver;
  
  // prepare a socket at port No.10000
  // this function will block the flow of this program until a client establishes a connection
  testserver.initializeSocket(10000);

  // wait to read the return message form the client
  testserver.readFromSock(test,sizeof(test));
  printf("message: \"%s\" was received\n",test);
  
  // send the message to the client program
  sprintf(test,"this is a server");
  testserver.writeToSock(test,strlen(test));
  printf("message: \"%s\" was sent\n",test);
  
  // if you want to cancel waiting to receive a message,
  // you can use "readFromSockWithSelect" function.
  //
  // ex) testserver.readFromSockWithSelect(test,100);
  //
  // then you can read only if a message is queued in the socket.
  // since this function is using select, Unix/Linux environment is recommended.

  // cut the connection
  testserver.terminateSocket();

  return 0;    
}

