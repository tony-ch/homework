// This file contain "main()" Function.

#include "headers/ftp_client.h"
#include "libs/utility.h"

// This function takes Server name and Port value via commandline argument :- 	[[hostname] port]
// where [] = optional clause.
int main(int argc, char* argv[]){
    if(argc==3){
        std::string host(argv[1]);
        if(std::count(host.begin(), host.end(), '.') == 3 && is_number(argv[2]) && atoi(argv[2]) < 65536){
            int port = atoi(argv[2]);
            std::string user,pass;
            std::cout<<"Enter User Name : ";
            std::cin>>user;
            std::cout<<"Enter Password : ";
            pass = getPassword();
            FTPClient client(host, port, user, pass);
            client.start();
            client.communicate();
        }
        else{
            std::cout<<"Input incorrectly formatted."<<std::endl;
            std::cout<<argv[0]<<" [int].[int].[int].[int] [int(1-65536)]"<<std::endl;
        }
    }
        // Incorrect arguments specified by user.
    else{
        std::cout<<"Incorrect usage."<<std::endl;
        std::cout<<argv[0]<<" [host] [port] : to connect to ftp-server hosted at host:port"<<std::endl;
    }
    return 0;
}
