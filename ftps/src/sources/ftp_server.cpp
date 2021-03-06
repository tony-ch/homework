// Class file for ftp server socket.
#include "../headers/ftp_server.h"

// Constructor function to create ftp server socket with port number given as argument.
FTPServer::FTPServer(int port_number){
	std::cout<<"FTPS-Server Started\n";
	port = port_number;
}

// Destructor function to ftp server socket.
FTPServer::~FTPServer() {
#ifdef USE_SSL
    SSL_CTX_free(_ctx);
    EVP_cleanup();
#endif
}

// Start ftp server on default port and listen to requests form clients.
void FTPServer::start(){
	std::cout<<"Starting server on port :"<<port<<std::endl;

	try{
#ifdef USE_SSL
        //init ssl
        SSL_load_error_strings();
        OpenSSL_add_ssl_algorithms();
        //create context
        _ctx = SSL_CTX_new(SSLv23_method());
        if (!_ctx) {
            perror("Unable to create SSL context");
            ERR_print_errors_fp(stderr);
            exit(EXIT_FAILURE);
        }
        //configure context
        SSL_CTX_set_ecdh_auto(ctx, 1);

        /* Set the key and cert */
        if (SSL_CTX_use_certificate_file(_ctx, "/etc/ssl/certs/vsftpd.pem", SSL_FILETYPE_PEM) <= 0) {
            ERR_print_errors_fp(stderr);
            exit(EXIT_FAILURE);
        }

        if (SSL_CTX_use_PrivateKey_file(_ctx, "/etc/ssl/certs/vsftpd.pem", SSL_FILETYPE_PEM) <= 0 ) {
            ERR_print_errors_fp(stderr);
            exit(EXIT_FAILURE);
        }
#endif
		//Creates new socket which listens to requests from clients
		ServerSocket control_socket(port);
		ServerSocket *server_socket = new ServerSocket();
		// wait for request from client
		while(1){
			try{
				control_socket.accept(*server_socket);
				if(!fork()){
					control_socket.close();
					communicate(server_socket);
					(*server_socket).close();
					exit(0);
				}
				(*server_socket).close();
			} catch(SocketException &e){
				std::cout<<"Exception occurred : "<<e.description()<<std::endl;
				continue ;
			}
		}
	// check for any exceptions
	} catch(SocketException &e){
		std::cout<<"Exception occurred : "<<e.description()<<std::endl;
		return ;
	}
}

