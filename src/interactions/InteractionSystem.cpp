#include <InteractionSystem.h>
#include "Gemini.h"
#include "external.h"
#include "NPC.h"
#include "Player.h"


// Constructor for DialogueNode
DialogueNode::DialogueNode(const std::string& text, int initialScore) 
    : text(text), score(initialScore), miniGameTrigger(false) {}

// Get the text of the dialogue node
const std::string& DialogueNode::getText() const {
    return text;
}   

// Check if this node triggers a mini-game
bool DialogueNode::triggersMiniGame() const {
    return miniGameTrigger;
}

// Get the score of this dialogue node
int DialogueNode::getScore() const {
    return score;
}

// Update the score based on relevance
void DialogueNode::updateScore(int deltaScore) {
    score += deltaScore;
}

// Constructor for InteractionManager
InteractionManager::InteractionManager(Player& player, NPC& npc)
    : player(player), npc(npc), currentNode(nullptr), totalScore(0) {}

// Start the interaction
void InteractionManager::startInteraction() {
    std::cout << "You approach " << npc.getName() << ", a " << npc.getRank() << " at " << npc.getLocName() << ".\n";
    std::cout << "Location: " << npc.getLocDesc() << " (Theme: " << npc.getLocthemeName() << ")\n";
    
    // Initialize the first dialogue node (example)
    currentNode = new DialogueNode("Hello, traveler! What brings you to " + npc.getLocName() + "?");
    displayDialogue();
}

// Display the current dialogue
void InteractionManager::displayDialogue() const {
    if (currentNode) {
        std::cout << currentNode->getText() << std::endl;
    }
}

// Get quest details from the player
std::string InteractionManager::getQuestDetails() const {
    std::string questDetails = "Current Quests:\n";
    questDetails += "- " + npc.getQuestName() + ": " + npc.getQuestDescription() + "\n";
    return questDetails;
}

// Progress the dialogue
void InteractionManager::progressDialogue() {
    // Get the NPC and quest details for the prompt
    std::vector<std::string> dialogueHistory; // This should be maintained through the conversation
    std::string questDetails = getQuestDetails();
    
    std::string response = Gemini().getResponse(dialogueHistory, npc.getName(), std::to_string(npc.getRank()),
                                                  npc.getBackStory(), npc.getLocName(),
                                                  npc.getLocDesc(), npc.getLocthemeName(),
                                                  questDetails);

    std::cout << response << std::endl;
    // Update the score based on the relevance of the response
    updateScore();
}

// Update the total score based on relevance
void InteractionManager::updateScore() {
    // Example logic for updating score
    totalScore += currentNode->getScore();
}

// End the interaction
void InteractionManager::endInteraction() {
    std::cout << "Thank you for your visit. Your total score is: " << totalScore << std::endl;
    delete currentNode;  // Clean up dynamically allocated memory
}
