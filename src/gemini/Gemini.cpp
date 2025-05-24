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
    // Three API keys; initially, all are the same.
    vector<string> apiKeys = {
        "AIzaSyCFvaakADA4GLAF8NbtWmdQm-OA_F-qrtQ",
        "AIzaSyCFuOzrXYStJBD8ExnY3uP3T1kakFVB5VI"};

    // Static index to remember last-used API key index for round robin
    static int keyIndex = 0;
    json jsonResp;
    bool success = false;

    // Attempt each API key in round-robin order
    for (int i = 0; i < apiKeys.size(); i++)
    {
        // Determine the current key index based on the static round-robin index.
        int currentKeyIndex = (keyIndex + i) % apiKeys.size();
        string currentKey = apiKeys[currentKeyIndex];
        // Build the URL using the current API key.
        string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + currentKey;

        CURL *curl = curl_easy_init();
        if (curl)
        {
            string readBuffer;
            struct curl_slist *headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            // Prepare the JSON payload.
            json jsonPayload = {
                {"contents", {{{"parts", {{{"text", prompt}}}}}}}};

            string jsonString = jsonPayload.dump();
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            // Perform the request.
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK)
            {
                // Successful call. Parse the response.
                try
                {
                    jsonResp = json::parse(readBuffer);
                   // cout << jsonResp.dump(4) << endl; // Print the response for debugging
                }
                catch (json::parse_error &e)
                {
                    std::cerr << "JSON parse error: " << e.what() << std::endl;
                    // Cleanup and try the next API key.
                }
                // Update round-robin index to the next key after the one that worked.
                keyIndex = (currentKeyIndex + 1) % apiKeys.size();
                success = true;
                curl_easy_cleanup(curl);
                curl_slist_free_all(headers);
                break; // Exit the loop; we got a valid response.
            }
            else
            {
                std::cerr << "API request failed with key " << currentKey
                          << ": " << curl_easy_strerror(res) << std::endl;
            }
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
          //  cout << "url: " << url << endl;
        }

        i == apiKeys.size() - 1 ? keyIndex = 0 : keyIndex;
    }

    if (!success)
    {
        std::cerr << "All API key attempts failed." << std::endl;
        // Depending on your needs, throw an exception or handle the error accordingly.
    }

    return jsonResp;
}

json Gemini::sendBatchGeminiReq(const vector<string> &prompts)
{
    // Three API keys; initially, all are the same.

    vector<string> apiKeys = {
        "AIzaSyCFvaakADA4GLAF8NbtWmdQm-OA_F-qrtQ",
        "AIzaSyCFuOzrXYStJBD8ExnY3uP3T1kakFVB5VI"};


    // Static index to remember the last used API key.
    static int keyIndex = 0;
    json jsonResp;
    bool success = false;

    // Loop through all API keys in a round-robin manner.
    for (size_t i = 0; i < apiKeys.size(); i++)
    {
        int currentKeyIndex = (keyIndex + i) % apiKeys.size();
        string currentKey = apiKeys[currentKeyIndex];
        
        string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + currentKey;

        CURL *curl = curl_easy_init();
        if (curl)
        {
            string readBuffer;
            struct curl_slist *headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            // Build payload.
            json jsonPayload = {{"contents", json::array()}};
            for (const auto &prompt : prompts)
            {
                jsonPayload["contents"].push_back({{"parts", {{{"text", prompt}}}}});
            }
            string jsonString = jsonPayload.dump();

            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK)
            {
                try
                {
                    jsonResp = json::parse(readBuffer);

                 //   cout << jsonResp.dump(4) << endl; // Print the response for debugging
                }
                catch (json::parse_error &e)
                {
                    std::cerr << "JSON parse error: " << e.what() << std::endl;
                    // Cleanup and continue to try the next key.
                }
                // Update round-robin index to the next key after the successful one.
                keyIndex = (currentKeyIndex + 1) % apiKeys.size();
                success = true;
                curl_easy_cleanup(curl);
                curl_slist_free_all(headers);
                break;
            }
            else
            {
                std::cerr << "API request failed with key " << currentKey
                          << ": " << curl_easy_strerror(res) << std::endl;
            }
         //
         //   cout << "url: " << url << endl;
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
        }

        i == apiKeys.size() - 1 ? keyIndex = 0 : keyIndex;
    }

    if (!success)
    {
        std::cerr << "All API key attempts failed." << std::endl;
        // Handle error appropriately, e.g., by throwing an exception or returning an error json.
    }

    return jsonResp;
}