// Send Response to Client and Get command from client.
void FTPServer::communicate(ServerSocket *server_socket){
	std::string data="",responseMsg="",cmd,args,user,pass;
	ServerSocket *data_socket;
	LoginInfo login_list;
	bool logged_in = false, binary_mode = false;
	// communicating with client by sending hello message.
	try{
		responseMsg = FTPResponse("200","(FTPS v1.0)").formResponse();
		*server_socket << responseMsg;
	} catch(SocketException &e){
		std::cout<<"Exception occurred : "<<e.description()<<std::endl;
		return ; 
	}
	// always ready to listen.
	while(1){
		args.clear();
		cmd.clear();
		try{
			//recv data from the client and store it in buffer
			*server_socket >> data;
			if(parseCommand(data,cmd,args)){
				std::cout<<"Recieved Command : "<<cmd<<std::endl;
				// get user command from client side.
				if(cmd=="USER"  && args.length()!=0){
					login_list = formLoginInfoList();
					user = args;	 
					responseMsg = FTPResponse("331","Please specify the password.").formResponse();
					*server_socket << responseMsg;
				}
				// get pass command from client side.
				else if(cmd=="PASS" && args.length()!=0){
					pass = args;
					if(login_list.find(user) != login_list.end()){
						if(login_list[user].first == pass){
							cd(login_list[user].second);
							logged_in = true;
							responseMsg = FTPResponse("230","Login successful.").formResponse();
						}
					}

					if(logged_in == false){
						responseMsg = FTPResponse("530","Login incorrect.").formResponse();
					}

					*server_socket << responseMsg;
				}
#ifdef USE_SSL
				else if(cmd=="AUTH" && args.size()!=0){
					responseMsg = FTPResponse("234","Proceed with negotiation.").formResponse();
					*server_socket << responseMsg;
                    SSL* ssl = SSL_new(_ctx);
                    SSL_set_fd(ssl,server_socket->fd());
                    if (SSL_accept(ssl) <= 0) {
                        ERR_print_errors_fp(stderr);
                    }
                    server_socket->ssl(ssl);
				}
#endif
                else if(cmd=="FEAT" && args.size()==0){
                    responseMsg = FTPResponse("211-Features:").formResponse();
                    *server_socket << responseMsg;
#ifdef USE_SSL
                    responseMsg = FTPResponse(" AUTH SSL").formResponse();
                    *server_socket << responseMsg;
#endif
                    responseMsg = FTPResponse(" PASV").formResponse();
                    *server_socket << responseMsg;
                    responseMsg = FTPResponse(" UTF8").formResponse();
                    *server_socket << responseMsg;
                    responseMsg = FTPResponse("211","End").formResponse();
                    *server_socket << responseMsg;
                }
                else if(cmd=="OPTS" && args.size()!=0){
                    if(args=="UTF8 ON"){
                        responseMsg = FTPResponse("200","Always in UTF8 mode.").formResponse();
                        *server_socket << responseMsg;
                    }else{
                        responseMsg = FTPResponse("500","UNKNOWN command").formResponse();
                        *server_socket << responseMsg;
                    }
                }
				else if(cmd=="PBSZ" && args.size()!=0){
					if(args=="0"){
						responseMsg = FTPResponse("200","PBSZ set to 0.").formResponse();
						*server_socket << responseMsg;
					}else{
						responseMsg = FTPResponse("500","UNKNOWN command").formResponse();
						*server_socket << responseMsg;
					}
				}
				// get syst command from client side for getting server system information.
				else if(cmd=="SYST" && args.size()==0){
					responseMsg = FTPResponse("215",syst()).formResponse();
					*server_socket << responseMsg;
				}
				// get pwd command from cient side for getting present working directory of server system.
				else if(cmd=="PWD"  && args.size()==0 && logged_in){
					responseMsg = FTPResponse("257","\""+pwd()+"\"").formResponse();
					*server_socket << responseMsg;
				}
				// get cwd command from cient side for changing working directory of server system.
				else if(cmd=="CWD" && args.size()!=0 && logged_in){

					if(cd(args)){
						responseMsg = FTPResponse("250","Directory successfully changed.").formResponse();
					}
					else{
						responseMsg = FTPResponse("550","Failed to change directory.").formResponse();
					}

					*server_socket << responseMsg;
				}
				// get mkd command from cient side for making new directory in server system.
				else if(cmd=="MKD" && args.size()!=0 && logged_in){
					std::string response;

					if(mkd(args,response)){
						responseMsg = FTPResponse("257",response).formResponse();
					}
					else{
						responseMsg = FTPResponse("550",response).formResponse();
					}

					*server_socket << responseMsg;
				}
				// get list command from cient side for getting info regarding all files and folders in 
				// present working directory of server system.
				else if(cmd=="LIST" && logged_in){
					// check for data socket connection with client.
					if((*data_socket).fd() != -1 ){
						std::string response;
						if(ls(args,response)){
							responseMsg = FTPResponse("150","Here comes the directory listing.").formResponse();
							*server_socket << responseMsg;
							// send list of directory to client.
							try{
								int pos = 0,len=response.length();
								std::string buffer;
								ServerSocket temp_socket;
								(*data_socket).accept(temp_socket);
#ifdef USE_SSL
								SSL* ssl = SSL_new(_ctx);
								SSL_set_fd(ssl,temp_socket.fd());
								if (SSL_accept(ssl) <= 0) {
									ERR_print_errors_fp(stderr);
								}
								temp_socket.ssl(ssl);
#endif
								while(pos<len){
									data = response.substr(pos,min(2048,len-pos));
									temp_socket << data;
									pos = pos + min(2048,len-pos);
								}
								responseMsg = FTPResponse("226", "Directory send OK.").formResponse();
							} catch(SocketException &e){
								std::cout<<"Exception occurred : "<<e.description()<<std::endl;
								responseMsg = FTPResponse("450", "Directory NOT send.").formResponse();
							}
						}else{
							responseMsg = FTPResponse("501","Syntax error in parameters or arguments.").formResponse();
						}

						(*data_socket).close();
					}
					else {
						responseMsg = FTPResponse("425","Use PASV first.").formResponse();
					}
					*server_socket << responseMsg;
				}
				// get type command from cient side for switching to Binary mode.
				else if(cmd=="TYPE"  && args.size()!=0 && logged_in){
					// check arguments for switching to binary mode.
					if(args == "I"|| args == "A"){
						binary_mode = true;
						responseMsg = FTPResponse("200","Switching to Binary mode.").formResponse();
					}else{
						responseMsg = FTPResponse("400","Mode not supported").formResponse();
					}

					*server_socket << responseMsg;
				}
				// get pasv command from cient side for switching to PASV mode.
				else if(cmd=="PASV" && args.size()==0 && logged_in){
					// try and catch for exception handling of socket exception event.
					try{
						data_socket =  new ServerSocket(0);
						std::string host =  (*server_socket).host();
						std::replace( host.begin(), host.end(), '.', ',');
						std::stringstream port;
						port << (*data_socket).port()/256 <<"," << (*data_socket).port()%256;
						responseMsg = FTPResponse("227","Entering Passive Mode ("+host+","+port.str()+").").formResponse();
					}catch(SocketException &e){
						std::cout<<"Exception occurred : "<<e.description()<<std::endl;
						responseMsg = FTPResponse("425","Cannot open data connection").formResponse();
					}

					*server_socket << responseMsg;
				}
                else if(cmd=="DELE" && args.size()!=0 && logged_in){

                    if(del(args)){
                        responseMsg = FTPResponse("250","DELE command successful.").formResponse();
                    }
                    else{
                        responseMsg = FTPResponse("550","Delete operation failed.").formResponse();
                    }

                    *server_socket << responseMsg;
                }
                else if(cmd=="RMD" && args.size()!=0 && logged_in){

                    if(rmd(args)){
                        responseMsg = FTPResponse("200","RMD command successful.").formResponse();
                    }
                    else{
                        responseMsg = FTPResponse("550","Remove directory operation failed.").formResponse();
                    }

                    *server_socket << responseMsg;
                }
				// get stor command from cient side for storing file in present working directory of server system.
				else if(cmd=="STOR" && args.size()!=0 && logged_in){
					//  check for binary mode to send file to server
					if(binary_mode){
						if((*data_socket).fd() != -1 ){
							std::ofstream out(args.c_str(), std::ios::out| std::ios::binary);
							// check for file to be send
							if(out){
								std::string buff;
								ServerSocket temp_socket;
								responseMsg = FTPResponse("150", "Ok to send data.").formResponse();
								*server_socket<<responseMsg;
								(*data_socket).accept(temp_socket);
								// put whole data on buffer named buff.
#ifdef USE_SSL
								SSL* ssl = SSL_new(_ctx);
								SSL_set_fd(ssl,temp_socket.fd());
								if (SSL_accept(ssl) <= 0) {
									ERR_print_errors_fp(stderr);
								}
								temp_socket.ssl(ssl);
#endif
								while(1){
									try{
										buff = "";
										temp_socket >> buff;
										if(buff.length()==0){
											break;
										}
									}catch (SocketException &e){
										std::cout<<"Exception occurred : "<<e.description()<<std::endl;
										break;
									}
									out << buff;																					
								}
								// close connection.
								out.close();
								temp_socket.close();
								(*data_socket).close();
								responseMsg = FTPResponse("226", "Transfer complete.").formResponse();
							}else{
								responseMsg = FTPResponse("550","Create directory operation failed.").formResponse();
							}
						}
						else {
							responseMsg = FTPResponse("425","Use PASV first.").formResponse();
						}

					}else{
						responseMsg = FTPResponse("550","Switch to Binary mode first.").formResponse();
					}

					*server_socket << responseMsg;
				}
				// get retr command from cient side for getting particular file from present working directory of server system.
				else if(cmd=="RETR" && args.size()!=0 && logged_in){
					std::string data;
					ServerSocket temp_socket;
					std::stringstream res_stream;
					// check for transmission mode to be binary
					if(binary_mode){
						if((*data_socket).fd() != -1 ){
							std::ifstream in(args.c_str(), std::ios::in| std::ios::binary| std::ios::ate);
							// cheeck for retrieve a file from server.
							if(in){
								long length = in.tellg();
								in.seekg (0, in.beg);
								res_stream << "Opening BINARY mode data connection for "<<args[0]<<" ("<<length<<" bytes).";
								responseMsg = FTPResponse("150",res_stream.str()).formResponse();
								*server_socket<<responseMsg;
								(*data_socket).accept(temp_socket);
#ifdef USE_SSL
								SSL* ssl = SSL_new(_ctx);
								SSL_set_fd(ssl,temp_socket.fd());
								if (SSL_accept(ssl) <= 0) {
									ERR_print_errors_fp(stderr);
								}
								temp_socket.ssl(ssl);
#endif
								while (length>0){
									int read_sz = MAXRECV<length ? MAXRECV : length;
									char buf[MAXRECV+1];
									in.read(buf,read_sz);
									data.assign(buf,read_sz);
									temp_socket<<data;
									length -= read_sz;
								}
								in.close();
								temp_socket.close();
								(*data_socket).close();
								responseMsg = FTPResponse("226", "Transfer complete.").formResponse();
							}
							else{
								responseMsg = FTPResponse("550","Couldn't retrieve file.").formResponse();
							}
						}
						else {
							responseMsg = FTPResponse("425","Use PASV first.").formResponse();
						}

					}else{
						responseMsg = FTPResponse("550","Switch to Binary mode first.").formResponse();
					}

					*server_socket << responseMsg;
				}
				// get quit command for stop server system.
				else if(cmd=="QUIT"  && args.size()==0){

					try{
						responseMsg = FTPResponse("221","Goodbye.").formResponse();
						*server_socket << responseMsg;
						return ;
					}catch(SocketException &e){
						std::cout<<"Exception occurred : "<<e.description()<<std::endl;
						responseMsg = FTPResponse("500","Couldn't close the connection.").formResponse();
						*server_socket << responseMsg;
					}
				}
				// for login request.
				else if(!logged_in){
					responseMsg = FTPResponse("332","Need account for login.").formResponse();
					*server_socket << responseMsg;
				} else{
					responseMsg = FTPResponse("500","UNKNOWN command").formResponse();
					*server_socket << responseMsg;
				}
			}
			else{
				responseMsg = FTPResponse("500","UNKNOWN command").formResponse();
				*server_socket << responseMsg;
			}
		} catch(SocketException &e){
			std::cout<<"Exception occurred : "<<e.description()<<std::endl;
			return ;
		}
	}
}

