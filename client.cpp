#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>

 int main(int argc, char const *argv[]) {

     sockaddr_in myaddr,remaddr; //basic structures for all syscalls and functions that deal with internet addresses
     int fd, slen = sizeof (remaddr), recvlen;
     char buf [2048]; //message buffer
     int rcvlen;  //bytes in acknowledgement message
     char *server = "127.0.0.1";  //change this to use a different server
     //create socket
     if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) == -1 ) { //AF_NET (Family of network addresses)
         std::cout << "\n\t Socket creation failed...\n\t Exiting..." << '\n';
         return 0;
     }

     std::cout << "\n Socket CREATED" << '\n';
     //bind it to all local addresses and pick any port number
     memset ((char*)&myaddr, 0, sizeof (myaddr)); //clear memory
     myaddr.sin_family = AF_INET;
     myaddr.sin_addr.s_addr = htonl (INADDR_ANY); //host to network long
     myaddr.sin_port = htons (0); //host to network short

     //bind
     if (bind(fd, (sockaddr*) &myaddr, sizeof (myaddr)) < 0) {
         std::cout << "\nBind failed...\n\t Exiting..." << '\n';
         return 0; //creates socket using ip address + port number
     }

     std::cout << "\nBind SUCCESSFULL" << '\n';

     /* now define remaddr, the address to whom we want to send messages */
	/* For convenience, the host address is expressed as a numeric IP address */
	/* that we will convert to a binary format via inet_aton */

     memset ((char*) &remaddr, 0, sizeof (remaddr));
     remaddr.sin_family = AF_INET;
     remaddr.sin_port = htons (2222);
     if (inet_aton (server, &remaddr.sin_addr) == 0) {
         std::cout << "\ninet_aton() failed...\n\t Exiting..." << '\n';
         return 0; //converts the Internet host address cp from the IPv4 numbers-and-dots
         //notation into binary form (in network byte order) and stores it in the structure that inp points to.
     }

    while(1){
         int i = 0;
         std::cout.flush();
         std::cout << "\nEnter the number of Armstrong Numbers you want: ";
         std::cin.getline (buf, 2048);
         std::cout << "\nSending packet " << i << " to " << server << " on port " << remaddr.sin_port << '\n';
         sendto (fd, buf, strlen (buf), 0, (sockaddr*) &remaddr, slen);

         memset (buf, 0, sizeof (buf));
         socklen_t len;
         while(1){

         recvlen = recvfrom (fd, buf, 2048, 0, (sockaddr*) &remaddr, &len);
         if (strcmp(buf,"exit")==0)
         {
           break;
         }
         std::cout << "\nRECIEVED: " << buf << '\n';
         memset (buf, 0, sizeof (buf));


       }

   }
     close (fd);

     return 0;
 }
