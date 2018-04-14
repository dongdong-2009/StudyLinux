/*
 * thread.h
 *
 *  Created on: 2013-9-17
 *      Author: LiuHao
 */

#ifndef _THREAD_H_
#define _THREAD_H_

#if defined(__cplusplus)
extern "C"
{
#endif  //#if defined(__cplusplus)

void* TestMutexes(void*);
void TestThread();
void TestSemaphore();
void TestConditionVariable();

#if defined(__cplusplus)
}
#endif  //#if defined(__cplusplus)

#endif /* THREAD_H_ */
