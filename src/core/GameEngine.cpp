#include <external.h>
#include <GameEngine.h>

// Initialize the constant `theme` variable
const std::string theme = []() {
    std::string input;
    std::cout << "Enter a theme: ";
    getline(std::cin, input);
    return input;
}();


int main()
{
    // cout<<"MAIN FILE RUNNING\n\n";

    int rank;

    cout << "Enter the Rank: ";
    cin>>rank;
    
    // currently generating 1 NPC for location
    Location loc(rank, theme);
    cout << endl;
    cout << endl;
    cout << "Location Name: " << loc.getName() << endl;
    cout << "Location Description: " << loc.getDesc() << endl;
    
    return 0;
}

string GameEngine::getRankName(int rank) {
    switch (rank) {
        case 1: return "Inferior";
        case 2: return "Basic";
        case 3: return "Common";
        case 4: return "Uncommon";
        case 5: return "Rare";
        case 6: return "Unique";
        case 7: return "Epic";
        case 8: return "Legendary";
        case 9: return "Mythic";
        case 10: return "Divine";
        default: return "Unknown Rank";  // For invalid rank numbers
    }
}
