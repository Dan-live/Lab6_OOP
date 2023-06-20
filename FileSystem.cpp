#include "FileSystem.hpp"

FileSystem::FileSystem() {
    m_root = new Folder("/");
}

FileSystem::~FileSystem() {
    delete m_root;
}

// A method to get the entity (folder or file) from the path

Entity* FileSystem::get(const string& path) {
    auto parts = splitPath(path); // Partitioning the path
    Entity* current = m_root;      // Specify the current entity - the root folder   
    for (const auto& part : parts) {  // Going all the way through
        if (!part.empty() && current) {// If the current part is not empty and there is a current 
            Folder* current_folder = dynamic_cast<Folder*>(current);
            if (current_folder)
            {
                current = current_folder->get_f(part);
            }
            else
            {
                return nullptr;
            }
        }
    }
    return current;
}

Entity* FileSystem::get(const string& path, string name) {
    return get(path + "/" + name);
}

Entity* FileSystem::getRoot() {
    return m_root;
}

// Method for resizing a file

void FileSystem::modify(const string& filePath, size_t newSize) {
    auto parts = splitPath(filePath); // Partitioning the path
    auto curr = m_root;
    for (size_t i = 0; i < parts.size(); i++) { // Going all the way through
        if (curr == nullptr) { // If the current directory is zero, then the path is invalid
            cout << "Invalid path: " << filePath << endl;
            return;
        }
        auto entity = curr->get_f(parts[i]); // Retrieve an entity from the current directory
        if (i == parts.size() - 1) { // If this is the last element of the path
            auto file = dynamic_cast<File*>(entity); // Reduce the entity to the File type
            if (file == nullptr) { // If the entity is not a file, then the path is invalid
                cout << "Invalid path: " << filePath << endl;
                return;
            }
            auto oldSize = file->size(); // Get the current file size
            file->modify(newSize); // Changing the file size
            curr->setM_Size(curr->size() + newSize - oldSize); // Update the size of the folder containing the file
            curr->modified = time(NULL); // Updating the modification time of the folder
            return;
        }
        curr = dynamic_cast<Folder*>(entity); // We convert the entity to the Folder type and navigate to it
    }
    cout << "Invalid path: " << filePath << endl; // If we have reached the end of the path, then the path is invalid
}

void FileSystem::createFolder(const string& path, const string& name) {
    auto parts = splitPath(path);
    Entity* current = m_root;
    for (const auto& part : parts) {// Going all the way through
        if (!part.empty() && current) { // If the current part is not empty and there is a current entity, 
            //then we get the following entity
            current = dynamic_cast<Folder*>(current)->get_f(part);
        }
    }
    if (current) { // If there is a current entity, then create a new folder
        dynamic_cast<Folder*>(current)->createFolder_f(name);
    }
}

void FileSystem::createFile(const string& path, const string& name, size_t size) {
    auto parts = splitPath(path);
    Entity* current = m_root;
    for (const auto& part : parts) {
        if (!part.empty() && current) {
            current = dynamic_cast<Folder*>(current)->get_f(part);
        }
    }
    if (current)
    {
        dynamic_cast<Folder*>(current)->createFile_f(name, size);
    }
}

void FileSystem::remove(const string& path) {
    auto parts = splitPath(path);
    Entity* current = m_root;
    for (size_t i = 0; i < parts.size() - 1; i++) {// Go through all but the last part of the path, 
        //and get the last current entity
        if (!parts[i].empty() && current) {
            current = dynamic_cast<Folder*>(current)->get_f(parts[i]);
        }
    }
    if (current) {// If the last current entity exists, find the entity, 
        //that you want to remove and delete it
        Entity* toRemove = dynamic_cast<Folder*>(current)->get_f(parts.back());
        if (toRemove) {
            dynamic_cast<Folder*>(current)->remove_f(toRemove);
        }
    }
}

