# Profiling Your Code

Before starting, make sure that `main.cpp` returns. The program must exit cleanly in order for the profiling tools to work. To ensure this, change the `return` statement in `main.cpp` to `return 0;` rather than `return app.exec();` 


There are a couple of ways to profile GRIF:
## Profilers
  * GNU prof
  * Callgrind (Valgrind)



### GNU Prof
First, download GNU prof (duh).

Make sure you add these lines to the .pro file of your project:

    QMAKE_CXXFLAGS_DEBUG += -pg
    QMAKE_LFLAGS_DEBUG += -pg
    
Next, navigate to the Project tab in Qt Creator, and make sure you are in the Debug build Configuration.

Now compile and run your project. In the Build directory, you should see a file called gmon.out

All you have to do now is execute `gprof *executable* > output`
Then check the output file in any text editor of your choice!


### Callgrind

All you need to do is run:

    valgrind --tool=callgrind ./yourapp
    
This produces a file named `callgrind.out.<pid>` which you can analyze using KCacheGrind (the text file is a little cryptic).