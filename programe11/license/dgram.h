int make_internet_address(char *host,int port,struct sockaddr_in *addrp);
int make_dgram_server_socket(int portnum);
int get_internet_address(char *host,int len,int *portp,struct sockaddr_in *addrp);
int make_dgram_client_socket();
void reply_to_sender(int sock,char *msg,struct sockaddr_in *addrp,socklen_t len);
void recv_from_recv(int sock,struct sockaddr_in *addrp,socklen_t len);
