int setup();
void free_all_tickets();
void narrate(char *msg1,char *msg2,struct sockaddr_in *clientp);
void do_request(char *req,struct sockaddr_in *clientp,socklen_t addlen);
char *do_hello(char *req);
char *do_goodbye(char *req);
void ticket_reclaim();
