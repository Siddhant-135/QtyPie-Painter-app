#include "Svg2Data.h"
#include <cctype>

void SvgParser::ParseXml(const std::string& data, std::function<void(std::string, AttrMap)> Parse_Tag) {
    enum State { FIND_NEXT, GET_NAME, GET_KEY, GET_VAL }; // FIND_NEXT looks for < to FIND_NEXT parsing, GET_NAME reads the tag name, GET_KEY reads attribute keys, GET_VAL reads attribute values (expects them to be quoted)
    State state = FIND_NEXT;
    std::string buffer, tag, key; // buffer is where the characters are accumulated in, tag is the current tag name, key is the current attribute key being read
    AttrMap attrs; // attributes are stored as a map for easy lookup + order invariance
    bool quoted = false; // counter for whether we have reached end of "" or not.

  for (char c : data) {
    if (c == '<') // FIRST move from FIND_NEXT state to actual parsing
    {
      attrs.clear(); buffer.clear();
      state = GET_NAME;
      continue;
    }
    if (state == GET_NAME)
    {
      if (std::isspace(static_cast<unsigned char>(c))) // Whitespace
      {
        tag = buffer;
        buffer.clear();
        state = GET_KEY;
      }
      else if (c == '>')
      {
        tag = buffer;
        buffer.clear();
        Parse_Tag(tag, attrs);
        state = FIND_NEXT;
      }
      else 
      {
        buffer += c;
      }
    } 
    else if (state == GET_KEY) 
    {
      if (c == '=') 
      {
        key = buffer;
        buffer.clear();
        state = GET_VAL;
        quoted = false;
      } 
      else if (std::isspace(static_cast<unsigned char>(c))) 
      {
        continue; // Ignore whitespace after key till =
      }
      else if (c == '>') // End of tag, move back to FIND_NEXT state and callback with the tag + attributes read
      {
        Parse_Tag(tag, attrs);
        state = FIND_NEXT;
      } 
      else
      {
        buffer += c;
      }
    } 
    else if (state == GET_VAL) 
    {
        if (c == '"') // c = double quotes or not
        {
            if (quoted) 
            {
                attrs[key] = buffer;
                buffer.clear();
                state = GET_KEY;
                quoted = false;
            }
            else {quoted = true;}
        }
        else if (quoted) 
        {
            buffer += c;
        }
        else continue;
    }
  }
}
