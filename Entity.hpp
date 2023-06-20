#ifndef ENTITY_H_ 
#define ENTITY_H_

#include <iostream>
#include <string>
#include <ctime>
using namespace std;

class Entity {            // definition of the Entity class
public:                   // declaring public class members
    string name;          // entity name
    time_t created;       // entity creation time
    time_t modified;      // time of the last entity change
    string m_parentPath;
    Entity(const string& name) : name(name), created(time(NULL)), modified(time(NULL)) {}
    // constructor that initializes the name and time of creation and changes
    virtual ~Entity() {}  // destructor, which makes the class abstract
    virtual size_t size() const = 0;   // purely virtual method to return the size of the entity
    virtual size_t count() const = 0; // purely virtual method to return the number of entities
    virtual bool isFolder() const = 0; // adding a virtual method
    
    const string& getName() const; // return the name
    time_t getCreated() const; // creation date
    time_t getModified() const; // modification date 
    virtual void setName(const string& new_name);
    string getParentPath() const;
    void setParentPath(const string& newPath);
};

#endif