json Gemini::sendGeminiReq(const string &promptFilePath, const string &prompt)
{
    // Read and parse the JSON file containing the prompt payload.
    json jsonPayload;
    ifstream file(promptFilePath);
    if (!file.is_open())
    {
        throw runtime_error("Could not open the file : " + promptFilePath);
    }
    file >> jsonPayload;
    file.close();

    // Create the user prompt JSON structure.
    json jsonPrompt = {
        {"role", "user"},
        {"parts", {{{"text", prompt}}}}};

    // Append the user prompt JSON object into the payload.
    jsonPayload["contents"].push_back(jsonPrompt);

    // Define three API keys (initially all the same, but can be distinct).

    vector<string> apiKeys = {
        "AIzaSyCFvaakADA4GLAF8NbtWmdQm-OA_F-qrtQ",
        "AIzaSyCFuOzrXYStJBD8ExnY3uP3T1kakFVB5VI"};

    // Static variable to keep track of the last API key used.
    static int keyIndex = 0;
    json jsonResp;
    bool success = false;

    // Try each API key in round-robin fashion.
    for (size_t i = 0; i < apiKeys.size(); i++)
    {

        int currentKeyIndex = (keyIndex + i) % apiKeys.size();
        string currentKey = apiKeys[currentKeyIndex];
        string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + currentKey;

        CURL *curl = curl_easy_init();
        if (curl)
        {
            string readBuffer;
            struct curl_slist *headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            // Dump the JSON payload into a string.
            string jsonString = jsonPayload.dump();
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            // Perform the HTTP request.
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK)
            {
                try
                {
                    jsonResp = json::parse(readBuffer);
                //    cout << jsonResp.dump(4) << endl; // Print the response for debugging
                }
                catch (json::parse_error &e)
                {
                    std::cerr << "JSON parse error: " << e.what() << std::endl;
                    // Cleanup and continue to try the next key.
                }
                // Update the round-robin index to the next key after the one that succeeded.
                keyIndex = (currentKeyIndex + 1) % apiKeys.size();
                success = true;
                curl_easy_cleanup(curl);
                curl_slist_free_all(headers);
                break; // Exit loop on successful response.
            }
            else
            {
                std::cerr << "API request failed with key " << currentKey
                          << ": " << curl_easy_strerror(res) << std::endl;
            }
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);

         //   cout << "url: " << url << endl;
        }
        i==apiKeys.size()-1 ? keyIndex=0 : keyIndex;
    }

    if (!success)
    {
        std::cerr << "All API key attempts failed." << std::endl;
        // Handle the error appropriately (e.g., throw exception, return error JSON, etc.)
    }

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

future<json> Gemini::genStatsForTheme()
{
    return async(launch::async, [this]()
    {
        json defaultResp = {
            { "attributes", {
                {{ "health", 100 }},
                {{ "strength", 5 }},
                {{ "agility", 5 }},
                {{ "willpower", 5 }},
                {{ "defence", 5 }}
            }}
        };

        string prompt = "Based on the provided JSON structure, can you return the attributes for an "+theme+"-themed game?";

        try
        {
            json jsonResp = this->sendGeminiReq("../json/stats.json", prompt);
            json final    = isRespCorrect(jsonResp, defaultResp);

            // Validate that each attribute is an object mapping to an integer
            bool valid = final.contains("attributes")
                         && final["attributes"].is_array();
            if (valid)
            {
                for (auto &attr : final["attributes"])
                {
                    if (!attr.is_object() || attr.size() != 1)
                    {
                        valid = false;
                        break;
                    }
                    auto it = attr.begin();
                    if (!it.value().is_number_integer())
                    {
                        valid = false;
                        break;
                    }
                }
            }
            if (!valid) return defaultResp;
            return final;
        }
        catch (const nlohmann::json::type_error& e)
        {
            std::cerr << "JSON Type Error in genStatsForTheme: " << e.what() << std::endl;
            return defaultResp;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error in genStatsForTheme: " << e.what() << std::endl;
            return defaultResp;
        }
    });
}
// // json should not be empty -- for any null value send a defualt json
// future<json> Gemini::genStatsForTheme()
// {
//     return async(launch::async, [this]()
//     {
//         json defaultResp ={
//             { "attributes", {
//                 {{ "health", 100 }}, 
//                 {{ "strength", 5 }}, 
//                 {{ "agility", 5 }}, 
//                 {{ "willpower", 5 }}, 
//                 {{ "defence", 5 } }
//             } }
//         };
//         // cout<<defaultJson;
//         string prompt = "Based on the provided JSON structure, can you return the attributes for an "+theme+"-themed game?";
//         json jsonResp=this->sendGeminiReq("../json/stats.json",prompt);

