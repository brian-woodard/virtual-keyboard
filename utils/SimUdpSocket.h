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
//  Title:      SimUdpSocket Header
//  Class:      C++ Header
//  Filename:   SimUdpSocket.h
//  Author:     Jim Woodward
//  Purpose:
//
//--------------------------------------------------------------------
//  Change History is automatically maintained -- DO NOT EDIT
//--------------------------------------------------------------------
//
//  Change History:
//
//       SimUdpSocket.h
//
//
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#pragma once

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <netinet/in.h>
#include <features.h> /* for the glibc version number */


#if __GLIBC__ >= 2 && __GLIBC_MINOR >= 1
#include <netpacket/packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#else
#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h> /* the L2 protocols */
#endif
#endif

                                                                                                                            
class CSimUdpSocket
{
public:
                                                                                                                            
   CSimUdpSocket();
   CSimUdpSocket(char* IpAddr, int SendPort, int ReceivePort);
   ~CSimUdpSocket();

   bool Open(char* IpAddr, int SendPort, int ReceivePort);

   int  SendToSocket(char* DataBuffer, int SizeInBytes);
   int  ReceiveFromSocket(char* DataBuffer, int MaxSizeToRead);

   void SetNonBlockingFlag();
   void ClearNonBlockingFlag();

private:

   bool   mIsOpen;
#ifdef WIN32
   SOCKET mSocket;
#else
   int    mSocket;
#endif

   struct sockaddr_in mAddressOut;
   struct sockaddr_in mAddressIn;
};
                                                                                                                          