void FileSystem::rename(const string& path, const string& old_name, const string& new_name) {
    auto parts = splitPath(path);
    Entity* current = m_root;
    for (size_t i = 0; i < parts.size() - 1; i++) {// Go through all but the last part of the path,
        //and get the last current entity
        if (!parts[i].empty() && current) {
            current = dynamic_cast<Folder*>(current)->get_f(parts[i]);
        }
    }
    if (current) {// If the last current entity exists, find the entity,
        //that you want to remove and delete it
        dynamic_cast<Folder*>(current)->rename_f(old_name, new_name);
    }
}

 void FileSystem::move(const string& newPath, const string& currentPath) {
    // We get the entity you want to move
    Entity* entity = get(currentPath);
    if (!entity) {
        cout << "Invalid current path: " << currentPath << endl;
        return;
    }

    // Get the parent folder of the new path
    size_t pos = newPath.find_last_of('/');
    if (pos == string::npos) {
        cout << "Invalid new path: " << newPath << endl;
        return;
    }
    string newParentPath = newPath.substr(0, pos);

    // Get a new name for the entity
    string newName = newPath.substr(pos + 1);

    // Check that the new parent folder exists
    Entity* newParent = get(newParentPath);
    if (!newParent || !newParent->isFolder()) {
        cout << "Invalid new parent path: " << newParentPath << endl;
        return;
    }

    Folder* newParentFolder = dynamic_cast<Folder*>(newParent);

    // Check that the new name is not occupied by another entity in the parent folder
    if (newParentFolder->get_f(newName)) {
        cout << "An entity with the same name already exists in the new parent folder" << endl;
        return;
    }

    // Check that moving inside the current folder does not happen
    if (currentPath.substr(0, currentPath.find_last_of('/')) == newParentPath) {
        cout << "Moving within the same folder is not allowed" << endl;
        return;
    }

    // Moving an entity to a new location
    Folder* currentParent = dynamic_cast<Folder*>(get(currentPath.substr(0, currentPath.find_last_of('/'))));
    currentParent->move_f(entity->getName(), entity);

    // Updating the parent folder of the migrated entity
    entity->setParentPath(newParentPath);

    // Adding an entity to a new parent folder
    newParentFolder->add_f(entity);
}

Entity* FileSystem::findEntity(string path) {
     vector<string> pathElements = splitPath(path);
     Entity* currentEntity = m_root;
     for (int i = 0; i < pathElements.size(); i++) {
         bool found = false;
         for (auto& entity : dynamic_cast<Folder*>(currentEntity)->contents()) {
             if (entity.second->getName() == pathElements[i]) {
                 currentEntity = entity.second;
                 found = true;
                 break;
             }
         }
         if (!found) {
             return nullptr;
         }
     }
     return currentEntity;
 }

void FileSystem::printFolderStructure(const std::string& path) {
    // find the folder by the specified path
    Entity* entity = findEntity(path);
    if (!entity || !entity->isFolder()) {
        std::cout << "Error: the path is invalid or does not point to a folder\n";
        return;
    }
    Folder* folder = static_cast<Folder*>(entity);

    // display the folder structure
    printFolderStructure(folder, 0);
}



/// 
// Private
/// 


vector<string> FileSystem::splitPath(const string& path) {
    vector<string> parts;
    size_t start = 0;
    size_t end = 0;
    // Look for the separator in the path line and get the parts
    while ((end = path.find("/", start)) != string::npos) {
        if (end != start) {
            parts.push_back(path.substr(start, end - start));
        }
        start = end + 1;
    }
    // Add the last part of the path, if there is one
    if (start != path.length()) {
        parts.push_back(path.substr(start));
    }
    return parts;
}

void FileSystem::printFolderStructure(Folder* folder, int level) {
    for (auto& entity : folder->contents()) {
        for (int i = 0; i < level; ++i) {
            std::cout << "\t";
        }
        std::cout << "+ " << entity.first;
        if (entity.second->isFolder()) {
            std::cout << "/";
            printFolderStructure(static_cast<Folder*>(entity.second), level + 1);
        }
        else {
            std::cout << " (" << entity.second->size() << " bytes)";
        }
        std::cout << std::endl;
    }
}