// Return present working directory of server to client.
std::string FTPServer::pwd( bool print){
	std::string request = FTPRequest("pwd","").getRequest("\n");
	std::string response = exec_cmd("pwd",request);

	if(print){
		std::cout<<response;
	}

	return response.substr(1,response.length()-3);
}

// Change directory on server side and return status code.
int FTPServer::cd(std::string args, bool print){
	int return_code;
	std::string response = exec_cmd("cd",args,return_code);

	if(print){
		std::cout<<response;
	}

	return return_code;
}


// Change directory on server side and return status code.
int FTPServer::del(std::string args, bool print){
    int return_code;
    std::string response = exec_cmd("del",args,return_code);

    if(print){
        std::cout<<response;
    }

    return return_code;
}

// Change directory on server side and return status code.
int FTPServer::rmd(std::string args, bool print){
    int return_code;
    std::string response = exec_cmd("rmd",args,return_code);

    if(print){
        std::cout<<response;
    }

    return return_code;
}

// return list of files and folders of current working directory from server side.
int FTPServer::ls(std::string args,std::string& response, bool print){
	int return_code;
	std::string request = FTPRequest("ls -l",args).getRequest("\n");
	response = exec_cmd("ls",request,return_code);

	if(print){
		std::cout<<response;
	}

	return return_code;
}

// return status code corresponding to make directory from server side.
int FTPServer::mkd(std::string args,std::string& response,bool print){
	int return_code;
	response = exec_cmd("mkdir",args,return_code);

	if(print){
		std::cout<<"Here"<<response;
	}

	return return_code;
}

// return system information of server system.
std::string FTPServer::syst(bool print){
	std::string request = FTPRequest("uname").getRequest("\n");
	std::string response = exec_cmd("uname",request);

	if(print){
		std::cout<<response;
	}

	return response;
}

// Set  root directory on user and return status code.
int FTPServer::setRootDir(std::string args, bool print){
	int return_code;
	std::string response = exec_cmd("chroot",args,return_code);

	if(print){
		std::cout<<response;
	}

	return return_code;
}

// return list of all users subscribed to server.
LoginInfo FTPServer::formLoginInfoList(){
	LoginInfo login_list;
	std::ifstream in("src/data/login.info", std::ios::in| std::ios::binary);

	if(in){
		std::string _user, _pass, _home;
		while (in >> _user >> _pass >> _home){
		    login_list[_user] = make_pair(_pass,_home);
		}
	}
	
	return login_list;
}