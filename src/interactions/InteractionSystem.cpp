#include "InteractionSystem.h"
#include "Gemini.h"
#include "NPC.h"
#include "Player.h"

// Constructor for DialogueNode
DialogueNode::DialogueNode(const std::string& text, int initialScore) 
    : text(text), score(initialScore) {}

// Get the text of the dialogue node
const std::string& DialogueNode::getText() const {
    return text;
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
    : player(player), npc(npc), currentNode(nullptr), totalScore(10), dialogueCount(0) {}

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

// Get quest details from the NPC
std::string InteractionManager::getQuestDetails() const {
    return "- " + npc.getQuestName() + ": " + npc.getQuestDescription() + "\n";
}

// Generate prompt for score calculation with examples
std::string InteractionManager::generateScorePrompt() const {
    std::string prompt = "Based on previous dialogues, generate a score as an integer (-3 to +3) for interaction relevance.\n";
    prompt += "Example:\n";
    prompt += "- If NPC response matches player's query well, score = 2.\n";
    prompt += "- If response is generic or neutral, score = 1.\n";
    prompt += "- If response shows frustration, score = -2.\n";
    prompt += "Previous dialogue: \n";
    for (const auto& dialogue : conversationHistory) {
        prompt += "- " + dialogue + "\n";
    }
    return prompt;
}

// Progress the dialogue
void InteractionManager::progressDialogue() {
    if (dialogueCount >= 5 || totalScore <= 0) {
        std::cout << "The NPC denies your quest." << std::endl;
        endInteraction();
        return;
    }

    // Use Gemini::getResponse to generate response based on NPC context
    std::string response = Gemini().getResponse(
        conversationHistory, npc.getName(), std::to_string(npc.getRank()), npc.getBackStory(),
        npc.getLocName(), npc.getLocDesc(), npc.getLocthemeName(), getQuestDetails()
    );

    // Add response to conversation history
    conversationHistory.push_back(response);

    // Display the response
    std::cout << "NPC Response: " << response << std::endl;

    // Generate prompt and get score response
    std::string scorePrompt = generateScorePrompt();
    std::string scoreResponse = Gemini().query(scorePrompt);

    // Convert response to integer score (assume valid response)
    int deltaScore = std::stoi(scoreResponse);

    // Update score and check for conditions
    updateScore(deltaScore);
    dialogueCount++;

    if (totalScore >= 20) {
        std::cout << "Congratulations! You reached the required score. Starting the mini-game..." << std::endl;
        if (miniGame.play()) {
            outcome.calculate(player, npc);  // Add quest rewards based on mini-game result
            std::cout << "You won the mini-game and completed the quest!" << std::endl;
        } else {
            std::cout << "You failed the mini-game. Quest ends here." << std::endl;
        }
        endInteraction();
    }
}

// Update the total score based on deltaScore
void InteractionManager::updateScore(int deltaScore) {
    totalScore += deltaScore;
}

// End the interaction
void InteractionManager::endInteraction() {
    std::cout << "Thank you for your visit. Your total score is: " << totalScore << std::endl;
    delete currentNode;  // Clean up dynamically allocated memory
}

// Get total score for testing purposes
int InteractionManager::getTotalScore() const {
    return totalScore;
}

void Outcome::calculate(Player& player, NPC& npc) {
    // Apply results to player and NPC based on interaction
    // For example, increase player's rank, give rewards, update NPC status, etc.
    std::cout << "calculating outcome" << std::endl;
}

bool MiniGame::play() {
    // Simulate a simple mini-game
    // Return true if successful, false if failed
    std::cout << "Playing mini-game..." << std::endl;
    return true;  // For testing purposes, always return true
}