#include "FileSystem.hpp"
#include "Entity.hpp"
#include "File.hpp"
#include "Folder.hpp"

using namespace std;



int main() {
    FileSystem fs;
    fs.createFolder("/", "folder1");
    fs.createFolder("/", "folder2");
    fs.createFolder("/folder1", "subfolder");
    fs.createFile("/folder1/subfolder", "file1", 1024);
    fs.createFile("/folder2", "file2", 2048);
    Entity* folder1 = fs.get("/folder1");
    Entity* subfolder = fs.get("/folder1/subfolder");
    Entity* file2 = fs.get("/folder2/file2");
    cout << "------------------------ Getiing Folder by Name ------------------------------\n" << endl;
  
    Entity* file1 = fs.get("/folder1/subfolder", "file1");
    cout << "Getting file/folder by name " << file1 << endl;
    cout << "----------------------- Getting root -----------------------------------------\n" << endl;

    cout << "Root Folder " << fs.getRoot() << endl;

    cout << "----------------------- Checking is Folder or File ---------------------------\n" << endl;

    cout << "obj1 is a file: " << file1->isFolder() << std::endl;

    cout << "----------------------- Printing Name of the Folder --------------------------\n" << endl;

    cout << "Name of folder1: " << folder1->getName() << endl;

    cout << "----------------------- Changing size ----------------------------------------\n" << endl;
    fs.printFolderStructure("/");
    cout << "subfolder size(file1): " << subfolder->size() << endl;
    fs.modify("/folder1/subfolder/file1", 2048);
    cout << "subfolder size(file1): " << subfolder->size() << endl;

    cout << "----------------------- Renaming Folder/File ---------------------------------\n" << endl;

    fs.printFolderStructure("/");
    fs.rename("/folder1/subfolder/file1", "file1", "file1.1");
    fs.printFolderStructure("/");

    cout << "----------------------- Moving Folder/File -----------------------------------\n" << endl;

    fs.createFile("/folder1/subfolder", "file1_new", 1024);
    Entity* file1_new = fs.get("/folder1/subfolder", "file1_new");
    cout << "Getting file/folder by name " << file1_new << endl;


    fs.printFolderStructure("/");

    fs.move( "/folder1/subfolder/", "/folder2/file2");

    fs.printFolderStructure("/");

    cout << "----------------------- Time when was Created/Modified -----------------------\n" << endl;
    time_t createdTime = folder1->getCreated();
    char buffer[26];
    ctime_s(buffer, sizeof buffer, &createdTime);
    cout << "Time when was created: " << buffer << endl;

    cout << "----------------------- Printing all stucture --------------------------------\n" << endl;

    fs.printFolderStructure("/");

    cout << "----------------------- Printing size ----------------------------------------\n" << endl;
    cout << "Folder 1 size: " << folder1->size() << endl;
    cout << "Subfolder size: " << subfolder->size() << endl;
    cout << "File 1 size: " << file1->size() << endl;
    cout << "File 2 size: " << file2->size() << endl;
    cout << "Folder 1 count: " << folder1->count() << endl;
    cout << "Subfolder count: " << subfolder->count() << endl;
    fs.printFolderStructure("/");

    fs.remove("/folder1/subfolder/file2");
    Entity* deletedFile = fs.get("/folder1/subfolder/file2");
    if (!deletedFile) {
        cout << "File 2 deleted successfully!" << endl;
    }
    fs.printFolderStructure("/");

    return 0;
}

