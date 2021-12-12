/*
 * Implementation of the calculator server
 * CSF Assignment 5
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */

#include <stdio.h>      /* for snprintf */
#include "csapp.h"
#include "calc.h"

/* buffer size for reading lines of input from user */
#define LINEBUF_SIZE 1024
#define MAX 25 //the maximum num of connections

typedef struct {
  struct Calc *calc;
  int in;
  int out;
  int i;
} thread;

sem_t sem;  //Semaphore
thread params[MAX];  
int listenfd = -1;
int busy[MAX] = {0};   
int results[MAX] = {0};   

int chat_with_client(struct Calc *calc, int infd, int outfd, int index);
void* clean_shut_down_chat_with_client(void *params);
void print_error(char* message);

int main(int argc, char **argv) {
  if(argc != 2){
    print_error("Error: invalid arguments");
  }
  // create a server
  listenfd = Open_listenfd(argv[1]);
  if (listenfd < 0) {
    print_error("Error: failed to open up server");
  }

  struct Calc *networkCalc = calc_create();
  pthread_t pid[MAX];

  // initiate semaphores
  if(sem_init(&sem, 0, MAX) != 0) {
    printf("Semaphore initialization failed\n");
  }
  
  while (listenfd >= 0) {
    int clientfd = Accept(listenfd, NULL, NULL);
    if (clientfd < 0){
      print_error("Error: client connection failed");
    }

    if(listenfd < 0) {
      // the server has been shutdown
      close(clientfd);
      break;
    }

    sem_wait(&sem);
    //update busy
    int i;

    for (i=0; i<MAX; i++) {
      if(busy[i] == 0) {
        break;
      }
    }

    busy[i] = 1;


    thread param = {
      networkCalc,
      clientfd,
      clientfd,
      i
    };
    params[i] = param;

    //create a pthread
    int ret = pthread_create(&pid[i],NULL,clean_shut_down_chat_with_client,(void *)&params[i]); 
    if(ret!=0){ 
      printf("Create pthread error!\n"); 
      exit(1); 
    } 
  }

  // join threads
  for(int i=0; i<MAX; i++) {
    if(busy[i]) {
      pthread_join(pid[i], NULL);
    }
  }

  calc_destroy(networkCalc);
  close(listenfd);

  return 0;
}

//print out error message
//Parameter: message - a pointer to a Cstring
void print_error(char* message){
  fprintf(stderr, "%s\n", message);
  exit(1);
}


/*
 * Takes in an input and writes out an output
 *
 * Parameters:
 *   *calc - pointer to the calc struct to be deleted
 *   infd - file descriptor specifiying where to read from
 *   outfd - file descriptor specifying where to output the result
 *
 * Returns:
 *   0 if the client chat is done, -1 if shutdown
 */
int chat_with_client(struct Calc *calc, int infd, int outfd, int index) {
  rio_t in;
  char linebuf[LINEBUF_SIZE];

  /* wrap standard input (which is file descriptor 0) */
  rio_readinitb(&in, infd);

  /*
  * Read lines of input, evaluate them as calculator expressions,
  * and (if evaluation was successful) print the result of each
  * expression.  Quit when "quit" command is received.
  */
  int done = 0;
  while (!done) {
    ssize_t n = rio_readlineb(&in, linebuf, LINEBUF_SIZE);
    if (n <= 0) {
      /* error or end of input */
      done = 1;
    }
    else if (strcmp(linebuf, "quit\n") == 0 || strcmp(linebuf, "quit\r\n") == 0) {
      /* quit command */
      done = 1;
    }
    else if (strcmp(linebuf, "shutdown\n") == 0 || strcmp(linebuf, "shutdown\r\n") == 0) {
      /*shutdown command */
      return -1;
    }
    else {
      /* process input line */
      int result;
      if (calc_eval(calc, linebuf, &result) == 0) {
	/* expression couldn't be evaluated */
	rio_writen(outfd, "Error\n", 6);
      } else {
	/* output result */
	int len = snprintf(linebuf, LINEBUF_SIZE, "%d\n", result);
	if (len < LINEBUF_SIZE) {
	  rio_writen(outfd, linebuf, len);
	}
      }
    }
  }

  // reset busy
  busy[index] = 0;
  sem_post(&sem);

  return 0;
}


//a function that update the array of results and close the listen when recieve shutdown
// Parameter: params - a void pointer in C
void* clean_shut_down_chat_with_client(void *params) {
  thread *p = (thread *)params;

  int ret = chat_with_client(p->calc, p->in, p->out, p->i);
  //update results
  results[p->i] = ret;

  close(p->in);

  //close if shutdown signal received
  if(ret == -1) {
    close(listenfd);
    listenfd = -1;
  }

	return NULL;
}