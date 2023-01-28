void addApartment(aptNode* &pAptList, char aptName, std::string position, int maxBandwidth); //Adds a new apartment at given position

void addFlat(aptNode* &pAptList, char aptName, int index, int initialBandwidth, int id); //Adds a new flat at given index

void removeFlats(aptNode* &pAptNode); //Helper function for removeApartment that removes all flats in an apartment

aptNode* removeApartment(aptNode* &pAptList, char aptName); //Removes the apartment with given name after removing it's flats

void makeFlatEmpty(aptNode* &pAptList, char aptName, int id); //Sets the ID given flat Empty, also sets initial bandwidth of that flat to 0

int findSumOfMaxBandwidths(aptNode* &pAptList); //Finds the sum of max bandwidth of entire street (all apartments)

aptNode* mergeTwoApartments(aptNode* &pAptList, char aptNameDest, char aptNameSrc); //Merges Source apartment's flats to Destination apartment's flats

void relocateFlatsToSameApartments(aptNode* pAptList, char aptName, int shiftID, int relocateID[], int size); //Relocates flats with given id list below another flat with shiftID given

void listApartments(std::ofstream &outputFile, aptNode* &pAptList); //Lists the street with all the apartments,flats and bandwidth values