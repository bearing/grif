#ifndef GRITHREAD_H
#define GRITHREAD_H

// Generic thread class for encapsulation
// Usage:
// myThread workThread;
//  myWorker workObject;

//  // move the worker objects to the new thread
//  workObject.moveToThread(&workThread);

//  // Start the work thread
//  workThread.start();

class GRIThread : public QThread
{
   public:
      void run () { exec (); }
};




#endif // GRITHREAD_H
