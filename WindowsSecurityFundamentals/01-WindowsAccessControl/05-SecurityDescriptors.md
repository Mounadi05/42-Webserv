```
When a securable object is created, the system assigns it a security descriptor that contains security information specified by its creator, or default security information if none is specified.

Applications can use functions to retrieve and set the security information for an existing object.
```
**Security Descriptor security information**
```
- Security identifiers (SIDs) for the owner and primary group of an object.
- A discretionary access control list (DACL) that identifies the users and groups allowed or denied access to the object.
- A system access control list (SACL) that controls how the system audits attempts to access the object.

- A set of control bits that qualify the meaning of a security descriptor or its individual members.
```
```
Applications must not directly manipulate the contents of a security descriptor.

The Windows API provides functions for setting and retrieving the security information in an object's security descriptor.

In addition, there are functions for creating and initializing a security descriptor for a new object.
```