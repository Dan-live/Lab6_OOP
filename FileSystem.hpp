#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_

#include "Entity.hpp"
#include "Folder.hpp"
#include "File.hpp"
#include <vector>
using namespace std;


class FileSystem {
private:
    Folder* m_root;
public:
    FileSystem();
    virtual ~FileSystem();
    // A method to get the entity (folder or file) from the path
    Entity* get(const string& path);
    Entity* get(const string& path, string name);
    Entity* getRoot();
    // Method for resizing a file
    void modify(const string& filePath, size_t newSize);
    void createFolder(const string& path, const string& name);
    void createFile(const string& path, const string& name, size_t size);
    void remove(const string& path);
    void rename(const string& path, const string& old_name, const string& new_name);
    void move(const string& newPath, const string& currentPath);
    Entity* findEntity(string path);
    void printFolderStructure(const std::string& path);

private:
    vector<string> splitPath(const string& path);

    void printFolderStructure(Folder* folder, int level);
};


#endif