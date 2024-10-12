#include <Item.h>

// Constructor
Item::Item(const int rank, const string& themeName) : rank(rank)
{
    //get item from db or from gemini
    future<json> futureJson = Gemini().genItem(rank, themeName);
    json item = futureJson.get(); // Block until the result is ready
    name, description =  item["item"]["name"], item["item"]["description"];
    
    // Create a unique ID by combining name and description using std::hash
    std::hash<std::string> hasher;
    id = hasher(name + description); // Combine both name and description for uniqueness


    isEquipped=false;
    stats =new Stats();
}

Item::Item(const int& rank,const string& name, const string& description,Stats *stats)
    : name(name), description(description), rank(rank) ,isEquipped(0),stats(stats)
{
    // get id from db and add here.
    std::cout << "DB not yet formed" << endl;
    return;
}

// Getters
int Item::getId() const {
    return id;
}

const string& Item::getName() const {
    return name;
}

const string& Item::getDescription() const {
    return description;
}

int Item::getRank() const {
    return rank;
}

// Display method for item details
void Item::display() const {
    cout << "Item ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Description: " << description << endl;
    cout << "Rank: " << rank << endl;
    stats->displayStats();
    cout << "Equipped Status: "<<isEquipped<< endl;
}

void Item::equip()
{
    isEquipped=1;
    cout << "Equipped "<< getName()<< endl;

}