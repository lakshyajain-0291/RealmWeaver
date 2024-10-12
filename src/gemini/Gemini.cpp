#include<Gemini.h>

Gemini::Gemini()
{
    getApiKey();
}

void Gemini::getApiKey() 
{
    ifstream configFile("../.env");
    string line;

    if (!configFile.is_open()) {
        cerr << "Could not open the configuration file!" << endl;
        return ; // Return an empty string if the file cannot be opened
    }

    while (getline(configFile, line)) {
        if (line.find("GEMINI_API_KEY=") == 0) 
        {
            apiKey = line.substr(15); // Get the value after "GEMINI_API_KEY="
            break;
        }
    }

    if (apiKey.empty()) {
        cerr << "API Key not found in configuration file!" << endl;
    }
}

int Gemini::WriteCallBack(void* contents, int size, int nmemb, void* userp)
{
    ((string*)userp)->append((char*)contents, size*nmemb);
    return size*nmemb;
}

json Gemini::sendGeminiReq(const string &prompt)
{
    CURL* curl;
    CURLcode res;
    string readBuffer;

    curl= curl_easy_init();
    if(curl)
    {
        struct curl_slist* headers = nullptr;
        headers=curl_slist_append(headers,"Content-Type: application/json");
        // headers=curl_slist_append(headers,"Authorization: Bearer ${api}");

        string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest:generateContent?key=" + apiKey;

        curl_easy_setopt(curl,CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER, headers);

        json jsonPayload = {
            {"contents", {{{"parts", {{{"text", prompt}}}}}}}
        };

        string jsonString =jsonPayload.dump();

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res=curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    json jsonResp = json::parse(readBuffer);
    return jsonResp;
}

json Gemini::sendBatchGeminiReq(const vector<string> &prompts)
{
    CURL* curl;
    CURLcode res;
    string readBuffer;

    curl= curl_easy_init();
    if(curl)
    {
        struct curl_slist* headers = nullptr;
        headers=curl_slist_append(headers,"Content-Type: application/json");
        // headers=curl_slist_append(headers,"Authorization: Bearer ${api}");

        string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest:generateContent?key=" + apiKey;

        curl_easy_setopt(curl,CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER, headers);

        json jsonPayload = 
        {
            {"contents",{}}
        };
        
        
        for(const auto& prompt: prompts)
        {
            jsonPayload["contents"].push_back({{"parts",{{{"text",prompt}}}}});   
        }

        string jsonString =jsonPayload.dump();

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res=curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    json jsonResp = json::parse(readBuffer);
    return jsonResp;
}

json Gemini::sendGeminiReq(const string &promptFilePath, const string &prompt)
{
    json jsonPayload;
    ifstream file(promptFilePath);

    if(!file.is_open())
    {
        throw runtime_error("Could not open the file : "+promptFilePath);
    }

    file >> jsonPayload;

    file.close();
    
    json jsonPrompt=
    {
        {"role","user"},
        {"parts", {
            {
                {"text",prompt}
            }
        }}

    };

    jsonPayload["contents"].push_back(jsonPrompt);

    CURL* curl;
    CURLcode res;
    string readBuffer;


    curl= curl_easy_init();
    if(curl)
    {
        struct curl_slist* headers = nullptr;
        headers=curl_slist_append(headers,"Content-Type: application/json");
        // headers=curl_slist_append(headers,"Authorization: Bearer ${api}");

        string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest:generateContent?key=AIzaSyBNRAo4aMQvwIhYbcNI_Bzp4Sicbxj49Ls";

        curl_easy_setopt(curl,CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl,CURLOPT_HTTPHEADER, headers);

        string jsonString =jsonPayload.dump();

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // cout<<apiKey<<endl;
        // string jsonString1 = jsonPayload.dump(4); // 4 spaces for indentation
        // cout << "Formatted Payload: " << jsonString1 << std::endl;        
        res=curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    json jsonResp = json::parse(readBuffer);
    return jsonResp;
}

json Gemini::isRespCorrect(const json& resp, const json& def)
{
        if(resp.contains("candidates") && 
            resp["candidates"].is_array() &&
            !resp["candidates"].empty() &&
            resp["candidates"][0].contains("content") &&
            resp["candidates"][0]["content"].contains("parts") &&
            resp["candidates"][0]["content"]["parts"].is_array() &&
            !resp["candidates"][0]["content"]["parts"].empty() &&
            resp["candidates"][0]["content"]["parts"][0].contains("text")) 
        {

            try 
            {
                // Try to access and parse the string into JSON
                json parsedJson = json::parse(resp["candidates"][0]["content"]["parts"][0]["text"].get<std::string>());

                // Print the parsed JSON nicely formatted
                // cout << parsedJson.dump(4) << endl; // Use 'dump(4)' for pretty printing with indentations
                return parsedJson;
            } 
            catch (const json::parse_error& e) 
            {
                // Output the specific error for debugging purposes
                cerr << "JSON Parse Error: " << e.what() << endl;
                return def; // Return an empty JSON object on parse error
            }
        }

        return def;
}

// json should not be empty -- for any null value send a defualt json
future<json> Gemini::genStatsForTheme()
{
    return async(launch::async, [this]()
    {
        json defaultResp ={
            { "attributes", {
                {{ "health", 100 }}, 
                {{ "strength", 5 }}, 
                {{ "agility", 5 }}, 
                {{ "willpower", 5 }}, 
                {{ "defence", 5 } }
            } }
        };
        // cout<<defaultJson;
        string prompt = "Based on the provided JSON structure, can you return the attributes for an "+theme+"-themed game?";
        json jsonResp=this->sendGeminiReq("../json/stats.json",prompt);

        // cout<<jsonResp;

        json final=isRespCorrect(jsonResp,defaultResp);

        return final;
    });
}

future<json> Gemini::genNPC(const int& rank,const string &locName, const string &locDesc)
{
    return async(launch::async, [this,rank,locName,locDesc]()
    {
        json defaultResp ={
            { "npc", {
                 {"name", "Common citizen"} , 
                 {"rank",  1}, 
                 {"backstory", "Just a common everyday citizen."}
            } }
        };
        // cout<<defaultJson;
        string prompt = "Generate an NPC for a " + theme + "-themed game. The NPC should be suitable for a location named '" + locName + "' described as '" + locDesc + "'. The NPC should be based on their rank, which is " + std::to_string(rank)+"/10 " + "("+GameEngine().getRankName(rank)+").";        
        cout<<prompt;
        json jsonResp=this->sendGeminiReq("../json/npc.json",prompt);

        // cout<<jsonResp;

        json final=isRespCorrect(jsonResp,defaultResp);

        return final;
    });
}

future<json> Gemini::genNPCStats(const int& rank, const string &npcName, const string &npcBackStory)
{
    return async(launch::async, [this, rank, npcName, npcBackStory]()
    {
        Stats npcStats;
        // Create a default response in case the API fails or the response is invalid.
        json defaultResp = {
            { "attributes", json::array() }
        };

        // Convert NPC stats from unordered_map to a JSON array of attributes
        for (const auto& stat : npcStats.getAllStats()) 
        {
            defaultResp["attributes"].push_back({
                {stat.first, stat.second}
            });
        }

        // Generate a list of attribute names for the prompt
        string attributesList;
        for (const auto& stat : npcStats.getAllStats()) 
        {
            attributesList += stat.first + " (" + std::to_string(stat.second) + "), ";
        }

        // Remove trailing comma and space
        if (!attributesList.empty()) {
            attributesList.pop_back();
            attributesList.pop_back();
        }

        string prompt = 
            "Generate attributes for an NPC named '" + npcName + "', "
            "with the following description: '" + npcBackStory + "'. "
            "The NPC has a rank of " + std::to_string(rank) + "/10 (" + GameEngine().getRankName(rank) + "). "
            "The attributes that need values are: " + attributesList + ". "
            "Here, the values in the brackets are the initial stats of a rank 1/10 (Inferior) player. "
            "Please provide suitable values for these attributes based on the NPC's rank.";
            // "Each attribute must be greater than (initial value * (rank - 1) ^ 10) and less than (initial value * rank ^ 10). "

        // cout << prompt; 

        json jsonResp = this->sendGeminiReq("../json/npcStats.json", prompt);
        // cout<<jsonResp;

        json final = isRespCorrect(jsonResp, defaultResp);

        return final;
    });
}


//where the quest will take place and wgame will play//////////////
future<json> Gemini::genQuests(const int &rank, const string &npcName, const string &npcBackStory)
{
    return async(launch::async, [this, rank, npcName, npcBackStory]()
    {
        // Default quest structure in case the API call fails or response is invalid
        json defaultResp = {
            { "quests", json::array() }
        };

        // Prepare prompt
        string prompt = 
            "Generate a quest or a series of quests for an NPC to give to the Player. NPC is named '" + npcName + "', "
            "with the following description: '" + npcBackStory + "'. "
            "The NPC has a rank of " + std::to_string(rank) + "/10 (" + GameEngine().getRankName(rank) + "). "
            "The backstory of the NPC should inform the quest themes.";

        cout<<prompt;

        // Send request to Gemini API
        json jsonResp = this->sendGeminiReq("../json/npcQuests.json", prompt);
        cout<<jsonResp;

        // Ensure response is valid or fallback to default
        json final = isRespCorrect(jsonResp, defaultResp);
        cout<<final;
        return final;
    });
}

future<json> Gemini::genLocationByGemini(const int &rank, const string &themeName)
{
    return async(launch::async, [this, rank, themeName]()
    {
        // Default response in case of failure
        json defaultResp = {
            { "location", {
                {"name", "Unknown Territory"},
                {"description", "An uncharted and mysterious area with no known history."}
            } }
        };

        // Construct the prompt
        string prompt = 
            "Generate a location for a " + themeName + "-themed game. "
            "The location should be suitable for a setting with a rank of " + std::to_string(rank) + "/10 "
            "(" + GameEngine().getRankName(rank) + "). "
            "Provide a name and a detailed description of the location, taking the theme into account.";

        // Send request to Gemini API
        json jsonResp = this->sendGeminiReq("../json/location.json", prompt);
        cout << jsonResp;

        // Validate the response, fallback to the default response if necessary
        json final = isRespCorrect(jsonResp, defaultResp);
        cout << final;

        return final;
    });
}

future<json> Gemini::genLocationByGemini(const int &rank, const string &name, const string &themeName)
{
    return async(launch::async, [this, rank, name, themeName]()
    {
        // Default response in case the API call fails or response is invalid
        json defaultResp = {
            { "location", {
                 {"name", "Unknown Location"}, 
                 {"description", "A mysterious place with unknown features."}
            } }
        };

        // Prepare the prompt for the API request
        string prompt = 
            "Generate a location for a " + themeName + "-themed game. "
            "The location should be named '" + name + "' and suitable for rank " + std::to_string(rank) + "/10 (" + GameEngine().getRankName(rank) + "). "
            "Provide a description of the location and any notable features.";

        // cout << prompt; // Optional: Print the prompt for debugging

        // Send request to Gemini API
        json jsonResp = this->sendGeminiReq("../json/location.json", prompt);

        // Ensure response is valid or fallback to default
        json final = isRespCorrect(jsonResp, defaultResp);
        cout << final; // Optional: Print the final JSON response for debugging

        return final;
    });
}


static future<vector<json>> genMutlipleLocations(const vector<int>& rank, const string &themeName){
    return async(launch::async, [rank, themeName]()
    {
        vector<future<json>> futures;
        vector<json> locations;

        for (const auto& r : rank) 
        {
            futures.push_back(Gemini().genLocationByGemini(r, themeName));
        }

        for (auto& f : futures) 
        {
            locations.push_back(f.get());
        }

        return locations;
    });
}


future<json> Gemini::genItem(int rank, const string &themeName)
{
    return async(launch::async, [this, rank, themeName]()
    {
        // Default response in case the API call fails or response is invalid
        json defaultResp = {
            { "item", {
                 {"name", "Unknown Item"}, 
                 {"description", "An item of mysterious origin."},
                 {"rank", rank}
            } }
        };

        // Prepare the prompt for the API request
        string prompt = 
            "Generate an item for a " + themeName + "-themed game. "
            "The item should be suitable for rank " + std::to_string(rank) + "/10 (" + GameEngine().getRankName(rank) + "). "
            "Provide a detailed description of the item, its properties, and how it can be used.";

        cout << prompt; // Optional: Print the prompt for debugging

        // Send request to Gemini API
        json jsonResp = this->sendGeminiReq("../json/item.json", prompt);

        // Ensure response is valid or fallback to default
        json final = isRespCorrect(jsonResp, defaultResp);
        cout << final; // Optional: Print the final JSON response for debugging
        return final;
    });
}
