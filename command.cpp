#include <fstream>
#include <iostream>
#include "node.h"
#include "operation.h"
#include "command.h"

void command(std::ifstream &inputFile, std::string line, std::ofstream &outputFile, aptNode* &pAptList)
{
    while(getline(inputFile, line))
    {
        std::string command = line.substr(0, 5);
        if(command == "add_a")
        {   
            char aptName = line.at(14);
            line = line.substr(16, line.length());
            int index = line.find('\t');
            std::string position = line.substr(0, index);
            int maxBandwidth = std::stoi(line.substr(index + 1, line.length()));
            addApartment(pAptList, aptName, position, maxBandwidth);
        }
        else if(command == "add_f")
        {
            char aptName = line.at(9);
            line = line.substr(11, line.length());
            int index = line.find('\t');
            int position = std::stoi(line.substr(0, index));
            line = line.substr(index + 1, line.length());
            index = line.find('\t');
            int initialBandwidth = std::stoi(line.substr(0, index));
            int id = std::stoi(line.substr(index + 1, command.length()));
            addFlat(pAptList, aptName, position, initialBandwidth, id);
        }
        else if(command == "remov")
        {
            removeApartment(pAptList, line.at(17));
        }
        else if(command == "make_")
        {
            char aptName = line.at(16);
            int id = std::stoi(line.substr(17, line.length()));
            makeFlatEmpty(pAptList, aptName, id);
        }
        else if(command == "find_")
        {
            outputFile << "sum of bandwidth: " << findSumOfMaxBandwidths(pAptList) << "\n\n";
        }
        else if(command == "merge")
        {
            mergeTwoApartments(pAptList, line.at(21), line.at(23));
        }
        else if(command == "reloc")
        {
            char aptName = line.at(33);
            line = line.substr(35, line.length());
            int index = line.find('\t');
            int shiftID = std::stoi(line.substr(0, index));
            line = line.substr(index + 2, line.length() - (index + 3)); //Gets rid of "[" and "]" characters. Only flat ID's and commas are left.
            
	    //ID's of the flats to be relocated are held in an int array "relocateID[]". Size of the array is "number of commas + 1".
	    int size = 0;
            for(char c:line)
            {
                if (c == ',')
                {
                    size++;
                }
            }
            int relocateID[size+1];
            for(int i = 0; i < size; i++)
            {
                int index = line.find(',');
                relocateID[i] = std::stoi(line.substr(0, index));
                line = line.substr(index + 1, line.length());
            }
            relocateID[size] = std::stoi(line);
            
	    relocateFlatsToSameApartments(pAptList, aptName, shiftID, relocateID, size+1);
        }
        else if(command == "list_")
        {
            listApartments(outputFile, pAptList);
        }
    }
    
    //Deletes the entire street to prevent memory leaks after the program is done.
    aptNode* pRearApt = nullptr;
    aptNode* pFrontApt = pAptList;
    if(pFrontApt != nullptr) //If the street is not empty(There is at least 1 apartment).
    {
        do
        {
            pRearApt = pFrontApt;
            pFrontApt = pFrontApt -> pNextApt;
            removeApartment(pAptList, pRearApt -> aptName);
        }while(pFrontApt != pAptList);
        removeApartment(pAptList, pFrontApt -> aptName);
    }

    inputFile.close();
}