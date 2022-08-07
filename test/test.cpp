#include <iostream>
#include "../src/Hlib.hpp"

using namespace Hlib;

void action() {
    printf("action listen");
}

int main(void) {

    Http app;

    app.Listen(8000, action);
    
    return 0;
}
