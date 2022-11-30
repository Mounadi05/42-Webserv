```
Permissions define the type of access that is granted to a user or group to an object or object property.

For example, the Finance group can be granted Read and Write permissions to a file named Payroll.dat. By using the access control user interface, you can set NTFS permissions for objects such as files, Active Directory objects, registry objects, or system objects such as processes.

Permissions can be granted to any user, group, or computer. 

It is a good practice to assign permissions to groups because it improves system performance when verifying access to an object.
```
```
For Any object, you can grant permissions to :

- Groups, users, and other objects with security identifiers in the domain.

- Groups and users in any trusted domains.

- Local groups and users.
```
```
The permissions attached to an object depend on the type of object.

For example, the permissions that can be attached to a file are different from those that can be attached to a registry key.

Some permissions, however, are common to most types of objects.

These common permissions include :

- Read.
- Modify.
- Change Owner.
- Delete.
```
```
When you set permissions, you specify the level of access for groups and users. 

For example, you can let one user read the contents of a file, let another user make changes to the file, and prevent all other users from accessing the file. 

You can set similar permissions for printers so that certain users can configure the printer and other users can only print.
```
```
An owner is assigned to an object when that object is created.

By default, the owner is the creator of the object.

No matter what permissions are set for an object, the owner of the object can always change the permissions.
```
```
Inheritance allows administrators to easily assign and manage permissions.

This feature automatically allows objects within a container to inherit all the inheritable permissions of that container.

For example, the files within a folder inherit the permissions of the folder.

Only permissions marked as inherited will be inherited.
```
