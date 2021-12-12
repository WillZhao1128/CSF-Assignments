Discussion: 
    In calcServer, we enabled access from 25 different threads concurently by using the Sempahore synchonization object. Two different arrays are created to keep track
of the status and result of each thread. Waiting threads are awaken after finishing chat with client, and a free slot will be found based on the statuses of the busy array.

    In the calc instance, we made it data safe by locking the critical region in the evalExpr function with Mutex. The critical region is determined to be the region in which
the "expr_vec" is being manipulated after it has been tokenized, and the calculated result is being stored into "result". This is because when multiple threads are executing concurrently, 
a variable could be changed between the time it is updated and when it is loaded back into the vector due to multithreading. With Mutex locking, this multithreading is disabled, so the 
variables and datastructures of different threads are mutually exclusive, and the calc is data safe.

HW 6 Contributions:
    Aidan Aug: modified Calc to enable concurent modification, debugging, testing and README
    Will Zhao: Rewrote calcServer for multiple threads