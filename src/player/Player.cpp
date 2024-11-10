#include<Player.h>

Player::Player(string name,int stlvl):name(name),lvl(stlvl),exp(0),rank(1), currentLocation(nullptr), stats(new Stats()){}

// Getters
string Player::getName() const {
    return name;
}

int Player::getLevel() const {
    return lvl;
}

int Player::getExperiencePoints() const {
    return exp;
}

int Player::getRank() const {
    return rank;
}

Location* Player::getCurrentLocation() const {
    return currentLocation;
}

vector<Item> Player::getInventory() const {
    return inventory;
}

// Setters
void Player::setCurrentLocation(Location* loc) {
    currentLocation = loc;
}

void Player::addItem(const Item& item) {
    inventory.push_back(item);
    std::cout << item.getName() << " added to inventory." << std::endl;
}

void Player::removeItem(const Item& item)
{
    auto it = remove_if(inventory.begin(),inventory.end(),[&](const Item &i)
    {
        return i.getId()==item.getId();
    });

    if(it != inventory.end())
    {
        std::cout << item.getName() << " removed from inventory." << std::endl;
        inventory.erase(it, inventory.end());
    }
    else
    {
        std::cout << "Item not found in inventory." << std::endl;
    }

}

void Player::displayInventory() const 
{
    std::cout << "Inventory:" << std::endl;
    for (const auto& item : inventory)
    {
        std::cout << " - " << item.getName() << " (Rank: " << item.getRank() << ")" << std::endl;
    }
}

void Player::equipItem(Item& item) 
{
    item.equip();//function to add stats in player.
    std::cout << "Equipped " << item.getName() << "." << std::endl;
}

void Player::addQuest(const Quest& quest) 
{
    ongoingQuests[quest.getId()] = quest;
    std::cout << "Quest added: " << quest.getDescription() << std::endl;
}

void Player::completeQuest(int questId) 
{
    auto it = ongoingQuests.find(questId);
    if (it != ongoingQuests.end()) 
    {
        it->second.setStatus("completed");
        std::cout << "Quest completed: " << it->second.getDescription() << std::endl;
        ongoingQuests.erase(it); // Remove from ongoing quests
    } 
    else 
    {
        std::cout << "Quest not found." << std::endl;
    }
}

void Player::displayOngoingQuests() const {
    std::cout << "Ongoing Quests:" << std::endl;
    for (const auto& pair : ongoingQuests) {
        std::cout << " - " << pair.second.getDescription() << " (Status: " << pair.second.getStatus() << ")" << std::endl;
    }
}

// Player actions
void Player::travel(Location* destination) 
{
    if (destination) {
        currentLocation = destination;
        std::cout << name << " traveled to " << currentLocation->getName() << "." << std::endl;
    } else {
        std::cout << "Invalid destination." << std::endl;
    }
}

void Player::gainExperience(int points) {
    addExp(points);
    // Add the logic to lvl up if the exp crosses a threshhold
    // Add the logic to rank up if lvl crosses a threshhold

    std::cout << name << " gained " << points << " experience points." << std::endl;
}

void Player::rankUp() 
{
    // Example rank-up logic
    rank++;
    std::cout << name << " ranked up to rank " << rank << "!" << std::endl;
}

void Player::lvlUp() {
    lvl++;
    std::cout << name << " leveled up to level " << lvl << "!" << std::endl;
}

void Player::addExp(int points) {
    exp += points;
    // Check for level up condition (arbitrary example)
    if (exp >= 100 * lvl) { // Example threshold for leveling up
        lvlUp();
    }
    // You can also implement rank up conditions here if needed
}

void Player::interact(const NPC* npc) {
    // Example interaction with NPC
    std::cout << name << " interacts with " << npc->getName() << "." << std::endl;
    // You can add more complex interactions here
}

void Player::displayStats() {
    std::cout << "Player Stats:" << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Level: " << lvl << std::endl;
    std::cout << "Experience Points: " << exp << std::endl;
    std::cout << "Rank: " << rank << std::endl;
    stats->displayStats(); // Assuming Stats class has a display method
    std::cout << "Inventory: ";
    displayInventory();
    std::cout  << std::endl;

}


