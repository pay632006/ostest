/* Tests producer/consumer communication with different numbers of threads.
 * Automatic checks only catch severe problems like crashes.
 */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"

#define BUF_SIZE 10
char buf[BUF_SIZE] ="hello";     /* Buffer. */
size_t n = 0;           /* 0 <= n <= BUF_SIZE: # of characters in buffer. */
size_t head = 0;        /* buf index of next char to write (mod BUF_SIZE). */
size_t tail = 0;        /* buf index of next char to read (mod BUF_SIZE). */
struct lock lock;       /* Monitor lock. */
struct condition not_empty; /* Signaled when the buffer is not empty. */
struct condition not_full; /* Signaled when the buffer is not full. */

void put (char ch);
void * Producer(void);
char get (void);
void * Consumer(void);
void producer_consumer(unsigned int num_producer, unsigned int num_consumer);


void test_producer_consumer(void)
{
    producer_consumer(0, 0);
      producer_consumer(1, 0);
    producer_consumer(0, 1);
    producer_consumer(1, 1);
   producer_consumer(3, 1);
   /*producer_consumer(1, 3);
    producer_consumer(4, 4);
    producer_consumer(7, 2);
    producer_consumer(2, 7);
    producer_consumer(6, 6);
    */
    pass();
}


void producer_consumer(unsigned int num_producer, unsigned int num_consumer)
{
	int i;
	char name[16];
    msg("IMPLEMENTED");
    /* FIXME implement */
    lock_init(&lock);
        cond_init(&not_empty);
        cond_init(&not_full);


        /* Create the consumer threads */
        for(i = 0; i < num_consumer; i++) {
           /* Create the thread */
     	   snprintf (name, sizeof name, "thread %d", i);
     	   thread_create (name, PRI_DEFAULT, Consumer,  0 );
        }
        msg("main: %s",buf);
        /* Create the producer threads */
           for(i = 0; i < num_producer; i++) {
              /* Create the thread */
        	   snprintf (name, sizeof name, "thread %d", i);
        	   thread_create (name, PRI_DEFAULT, Producer,  0);

            }





           /* Sleep for the specified amount of time in milliseconds */
           //sleep(100);




}


void put (char ch) {
  lock_acquire (&lock);
  while (n == BUF_SIZE)            /* Can't add to buf as long as it's full. */
    cond_wait (&not_full, &lock);
  buf[head++ % BUF_SIZE] = ch;     /* Add ch to buf. */
  n++;
  cond_signal (&not_empty, &lock); /* buf can't be empty anymore. */
  lock_release (&lock);
}

void * Producer()
{
  int i;
  for(i = 0; i < 10; i++)
    {
      put((char)('A'+ i % 26));
      //msg("produ: %s , %d",buf , n);
      msg("prod: %d", n);
    }
}

char get (void) {
  char ch;
  lock_acquire (&lock);
  while (n == 0)                  /* Can't read buf as long as it's empty. */
    cond_wait (&not_empty, &lock);
  ch = buf[tail++ % BUF_SIZE];    /* Get ch from buf. */
  n--;
  cond_signal (&not_full, &lock); /* buf can't be full anymore. */
  lock_release (&lock);
  return ch;
}

void * Consumer()
{
  int i;
  char c;
  while(1)
      {
       c= get();
       msg("cons:%d %c", n , c);
      }

  /*for(i = 0; i < 10; i++)
    {
     c= get();
    //msg("cons: %s %d %c",buf , n , c);
     msg("cons:%d %c", n , c);

    }
    */
}


