#pragma once
#include <external.h>
#include <Item.h>

//ADD PUNISHMENTS IF NEEDED..
class Stats;
class Item;
class Quest;
class Reward {
private:
    Stats *stats;     // Stat reward (can increase health, attack, etc.)
    int exp;         // Experience points as a reward
    vector<Item*> items;       // Item reward

public:
    // Constructor
    Reward();
    Reward(Stats *stats, int exp, vector<Item*> items);
    
    // Getters
    Stats *getStats() const;
    int getExp() const;
    Item *getItem() const;
    
    // Display Reward details
    void displayReward() const;
};

class Quest {
private:
    int id;                      // Unique identifier for the quest
    string name;
    string description;          // Description of the quest
    string status;               // Status of the quest ("ongoing", "completed", "failed", etc.)
    Reward reward;               // Reward for the quest (stats, exp, items)
    int rank;                    // Difficulty rank of the quest
    string task;// Descriptions for each objective
    bool isCompleted;
    bool isUnique;

    // Private helper for reward generation
    Reward genReward();

public:
    // Constructors
    Quest();  // Default constructor (could be used for creating quests from the DB)
    Quest(int id, const string& name, const string& description, int rank, const string& task, Reward reward,bool isUnique); // Construct from parameters

    // Getters
    int getId() const;
    const string& getName() const;
    const string& getDescription() const;
    const string& getStatus() const;
    const Reward& getReward() const;
    const string& getTask() const;
    
    int getRank() const;
    const string& getObjectiveDesc() const;

    // Setters
    void setStatus(const string& newStatus);

    // Add methods
    // void addObjective(int objectiveId, const string& objectiveDesc); // Add objective ID with description
    void markObjectiveComplete(int objectiveId);                      // Mark an objective as completed
    bool isQuestComplete() const;                                    // Check if all objectives are completed

    // Display details
    void display() const;
};

