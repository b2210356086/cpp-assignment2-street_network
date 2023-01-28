#include <iostream>
#include <fstream>
#include "node.h"
#include "operation.h"

void addApartment(aptNode* &pAptList, char aptName, std::string position, int maxBandwidth)
{
    //Creates a new apartment and assigns given values to it.
    aptNode* pNewApt = new aptNode;
    pNewApt -> maxBandwidth = maxBandwidth;
    pNewApt -> spareBandwidth = maxBandwidth; //Spare Bandwidth equals max bandwidth at initialization
    pNewApt -> aptName = aptName;
    
    //Manipulates Links with the added apartment.
    if(position == "head") //If new apartment is head, links it to itself.
    {
        pNewApt -> pNextApt = pNewApt;
        pAptList = pNewApt;
    }
    else if(position.at(0) == 'b') //If new apartment is added "before" another apartment;
    {
        char nextAptName = position.at(7);
        aptNode* pTempApt = pAptList;
        while((pTempApt -> pNextApt) -> aptName != nextAptName) //Moves TempApt to the apartment before the apartment with nextAptName,
        {
            pTempApt = pTempApt -> pNextApt;
        } 
        if(pTempApt -> pNextApt == pAptList) //If adding new apt before head,
        {
            pNewApt -> pNextApt = pAptList;
            pTempApt -> pNextApt = pNewApt;
            pAptList = pNewApt; //New Apartment becomes the head of apartment list.
        }
        else //If adding new apt not before head,
        {
            pNewApt -> pNextApt = pTempApt -> pNextApt;
            pTempApt -> pNextApt = pNewApt; //Inserts new apt after tempApt and before the apartment with "nextAptName".
        }
    }
    else if(position.at(0) == 'a') //If new apartment is added "after" another apartment;
    {
        char prevAptName = position.at(6);
        aptNode* pTempApt = pAptList;
        while(pTempApt -> aptName != prevAptName) //Moves tempApt to the apartment with prevAptName,
        {
            pTempApt = pTempApt -> pNextApt;
        }  
        pNewApt -> pNextApt = pTempApt -> pNextApt;
        pTempApt -> pNextApt = pNewApt; //Inserts new apt after the apartment with "prevAptName".
    }
}

void addFlat(aptNode* &pAptList, char aptName, int index, int initialBandwidth, int id)
{
    //Traverses the apartments to find the correct apartment with aptName to add new flat to.
    aptNode* pTempApt = pAptList;
    while(pTempApt -> aptName != aptName)
    {
        pTempApt = pTempApt -> pNextApt;
    }

    //Creates a new flat and assigns given values to it. If initial bandwidth value exceeds the apartment's spare bandwidth value then sets the initial bandwidth to the maximum it can be which is the apartment's spare bandwidth value.
    flatNode* pNewFlat = new flatNode;
    pNewFlat -> id = id;
    if(initialBandwidth > pTempApt -> spareBandwidth)
    {
        pNewFlat -> initialBandwidth = pTempApt -> spareBandwidth;
    }
    else
    {
        pNewFlat -> initialBandwidth = initialBandwidth;
    }
    pTempApt -> spareBandwidth -= pNewFlat -> initialBandwidth; //Spare bandwidth value of the apartment decreases because a new flat is added.
    
    //Manipulates links of the new flat with other flats and the apartment.
    if(pTempApt -> pFlatList == nullptr) //If the apartment has no flats,
    {
        pTempApt -> pFlatList = pNewFlat; //New flat is set to be the apartment's doubly linked flat list's head.
    }
    else //If the apartment already has flats;
    {
        if(index == 0) //If the new flat will be the head,
        {
            pNewFlat -> pNextFlat = pTempApt -> pFlatList;
            pTempApt -> pFlatList -> pPrevFlat = pNewFlat;
            pTempApt -> pFlatList = pNewFlat; //Sets the new flat to be the head of the apartment's doubly linked flat list.
        }
        else //If the new flat will not be the head,
        {
            //Traverses the flat list until index-1 to find the tempFlat that will be before the newFlat.
            flatNode* pTempFlat = pTempApt -> pFlatList;
            for(int i = 0; i < index - 1; i++)
            {
                pTempFlat = pTempFlat -> pNextFlat;
            }
            
            pNewFlat -> pNextFlat = pTempFlat -> pNextFlat;
            if(pTempFlat -> pNextFlat != nullptr) //Checks if there exists a flat above tempFlat so that there will not be any errors when setting prev value of the above flat. (nullptr -> pPrevFlat would cause error).
            {
                pTempFlat -> pNextFlat -> pPrevFlat = pNewFlat;
            }
            pTempFlat -> pNextFlat = pNewFlat;
            pNewFlat -> pPrevFlat = pTempFlat; //Inserts the new flat in the middle of TempFlat and "the flat above TempFlat(if it exists)".
        }
    }
}

