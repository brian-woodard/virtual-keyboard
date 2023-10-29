//-----------------------------------------------------------------------------
//                              UNCLASSIFIED
//-----------------------------------------------------------------------------
//                    DO NOT REMOVE OR MODIFY THIS HEADER
//-----------------------------------------------------------------------------
//
//  This software and the accompanying documentation are provided to the U.S.
//  Government with unlimited rights as provided in DFARS § 252.227-7014.  The
//  contractor, Veraxx Engineering Corporation, retains ownership, the
//  copyrights, and all other rights.
//
//  © Veraxx Engineering Corporation 2022.  All rights reserved.
//
// DEVELOPED BY:
//  Veraxx Engineering Corporation
//  14130 Sullyfield Circle Ste B
//  Chantilly, VA 20151
//  (703)880-9000 (Voice)
//  (703)880-9005 (Fax)
//
//-----------------------------------------------------------------------------
//
//  Title:      SimUdpSocket Source
//  Class:      C++ Source
//  Filename:   SimUdpSocket.cpp
//  Author:     Jim Woodward
//  Purpose:
//
//--------------------------------------------------------------------
//  Change History is automatically maintained -- DO NOT EDIT
//--------------------------------------------------------------------
//
//  Change History:
//
//       SimUdpSocket.cpp
//
//
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#ifndef WIN32
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#endif
#include "SimUdpSocket.h"

using namespace std;


CSimUdpSocket::CSimUdpSocket()
{

   // initialize
   mIsOpen = false;
}


CSimUdpSocket::CSimUdpSocket(char *IpAddress, int SendPort, int RecvPort)
{

   // initialize
   mIsOpen = false;

   // open the socket
   mIsOpen = Open(IpAddress, SendPort, RecvPort);
}

// ----------------------------------------------------------------------------------------------------
CSimUdpSocket::~CSimUdpSocket()
{
#ifdef WIN32
   closesocket(mSocket);
   WSACleanup();
#else
   close (mSocket);
#endif
}


bool CSimUdpSocket::Open(char *IpAddress, int SendPort, int RecvPort)
{
   int optval = 1;

#ifdef WIN32
   WORD VersionRequested = MAKEWORD(2,2);
   WSADATA wsaData;
   int Stat;

   Stat = WSAStartup(VersionRequested, &wsaData);
   if (Stat != 0)
      return false;
#endif

   // check if already open
   if (mIsOpen) return true;

   // Initialize the output socket structure
   memset (&mAddressOut, 0, sizeof(mAddressOut));
   mAddressOut.sin_addr.s_addr = inet_addr(IpAddress);
   mAddressOut.sin_family      = AF_INET;
   mAddressOut.sin_port        = htons(SendPort);

   // Initialize the input socket structure
   memset (&mAddressIn, 0, sizeof(mAddressIn));
   mAddressIn.sin_addr.s_addr = INADDR_ANY;
   mAddressIn.sin_family      = AF_INET;
   mAddressIn.sin_port        = htons(RecvPort);
  
  // open the sockets
  if ( (mSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror(" UDP Port Constructor: Output socket()");
    return false;
  }

  // Set the socket options for broadcast address
#ifdef WIN32
  if (setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval)) < 0) {
    perror(" UDP Port Constructor: SetSockOpt()");
    return false;
  }

  if (setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) < 0) {
    perror(" UDP Port Constructor: SetSockOpt()");
    return false;
  }
#else
  if (setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0) {
    perror(" UDP Port Constructor: SetSockOpt()");
    return false;
  }

  if (setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
    perror(" UDP Port Constructor: SetSockOpt()");
    return false;
  }
#endif

  //  Bind to the  input socket 
  if (bind (mSocket, (struct sockaddr*)&mAddressIn, sizeof(mAddressIn)) == -1 )  {
     perror("UDP Port Constructor(): bind()");
     return false;
  }

  return true;
}


// ----------------------------------------------------------------------------------------------------

int CSimUdpSocket::SendToSocket(char *DataBuffer, int SizeInBytes) 
{
  int send_status;  /* Status of sendto function call */

  /* Send data out on the socket */
  send_status = sendto(mSocket, DataBuffer, SizeInBytes, 0, (struct sockaddr*)&mAddressOut,sizeof(mAddressOut));
    
  if ( send_status < 0 ) {
      return (-1);
  }

  return send_status;
}

// ----------------------------------------------------------------------------------------------------

int  CSimUdpSocket::ReceiveFromSocket(char *DataBuffer, int MaxSizeToRead)
{
  int bytes_returned;
 #ifdef WIN32
  int fromlen;
 #else
  socklen_t fromlen;
 #endif
  struct sockaddr insock;
  
  fromlen = MaxSizeToRead;  /* This will get changed to actual number of bytes read in */

  /* Read any data received on the socket. */
  bytes_returned = recvfrom (mSocket, DataBuffer, MaxSizeToRead, 0, (struct sockaddr*)&insock,&fromlen);

  if (bytes_returned == -1)
  {
     if (errno == EAGAIN)
        return -3;
     else {
        //perror("SimUdpSocket::ReceiveFromSocket(): recvfrom()");
        return -1;
     }
  }

  return(bytes_returned);
}

void CSimUdpSocket::SetNonBlockingFlag()
{
#ifdef WIN32
   unsigned long socket_flags = 1;

   //Enable non blocking
   ioctlsocket(mSocket, FIONBIO, &socket_flags);
#else
   int socket_flags;
 
   // Get Socket flags
   socket_flags = fcntl(mSocket,F_GETFL);

   // Set non-blocking status
   socket_flags |= FNDELAY;

   // Re-set the socket flags
   fcntl(mSocket,F_SETFL, socket_flags);
#endif
}

void CSimUdpSocket::ClearNonBlockingFlag()
{
#ifdef WIN32
   unsigned long socket_flags = 1;

   //Enable non blocking
   ioctlsocket(mSocket, FIONBIO, &socket_flags);
#else
   int socket_flags;
 
   // Get Socket flags
   socket_flags = fcntl(mSocket,F_GETFL);

   // Set blocking status
   socket_flags &= ~FNDELAY;

   // Re-set the socket flags
   fcntl(mSocket,F_SETFL, socket_flags);
#endif
}


