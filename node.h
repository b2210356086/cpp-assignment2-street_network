//Struct for Flat Doubly Linked List nodes
struct flatNode
{
    int id;
    int initialBandwidth;
    bool isEmpty;
    flatNode* pNextFlat = nullptr;
    flatNode* pPrevFlat = nullptr;
};

//Struct for Apartment Circular Linked List nodes
struct aptNode
{
    flatNode* pFlatList = nullptr;
    int maxBandwidth;
    int spareBandwidth; //spareBandwidth value is held as an extra to control exceeding maxBandwidth values when a new flat is added.
    char aptName;
    aptNode* pNextApt = nullptr;
};