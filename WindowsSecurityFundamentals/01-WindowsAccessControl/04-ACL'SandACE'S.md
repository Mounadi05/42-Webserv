```
An ACL contains a list of access control entries (ACEs). Each ACE specifies a set of access rights and contains a SID that identifies a trustee for whom the rights are allowed, denied, or audited.

A trustee can be a user account, a group account, or a logon session.
```
**Some basic access controls**
```
- If a Windows object does not have a discretionary access control list (DACL), the system allows everyone full access to it.

- If an object has a DACL, the system allows only the access that is explicitly allowed by the access control entries (ACEs) in the DACL.

- If there are no ACEs in the DACL, the system does not allow access to anyone.

- If a DACL has ACEs that allow access to a limited set of users or groups, the system implicitly denies access to all trustees not included in the ACEs.
```
```
Controlling access to an object is generally done using access-allowed ACEs.

There is no need to explicitly deny access to an object. The exception is when an ACE allows access to a group and there is a need to deny access to a member of the group.

In this case, an access-denied ACE for this user needs to be added in the DACL ahead of the access-allowed ACE for the group.

The order of the ACEs is important because the system reads the ACEs in sequence until access is granted or denied. 

/ ! \ The user's access-denied ACE must appear first; otherwise, when the system reads the group's access-allowed ACE, it will grant access to the restricted user.
```
**Attributing a DACL for most types of new securable objects is done following the below algorithm :**
```
1- The object's DACL is the DACL from the security descriptor specified by the object's creator. The system merges any inheritable ACEs into the specified DACL unless the SE_DACL_PROTECTED bit is set in the security descriptor's control bits.

2- If the creator does not specify a security descriptor, the system builds the object's DACL from inheritable ACEs.

3- If no security descriptor is specified and there are no inheritable ACEs, the object's DACL is the default DACL from the primary or impersonation token of the creator.

4- If there is no specified, inherited, or default DACL, the system creates the object with no DACL, which allows everyone full access to the object.
```
**Description of files’ set of permissions :**
```
1- Full Control allows you to read, write, modify, execute, change attributes and permissions, and take ownership of the file.

2- Modify allows you to read, write, modify, execute, and change the file’s attributes.

3- Read & Execute will allow you to display the file’s data, attributes, owner, and permissions, and run the file if it’s a program.

4- Read will allow you to open the file, view its attributes, owner, and permissions.

5- Write will allow you to write data to the file, append it to the file, and read or change its attributes.
```
**Description of folders’ set of permissions :**
```
1- Full Control allows you to read, write, modify, and execute files in the folder, change attributes and permissions, and take ownership of the folder or files within.

2- Modify allows you to read, write, modify, and execute files in the folder, and change attributes of the folder or files within.

3- Read & Execute will allow you to display the folder’s contents and display the data, attributes, owner, and permissions for files within the folder, and run files within the folder.

4- List Folder Contents will allow you to display the folder’s contents and display the data, attributes, owner, and permissions for files within the folder.

5- Read will allow you to display the file’s data, attributes, owner, and permissions.

6- Write will allow you to write data to the file, append it to the file, and read or change its attributes.
```