#pragma once

#include <string>
#include <vector>
#include <iostream>

class Player;
class NPC;

// DialogueNode: Represents a single point in dialogue without branching choices
class DialogueNode {
public:
    DialogueNode(const std::string& text, int initialScore = 0);
    const std::string& getText() const;
    int getScore() const;
    void updateScore(int deltaScore);  // Update the score based on relevance

private:
    std::string text;  // Text displayed for this dialogue node
    int score;         // Relevance score for this dialogue
};

// MiniGame: A simple mini-game that affects interaction outcome
class MiniGame {
public:
    bool play();  // Returns true if mini-game is successful
};

// Outcome: Handles the results of an interaction (stats, quest rewards, etc.)
class Outcome {
public:
    void calculate(Player& player, NPC& npc);  // Apply results to player/NPC based on interaction
};

// InteractionManager: Oversees player-NPC interactions
class InteractionManager {
public:
    InteractionManager(Player& player, NPC& npc);
    int startInteraction();
    int progressDialogue();  // Progress to the next dialogue node
    void endInteraction();
    int getTotalScore() const;  // Returns total score for testing purposes

private:
    int initial_score = 5;
    int req_score = 15;
    int max_dialogues = 7;
    
    Player& player;                // Reference to the player object
    NPC& npc;                      // Reference to the NPC object
    DialogueNode* currentNode;     // Current dialogue node in the interaction
    MiniGame miniGame;             // Mini-game component
    Outcome outcome;               // Outcome component for interaction results
    int totalScore;                // Total score accumulated through the interaction
    int dialogueCount;             // Number of dialogues in the current interaction
    std::vector<std::string> conversationHistory;  // Store all conversation history

    void displayDialogue() const;
    void updateScore(int deltaScore); // Update the total score based on deltaScore
    std::string generateScorePrompt() const; // Create prompt for score calculation with examples
    std::string getQuestDetails() const; // Retrieve quest details for interaction
};
