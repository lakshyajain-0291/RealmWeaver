#include "NPC.h"

// Constructor that generates NPC dynamically from Gemini
NPC::NPC(int rank,Location &loc) : rank(rank),loc(&loc) {
    // Generate name and backstory via Gemini API
    future<json> futureResp = Gemini().genNPC(rank,loc.getName(),loc.getDesc());
    
    try
    {
        json resp = futureResp.get();
        std::cout << "NPC JSON data: " << resp.dump(4) << std::endl; // Print formatted JSON

        this->name = resp["npc"]["name"];
        this->backStory = resp["npc"]["backstory"];
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl; // Handle exceptions
    }

    // Generate dynamic stats based on rank or theme
    this->stats = new Stats(rank,name,backStory);
    // Generate NPC quests
    generateQuests();
}

// Constructor with manually assigned values
NPC::NPC(int rank, const std::string& name, const std::string& backStory, Location* loc)
    : rank(rank), name(name), backStory(backStory), loc(loc){
    // Generate dynamic stats for NPC manually based on rank or theme
    this->stats = new Stats(rank,name,backStory);
}

// Generate initial quests for NPC (from Gemini API or DB)
void NPC::generateQuests() 
{
    future<json> futureResp = Gemini().genQuests(rank,name,backStory);

    try
    {
        json questData = futureResp.get();
        std::cout << "Quest data: " << questData.dump(4) << std::endl; // Print formatted JSON

        // Check if "quests" is an array in the JSON
        if (questData.contains("quests") && questData["quests"].is_array()) 
        {
            for (const auto& quest : questData["quests"]) {

                const auto& questDetails= quest["quest_details"];
                int questId=quest["quest_id"];


                // Extract quest details
                std::string name = questDetails["name"];
                std::string description = questDetails["description"];
                int rank = questDetails["rank"];
                std::string task = questDetails["task"];
                bool isUnique = questDetails["unique"];
                
                //get Reward
                int expReward = questDetails["reward"].contains("exp") ? int(questDetails["reward"]["exp"]):0;                
                Stats *rewardStats;

                if (questDetails["reward"].contains("statsAdder")) 
                {
                    rewardStats=new Stats(questDetails["reward"]["statsAdder"]);
                }

                vector<Item*> items;
                if (questDetails["reward"].contains("item") && questDetails["reward"]["item"].is_array()) 
                {
                    for (const auto& itemJson : questDetails["reward"]["item"]) 
                    {
                        Stats itemStats(itemJson["statsAdder"]);
                        Item *newItem= new Item(itemJson["rank"],itemJson["name"],itemJson["desc"],&itemStats);

                        items.push_back(newItem);
                    }
                }
                
                Reward questReward(rewardStats,expReward, items);

                // Create the new quest and add it
                Quest* newQuest = new Quest(questId,name, description, rank, task, questReward,isUnique);
                addQuest(newQuest);
            }
        } 
        else 
        {
            std::cerr << "Invalid quest data received from Gemini API.\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl; // Handle exceptions
    }
}

// Get the NPC's rank
int NPC::getRank() const {
    return rank;
}

// Get the NPC's name
std::string NPC::getName() const {
    return name;
}

// Get the NPC's backstory
std::string NPC::getBackStory() const {
    return backStory;
}


std::string NPC::getLocName() const {
    return (*loc).getName();
}

std::string NPC::getLocDesc() const {
    return (*loc).getDesc();
}

std::string NPC::getLocthemeName() const {
    return (*loc).getthemeName();
}

int NPC::getQuestId() const {
    if (!quests.empty())
    return quests.front()->getId();
    else return -1;
}

std::string NPC:: getQuestName() const {
    if (!quests.empty())
    return quests.front()->getName();
    else return "";
}

std::string NPC:: getQuestDescription() const{
    if (!quests.empty())
    return quests.front()->getDescription();
    else return "";
}


// Get the NPC's stats
Stats* NPC::getStats() const {
    return stats;
}

// Add a quest to the NPC's quest queue
void NPC::addQuest(Quest* quest) {
    quests.push(quest);
}

// Remove the first quest from the queue (when completed)
void NPC::removeQuest() {
    if (!quests.empty()) {
        quests.pop();
    }
}

// Get the current quest offered by NPC
Quest* NPC::getCurrentQuest() const {
    if (!quests.empty()) {
        return quests.front();
    }
    return nullptr;
}

// Check if NPC has any quests available
bool NPC::hasQuests() const {
    return !quests.empty();
}

// Display NPC's basic info
void NPC::displayNPCInfo() const {
    std::cout << "NPC Name: " << name << std::endl;
    std::cout << "NPC Rank: " << rank << std::endl;
    std::cout << "NPC Backstory: " << backStory << std::endl;
}

// Display all available quests
void NPC::displayQuests() const {
    if (!quests.empty()) {
        std::cout << "NPC has " << quests.size() << " quests available." << std::endl;
    } else {
        std::cout << "No quests available for this NPC." << std::endl;
    }
}

// Interaction logic for talking to the NPC
void NPC::interact() {
    std::cout << "Interacting with NPC: " << name << std::endl;
    // Display available quests
    displayQuests();
    // You can add dialogue options or more interactions here
}