void removeFlats(aptNode* &pAptNode)
{
    if(pAptNode -> pFlatList == nullptr) //If the apartment has no flats then does nothing and returns.
    {
        return;
    }
    
    if(pAptNode -> pFlatList -> pNextFlat == nullptr) //If the apartment has a single flat then deletes that flat and returns.
    {
        delete pAptNode -> pFlatList;
        return;
    }
    
    //If the apartment has more than a single flat then traverses the flat list with 2 flat pointers, deleting the previous flat after each step.
    flatNode* pFrontFlat = pAptNode -> pFlatList;
    flatNode* pRearFlat = nullptr;
    while(pFrontFlat -> pNextFlat != nullptr)
    {
        pRearFlat = pFrontFlat;
        pFrontFlat = pFrontFlat -> pNextFlat;
        delete pRearFlat;
    }
    delete pFrontFlat; //Deletes the front flat in the end.
}

aptNode* removeApartment(aptNode* &pAptList, char aptName)
{
    if(pAptList == pAptList -> pNextApt) //If there is a single apartment in the street then removes that apartment after removing it's flats, returns NULL.
    {
        removeFlats(pAptList);
        delete pAptList;
        return NULL;
    }
    else //If there is more than one apartment in the street;
    {
        if(pAptList -> aptName == aptName) //If the head apartment is to be deleted,
        {
            //Traverses the apartment list to find the tail apartment.
            aptNode* pLastApt = pAptList;
            do
            {
                pLastApt = pLastApt -> pNextApt;
            }while(pLastApt -> pNextApt != pAptList);
            
            pLastApt -> pNextApt = pAptList -> pNextApt; //Sets tail's next to head's next.
            removeFlats(pAptList);
            delete pAptList; //Removes the head apartment.
            pAptList = pLastApt -> pNextApt; //New head becomes tail's next.
        }
        else //If the apartment to be deleted is not the head,
        {
            //Traverses the apartment list with 2 apartment pointers front and rear until front apartment matches the aptName to be deleted.
   	    aptNode* pRearApt = nullptr;
            aptNode* pFrontApt = pAptList;
            while(pFrontApt -> aptName != aptName)
            {
                pRearApt = pFrontApt;
                pFrontApt = pFrontApt -> pNextApt;
            }
            
	    pRearApt -> pNextApt = pFrontApt -> pNextApt; //Connects rear apartment to the next of front apartment.
            removeFlats(pFrontApt);
            delete pFrontApt; //Removes the front apartment.
        }
        return pAptList; //Returns the apartment list after removals are done.
    }  
}

void makeFlatEmpty(aptNode* &pAptList, char aptName, int id)
{
    //Traverses firstly the apartments then the flats to find the flat with given info to be made empty.
    aptNode* pTempApt = pAptList;
    while(pTempApt -> aptName != aptName)
    {
        pTempApt = pTempApt -> pNextApt;
    }
    flatNode* pTempFlat = pTempApt -> pFlatList;
    while(pTempFlat -> id != id)
    {
        pTempFlat = pTempFlat -> pNextFlat;
    }
    
    pTempFlat -> isEmpty = 1; //Sets the flat to empty.
    pTempApt -> spareBandwidth += pTempFlat -> initialBandwidth; //Increments the apartment's spareBandwidth value by the amount of the flat's initialBandwidth value.
    pTempFlat -> initialBandwidth = 0; //Sets the flat's initialBandwidth value to 0.
}

int findSumOfMaxBandwidths(aptNode* &pAptList)
{
    aptNode* pTempApt = pAptList;
    if(pTempApt == nullptr) //If there are no apartments in the street then returns 0.
    {
        return 0;
    }
    //Traverses all the apartments in the street, adds their max bandwidth values to "sum" and then returns "sum".
    int sum = 0;
    do
    {
        sum += pTempApt -> maxBandwidth;
        pTempApt = pTempApt -> pNextApt;
    }while(pTempApt != pAptList);
    return sum;
}

aptNode* mergeTwoApartments(aptNode* &pAptList, char aptNameDest, char aptNameSrc)
{
    //Traverses the apartments to find the Source and Destination apartments.
    aptNode* pAptDest = pAptList;
    aptNode* pAptSrc = pAptList;
    while(pAptDest -> aptName != aptNameDest)
    {
        pAptDest = pAptDest -> pNextApt;
    }
    while(pAptSrc -> aptName != aptNameSrc)
    {
        pAptSrc = pAptSrc -> pNextApt;
    }
    
    pAptDest -> maxBandwidth += pAptSrc -> maxBandwidth; //Destination apartment's maxBandwidth value is incremented by the amount of Source apartment's maxBandwidth value.
    pAptDest -> spareBandwidth += pAptSrc -> spareBandwidth; //The same but with spareBandwidth values this time.
    
    flatNode* pTempFlat = pAptDest -> pFlatList;
    if(pAptDest -> pFlatList == nullptr) //If the Destination apartment has no flats then it's flat list is set to the Source apartment's flat list.
    {
        pAptDest -> pFlatList == pAptSrc -> pFlatList;
    }
    else //If the Destination apartment has flats,
    {
        //Traverses until the last flat of the destination apartment.
	while(pTempFlat -> pNextFlat != nullptr)
        {
            pTempFlat = pTempFlat -> pNextFlat;
        }
        
        if(pAptSrc -> pFlatList != nullptr) //If the source apartment also has flats,
        {
            pTempFlat -> pNextFlat = pAptSrc -> pFlatList; //Next of the last flat of the destination apartment is set to the flat list of source apartment.
            pAptSrc -> pFlatList -> pPrevFlat = pTempFlat; //Prev of the flat list of source apartment is set to the last flat of destination apartment.
            pAptSrc -> pFlatList = nullptr; //Removes the connection of source apartment with it's flat list.
        }
    }
    removeApartment(pAptList, aptNameSrc); //Removes the Source apartment.
    return pAptList; //Returns the apartment list after merging is done.
}

