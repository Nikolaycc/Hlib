#ifndef HLIB_HPP
#define HLIB_HPP

// std
#include <iostream>

// uv
#include <uv.h>

const int CLOSE = 1;
const int DONE = 0;
const int ERROR = -1;
const int NFD = -2;

namespace Hlib {
    class Http {
        public:
        Http();
        ~Http();

        int Get(std::string path, void(*act)(Res, Req));
        int Post(std::string path, void(*act)(Res, Req));
        int Put(std::string path, void(*act)(Res, Req));
        int Delete(std::string path, void(*act)(Res, Req));
 
        int Listen(int port, void(*p)());

        private:
        uv_loop_t* loop = uv_default_loop();
        uv_tcp_t server;
        struct sockaddr_in addr;

        std::string _ip;
        int _port;
        int _listen;

        int CreateHttp(std::string ip);
    };

    class Res {
        public:

        private:        
    };

    class Req {
        public:

        private:        
    };
}

Hlib::Http Http() {
    CreateHttp("0.0.0.0");
    printf("Created Http Server!");
}

Hlib::Http ~Http() {

}

int Hlib::Http::CreateHttp(std::string ip) {
    _ip = ip
    
    return DONE;
} 

int Hlib::Http::Listen(int port, void(*p)()) {
    uv_ip4_addr(_ip, port, &addr);

    uv_tcp_bind(&server, (const struct sockaddr*) & addr, 0);
    _listen = uv_listen((uv_stream_t*)&server, DEFAULT_BACKLOG, on_new_connection);
    if (_listen) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(_listen));
        return ERROR;
    }

    return DONE;
}

#endif
