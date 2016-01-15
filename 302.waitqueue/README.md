This version of kex uses a wait queue to implement a blocking read.
A wait queue is the mechanism by which a process is moved from the
runnable state to the sleeping state. When kex calls wait_event the
current process becomes ineligible for scheduling. The three things
that can happen from this point are: the wait queue becomes awoken
by an external event that issues a wake up (often from an interrupt
handler), or a time out expires, or the process receives a signal.
In any of those cases, the kex driver code returns from wait_event.

* parameter to set the timeout
* try it with a signal
* use another process write to wake up the wait queue
