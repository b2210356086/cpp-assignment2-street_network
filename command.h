/*command function reads the input file line by line, at each line decides at which operation function will be called with what arguments. This decision is made by splitting the line according to '\t'(tab) characters.
For example: if input file has a line that starts with "remov", that line will go into the if block of "remov". Then removeApartment() function will be called with correct arguments in that if block.
First five characters of a line are used to classify the operations(remov = removeApartment). This is because all 8 operations have different first 5 characters. If there are new operations to be added, for example "removeStreet",
it can be done easily by changing the classifier to be the entire operation word(removeApartment instead of remov) in the code.
command function also deletes the entire street to prevent memory leaks when the program is done.*/

void command(std::ifstream &inputFile, std::string line, std::ofstream &outputFile, aptNode* &pAptList);