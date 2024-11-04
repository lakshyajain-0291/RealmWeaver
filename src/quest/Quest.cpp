#include "Quest.h"


Reward::Reward() {
    stats = new Stats();
    exp = 0;
    vector<Item*> items;
    this->items = items;
}

Reward::Reward(Stats *stats, int exp, vector<Item*> items) 
    : stats(stats), exp(exp), items(items) {}

Stats *Reward::getStats() const {
    return stats;
}

int Reward::getExp() const {
    return exp;
}

// vector<Item*> Reward::getItem() const {
//     return items;
// }

void Reward::displayReward() const {
    cout << "Reward Stats: " << endl;
    stats->displayStats(); // Assuming Stats class has a display method
    cout << "Experience: " << exp << endl;
    cout << "Item: "  << endl; // Assuming Item has a getName() method
    for(const auto& item : items)
    {
        cout<<item->getName()<<", ";
    }
}

// Quest class methods

// Quest::Quest() : id(0), rank(1), status("ongoing") {
//     vector<Item*> items;
//     this->reward = Reward(new Stats(), 0, items);
// }

Quest::Quest(){
    id = 0;
    rank = 1;
    status = "ongoing";
    isCompleted = false;
}


Quest::Quest(int id,const string& name, const string& description, int rank, const string &task, Reward reward, bool isUnique)
    : id(id),name(name), description(description), rank(rank), task(task), reward(reward), status("ongoing"),isUnique(isUnique) {}

int Quest::getId() const {
    return id;
}

const string& Quest::getName() const {
    return name;
}

const string& Quest::getDescription() const {
    return description;
}

const string& Quest::getStatus() const {
    return status;
}

const Reward& Quest::getReward() const {
    return reward;
}

int Quest::getRank() const {
    return rank;
}

const string& Quest::getTask() const {
    return task;
}

const string& Quest::getObjectiveDesc() const {
    return task;
}

void Quest::setStatus(const string& newStatus) {
    status = newStatus;
}

// void Quest::addObjective(int objectiveId, const string& objectiveDesc) {
//     objectives.push_back(objectiveId);
//     this->objectiveDesc.push_back(objectiveDesc);
// }

void Quest::markObjectiveComplete(int objectiveId) {
    // Logic to mark objective as completed (for now we assume objective completion will be handled elsewhere)
}

bool Quest::isQuestComplete() const {
    // Return true if all objectives are marked as complete
    // You might want to integrate a way to track which objectives are completed (e.g., flags or status per objective)
    return isCompleted;
}

void Quest::display() const {
    cout << "Quest ID: " << id << endl;
    cout << "Description: " << description << endl;
    cout << "Status: " << status << endl;
    cout << "Rank: " << rank << endl;
    cout << "Objectives:" <<task<< endl;
    cout << "Reward:" << endl;
    reward.displayReward();
}
