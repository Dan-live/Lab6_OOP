#ifndef FOLDER_H_  
#define FOLDER_H_

#include "Entity.hpp"
#include "File.hpp"
#include <map>
using namespace std;

class Folder : public Entity {       // Definition of a Folder class inherited from Entity
private:                             // declaration of closed class members
    map<string, Entity*> m_contents; // the contents of the folder (associative array)
    size_t m_size;                   // folder size
public:                              // declaring public class members
    Folder(const string& name) : Entity(name), m_size(0) {}
    // constructor that initializes the folder name and size
    virtual ~Folder();
    virtual bool isFolder() const override; // method implementation for the Folder class
    virtual size_t size() const;              // The function that returns the size of the folder
    virtual size_t count() const;  // The function that returns the number of items in the folder
    Entity* get_f(const string& name);
    void createFolder_f(const string& name);
    void createFile_f(const string& name, size_t size);
    void remove_f(Entity* entity);
    void add_f(Entity* entity);
    size_t getM_Size()const;
    void setM_Size(size_t size);

    void rename_f(const string& name, const string& new_name);
    void move_f(const string& name, Entity* entity);
    const map<string, Entity*>& contents() const;
};


#endif