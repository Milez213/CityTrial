
/*
 * GameSettings.h
 * Author: Mustafa khafateh
 * 
 * Date: 2/6/2013
 *
 * Stores game options/settings, such as if playing music, screen resolution,
 * etc.
 *
 *
 * load: settings.load("settings file.ini")
 *  
 *
 *  settings file has the format:
 *
 *  property = value
 *
 *  property is a string, value is an int now.
 *
 *  
 * to get properties
 * settings["property"]
 * 
 * if it doesn't exist, it returns -1
 *
 *
 */
 
#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

#include <map>
#include <string>

#include <stdio.h>

#include <iostream>
#include <fstream>

using std::map;
using std::string;


typedef int propertyType;

class GameSettings {

public:

    // empty settings
    GameSettings() {
        m_settings["one"] = 1; // for sanity checking
        m_isLoaded = false;
    }

    // load game settings
    GameSettings(const char* fname) {
        m_isLoaded = load(fname);
    }

    bool save(const char* fname);

    bool load(const char* fname) {
        ifstream in(fname, ios::in);

        if (!in) {
            fprintf(stderr, "Couldn't open %s for reading the map\n", fname);
            return false;
        } else {
            string key;
            propertyType value;
            string equals;

            while (!in.eof()) {
                in >> key >> equals  >> value;
                m_settings[key] = value;
            }

        }
        in.close();
        return true;
    }


    // returns element. -1 if it doesn't exist
    propertyType get(string property) {

        map<string, propertyType>::iterator it;

        it = m_settings.find(property);

        if (it == m_settings.end()) {
            fprintf(stderr, "GameSettings.get(): couldn't find property '%s'\n",
                    property.c_str());
            return -1;
        }
        return it->second;
    }

    propertyType operator[] (const string& key) {
        return get(key);
    }
    

private:

    bool m_isLoaded;

    map<string, propertyType> m_settings;

};

#endif
