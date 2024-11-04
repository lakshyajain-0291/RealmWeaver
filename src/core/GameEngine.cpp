#include <external.h>
#include <GameEngine.h>
#include <Player.h>
#include <InteractionSystem.h>

// Initialize the constant `theme` variable
const std::string theme = []() {
    std::string input;
    std::cout << "Enter a theme: ";
    getline(std::cin, input);
    return input;
}();

void testInteraction(std::string theme) {
    // Create a player
    Player player("Hero", 5);  // Name, level

    // Create an NPC
    Location loc  = Location(3, theme);
    NPC npc = *(loc.getNPCs()[0]);
    
    // Create the InteractionManager
    InteractionManager interactionManager(player, npc);

    // Start the interaction
    interactionManager.startInteraction();

    // Test displaying the initial dialogue
    // std::string expectedInitialDialogue = "Hello, traveler! What brings you to Hogwarts?";
    // assert(interactionManager.getTotalScore() == 10); // Check initial score
    // std::cout << "Initial Dialogue: " << expectedInitialDialogue << std::endl;

    // Simulate progression of the dialogue
    interactionManager.progressDialogue();  // Simulate player selecting the next option

    // Check that the response is generated based on the NPC context
    // std::string geminiResponse = "Your journey is a noble one."; // Simulated response for test
    interactionManager.progressDialogue();  // Progress dialoguet
    interactionManager.progressDialogue();  // Progress dialoguet
    interactionManager.progressDialogue();  // Progress dialoguet
    interactionManager.progressDialogue();  // Progress dialoguet
    interactionManager.progressDialogue();  // Progress dialoguet

    // Assuming a mock function or a set score for this response
    assert(interactionManager.getTotalScore() > 10);  // Ensure score has increased

    // Check final score after interaction
    int finalScore = interactionManager.getTotalScore();
    std::cout << "Final score: " << finalScore << std::endl;

    // End the interaction
    interactionManager.endInteraction();
}

void testGeneration(std::string theme){
    int rank;
    cout << "Enter the Rank: ";
    cin>>rank;
    
    // currently generating 1 NPC for location
    Location loc(rank, theme);

    cout << endl;
    cout << endl;
    cout << "Location Name: " << loc.getName() << endl;
    cout << "Location Description: " << loc.getDesc() << endl;

    vector<NPC*> npcs = loc.getNPCs();

    for (auto npc : npcs) {
        cout << "NPC Name: " << npc->getName() << endl;
        cout << "NPC Backstory: " << npc->getBackStory() << endl;
        cout << "NPC Rank: " << npc->getRank() << endl;

        cout << "NPC Quests: " << endl;
        npc->displayQuests();
        
        cout << endl;
    }
    
}

int main()
{
    cout<<"MAIN FILE RUNNING\n\n";

    // testGeneration(theme);

    testInteraction(theme);

    
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
