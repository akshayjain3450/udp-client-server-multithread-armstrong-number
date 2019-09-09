#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <algorithm>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>
#include <string>
#include <stdlib.h>
#define INT_MAX 2147483647
using namespace std;

 pthread_t threads [50]; //thread declaration
 int threadno = 0, fd;

 void sig_handler(int signo) {

     if (signo == SIGINT) {
         std::cout << "Exiting..." << '\n';
         close (fd);
         exit (1);
     } //after CTRL-C port number need not be changed
 }

 struct req {
     int reqno;
     int des;
     char str [2048];
     socklen_t addlen;
     sockaddr_in clientaddr;
 };

 void* armstrong (void*);
 int check_armstrong(int);
 int power(int, int);

 int main(int argc, char const *argv[]) {

     sockaddr_in repladdr; // server address
     sockaddr_in clientaddr; // client address

     socklen_t addrlen = sizeof(clientaddr);
     int recvlen, msgcnt = 0;
     char buf [2048]; //buffer

     if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) == -1) {
         std::cout << "\nSocket creation failed...\n\t Exiting..." << '\n';
         return 0;
     }

     std::cout << "\nSocket CREATED" << '\n';
//bind it to all local addresses and pick any port number
     memset ((sockaddr*)&repladdr, 0, sizeof (repladdr));
     repladdr.sin_family = AF_INET;
     repladdr.sin_addr.s_addr = htonl (INADDR_ANY);
     repladdr.sin_port = htons (2222);

     if (bind (fd, (sockaddr*)&repladdr, sizeof (repladdr)) == -1) {
         std::cout << "\nBinding failed...\nExiting..." << '\n';
         return 0;//creates socket using ip address + port number
     }

     std::cout << "\nBinding SUCCESSFULL" << '\n';

     signal(SIGINT, sig_handler); //
     signal(SIGTSTP, sig_handler); //Ctrl+Z

     while (1) {
         std::cout << "\nWaiting on port " << repladdr.sin_port << '\n';
         recvlen = recvfrom (fd, buf, 2048, 0, (sockaddr*) &clientaddr, &addrlen);
         msgcnt++;

         req *r = new req;
         bzero (r, sizeof (req)); //clear
         r->reqno = msgcnt;
         r->addlen = addrlen;
         r->clientaddr = clientaddr;
         r->des = fd;
         strcpy (r->str, buf);

         pthread_create (&threads [threadno++], NULL, armstrong, (void*)r);
         if (threadno == 50)
             threadno = 0;

         memset (buf, 0, sizeof (buf));
     }

     return 0;
 }

//to find armstrong function

 void* armstrong (void* r) {

     req rq = *((req*)r);
     string str1;
     std::cout.flush();
     std::cout << "\nClient Input: " << rq.str << "  | From IP ADDRESS: " << inet_ntoa (rq.clientaddr.sin_addr); //server printing client ip address
     std::cout << " (message count: " << rq.reqno << ")" << '\n';
     //std::reverse( rq.str, &rq.str[ strlen( rq.str ) ] );
     int N = stoi(rq.str);
     int count = 0;
     for (int i = 1; i<=INT_MAX; i++)
     {
       if (count == N){
         break;
       }
       if (check_armstrong(i) == 1)
       {
         count = count + 1;
         str1 = to_string(i);
         strcpy(rq.str,str1.c_str());
         sendto (rq.des, rq.str, strlen (rq.str), 0, (sockaddr*) &rq.clientaddr, rq.addlen);

       }
     }
     strcpy(rq.str,"exit");
     sendto (rq.des, rq.str, strlen (rq.str), 0, (sockaddr*) &rq.clientaddr, rq.addlen);


 }
 int check_armstrong(int n) {
    long long sum = 0, temp;
    int remainder, digits = 0;

    temp = n;

    while (temp != 0) {
       digits++;
       temp = temp/10;
    }

    temp = n;

    while (temp != 0) {
       remainder = temp%10;
       sum = sum + power(remainder, digits);
       temp = temp/10;
    }

    if (n == sum)
       return 1;
    else
       return 0;
 }

 int  power(int n, int r) {
    int c, p = 1;

    for (c = 1; c <= r; c++)
       p = p*n;

    return p;
 }