//         // cout<<jsonResp;

//         json final=isRespCorrect(jsonResp,defaultResp);

//         return final;
//     });
// }

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
        // cout<<prompt;
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
            { "quests", json::array({
                {
                    { "quest_name", "Unknown Quest" },
                    { "description", "A generic quest given by an unknown NPC with no specific objective." },
                    { "difficulty", "Easy" }
                }
            }) }
        };

        // Construct the prompt for generating quests
        string prompt = 
            "Generate a quest or a series of quests for the NPC named '" + npcName + "', "
            "who has the following background: '" + npcBackStory + "'. "
            "The NPC holds a rank of " + to_string(rank) + "/10 ("
            + GameEngine().getRankName(rank) + "). "
            "Based on the NPC's rank and background, provide quests with objectives, "
            "potential rewards, and estimated difficulty levels. "
            "The quest should feel personalized to the NPC's backstory and rank, "
            "challenging the player accordingly.";

        // Optional debug output of the constructed prompt
        // cout << "Constructed Prompt: " << prompt << endl;

        // Send the request to the Gemini API
        json jsonResp = this->sendGeminiReq("../json/npcQuests.json", prompt);

        // Validate the response; use default response if validation fails
        json final = isRespCorrect(jsonResp, defaultResp);
        
        // Output the final response for debugging
        // cout << "Final Response: " << final.dump(4) << endl;

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
        // cout << jsonResp;

        // Validate the response, fallback to the default response if necessary
        json final = isRespCorrect(jsonResp, defaultResp);
        // cout << final;

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
        // cout << final; // Optional: Print the final JSON response for debugging

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

        // cout << prompt; // Optional: Print the prompt for debugging

        // Send request to Gemini API
        json jsonResp = this->sendGeminiReq("../json/item.json", prompt);

        // Ensure response is valid or fallback to default
        json final = isRespCorrect(jsonResp, defaultResp);
        // cout << final; // Optional: Print the final JSON response for debugging
        return final;
    });
}

std::string Gemini::query(const std::string& prompt) {
    try {
        // Assuming sendGeminiReq returns JSON data directly
        json jsonResponse = sendGeminiReq(prompt);

        // Check if the data contains the expected structure
        if (jsonResponse.contains("candidates") && 
            jsonResponse["candidates"][0]["content"].contains("parts")) {
            return jsonResponse["candidates"][0]["content"]["parts"][0]["text"];
        } else {
            return "Error: Expected structure not found in response.";
        }

    } catch (const json::parse_error& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        return "Error: Unable to parse JSON response.";
    } catch (const std::exception& e) {
        std::cerr << "Error extracting response from Gemini: " << e.what() << std::endl;
        return "Error: Unable to generate response.";
    }
}

