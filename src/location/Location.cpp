#include <GameEngine.h>
#include <Location.h>
#include <Gemini.h>


Location::Location(int rank, const string &themeName): rank(rank), themeName(themeName)
{
    name = "";
    desc = "";
    // json LocResp= Gemini::genLocationByGemini(rank);
    // name=LocResp["name"];
    // desc=LocResp["description"];

    // // setLocInMap();

    // //parse npcs

    // for(const auto& npcData : LocResp["npcs"])
    // {
    //     string npcName= npcData["name"];
    //     string npcDesc= npcData["description"];
    //     int npcRank= npcData["rank"];
        
    //     NPC* newNPC = new NPC(npcRank, npcName, npcDesc);  
    //     addNPC(newNPC);
    // }
}


Location::Location(int rank,const string &name, const string &themeName): rank(rank), name(name), themeName(themeName)
{
    // json LocResp= Gemini::genLocationByGemini(rank,name);
    // desc=LocResp["description"];

    // // setLocInMap();

    // //parse npcs

    // for(const auto& npcData : LocResp["npcs"])
    // {
    //     string npcName= npcData["name"];
    //     string npcDesc= npcData["description"];
    //     int npcRank= npcData["rank"];
        
    //     NPC* newNPC = new NPC(npcRank, npcName, npcDesc);  
    //     addNPC(newNPC);
    // }
}

Location::Location(int rank,const string &name, const string &desc, const string &themeName):rank(rank),name(name),
                        desc(desc),themeName(themeName){}

// void setLocInMap();//complete this to add location into the world map according to gemini;

string Location::getName() const
{
    return name;
}

string Location::getDesc() const
{
    return desc;
}

string Location::getthemeName() const
{
    return themeName;
}

vector<NPC*> Location::getNPCs() const
{
    return npcsPresent;
}

unordered_map<Location*,int> Location::getPaths() const
{
    return paths;
}

void Location::addNPC(NPC* npc) 
{
    npcsPresent.push_back(npc);
    std::cout << "NPC added to location: " << name << "\n";
}

void Location::removeNPC(NPC* npc) 
{
    auto it = std::remove(npcsPresent.begin(), npcsPresent.end(), npc);
    if (it != npcsPresent.end()) 
    {
        npcsPresent.erase(it, npcsPresent.end());
        std::cout << "NPC removed from location: " << name << "\n";
    }
}