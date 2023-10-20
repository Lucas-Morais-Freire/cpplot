#include "../headers/drawing.hpp"

#include <iostream>
#include <regex>
#include <algorithm>

std::list<std::string>* Drawing::init(std::string params) {
    std::list<std::string>* keys = new std::list<std::string>(_keys);
    std::list<std::string> done;
    std::regex pattern("( *\\w+ *= *([0-9]|\\.|\\-|\\w)+ *,)*( *\\w+ *= *([0-9]|\\.|\\-|\\w)+ *)");
    std::regex key_matches("\\w+ *=");
    std::regex arg_matches("= *([0-9]|\\.|\\-|\\w)+");
    std::regex kwarg_matches("\\w+ *= *([0-9]|\\.|\\-|\\w)+");
    if (std::regex_match(params, pattern)) { // if the parameter string matches the regex...
        // ...we will check for all keyword-arguments that were given.
        std::sregex_iterator kwarg_match_iter(params.begin(), params.end(), kwarg_matches);
        while (kwarg_match_iter != std::sregex_iterator()) {
            // get the position of the character just after the key:
            size_t pos = kwarg_match_iter->str().find(" ") != std::string::npos ? kwarg_match_iter->str().find(" ") : kwarg_match_iter->str().find("=");
            // extract only the key's string from the iterator and check if it is a valid key:
            std::string key = kwarg_match_iter->str().substr(0,pos);
            std::list<std::string>::iterator keys_list_iter = std::find(keys->begin(), keys->end(), key);
            if (keys_list_iter != keys->end()) { // if it is valid...
                // ...try to get the value it is supposed to be assigned to.
                // get position right before the beginning of the argument and extract the string after it:
                pos = kwarg_match_iter->str().rfind(" ") != std::string::npos ? kwarg_match_iter->str().rfind(" ") : kwarg_match_iter->str().rfind("=");
                std::string arg = kwarg_match_iter->str().substr(pos + 1);
                // now, check if it is possible to convert this string to a value and assign it to its variable:
                assign(key, arg);
                // erase that key from the list so that we don't re-assign any values to it.
                keys->erase(keys_list_iter);
            } else { // if its not a valid key, signal it to the user.
                std::cout << "\"" << key << "\" is not a valid key for this object or you have repeated it. valid keys are: ";
                std::list<std::string>::iterator last = _keys.end();
                last--;
                for (keys_list_iter = _keys.begin(); keys_list_iter != last; keys_list_iter++) {
                    std::cout << (*keys_list_iter) << ", ";
                }
                std::cout << (*keys_list_iter) << ".\n";
            }
            kwarg_match_iter++;
        }
    } else {
        std::cout << "invalid keyword-argument list format. the correct syntax is \"key1 = val1, key2 = val2, ...\"\n";
    }
    return keys;
}