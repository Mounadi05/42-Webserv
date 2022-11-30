**Interactive Authentication Components**

**User Logon :**
```
Winlogon.exe is the executable file responsible for managing secure user interactions.

The Winlogon service initiates the logon process for Windows operating systems by passing the credentials collected by user action on the secure desktop (Logon UI) to the Local Security Authority (LSA) through Secur32.dll.
```
**Application Logon :**
```
Application or service logons that do not require interactive logon. Most processes initiated by the user run in user mode by using Secur32.dll whereas processes initiated at startup, such as services, run in kernel mode by using Ksecdd.sys.

For more information about user mode and kernel mode, see Applications and User Mode or Services and Kernel Mode in this topic.

```
**Secure32.dll**
```
The multiple authentication providers that form the foundation of the authentication process.
```
**Lsasrv.dll**
```
The LSA Server service, which both enforces security policies and acts as the security package manager for the LSA.

The LSA contains the Negotiate function, which selects either the NTLM or Kerberos protocol after determining which protocol is to be successful.
```
**Security Support Providers**
```
A set of providers that can individually invoke one or more authentication protocols.

The default set of providers can change with each version of the Windows operating system, and custom providers can be written.
```
**Netlogon.dll**
```
The services that the Net Logon service performs are as follows:

- Maintains the computer's secure channel to a domain controller.

- Passes the user's credentials through a secure channel to the domain controller and returns the domain security identifiers (SIDs) and user rights for the user.

- Publishes service resource records in the Domain Name System (DNS) and uses DNS to resolve names to the Internet Protocol (IP) addresses of domain controllers.

- Implements the replication protocol based on remote procedure call (RPC) for synchronizing primary domain controllers (PDCs) and backup domain controllers (BDCs).

```
**Samsrv.dll**
```
The Security Accounts Manager (SAM), which stores local security accounts, enforces locally stored policies, and supports APIs.
```
**Registry**
```
The Registry contains a copy of the SAM database, local security policy settings, default security values, and account information that is only accessible to the system.
```