#include <iostream>
#include "clipboard.h"
int main(int argc, char* argv[]){

    Clipboard cb;

    cb.setText("你好,World");
    
    std::cout << cb.getText() << std::endl;
    return 0;
}