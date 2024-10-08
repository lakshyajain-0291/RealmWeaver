#pragma once

#include <external.h>
#include <Item.h>
#include <Stats.h>
#include <vector>
#include <string>

class Stats;
class Reward {
private:
    Stats stats;     // Stat reward (can increase health, attack, etc.)
    int exp;         // Experience points as a reward
    Item item;       // Item reward

public:
    // Constructor
    Reward(Stats stats, int exp, Item item);
    
    // Getters
    Stats getStats() const;
    int getExp() const;
    Item getItem() const;
    
    // Display Reward details
    void displayReward() const;
};

class Quest {
private:
    int id;                      // Unique identifier for the quest
    string description;          // Description of the quest
    string status;               // Status of the quest ("ongoing", "completed", "failed", etc.)
    Reward reward;               // Reward for the quest (stats, exp, items)
    int rank;                    // Difficulty rank of the quest
    vector<int> objectives;      // List of objective IDs
    vector<string> objectiveDesc;// Descriptions for each objective

    // Private helper for reward generation
    Reward genReward();

public:
    // Constructors
    Quest();  // Default constructor (could be used for creating quests from the DB)
    Quest(int id, const string& description, int rank, const vector<int>& objectives, Reward reward); // Construct from parameters

    // Getters
    int getId() const;
    const string& getDescription() const;
    const string& getStatus() const;
    const Reward& getReward() const;
    int getRank() const;
    const vector<int>& getObjectives() const;
    const vector<string>& getObjectiveDesc() const;

    // Setters
    void setStatus(const string& newStatus);

    // Add methods
    void addObjective(int objectiveId, const string& objectiveDesc); // Add objective ID with description
    void markObjectiveComplete(int objectiveId);                      // Mark an objective as completed
    bool isQuestComplete() const;                                    // Check if all objectives are completed

    // Display details
    void display() const;
};
