#include "HueCom.h"

int main(){
    
    HueCom hue;
    hue.init();
    hue.writeHUE(1);
    hue.cleanup();
    return 0;

}
