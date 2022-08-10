#include "../src/hlib.h"

void li() {
    printf("listeing 8000\n");
}

int main(void) {

    Http* app = h_default_server();

    h_listen(app, 8000, li);
    
    return 0; 
}
