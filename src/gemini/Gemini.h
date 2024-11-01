#pragma once

#include <external.h>
#include <GameEngine.h>
#include <NPC.h>
#include <Stats.h>

class Gemini
{
    private:
        string apiKey;
        mutex apiMutex;

        void getApiKey();
        static int WriteCallBack(void* contents, int size, int nmemb, void* userp);
        json sendGeminiReq(const string &prompt);
        json sendGeminiReq(const string &promptFilePath, const string &prompt);
        json sendBatchGeminiReq(const vector<string>& prompts);
        json isRespCorrect(const json& resp, const json& def);

    public:
        Gemini();

        future<json> genStatsForTheme();
        future<json> genNPC(const int &rank,const string &LocName, const string &LocDesc);
        future<json> genNPCStats(const int& rank, const string &npcName, const string &npcBackStory);
        future<json> genQuests(const int &rank,const string &name,const string &backStory);
        future<json> genLocationByGemini(const int &rank, const string &themeName);
        future<json> genLocationByGemini(const int &rank, const string &name,const string &themeName);

        future<json> genItem(int rank, const string &themeName);

        static future<vector<json>> genMutlipleLocations(const vector<int>& rank, const string &themeName);

        std::string query(const std::string& prompt);

        std::string getResponse(const std::vector<std::string>& dialogueHistory, const std::string& npcName, 
                                 const std::string& npcRank, const std::string& npcBackStory, 
                                 const std::string& locName, const std::string& locDesc, 
                                 const std::string& themeName, const std::string& questDetails);   
};
