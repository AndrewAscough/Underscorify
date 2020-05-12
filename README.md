# Underscorify
Have you ever thought your C++ code was **TOO** readable? Does this program ever provide the solution for you!
Simply feed it any c++ source files and watch as it transforms your original beautiful masterpiece of work into a pile of unreadable lines.

## A Nice Before and After
![Alt text](https://github.com/AndrewAscough/Underscorify/blob/master/images/before.png "Before")
![Alt text](https://github.com/AndrewAscough/Underscorify/blob/master/images/after.png "After")
## Running the Program
To use the program run ./compile then run ./Underscorify [filename 1] .. [filename n]

It will auto generate an output directory called "underscorified" and a similar directory structure to those of the files passed in (see ./compile for an example)

If there are multiple separate files make sure to include all of their names when calling ./Underscorify so that they all get linked to the same Underscores.h file.