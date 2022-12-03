```
Access control refers to security features that control who can access resources in the operating system.

The operating system will verify that a user/application has the required rights to access a resource (file, share, system object, ...).

Applications can also call access control functions to set who can access specific resources or control access to resources provided by the application.
```
```
After a user is authenticated, the Windows operating system uses built-in authorization and access control technologies to implement the second phase of protecting resources: determining if an authenticated user has the correct permissions to access a resource.

After a user authenticates by entering the password, the system validates the password and then gives the user an access token that will be inherited by all applications the user will be running.
```
```
The access token that has been given to the user will be verified by the system every time the user needs access to resources.

For example, shutting down the computer: this is mapped to a privilege SeShutdown, which is referenced by the access token.

Another example is a user trying to access a file only accessible to administrators.

In this case, the OS will inspect the user and group SIDs of the access token and verify if one of them matches or is part of the administrator’s group.
```
```
Shared resources are available to users and groups other than the resource’s owner, and they need to be protected from unauthorized use.

In the access control model, users and groups (also referred to as security principals) are represented by unique security identifiers (SIDs).

They are assigned rights and permissions that inform the operating system what each user and group can do. Each resource has an owner who grants permissions to security principals.

During the access control check, these permissions are examined to determine which security principals can access the resource and how they can access it.
```
```
Security principals perform actions, which include Read, Write, Modify, or Full Control of objects, such as files, folders, printers, registry keys, and Active Directory Domain Services (ADDS) objects.

Shared resources use Access Control Lists (ACLs) to assign permissions. This enables resource managers to enforce access control in the following ways:

- Deny access to unauthorized users and groups.
- Set well-defined limits on the access that is provided to authorized users and groups.
```
```
Object owners, generally, grant permissions to security groups rather than to individual users. Users and computers that are added to existing groups assume the permissions of that group.

If, for example, a folder object can hold other objects, such as subfolders and files, it is called a container.

In a hierarchy of objects, the relationship between a container and its content is expressed by referring to the container as the parent.

An object in the container is referred to as the child, and the child inherits the access control settings of the parent.

Object owners often define permissions for container objects rather than individual child objects to simplify access control management.
```
```
To handle access control, Windows has three forms of access control over objects, but in this course, we’ll only be focusing on Discretionary access control, which refers to the protection mechanism that most people think of when they think of OS security.

It’s the method by which owners of objects, such as files or printers grant or deny access to others. When users log in, they are given a set of security credentials or a security context.

When they attempt to access objects, their security context is compared to the access control list on the object they are trying to access to determine whether they have permission to perform the requested operation.
```