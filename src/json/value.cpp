#include "value.hpp"
#include <stdio.h>
#include <iostream>

namespace JETPP
{

    // Constructors
    JsonValue::JsonValue()
    {
    }

    JsonValue::JsonValue(const char *value)
    {
        setString(value);
    }
    JsonValue::JsonValue(double value)
    {
        setNumber(value);
    }
    JsonValue::JsonValue(bool value)
    {
        setBoolean(value);
    }

    // Methods to set values
    void JsonValue::setString(const std::string &value)
    {
        type = STRING;
        asString = value;
    }

    void JsonValue::setNumber(double value)
    {
        type = NUMBER;
        asNumber = value;
    }

    void JsonValue::setBoolean(bool value)
    {
        type = BOOLEAN;
        asBoolean = value;
    }

    void JsonValue::setObject(const std::map<std::string, JsonValue> &value)
    {
        type = OBJECT;
        asObject = value;
    }

    void JsonValue::setArray(const std::vector<JsonValue> &value)
    {
        type = ARRAY;
        asArray = value;
    }

    // Method to convert JSON value to JSON string
    std::string JsonValue::toJsonString() const
    {
        switch (type)
        {
        case OBJECT:
        {
            std::string jsonString = "{";
            bool first = true;
            for (const auto &pair : asObject)
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
        case ARRAY:
        {
            std::string jsonString = "[";
            for (size_t i = 0; i < asArray.size(); ++i)
            {
                if (i > 0)
                {
                    jsonString += ",";
                }
                jsonString += asArray[i].toJsonString();
            }
            jsonString += "]";
            return jsonString;
        }
        case STRING:
        {
            return "\"" + asString + "\"";
        }
        case NUMBER:
            return std::to_string(asNumber);
        case BOOLEAN:
            return asBoolean ? "true" : "false";
        case NULL_VALUE:
            return "null";
        default:
            return ""; // Handle unrecognized type
        }
    }

} // namespace JETPP
