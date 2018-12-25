// This file contain "main()" Function.

#include "headers/ftp_server.h"
#include "libs/utility.h"

// This function takes Server name and Port value via commandline argument :- 	[[hostname] port]
// where [] = optional clause.
int main(int argc, char* argv[]){
    // run ftp-server on localhost.
    if(argc==2){
        int port = atoi(argv[1]);
        FTPServer server(port);
        server.start();
    }
    else{
        std::cout<<"Incorrect usage."<<std::endl;
        std::cout<<argv[0]<<" [port] : to use is as an ftp-server hosted on [port]"<<std::endl;
    }
    return 0;
}
