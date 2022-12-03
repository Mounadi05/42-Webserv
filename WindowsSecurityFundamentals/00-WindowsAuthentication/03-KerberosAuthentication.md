**How Does it Work**
```
Kerberos was developed at the Massachusetts Institute of Technology in the 1980s and has now become the most widely-used system for authentication and authorization in computer networks.

The name Kerberos comes from ancient Greek mythology in which Kerberos is a three-headed dog who guards the underworld.

The three heads of Kerberos are represented in the protocol by a client seeking authentication, a server the client wants to access, and the key distribution center (KDC).

The KDC is a trusted third party that authenticates users.
```
```
1- The client sends an AS_REQ to the KDC requesting a Ticket Granting Ticket (TGT).

The client presents its principal name and can present pre-authentication information if it is required.

The pre-authentication information consists of the current computer’s time encrypted with the user’s password hash.

2- Once the KDC verifies the user’s authentication data, it responds back to the client with an AS_REP that contains a TGT and a session key that a client can use to encrypt and authenticate communication with the KDC for ticket-granting service (TGS) requests.

The KDC can decrypt the AS_REQ because the principal’s password hash is stored within Active Directory; it then verifies the timestamp in the AS_REQ to make sure that the systems are within 5 minutes of time skew.

This process validates that the principal authenticating knows the user’s account and password.

3- The client is then able to request service tickets since it has a valid TGT for the Active Directory domain.

The client sends a request to the KDC for a ticket to a service.

The client presents TGT ticket received in AS-REP, an authenticator, and the SPN of the service.

Authenticator data is the User Principal Name, as well as the current system timestamp.

4- After validating the TGT and authenticator.

The KDC returns a service ticket (encrypted by the server’s key) and a session key for further communication between the client and server.

5- To authenticate to the required server, the client sends the received ticket to the server.

6- The server uses its own password to decrypt the ticket, if valid, it sends an encrypted timestamp for client validation.
```