#include "jetplusplus/json/jsonConverter.hpp"
//#include <regex>
#include <algorithm>

namespace JETPP{    

    JsonConverter::JsonConverter(){

    }

    std::string JsonConverter::jsonToString(JETPP::JsonValue value){

        switch (value.type)
        {
            case JETPP::JsonValue::OBJECT:
            {
                std::string jsonString = "{";
                bool first = true;
                for (const auto &pair : value.asObject)
                {
                    if (!first)
                    {
                        jsonString += ",";
                    }
                    jsonString += "\"" + pair.first + "\":" + pair.second.toJsonString();
                    first = false;
                }
                jsonString += "}";
                return jsonString;
            }
            case JETPP::JsonValue::ARRAY:
            {
                std::string jsonString = "[";
                for (size_t i = 0; i < value.asArray.size(); ++i)
                {
                    if (i > 0)
                    {
                        jsonString += ",";
                    }
                    jsonString += value.asArray[i].toJsonString();
                }
                jsonString += "]";
                return jsonString;
            }
            case JETPP::JsonValue::STRING:
            {
                return "\"" + value.asString + "\"";
            }
            case JETPP::JsonValue::NUMBER:
                return std::to_string(value.asNumber);
            case JETPP::JsonValue::BOOLEAN:
                return value.asBoolean ? "true" : "false";
            case JETPP::JsonValue::NULL_VALUE:
                return "null";
            default:
                return ""; // Handle unrecognized type
        }
    }

    JETPP::JsonValue JsonConverter::stringToJson(std::string value) {
        JsonValue valueJson;

        /*std::regex whiteSpaceRegex("\\s+");
        value = std::regex_replace(value, whiteSpaceRegex, "");*/

        char firstChar = value.empty() ? '\0' : value[0];

        if (firstChar == '[') { // Convert array
            std::vector<JsonValue> values;
            std::string slicedArray = value.substr(1, value.size() - 2);

            std::vector<std::string> valueSegments;
            int countOpen = 0, countClose = 0;
            bool append = false;

            for (int i = 0, j = 0; i < slicedArray.size(); ++i) {
                if (slicedArray.at(i) == '{' || slicedArray.at(i) == '[') {
                    countOpen++;
                } else if (slicedArray.at(i) == '}' || slicedArray.at(i) == ']') {
                    countClose++;
                    if (countOpen == countClose) {
                        append = true;
                    }
                } else if (slicedArray.at(i) == ',' && append) {
                    valueSegments.push_back(slicedArray.substr(j, i - j));
                    j = i + 1;
                    append = false;
                    countOpen = 0;
                    countClose = 0;
                }
                if (append && i + 1 == slicedArray.size()) {
                    valueSegments.push_back(slicedArray.substr(j, i - j + 1));
                }
            }

            if(valueSegments.empty())valueSegments.push_back(slicedArray);

            for (const std::string &v : valueSegments) {
                values.push_back(stringToJson(v));
            }

            valueJson.setArray(values);
        } else if (firstChar == '{') { // Convert object
            std::string slicedObject = value.substr(1, value.size() - 2);
            std::map<std::string, JsonValue> objectValue;
            std::vector<std::string> valueSegments;
            int countOpen = 0, countClose = 0;
            bool append = false;

            for (int i = 0, j = 0; i < slicedObject.size(); ++i) {
                if (slicedObject.at(i) == '{' || slicedObject.at(i) == '[') {
                    countOpen++;
                } else if (slicedObject.at(i) == '}' || slicedObject.at(i) == ']') {
                    countClose++;
                    if (countOpen == countClose) {
                        append = true;
                    }
                } else if (slicedObject.at(i) == ',' && append) {
                    valueSegments.push_back(slicedObject.substr(j, i - j));
                    j = i + 1;
                    append = false;
                    countOpen = 0;
                    countClose = 0;
                }
                if (append && i + 1 == slicedObject.size()) {
                    valueSegments.push_back(slicedObject.substr(j, i - j + 1));
                }
            }

            if(valueSegments.empty())valueSegments.push_back(slicedObject);

            for (const std::string &v : valueSegments) {
                size_t colonPos = v.find(':');
                if (colonPos != std::string::npos) {
                    std::string key = v.substr(0, colonPos);
                    std::string val = v.substr(colonPos + 1, v.size()-colonPos-1);
                    
                    char firstValChar = value.empty() ? '\0' : value[0];
                    char lastValChar = value.empty() ? '\0' : value[val.size()-1];

                    key=key.substr(1, key.size()-2);
                    if(firstValChar=='\"' && lastValChar=='\"'){
                        val=val.substr(1, val.size()-3);
                    }

                    objectValue.insert({key,stringToJson(val)});
                }
            }
            valueJson.setObject(objectValue);
        } else if (firstChar == '\"') { // Convert string
            std::string stringValue = value.substr(1, value.size() - 2);
            valueJson.setString(stringValue);
        } else if (firstChar == 't' || firstChar == 'f') { // Convert boolean
            valueJson.setBoolean(value == "true");
        }else { // Convert number
            valueJson.setNumber(std::stod(value));
        }

        return valueJson;
    }
}