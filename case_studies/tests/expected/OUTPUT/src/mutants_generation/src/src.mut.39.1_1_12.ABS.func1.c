typedef struct {
    int x;
    int y;
    char z[5];
} MyType;

int func1(int v, MyType *mytype_var)
{
    int ret = 0;
    int x = 6*v; float y = 2.5, /* */ z=9 * x*1;

    x+=y
        || z;
    if (x+y > z
            && 2*x+y==7) {
        x++;
        if (x || v){
            ret = 1;
        }else
            ret = 2;
    }

    for (int i=0; i<y+z && i<5; ++i) 
    {
        // do stuffz
        ret =3;
        y+=z+i;
    }
    switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + x; break;
        default:
            mytype_var->z[0] = 'a';
    }
    return -(ret);
}