void relocateFlatsToSameApartments(aptNode* pAptList, char aptName, int shiftID, int relocateID[], int size)
{
    //Traverses the apartments to find the Destination apartment with given aptName.
    aptNode* pDestApt = pAptList;
    while(pDestApt -> aptName != aptName)
    {
        pDestApt = pDestApt -> pNextApt;
    }
    //Traverses the flats of the destination apartment to find the flat with given shiftID.
    flatNode* pShiftFlat = pDestApt -> pFlatList;
    while(pShiftFlat -> id != shiftID)
    {
        pShiftFlat = pShiftFlat -> pNextFlat;
    }
    
    for(int i = 0; i < size; i++) //For loop that runs for every flat to be relocated.
    {
        //Traverses all the apartments and all the flats until the flat to be relocated is found.
        aptNode* pTempApt = pAptList;
        flatNode* pTempFlat = pTempApt -> pFlatList;
        bool isFound = false; //Condition to check if the flat to be relocated is found yet.
        while(!isFound)
        {
            while(pTempFlat -> id != relocateID[i] && pTempFlat -> pNextFlat != nullptr) //Goes through all the flats of an apartment until either the flat to be relocated is found or no more flats are left in the apartment.
            {
                pTempFlat = pTempFlat -> pNextFlat;
            }
            if(pTempFlat -> id == relocateID[i])
            {
                isFound = true;
            }
            else
            {
                pTempApt = pTempApt -> pNextApt; //Go to the next apartment if the flat to be relocated was not found in the current apartment.
                pTempFlat = pTempApt -> pFlatList;
            }
        }
        
	//Change the maxBandwidth values of the apartments according to the initialBandwidth value of the flat to be relocated.
        pTempApt -> maxBandwidth -= pTempFlat -> initialBandwidth;
        pDestApt -> maxBandwidth += pTempFlat -> initialBandwidth;
        
        if(pTempFlat == pTempApt -> pFlatList) //If the head flat of a source apartment is to be relocated,
        {
            pTempApt -> pFlatList = pTempFlat -> pNextFlat; //That apartment's flat list is set to the next of the flat to be relocated,
            if(pTempFlat -> pNextFlat != nullptr) //If that next is not nullptr and actually points to a flat,
            {
                pTempFlat -> pNextFlat -> pPrevFlat = nullptr; //Then that flat's prev is set to nullptr.
            }
        }
        else //If the flat to be relocated is not a head flat then it's next and prev flats are linked.
        {
            pTempFlat -> pPrevFlat -> pNextFlat = pTempFlat -> pNextFlat;
            if(pTempFlat -> pNextFlat != nullptr)
            {
                pTempFlat -> pNextFlat -> pPrevFlat = pTempFlat -> pPrevFlat;
            }
        }
        
        if(pShiftFlat == pDestApt -> pFlatList) //If the head of the flat list of the destination apartment is to be shifted then relocated flat links to that head flat and destination apartment's flat list becomes relocated flat.
        {
            pTempFlat -> pNextFlat = pShiftFlat;
            pShiftFlat -> pPrevFlat = pTempFlat;
            pDestApt -> pFlatList = pTempFlat;
        }
        else //If the flat to be shifted is not the head flat then relocated flat is inserted between shift flat and it's prev flat.
        {
            pTempFlat -> pNextFlat = pShiftFlat;
            pShiftFlat -> pPrevFlat -> pNextFlat = pTempFlat;
            pTempFlat -> pPrevFlat = pShiftFlat -> pPrevFlat;
            pShiftFlat -> pPrevFlat = pTempFlat;
        }
    }
}

void listApartments(std::ofstream &outputFile, aptNode* &pAptList)
{
    aptNode* pTempApt = pAptList;
    if(pTempApt == nullptr) //If there are no apartments in the street then outputs "There are no apartments.".
    {
        outputFile << "There are no apartments.\n";
        return;
    }
    
    //If there are apartments in the street then outputs them one by one with their max bandwidth values, flat's and flats' initial bandwidth values.
    do
    {
        outputFile << pTempApt -> aptName << '(' << pTempApt -> maxBandwidth << ')';
        flatNode* pTempFlat = pTempApt -> pFlatList;
        while(pTempFlat != nullptr)
        {
            outputFile << "\tFlat" << pTempFlat -> id << '(' << pTempFlat -> initialBandwidth << ')';
            pTempFlat = pTempFlat -> pNextFlat;
        }
        outputFile << '\n';
        pTempApt = pTempApt -> pNextApt;
    }while(pTempApt != pAptList);
    outputFile << '\n';
}