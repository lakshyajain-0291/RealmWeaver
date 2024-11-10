#include <Stats.h>

// Static variable definition
unordered_map<string, int> Stats::defaultStats;

// Constructor that initializes stats based on theme
Stats::Stats() {
    // If defaultStats is empty, it's the first time this is being called
    if (defaultStats.empty()) {
        // Simulate getting a dynamic stat structure from Gemini API
        future<json> statResponseFuture = Gemini().genStatsForTheme(); // replace with your theme variable
        
        // Wait for the result from the asynchronous call
        json statResponse = statResponseFuture.get();
        // std::cout << "Stats JSON data: " << statResponse.dump(4) << std::endl; // Print formatted JSON

        // Parse the JSON response and populate the default stats
        if (statResponse.contains("attributes") && statResponse["attributes"].is_array()) 
        {
            for (const auto& stat : statResponse["attributes"]) 
            {
                for (const auto& [statName, statValue] : stat.items()) 
                {
                    // Assuming all stat values are integers
                    int value = statValue.get<int>();

                    // Store the stat in defaultStats
                    defaultStats[statName] = value;
                }
            }
        } else {
            std::cerr << "Invalid response format: 'attributes' not found or is not an array." << std::endl;
        }
    }


    attributes = defaultStats;
}

Stats::Stats(const int& rank, const string &npcName, const string &npcBackStory) {
    // If defaultStats is empty, it's the first time this is being called
    if (defaultStats.empty()) {
        // Simulate getting a dynamic stat structure from Gemini API
        future<json> statResponseFuture = Gemini().genStatsForTheme(); // replace with your theme variable
        
        // Wait for the result from the asynchronous call
        json statResponse = statResponseFuture.get();
        // std::cout << "Stats JSON data: " << statResponse.dump(4) << std::endl; // Print formatted JSON

        // Parse the JSON response and populate the default stats
        if (statResponse.contains("attributes") && statResponse["attributes"].is_array()) 
        {
            for (const auto& stat : statResponse["attributes"]) 
            {
                for (const auto& [statName, statValue] : stat.items()) 
                {
                    // Assuming all stat values are integers
                    int value = statValue.get<int>();

                    // Store the stat in defaultStats
                    defaultStats[statName] = value;
                }
            }
        } else {
            std::cerr << "Invalid response format: 'attributes' not found or is not an array." << std::endl;
        }
    }

    future<json> futureResp = Gemini().genNPCStats(rank,npcName,npcBackStory);

    try 
    {
        json statResponse = futureResp.get(); // Block until the result is ready
        // std::cout << "NPC STATS JSON data: " << statResponse.dump(4) << std::endl; // Print formatted JSON

        for (auto& stat : defaultStats) 
        {
            auto& statName = stat.first;        // Access first element of the pair
            auto& defaultValue = stat.second;   // Access second element of the pair


            // Check if the statName exists in the response from Gemini and if the value is valid
            if (statResponse.contains(statName) && statResponse[statName].is_number()) 
            {
                // Try to get the stat value from the response
                try 
                {
                    int statValue = statResponse[statName].get<int>();
                    attributes[statName] = statValue;  // Store the valid stat value
                } 
                catch (const std::exception& e) 
                {
                    std::cerr << "Error parsing stat value for " << statName << ": " << e.what() << std::endl;
                    attributes[statName] = defaultValue;  // Fallback to default value in case of error
                }
            } 
            else 
            {
                // If the stat is missing or invalid, use the default value
                attributes[statName] = defaultValue;
            }
        }
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl; // Handle exceptions
    }    
}

Stats::Stats(int n) {
    for (const auto& stat : defaultStats) {
        attributes[stat.first] = stat.second * n; // Multiply default stats by n
    }
}

// Set a specific stat
void Stats::setStat(const string& statName, int value) {
    attributes[statName] = value;
}

// Modify a specific stat by a delta
void Stats::modifyStat(const string& statName, int delta) {
    if (attributes.find(statName) != attributes.end()) {
        attributes[statName] += delta;
    } else {
        cout << "Stat " << statName << " not found!" << endl;
    }
}

// Get the value of a specific stat
int Stats::getStat(const string& statName) const {
    auto it = attributes.find(statName);
    if (it != attributes.end()) {
        return it->second;
    }
    cout << "Stat " << statName << " not found!" << endl;
    return 0; // Default value if stat not found
}

// Check if a specific stat exists
bool Stats::hasStat(const string& statName) const {
    return attributes.find(statName) != attributes.end();
}

// Get all attributes
unordered_map<string, int> Stats::getAllStats() const {
    return attributes;
}

// Dynamically add a stat (if it was not created initially)
void Stats::addStat(const string& statName, int initialValue) {
    attributes[statName] = initialValue;
}

// Remove a stat
void Stats::removeStat(const string& statName) {
    attributes.erase(statName);
}

// Display all attributes
void Stats::displayStats() const {
    cout << "Player Stats:" << endl;
    for (const auto& stat : attributes) {
        const auto& statName = stat.first;   // Access the key (stat name)
        const auto& value = stat.second;     // Access the value (stat value)
        
        cout << statName << ": " << value << endl;
    }
}


// Check if a stat is below a certain threshold
bool Stats::isStatBelow(const string& statName, int threshold) const {
    if (attributes.find(statName) != attributes.end()) {
        return attributes.at(statName) < threshold;
    }
    return false; // Return false if stat doesn't exist
}

// Check if a stat is above a certain threshold
bool Stats::isStatAbove(const string& statName, int threshold) const {
    if (attributes.find(statName) != attributes.end()) {
        return attributes.at(statName) > threshold;
    }
    return false; // Return false if stat doesn't exist
}

void Stats::giveRewardStats(const Stats& rewardStats)
{
    for(const auto& stat : rewardStats.getAllStats())
    {
        attributes[stat.first]+=stat.second;
    }
}
