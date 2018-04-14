/*
 * stdafx.h
 *
 *  Created on: 2013-9-12
 *      Author: LiuHao
 */

#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>   //for wait() function.
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/un.h>

#include <pthread.h>
#include <semaphore.h>

#if defined(__cplusplus)
#include <iostream>
#include <iomanip>
#include <sstream>
#include <assert.h>

using namespace std ;
#endif  //#if defined(__cplusplus)

#endif /* _STDAFX_H_ */
