#include <iostream>
#include <map>
#include <string>
#include <ctime>
#include <vector>

using namespace std;

class Entity {            // ����������� ������ Entity (��������)
public:                   // ���������� �������� ������ ������
    string name;          // ��� ��������
    time_t created;       // ����� �������� ��������
    time_t modified;      // ����� ���������� ��������� ��������
    string m_parentPath;
    Entity(const string& name) : name(name), created(time(NULL)), modified(time(NULL)) {}
    // �����������, ������� �������������� ��� � ����� �������� � ���������
    virtual ~Entity() {}  // ����������, ������� ������ ����� �����������
    virtual size_t size() const = 0;   // ����� ����������� ����� �������� ������� ��������
    virtual size_t count() const = 0; // ����� ����������� ����� �������� ���������� ���������
    virtual bool isFolder() const = 0; // ���������� ������������ ������
    const string& getName() const { return name; } // ���������� ���
    time_t getCreated() const { return created; } //���� ��������
    time_t getModified() const { return modified; } //���� ����������� 
    virtual void setName(const string& new_name) { 
        name = new_name; modified = time(NULL); 
    }
    string getParentPath() const { return m_parentPath; }
    void setParentPath(const string& newPath) { m_parentPath = newPath; }
};

class File : public Entity {         // ����������� ������ File, ��������������� �� Entity
private:                             // ���������� �������� ������ ������
    size_t m_size;                   // ������ �����
public:                              // ���������� �������� ������ ������
    File(const string& name, size_t size) : Entity(name), m_size(size) {}
    // �����������, ������� �������������� ��� � ������ �����
    virtual ~File() {}               // ����������
    virtual size_t size() const { return m_size; }
    // ���������� ������ �����
    virtual size_t count() const { return 1; }
    // ���������� �������, ��� ��� ��� ����
    virtual bool isFolder() const override { return false; } // ���������� ������ ��� ������ File
    // ����� ��� ��������� ������� �����
    virtual void modify(size_t newSize) { m_size = newSize; }

};

class Folder : public Entity {       // ����������� ������ Folder, ��������������� �� Entity
private:                             // ���������� �������� ������ ������
    map<string, Entity*> m_contents; // ���������� ����� (������������� ������)
    size_t m_size;                   // ������ �����
public:                              // ���������� �������� ������ ������
    Folder(const string& name) : Entity(name), m_size(0) {}
    // �����������, ������� �������������� ��� ����� � �� ������
    virtual ~Folder() {              // ����������
        for (auto& entity : m_contents) {  // ������
            if (entity.second != nullptr) {
                delete entity.second;// �������� ��������
            }
        }
    }
    virtual bool isFolder() const override { return true; } // ���������� ������ ��� ������ Folder
    virtual size_t size() const { return m_size; }              // �������, ������� ���������� ������ �����
    virtual size_t count() const { return m_contents.size(); }  // �������, ������� ���������� ���������� ��������� � �����
    Entity* get_f(const string& name) {                           // �������, ������� ���������� ��������� �� �������� �� �����
        auto it = m_contents.find(name);
        if (it != m_contents.end()) {
            return it->second;
        }
        return nullptr;
    }
    void createFolder_f(const string& name) {                     // �������, ������� ������� �����
        auto folder = new Folder(name);
        folder->setName(name); // ������������� ��� ����������� �����
        folder->setParentPath(getParentPath());// ������������� ������������ ����
        m_contents[name] = folder;
        m_size += folder->size();
        modified = time(NULL);
    }
    void createFile_f(const string& name, size_t size) {          // �������, ������� ������� ����
        auto file = new File(name, size);
        file->setName(name); // ������������� ��� ������������ �����
        file->setParentPath(getParentPath()); // ������������� ������������ ����
        m_contents[name] = file;
        m_size += file->size();
        modified = time(NULL);
    }
    void remove_f(Entity* entity) {                               // �������, ������� ������� �������� (���� ��� �����)
        auto it = m_contents.find(entity->name);
        if (it != m_contents.end()) {
            m_size -= entity->size();
            modified = time(NULL);
            m_contents.erase(it);
            delete entity;
        }
    }
    void add_f(Entity* entity) {                                 // �������, ������� ��������� �������� (���� ��� �����)
        auto it = m_contents.find(entity->name);
        if (it != m_contents.end()) { // ���� �������� � ����� ������ ��� ����������
            throw invalid_argument("Entity with the same name already exists");
        }
        m_contents[entity->name] = entity;
        m_size += entity->size();
    }
    size_t getM_Size()const {
        return m_size;
    }
    void setM_Size(size_t size) {
        m_size = size;
    }
    //void rename_f(const string& name, const string& new_name) {
    //    auto entity = get_f(name);
    //    if (entity) {
    //        entity->name = new_name;
    //        entity->modified = time(NULL);
    //    }
    //}
    void rename_f(const string& name, const string& new_name) {
        auto entity = get_f(name);
        if (entity) {
            entity->name = new_name;
            entity->modified = time(NULL);
            // ������� ������ ������� �� ����
            m_contents.erase(name);
            // �������� ����� ������� � ����� ������ � ������ ���������
            m_contents.insert(make_pair(new_name, entity));
        }
    }

