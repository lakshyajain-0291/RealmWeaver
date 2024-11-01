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


void testInteraction() {
    // Create a player
    Player player("Hero", 5);  // Name, level

    // Create an NPC
    Location loc  = Location(3, "space");
    NPC npc = *(loc.getNPCs()[0]);
    
    // Create the InteractionManager
    InteractionManager IM(player, npc);

    // Start the interaction
    IM.startInteraction();

    // Test displaying the initial dialogue
    // std::string expectedInitialDialogue = "Welcome to the castle, Hero. How can I assist you today?";
    // assert(interactionManager.getCurrentDialogueText() == expectedInitialDialogue);
    
    // Simulate progression of the dialogue
    IM.progressDialogue();  // Simulate player selecting the next option

    // Check that the response is generated based on the NPC context
    // std::string geminiResponse = interactionManager.getLastResponse();
    // assert(!geminiResponse.empty());  // Ensure we received a response

    // Test score updating
    // int initialScore = interactionManager.getTotalScore();
    // interactionManager.progressDialogue();  // Advance dialogue to check score
    // int newScore = interactionManager.getTotalScore();
    // assert(newScore > initialScore);  // Ensure score has increased

    // End the interaction
    IM.endInteraction();
    
    // Final score verification
    // int finalScore = interactionManager.getTotalScore();
    // std::cout << "Final score: " << finalScore << std::endl;
}


int main()
{
    // cout<<"MAIN FILE RUNNING\n\n";

    // int rank;

    // cout << "Enter the Rank: ";
    // cin>>rank;
    
    // // currently generating 1 NPC for location
    // Location loc(rank, theme);
    // cout << endl;
    // cout << endl;
    // cout << "Location Name: " << loc.getName() << endl;
    // cout << "Location Description: " << loc.getDesc() << endl;

    testInteraction();
    
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
