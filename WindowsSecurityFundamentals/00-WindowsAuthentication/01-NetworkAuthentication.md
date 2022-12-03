# **NTLM Authentication**
```
NT Lan Manager authentication protocol is used for authentication between clients and servers.
There are two possible scenarios:

- Either the user uses the credentials of a local account of the server, in which the server has the user’s secret in its local SAM database and will be able to authenticate the user.

- Or in an Active Directory environment, the user uses a domain account during authentication, in which the server will have to ask the domain controller to verify the information provided by the user.
```
**local Accounts :**
```
NTLM Hashes are stored in the Security Account Manger (SAM Database)
```
**Domain Accounts :**
```
NTLM Hashes are stored in the Domain Controller (NTDS.dit Database)
```

**NTLM Challenge-Response Authentication**
```
1- The user provides their username and password.

2- The server sends a 16-byte random number as the challenge.

3- The user responds with the result of encrypting the challenge by the hash of its password
    1- The server extracts the NTLM hash from the SAM database.
    2- The server encrypts the challenge with the extracted NTLM hash, then compares the result with what the user has sent.
    3- If the comparison matches, the user has provided the right password, else, he didn’t.

4- The server responds back to the user stating if he’s allowed or denied login.

```

**NTLM HASH Example**
```
"aad3b435b51404eeaad3b435b51404ee:e19ccf75ee54e06b06a5907af13cef42"
The part before the colon mark ":" is the LM hash and the part after is the NT hash. 
```

**Weaknesses**
```
The main weakness of NTLM authentication has to do with the use of the NTLM hash as a shared secret. The calculated challenge is based on the NTLM hash of the user password, and the algorithm is publicly known: an attacker can calculate an NTLM challenge for a specific password candidate.

This can expose credentials in the case an attacker is able to capture a response to an NTLM challenge (Man-in-the-Middle attack for example). The attacker will then try a list of password candidates until recovering the password that was used by the user. Moreover, an attacker with access to the user’s NTLM hash does not have to recover the corresponding password: this is because NTLM authentication does not rely on the plaintext password, only the hash is needed.
```
```
Another weakness is related to relay attacks demonstrated in the following example: Considering a user Alice that has a session on the machine A. Alice wants to access an SMB share located on the machine B:

1- Machine A uses the LLMNR protocol to search for the IP address of machine B.

2- An attacker, Eve, on the same network responds back to this request specifying her own IP address.

3- Machine A, gets the spoofed IP and initiates an SMB connection toward Eve’s machine.

4- In the meantime, Eve presents herself to a machine B as Alice, initiating an SMB connection to it.

5- The C machine sends a challenge to the attacker’s machine to prove access to Alice’s password.

6- Eve sends back this challenge to Alice as an answer to the SMB access previously requested in step 3.

7- Machine A uses Alice’s user account identifiers to answer the challenge and then sends the answer to the attacker.

8- The answer received in the last step by the attacker is then relayed to the machine B.

9- Machine B validates the received answer coming from the attacker and authorizes the attacker access as the user Alice.
```
**Attacks**
```
The shared secret between the server and the client is the NTLM hash, introducing “Pass the hash” attack. The attack consists of using a user’s NTLM hash to authenticate to Windows services. The part where this becomes interesting and saves a malicious actor some precious time, is that there is no need to use brute force to recover the initial password.

The complexity of users’ passwords loses all its interest. Mitigation measures pass the hash is a technique that works when NTLM authentication is enabled on the server, which it is by default. The main idea is to steal a password hash and use it on other computers/servers. Here we should explain that pass the hash is something related to how windows authentication works, and how it implements SSO by caching credentials.

There is no official solution against Pass-the-Hash. The following document can be used as an inspiration:

https://www.blackhat.com/docs/us-15/materials/us-15-Moore-Defeating%20Pass-the-Hash-Separation-Of-Powers-wp.pdf
```