#ifndef FILE_H_   
#define FILE_H_

#include "Entity.hpp"
using namespace std;

class File : public Entity {         // definition of the File class inherited from Entity
private:                             // declaration of closed class members
    size_t m_size;                   // file size
public:                              // declaring public class members
    File(const string& name, size_t size) : Entity(name), m_size(size) {}
    // constructor that initializes the file name and size
    virtual ~File() {}               // destructor
    virtual size_t size() const;
    // returns the file size
    virtual size_t count() const;
    // returns one, since it is a file
    virtual bool isFolder() const override; // method implementation for the File class
    // Method for resizing a file
    virtual void modify(size_t newSize);
};

#endif