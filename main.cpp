#include <iostream>
#include <fstream>
#include "node.h"
#include "command.h"

int main(int argc, char* argv[])
{
    aptNode* pAptList = nullptr; //Apartment Doubly Linked List initialized with null(No apartments).
    std::ofstream outputFile(argv[2]); //Output file is created.
    std::string line; //Helper for Input file reading (getline() function).
    std::ifstream inputFile(argv[1]); //Input file is created.
    
    if(inputFile.is_open())
    {
        command(inputFile, line, outputFile, pAptList); //command function is called with these arguments to read the input file and perform the operations on the Apartment Doubly Linked List. It also deletes the street to prevent memory leaks when the program is done. 
    }
    
    if(outputFile.is_open())
    {
        outputFile.close();
    }
    return 0;
}

/*Assignment 2
Semih Can Yavuz
b2210356086*/