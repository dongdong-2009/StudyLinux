/*
 * MappedMemory.cpp
 *
 *  Created on: 2013-9-16
 *      Author: LiuHao
 */

#include "stdafx.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "process.h"
#define FILE_LENGTH 0x100

const char* fileName = "/home/liuhao/temp.txt";
/* Return a uniformly random number in the range [low,high]. */
int random_range (unsigned const low, unsigned const high)
{
    unsigned const range = high - low + 1;
    return low + (int) (((double) range) * rand () / (RAND_MAX + 1.0));
}

void TestMappedMemory1()
{
    int fd;
    void* file_memory;
    /* Seed the random number generator. */
    srand (time (NULL));
    /* Prepare a file large enough to hold an unsigned integer. */
    fd = open (fileName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    lseek (fd, FILE_LENGTH+1, SEEK_SET);
    write (fd, "", 1);
    lseek (fd, 0, SEEK_SET);
    /* Create the memory mapping. */
    file_memory = mmap (0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0);
    close (fd);

    int integer = random_range (0, 100);
    /* Write a random integer to memory-mapped area. */
    sprintf((char*) file_memory, "%d\n", integer);

    /* Read the integer, print it out. */
    sscanf ((const char*)file_memory, "%d", &integer);
    printf ("value: %d\n", integer);

    /* Release the memory (unnecessary because the program exits). */
    munmap (file_memory, FILE_LENGTH);
}





