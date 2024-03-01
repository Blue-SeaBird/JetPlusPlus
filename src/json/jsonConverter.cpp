#include "../../include/jetplusplus/json/jsonConverter.hpp"

namespace JETPP{    

        JsonConverter::JsonConverter(){

        }
        std::string JsonConverter::jsonToString(JETPP::JsonValue value) const{

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

        JETPP::JsonValue JsonConverter::stringToJson(std::string value) const{
            JETPP::JsonValue valueJson;

            if(value.find("[",0)){//convert array
                std::vector<JETPP::JsonValue> values;
                std::string slicedValue=value.substr(1, value.size()-2);

                std::vector<std::string> valueSegments;
                splitString(slicedValue, valueSegments, ',');

                for(std::string v: valueSegments){
                    values.push_back(stringToJson(v));
                }
                valueJson.setArray(values);

            }else if(value.find("{",0) || value.find(" {",0)){//convert object
                std::map<std::string, JsonValue> objectValue;
                
                std::vector<std::string> fieldsSegments;
                splitString(value, fieldsSegments, ',');

                for(std::string v: fieldsSegments){
                    std::vector<std::string> fieldSegments;

                    splitString(v, fieldSegments, ':');

                    for(int i=0;i<fieldSegments.size();i+=2){
                        objectValue.insert({fieldSegments[i],stringToJson(fieldSegments[i+1])});
                    }
                }
                valueJson.setObject(objectValue);
            }else{//convert field type
                std::istringstream iss(value);
                double number;
                iss >> number;

                if(value[0]=='t'){
                    valueJson.setBoolean(true);
                }else if(value[0]=='f'){
                    valueJson.setBoolean(false);
                }else if(!iss.fail() && iss.eof()){
                    valueJson.setNumber(number);
                }else{
                    valueJson.setString(value);
                }
            }
            return valueJson;
        }

        void JsonConverter::splitString(std::string str, std::vector<std::string> &segments, char delimiter) const{
            std::istringstream isstream(str);
            std::string segment;
            while (std::getline(isstream, segment, delimiter))
            {
                segments.push_back(segment);
            }
        }
    }