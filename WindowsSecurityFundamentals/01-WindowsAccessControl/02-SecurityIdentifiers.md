```
A security identifier (SID) is a unique value of variable length used to identify a trustee.

Each account has a unique SID issued by an authority, such as a Windows domain controller, and stored in a secure database.

Each time a user logs on, the system retrieves the SID for that user from the database and places it in the access token for that user.

The system uses the SID in the access token to identify the user in all subsequent interactions with Windows security. 

When a SID has been used as the unique identifier for a user or group, it cannot ever be used again to identify another user or group.
```
```
Windows security uses SIDs in the following security elements :

- In access tokens to identify the user and the groups to which the user belongs.

- In security descriptors to identify the owner of an object and primary group.

- In access control entries to identify the trustee for whom access is allowed, denied, or audited.
```
```
In addition to the uniquely created, domain-specific SIDs assigned to specific users and groups, there are well-known SIDs that identify generic groups and generic users.

For example, the well-known SIDs, Everyone and World, identify a group that includes all users.

The components of a SID are easier to visualize when SIDs are converted from a binary to a string format by using the standard notation: 

S-R-X-Y1-Y2-…-Yn-1-Yn

In this notation, the components of a SID are represented as shown in the following table.
```
```
S : indicates that the string is an SID.
R : indicates the revision Level.
X : indicates the identifier authority Value.
Y : Represents a series of sub-authority values, where 'n' is the number of values.
```
```
The SID's most important information is contained in the series of sub-authority values.

The first part of the series (-Y1-Y2-Yn-1) is the domain identifier.

The last item in the series of sub-authority values (-Yn) is the relative identifier.

It distinguishes one account or group from all other accounts and groups in the domain.

Two accounts or groups in any domain neither share the same domain identifier nor relative identifier.
```
```
S-1-5-32-544  : (1) RevisionLevel : (5) IdentifierAuthority : (32) Builtin : (544) Administrators
S-1-5-21-1004336348-1177238915-682003330-512
```