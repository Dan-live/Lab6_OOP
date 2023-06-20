#include "File.hpp"

size_t File::size() const { return m_size; }

// returns the file size

size_t File::count() const { return 1; }

// returns one, since it is a file

bool File::isFolder() const { return false; }

// Method for resizing a file

void File::modify(size_t newSize) { m_size = newSize; }