future<json> Gemini::getResponse(const std::vector<std::string>& dialogueHistory, 
                                  const std::string& npcName, 
                                  const std::string& npcRank, 
                                  const std::string& npcBackStory, 
                                  const std::string& locName, 
                                  const std::string& locDesc, 
                                  const std::string& themeName, 
                                  const std::string& questName, 
                                  const std::string& questDescription, 
                                  const std::string& questTask) 
{
    return async(launch::async, [this, dialogueHistory, npcName, npcRank, npcBackStory, locName, locDesc, themeName, questName, questDescription, questTask]()
    {
        // Construct the prompt for the NPC's response
        std::string prompt = "You are an NPC with the name "+ npcName + " and following context:\n\n";
        prompt += "NPC Details:\n";
        prompt += "Name: " + npcName + "\n";
        prompt += "Rank: " + npcRank + "\n";
        prompt += "Backstory: " + npcBackStory + "\n\n";
        
        // Location context
        prompt += "Location:\n";
        prompt += "Name: " + locName + "\n";
        prompt += "Description: " + locDesc + "\n";
        prompt += "Theme: " + themeName + "\n\n";
        
        // Quest context
        prompt += "Quest Details:\n";
        prompt += "Quest Name: " + questName + "\n";
        prompt += "Description: " + questDescription + "\n";
        prompt += "Task: " + questTask + "\n\n";

        // Conversation history
        prompt += "Conversation History:\n";
        for (size_t i = 0; i < dialogueHistory.size(); i++) 
        {
            // Alternating NPC and Player dialogue
            if (i % 2 == 0) // NPC's turn
            {
                prompt += std::to_string(i+1) + ". " + npcName + ": " + dialogueHistory[i] + "\n";
            } 
            else // Player's turn
            {
                prompt += std::to_string(i+1) + ". " + "User: " + dialogueHistory[i] + "\n";
            }
        }

        // Final instruction for response generation
        prompt += "\nGenerate a response that respects your backstory, location, and quest context, and interact with me:\n";

        // Default response structure in case the API call fails or the response is invalid
        json defaultResp = {
            { "response", "I'm sorry, but I cannot provide a response at this time." }
        };

        // Output the constructed prompt for debugging
        // std::cout << "Constructed Prompt: " << prompt << std::endl;

        // Send request to Gemini API
        json jsonResp = this->sendGeminiReq("../json/npcResponse.json", prompt);

        // Ensure response is valid or fallback to default
        json final = isRespCorrect(jsonResp, defaultResp);
        // Optional: Print the final JSON response for debugging
        // std::cout << "Final Response: " << final.dump(4) << std::endl;

        return final;
    });
}

std::future<json> Gemini::get_loc_json_from_quest(std::unordered_map<int, std::shared_ptr<Location>> loc_map,
                                                  const std::string &quest_name, const std::string &quest_disc)
{
    return std::async(std::launch::async, [this, loc_map, quest_name, quest_disc]() 
    {
        // Prepare the prompt for the Gemini API request
        std::string prompt = "Based on the quest '" + quest_name + "' with the description: '" + quest_disc + "', "
                             "determine the location most suited for this quest from the following list of locations:\n";

        // Add location details to the prompt
        for (const auto &entry : loc_map) {
            int key = entry.first;  // Location key
            std::shared_ptr<Location> loc_ptr = entry.second; // Location pointer

            std::string loc_name = loc_ptr->getName();
            std::string loc_desc = loc_ptr->getDesc();

            // Append the location details to the prompt
            prompt += "Location " + std::to_string(key) + ": " + loc_name + " - " + loc_desc + "\n";
        }

        // Send the request to Gemini API
        json jsonResp = this->sendGeminiReq("../json/Questlocation.json", prompt);

        // Default location key (fallback if the response is invalid)
        int location_key = -1;

        // Ensure the response is valid and check for the most suited location
        if (jsonResp.contains("response") && jsonResp["response"].is_string()) {
            std::string response = jsonResp["response"];

            // Logic to extract the most suited location key from Gemini's response
            for (const auto &entry : loc_map) {
                int key = entry.first;
                std::shared_ptr<Location> loc_ptr = entry.second;
                std::string loc_name = loc_ptr->getName();
                
                // Check if the response mentions a location name (simplified matching logic here)
                if (response.find(loc_name) != std::string::npos) {
                    location_key = key;
                    break; // Once we find a match, we can exit the loop
                }
            }
        }

        // Prepare JSON output with the location key
        json output_json = {
            {"location_key", location_key}
        };

        return output_json; // Return the location key in JSON format
    });
}


int Gemini::get_loc_key_from_quest(std::unordered_map<int, std::shared_ptr<Location>> loc_map, 
    const std::string &quest_name, const std::string &quest_disc){

    future<json> LocResp = Gemini().get_loc_json_from_quest(loc_map, quest_name, quest_disc);
    json locJson = LocResp.get();

    if (locJson.contains("location_key")) {
        int location_key = locJson["location_key"];
        return location_key;
    } else {
        return 0;
    }
}

std::string Gemini::gen_intro(std::string theme_name){
    std::string prompt = "Given the theme name of the game, create an engaging and immersive backstory for the player's character. The character should have a rich history, personal motivations, and past experiences that influence their current journey. Include key details such as their origins, backstory, and their ultimate goals. Make sure the backstory ties into the game's world, setting, and plot, and aligns with the tone and style of the theme. The backstory should serve as a foundation for the player's role in the game and provide a compelling reason for their involvement in the adventure. And note that ouput must be less than 200 words. Theme name : " + theme_name;
    return Gemini().query(prompt);
}
