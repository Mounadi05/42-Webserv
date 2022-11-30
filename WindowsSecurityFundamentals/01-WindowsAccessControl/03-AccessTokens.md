```
An access token is an object that describes the security context of a process or thread.

The information in a token includes the identity and privileges of the user account associated with the process or thread. When a user logs on, the system authenticates the user's account name and password.

If the logon is successful, the system creates an access token.

Every process executed on behalf of this user will have a copy of this access token.
```
```
The access token contains security identifiers that identify the user's account and any group accounts to which the user belongs.

The token also contains a list of the privileges held by the user or the user's groups.

The system uses this token to identify the associated user when a process tries to access a securable object or perform a system administration task that requires privileges.
```
**Access tokens contain the following information :**
```
- The security identifier (SID) for the user's account

- SIDs for the groups of which the user is a member

- A logon SID that identifies the current logon session

- A list of the privileges held by either the user or the user's groups

- An owner of SID

- The SID for the primary group

- The default DACL that the system uses when the user creates a securable object without specifying a security descriptor

- The source of the access token

- Whether the token is a primary or impersonation token

- An optional list of restricting SIDs

- Current impersonation levels

- Other statistics
```