#include "HueCom.h"
#include <unistd.h>

int main(){
    
    HueCom hue;
    hue.init();
    int i;
    for (i=0;i<65;i++){
        hue.writeHUE(2,1000*i, 255,255);
        sleep(1);
   }
    hue.cleanup();
    return 0;

}
