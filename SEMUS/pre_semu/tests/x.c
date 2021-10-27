
#include <stdio.h>

void func1(int v)
{
    typedef int myint;

    int x = 6*v; float y = 2.5, /* */ z=9 * x*1;

    x=y
        || z;
    if (x+y > z
            && 2*x+y==7) {
        x++;
        if (x || v){
            printf("is x");
        }else
            printf("not x");
    }

    for (int i=0; i<y+z; ++i) 
    {
        // do stuffz
        printf("alpha");
        y+=z+i;
    }
}

int main(int argc, char **argv) {
    int t;
    switch(argc)
    {
        case 1: printf("input t: "); 
                break;
        default:
                printf("input default t: ");
                break;
    }
    scanf("%d", &t);
    func1(t);
    return 0;
}
