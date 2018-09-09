/* exceptions and errors */

void DieWithUserMessage(const char*, const char*);
void DieWithSystemMessage(const char*);

/* client / server */

int SetupTCPServerSocket(int);
int AcceptTCPConnection(int, struct sockaddr *);

