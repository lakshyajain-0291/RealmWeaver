// interaction.h
#pragma once

#include <string>
#include <vector>
#include <iostream>


class Player;
class NPC;
// class Quest;

// DialogueNode: Represents a single point in dialogue without branching choices
class DialogueNode {
public:
    DialogueNode(const std::string& text, int initialScore = 0);
    const std::string& getText() const;
    bool triggersMiniGame() const;
    int getScore() const;
    void updateScore(int deltaScore);  // Update the score based on relevance

private:
    std::string text;  // Text displayed for this dialogue node
    int score;         // Relevance score for this dialogue
    bool miniGameTrigger;  // Flag to check if this node triggers a mini-game
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
    void startInteraction();
    void progressDialogue();  // Progress to the next dialogue node
    void endInteraction();

private:
    Player& player;           // Reference to the player object
    NPC& npc;                 // Reference to the NPC object
    DialogueNode* currentNode;  // Current dialogue node in the interaction
    MiniGame miniGame;        // Mini-game component
    Outcome outcome;          // Outcome component for interaction results
    int totalScore;           // Total score accumulated through the interaction

    void displayDialogue() const;
    void updateScore();       // Update the total score based on the relevance score of the current dialogue
    std::string getQuestDetails() const; // Retrieve quest details for interaction
};
