# Lab6_OOP
The following features should be implemented:
 Get a file/folder by full path (fs.get(name)).
 Getting a file/folder by name in a given folder (fs.get(name, folder)).
 Getting the root folder (fs.getRoot()).
 Determining whether an object is a file or a folder (obj.isFolder()).
 Getting file/folder attributes: name, creation date, modification date, size, for folders
modification date, size, for folders the number of subfolders
(directly and in total) (obj.getName(), obj.getCreationDate(), ...).
 Adding a new folder (fs.createFolder(folder, name)).
 Adding a new file (fs.createFile(folder, name, size)).
 Modifying a file (including resizing the file)
(fs.modify(file, new_size)).
 Deleting a file or folder (including nested entities)
(fs.remove(obj)).
 Rename a file/folder (fs.rename(file, new_name)).
 Moving a file/folder to a specific folder (fs.move(file, folder)).
 Copying a file or folder (including nested entities)
(fs.copy(file, folder, [new_name])).

 Searching for all subfolders in a folder by name (not exact
additional points, you can use masks in the
names): "?" (any character), "*" (any number of any
characters), size, dates (allow to specify a range). В
the result should be a set (collection) of entities (find(folder,
name_mask), find(folder, min_size, max_size)).
 Print the folder structure (fs.print(folder)).
#
![image](https://github.com/Dan-live/Lab6_OOP/assets/109356212/b1d200f8-8ba5-4985-b915-5ede44b4d674)
![image](https://github.com/Dan-live/Lab6_OOP/assets/109356212/ec35ec13-e343-4b6d-851c-3659f8c3109f)
![image](https://github.com/Dan-live/Lab6_OOP/assets/109356212/bdc85c63-3da2-4b68-80ae-3758b968f622)
