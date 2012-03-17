# Networking Example

## Important Classes
  * GRIServer.h
  * TcpDAQThread
  * TcpAnalysisThread
    

### GRIServer.h
Listens on LocalHost by default, port 8081, but this should be changed into an option in the Configuration file. The default implementation is that everything that is written to the Server gets written to all the sockets connected to it. This is good for simple applications and on the off chance that you need to access to the written data in the DAQ thread, but easy to change by altering the inner class' `void readAndWriteData()` to only write to those sockets that haven't written to the server (i.e. sockets that are from Analysis threads, not from DAQ threads)

### TcpDAQThread
Simple implementation of a DAQ thread that uses a `QTcpSocket` to connect to and write `char[]` to the Server, the Server then reroutes this to the analysis thread. 

### TcpAnalysisThread
Again, a basic implementation that listens on a `QTcpSocket`, it uses a blocking call in its own thread to wait until there's something to read. You can change this by connecting the `readyRead()` signal to your own slot, and then do the analysis in that slot. Of course, a blocking call in this thread doesn't affect the rest of the application so it's safe to use.