#ifndef common_h
#define common_h

// keep one direction of data flow on one FIFO

// from server to client
#define FIFO_SERVER_TO_CLIENT "/tmp/FIFO_SERVER_TO_CLIENT"

// from client to server, create another direction if needed
// #define FIFO_CLIENT_TO_SERVER "/tmp/FIFO_CLIENT_TO_SERVER"

#endif
