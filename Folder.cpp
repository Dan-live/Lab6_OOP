#include "Folder.hpp"

// constructor that initializes the folder name and size

Folder::~Folder() {              // destructor
    for (auto& entity : m_contents) {  // Passage
        if (entity.second != nullptr) {
            delete entity.second;// entity deletion
        }
    }
}

bool Folder::isFolder() const { return true; }

// method implementation for the Folder class

size_t Folder::size() const { return m_size; }

// The function that returns the size of the folder

size_t Folder::count() const { return m_contents.size(); }

// The function that returns the number of items in the folder

Entity* Folder::get_f(const string& name) {                           // A function that returns a pointer to an entity by name
    auto it = m_contents.find(name);
    if (it != m_contents.end()) {
        return it->second;
    }
    return nullptr;
}

void Folder::createFolder_f(const string& name) {                     // The function that creates the folder
    auto folder = new Folder(name);
    folder->setName(name); // set the name of the folder to be created
    folder->setParentPath(getParentPath());// set the parent path
    m_contents[name] = folder;
    m_size += folder->size();
    modified = time(NULL);
}

void Folder::createFile_f(const string& name, size_t size) {          //  The function that creates the file
    auto file = new File(name, size);
    file->setName(name); // Set the name of the file to be created
    file->setParentPath(getParentPath()); // set the parent path
    m_contents[name] = file;
    m_size += file->size();
    modified = time(NULL);
}

void Folder::remove_f(Entity* entity) {                               // Function that deletes an entity (file or folder)
    auto it = m_contents.find(entity->name);
    if (it != m_contents.end()) {
        m_size -= entity->size();
        modified = time(NULL);
        m_contents.erase(it);
        delete entity;
    }
}

void Folder::add_f(Entity* entity) {                                 // Function that adds an entity (file or folder)
    auto it = m_contents.find(entity->name);
    if (it != m_contents.end()) { // if an entity with this name already exists
        throw invalid_argument("Entity with the same name already exists");
    }
    m_contents[entity->name] = entity;
    m_size += entity->size();
}

size_t Folder::getM_Size() const {
    return m_size;
}

void Folder::setM_Size(size_t size) {
    m_size = size;
}

void Folder::rename_f(const string& name, const string& new_name) {
    auto entity = get_f(name);
    if (entity) {
        entity->name = new_name;
        entity->modified = time(NULL);
        // remove the old item from the map
        m_contents.erase(name);
        // add a new element with the new key and the old value
        m_contents.emplace(new_name, entity);
    }
}

void Folder::move_f(const string& name, Entity* entity) {
    auto it = m_contents.find(name);
    if (it != m_contents.end()) {
        m_contents.erase(it);  // Removing an entity from the old location
        m_size -= entity->size();
        modified = time(NULL);

    }

}

const map<string, Entity*>& Folder::contents() const {
    return m_contents;
}