    const map<string, Entity*>& contents() const {
        return m_contents;
    }
};

class FileSystem {
private:
    Folder* m_root;
public:
    FileSystem() {
        m_root = new Folder("/");
    }
    virtual ~FileSystem() {
        delete m_root;
    }
    // ����� ��� ��������� �������� (����� ��� �����) �� ����
    Entity* get(const string& path) {
        auto parts = splitPath(path); // ��������� ���� �� �����
        Entity* current = m_root;      // ��������� ������� �������� - �������� �����   
        for (const auto& part : parts) {  // �������� �� ���� ������ ����
            if (!part.empty() && current) {// ���� ������� ����� �� ����� � ���������� ������� 
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
    Entity* get(const string& path, string name) {
        return get(path + "/" + name);
    }

    Entity* getRoot() {
        return m_root;
    }
        // ����� ��� ��������� ������� �����
    void modify(const string& filePath, size_t newSize) {
        auto parts = splitPath(filePath); // ��������� ���� �� �����
        auto curr = m_root;
        for (size_t i = 0; i < parts.size(); i++) { // �������� �� ���� ������ ����
            if (curr == nullptr) { // ���� ������� ���������� ����� ����, ������ ���� ����������
                cout << "Invalid path: " << filePath << endl;
                return;
            }
            auto entity = curr->get_f(parts[i]); // �������� �������� �� ������� ����������
            if (i == parts.size() - 1) { // ���� ��� ��������� ������� ����
                auto file = dynamic_cast<File*>(entity); // �������� �������� � ���� File
                if (file == nullptr) { // ���� �������� �� �������� ������, ������ ���� ����������
                    cout << "Invalid path: " << filePath << endl;
                    return;
                }
                auto oldSize = file->size(); // �������� ������� ������ �����
                file->modify(newSize); // �������� ������ �����
                curr->setM_Size(curr->size() + newSize - oldSize) ; // ��������� ������ �����, ���������� ����
                curr->modified = time(NULL); // ��������� ����� ����������� �����
                return;
            }
            curr = dynamic_cast<Folder*>(entity); // �������� �������� � ���� Folder � ��������� � ���
        }
        cout << "Invalid path: " << filePath << endl; // ���� �� ����� �� ����� ����, ������ ���� ����������
    }
    void createFolder(const string& path, const string& name) {
        auto parts = splitPath(path);
        Entity* current = m_root;
        for (const auto& part : parts) {// �������� �� ���� ������ ����
            if (!part.empty() && current) { // ���� ������� ����� �� ����� � ���������� ������� ��������, 
                //�� �������� ��������� ��������
                current = dynamic_cast<Folder*>(current)->get_f(part);
            }
        }
        if (current) { // ���� ���������� ������� ��������, �� ������� ����� �����
            dynamic_cast<Folder*>(current)->createFolder_f(name);
        }
    }
    void createFile(const string& path, const string& name, size_t size) {
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
    void remove(const string& path) {
        auto parts = splitPath(path);
        Entity* current = m_root;
        for (size_t i = 0; i < parts.size() - 1; i++) {// �������� �� ���� ������ ����, ����� ���������,
            //� �������� ��������� ������� ��������
            if (!parts[i].empty() && current) {
                current = dynamic_cast<Folder*>(current)->get_f(parts[i]);
            }
        }
        if (current) {// ���� ��������� ������� �������� ����������, �� ������� ��������, 
            //������� ����� �������, � ������� ��
            Entity* toRemove = dynamic_cast<Folder*>(current)->get_f(parts.back());
            if (toRemove) {
                dynamic_cast<Folder*>(current)->remove_f(toRemove);
            }
        }
    }

    void rename(const string& path, const string& name) {
        auto entity = get(path); // �������� �������� �� ����
        if (entity) { // ���� �������� ����������, �� �������� �� ���
            entity->setName(name);
        }
    }


    
    void move(Entity* entity, const string& newPath) {
        auto parts = splitPath(newPath);
        auto new_name = parts.back();
        parts.pop_back();
        string new_parent_path = "";
        for (const auto& part : parts) {
            new_parent_path += "/" + part;
        }
        auto parent = dynamic_cast<Folder*>(get(new_parent_path));
        if (!parent) {
            throw invalid_argument("Invalid path: " + new_parent_path);
        }
        auto old_parent_path = entity->getParentPath();
        if (!old_parent_path.empty()) {
            auto old_parent = dynamic_cast<Folder*>(get(old_parent_path));
            if (old_parent) {
                old_parent->remove_f(entity); // ������� �������� �� ������ �����
            }
        }
        entity->setParentPath(new_parent_path); // ������������� ����� ����
        parent->add_f(entity); // ��������� �������� � ����� �����
        entity->setName(new_name); // �������� ��� ��������
    }


    //void move(const string& currentPath, const string& newPath) {
    //    try {
    //        Entity* entity = m_root->get_f(currentPath);
    //        if (!entity) {
    //            throw invalid_argument("Invalid entity path");
    //        }

    //        Entity* newParent = m_root->get_f(newPath);
    //        if (!newParent || !newParent->isFolder()) {
    //            throw invalid_argument("Invalid destination path");
    //        }

    //        Entity* currentParent = m_root->get_f(entity->getParentPath());
    //        if (!currentParent || !currentParent->isFolder()) {
    //            throw invalid_argument("Entity is not contained in a folder");
    //        }

    //        if (newParent == currentParent) {
    //            return;
    //        }

    //        dynamic_cast<Folder*>(currentParent)->remove_f(entity);

    //        dynamic_cast<Folder*>(newParent)->add_f(entity);
    //        entity->setParentPath(newPath);
    //        entity->modified = time(NULL);
    //    }
    //    catch (const std::invalid_argument& e) {
    //        // ��������� ����������, ��������:
    //        std::cerr << "Error: " << e.what() << std::endl;
    //        return;
    //    }

    //}   
    Entity* findEntity(string path) {
        vector<string> pathElements = splitPath(path);
        Entity* currentEntity = m_root;
        for (int i = 0; i < pathElements.size(); i++) {
            bool found = false;
            for (auto& entity : dynamic_cast<Folder*>(currentEntity)->contents()) {
                if (entity.second->getName()  == pathElements[i]) {
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

    void printFolderStructure(const std::string& path) {
        // ������� ����� �� ��������� ����
        Entity* entity = findEntity(path);
        if (!entity || !entity->isFolder()) {
            std::cout << "Error: the path is invalid or does not point to a folder\n";
            return;
        }
        Folder* folder = static_cast<Folder*>(entity);

        // ������� ��������� �����
        printFolderStructure(folder, 0);
    }



private:
    vector<string> splitPath(const string& path) {
        vector<string> parts;
        size_t start = 0;
        size_t end = 0;
        // ���� ����������� � ������ ���� � �������� �����
        while ((end = path.find("/", start)) != string::npos) {
            if (end != start) {
                parts.push_back(path.substr(start, end - start));
            }
            start = end + 1;
        }
        // ��������� ��������� ����� ����, ���� ��� ����
        if (start != path.length()) {
            parts.push_back(path.substr(start));
        }
        return parts;
    }

    void printFolderStructure(Folder* folder, int level) {
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
};

int main() {
    FileSystem fs;
    fs.createFolder("/", "folder1");
    fs.createFolder("/", "folder2");
    fs.createFolder("/folder1", "subfolder");
    fs.createFile("/folder1/subfolder", "file1", 1024);
    fs.createFile("/folder2", "file2", 2048);
    Entity* folder1 = fs.get("/folder1");
    Entity* subfolder = fs.get("/folder1/subfolder");
    //Entity* file1 = fs.get("/folder1/subfolder/file1");
    Entity* file2 = fs.get("/folder2/file2");
    cout << "------------------------ Getiing Folder by Name -----------------------\n" << endl;
  
    Entity* file1 = fs.get("/folder1/subfolder", "file1");
    cout << "Getting file/folder by name " << file1 << endl;
    cout << "----------------------- Getting root -----------------------\n" << endl;

    cout << "Root Folder " << fs.getRoot() << endl;

    cout << "----------------------- Checking is Folder or File -----------------------\n" << endl;

    cout << "obj1 is a file: " << file1->isFolder() << std::endl;

    cout << "----------------------- Printing Name of the Folder -----------------------\n" << endl;

    cout << "Name of folder1: " << folder1->getName() << endl;

    cout << "----------------------- Changing size -----------------------\n" << endl;

    cout << "subfolder size(file1): " << subfolder->size() << endl;
    fs.modify("/folder1/subfolder/file1", 2048);
    cout << "subfolder size(file1): " << subfolder->size() << endl;

    cout << "----------------------- Renaming Folder/File -----------------------\n" << endl;

    fs.printFolderStructure("/");
    fs.rename("/folder1/subfolder/file1", "file1.1");
    Entity* newfile = fs.get("/folder1/subfolder/file1");   //������-�� �� ��������� ��������
    cout << "Changed name: " << newfile->getName() << endl;
    fs.printFolderStructure("/");

    cout << "----------------------- Moving Folder/File -----------------------\n" << endl;

    fs.createFile("/folder1/subfolder", "file1_new", 2048);
    Entity* file1_new = fs.get("/folder1/subfolder", "file1_new");
    cout << "Getting file/folder by name " << file1_new << endl;
  //  // Move file1 from /folder1/subfolder to /folder2
  //  fs.move("/folder1/subfolder/file1_new", "/folder2");
  //  // Get file1 entity at the new location
  //  Entity* movedFile = fs.get("/folder2", "file1_new");
  /////   Check that the entity was moved correctly
  //  if (movedFile) {
  //      cout << "File1 moved successfully to " << movedFile->getParentPath() << endl;
  //  }
  //  else {
  //      cout << "Failed to move file1" << endl;
  //  }
    
    // Move the subfolder entity from folder1 to folder2
    //fs.move(file1_new, "/folder2");

    //// Get the new location of subfolder after the move
    //Entity* newSubfolderLocation = fs.get("/folder2/subfolder");
    fs.printFolderStructure("/");

    cout << "----------------------- Time when was Created/Modified -----------------------\n" << endl;
    time_t createdTime = folder1->getCreated();
    char buffer[26];
    ctime_s(buffer, sizeof buffer, &createdTime);
    cout << "Time when was created: " << buffer << endl;
    //cout << "Time when created: " << ctime(&createdTime) << endl;

    cout << "----------------------- Printing all stucture -----------------------\n" << endl;

    fs.printFolderStructure("/");

    cout << "----------------------- Printing size -----------------------\n" << endl;
    cout << "Folder 1 size: " << folder1->size() << endl;
    cout << "Subfolder size: " << subfolder->size() << endl;
    cout << "File 1 size: " << file1->size() << endl;
    cout << "File 2 size: " << file2->size() << endl;
    cout << "Folder 1 count: " << folder1->count() << endl;
    cout << "Subfolder count: " << subfolder->count() << endl;
    fs.printFolderStructure("/");

    fs.remove("/folder2/file2");
    Entity* deletedFile = fs.get("/folder2/file2");
    if (!deletedFile) {
        cout << "File 2 deleted successfully!" << endl;
    }
    fs.printFolderStructure("/");

    return 0;
}

