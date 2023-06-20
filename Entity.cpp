#include "Entity.hpp"

const string &Entity::getName() const { return name; }

// return the name

time_t Entity::getCreated() const { return created; }

// creation date

time_t Entity::getModified() const { return modified; }

// modification date 

void Entity::setName(const string& new_name) {
	name = new_name; modified = time(NULL);
}

string Entity::getParentPath() const { return m_parentPath; }

void Entity::setParentPath(const string& newPath) {	m_parentPath = newPath; }

