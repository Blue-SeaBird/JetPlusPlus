#include "jetplusplus/json/jsonConverter.hpp"

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

        JETPP::JsonValue JsonConverter::stringToJson(std::string value){
            JETPP::JsonValue valueJson;
            char firstChar = value.empty() ? '\0' : value[0];

            if(firstChar=='['){//convert array
                std::vector<JETPP::JsonValue> values;
                std::string slicedArray=value.substr(1, value.size()-2);

                std::vector<std::string> valueSegments;
                splitString(slicedArray, valueSegments, ',');

                for(std::string v: valueSegments){
                    values.push_back(stringToJson(v));
                }
                valueJson.setArray(values);

            }else if(firstChar=='{'){//convert object
                std::string slicedObject=value.substr(1, value.size()-2);

                std::map<std::string, JsonValue> objectValue;
                
                std::vector<std::string> fieldsSegments;
                splitString(slicedObject, fieldsSegments, ',');

                for(std::string v: fieldsSegments){
                    std::vector<std::string> fieldSegments;
                    std::cout << "to splitt object: " << v << std::endl;
                    splitString(v, fieldSegments, ':');

                    for(int i=0;i<fieldSegments.size()-1;i++){
                        std::string fieldName = fieldSegments[i];
                        std::string valueName = fieldSegments[i+1];

                        fieldName.erase(std::remove(fieldName.begin(), fieldName.end(), '\"'), fieldName.end());
                        valueName.erase(std::remove(valueName.begin(), valueName.end(), '\"'), valueName.end());

                        std::cout << "Field: " << fieldName << ", Value: " << valueName << std::endl;

                        objectValue.insert({fieldName,stringToJson(valueName)});
                    }
                }
                valueJson.setObject(objectValue);
            }else{//convert field type
                std::istringstream iss(value);
                double number;
                iss >> number;

                if(value=="true"){
                    valueJson.setBoolean(true);
                    std::cout << "bool" << std::endl;
                }else if(value=="false"){
                    valueJson.setBoolean(false);
                    std::cout << "bool" << std::endl;
                }else if(!iss.fail() && iss.eof()){
                    valueJson.setNumber(number);
                    std::cout << "number" << std::endl;

                }else{
                    valueJson.setString(value);
                    std::cout << "string" << std::endl;
                }
            }
            return valueJson;
        }

        void JsonConverter::splitString(std::string str, std::vector<std::string> &segments, char delimiter){
            std::istringstream isstream(str);
            std::string segment;
            while (std::getline(isstream, segment, delimiter))
            {
                segments.push_back(segment);
            }
        }
    }