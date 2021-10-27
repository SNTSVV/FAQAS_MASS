unsigned long  klee_semu_GenMu_Mutant_ID_Selector = 240;
void klee_semu_GenMu_Mutant_ID_Selector_Func (unsigned long fromID, unsigned long toID);
void klee_semu_GenMu_Post_Mutation_Point_Func (unsigned long fromID, unsigned long toID);
typedef struct {
    int x;
    int y;
    char z[5];
} MyType;

int func1(int v, MyType *mytype_var)
{
    
klee_semu_GenMu_Mutant_ID_Selector_Func(1,2);
int ret = (klee_semu_GenMu_Mutant_ID_Selector==2?
(1):
(klee_semu_GenMu_Mutant_ID_Selector==1?
((-1)):
(0)));
klee_semu_GenMu_Post_Mutation_Point_Func(0,0);
klee_semu_GenMu_Post_Mutation_Point_Func(1,2);

    
klee_semu_GenMu_Mutant_ID_Selector_Func(3,19);
int x = (klee_semu_GenMu_Mutant_ID_Selector==14?
(6):
(klee_semu_GenMu_Mutant_ID_Selector==13?
(v):
(klee_semu_GenMu_Mutant_ID_Selector==12?
(6/v):
(klee_semu_GenMu_Mutant_ID_Selector==11?
(6+v):
(klee_semu_GenMu_Mutant_ID_Selector==10?
(6-v):
(klee_semu_GenMu_Mutant_ID_Selector==9?
(6%v):
((klee_semu_GenMu_Mutant_ID_Selector==8?
(-(6)):
(klee_semu_GenMu_Mutant_ID_Selector==7?
((6 + 1)):
(klee_semu_GenMu_Mutant_ID_Selector==6?
(0):
(klee_semu_GenMu_Mutant_ID_Selector==5?
(1):
(klee_semu_GenMu_Mutant_ID_Selector==4?
((6 - 1)):
(klee_semu_GenMu_Mutant_ID_Selector==3?
((-1)):
(6)))))))*(klee_semu_GenMu_Mutant_ID_Selector==19?
(-(v)):
(klee_semu_GenMu_Mutant_ID_Selector==18?
((v--)):
(klee_semu_GenMu_Mutant_ID_Selector==17?
((--v)):
(klee_semu_GenMu_Mutant_ID_Selector==16?
((v++)):
(klee_semu_GenMu_Mutant_ID_Selector==15?
((++v)):
(v)))))))))))));
klee_semu_GenMu_Post_Mutation_Point_Func(0,0);
klee_semu_GenMu_Post_Mutation_Point_Func(3,19);
 
klee_semu_GenMu_Mutant_ID_Selector_Func(20,47);
float y = (klee_semu_GenMu_Mutant_ID_Selector==21?
(-(2.5)):
(klee_semu_GenMu_Mutant_ID_Selector==20?
(0.0):
(2.5))), /* */ z=(klee_semu_GenMu_Mutant_ID_Selector==39?
(9 * x-1):
(klee_semu_GenMu_Mutant_ID_Selector==38?
(9 * x+1):
(klee_semu_GenMu_Mutant_ID_Selector==37?
(9 * x%1):
(klee_semu_GenMu_Mutant_ID_Selector==36?
(9 * x/1):
(klee_semu_GenMu_Mutant_ID_Selector==35?
(1):
(klee_semu_GenMu_Mutant_ID_Selector==34?
(9 * x):
((klee_semu_GenMu_Mutant_ID_Selector==33?
( x):
(klee_semu_GenMu_Mutant_ID_Selector==32?
(9 % x):
(klee_semu_GenMu_Mutant_ID_Selector==31?
(9 ):
(klee_semu_GenMu_Mutant_ID_Selector==30?
(9 - x):
(klee_semu_GenMu_Mutant_ID_Selector==29?
(9 / x):
(klee_semu_GenMu_Mutant_ID_Selector==28?
(9 + x):
((klee_semu_GenMu_Mutant_ID_Selector==27?
(1):
(klee_semu_GenMu_Mutant_ID_Selector==26?
(-(9)):
(klee_semu_GenMu_Mutant_ID_Selector==25?
((9 - 1)):
(klee_semu_GenMu_Mutant_ID_Selector==24?
(0):
(klee_semu_GenMu_Mutant_ID_Selector==23?
((-1)):
(klee_semu_GenMu_Mutant_ID_Selector==22?
((9 + 1)):
(9))))))) * (klee_semu_GenMu_Mutant_ID_Selector==44?
(-(x)):
(klee_semu_GenMu_Mutant_ID_Selector==43?
((++x)):
(klee_semu_GenMu_Mutant_ID_Selector==42?
((--x)):
(klee_semu_GenMu_Mutant_ID_Selector==41?
((x++)):
(klee_semu_GenMu_Mutant_ID_Selector==40?
((x--)):
(x)))))))))))))*(klee_semu_GenMu_Mutant_ID_Selector==47?
(2):
(klee_semu_GenMu_Mutant_ID_Selector==46?
((-1)):
(klee_semu_GenMu_Mutant_ID_Selector==45?
(0):
(1)))))))))));
klee_semu_GenMu_Post_Mutation_Point_Func(0,0);
klee_semu_GenMu_Post_Mutation_Point_Func(20,47);


    
klee_semu_GenMu_Mutant_ID_Selector_Func(48,57);
switch (klee_semu_GenMu_Mutant_ID_Selector) {
    case 48: { break;}
    case 49: {x/=y
        || z; break;}
    case 50: {x-=y
        || z; break;}
    case 51: {x%=y
        || z; break;}
    case 52: {x*=y
        || z; break;}
    default: {x+=(klee_semu_GenMu_Mutant_ID_Selector==56?
(y
        && z):
(klee_semu_GenMu_Mutant_ID_Selector==55?
(y
        ):
(klee_semu_GenMu_Mutant_ID_Selector==54?
( z):
((klee_semu_GenMu_Mutant_ID_Selector==53?
(-(y)):
(y))
        || (klee_semu_GenMu_Mutant_ID_Selector==57?
(-(z)):
(z)))))); break;}
};
klee_semu_GenMu_Post_Mutation_Point_Func(0,0);
klee_semu_GenMu_Post_Mutation_Point_Func(48,57);

    
klee_semu_GenMu_Mutant_ID_Selector_Func(58,116);
switch (klee_semu_GenMu_Mutant_ID_Selector) {
    case 58: { break;}
    default: {if ((klee_semu_GenMu_Mutant_ID_Selector==79?
( 2*x+y==7):
(klee_semu_GenMu_Mutant_ID_Selector==78?
(!(x+y > z
            && 2*x+y==7)):
(klee_semu_GenMu_Mutant_ID_Selector==77?
(x+y > z
            ):
(klee_semu_GenMu_Mutant_ID_Selector==76?
(x+y > z
            || 2*x+y==7):
((klee_semu_GenMu_Mutant_ID_Selector==75?
(x+y == z):
(klee_semu_GenMu_Mutant_ID_Selector==74?
(x+y < z):
(klee_semu_GenMu_Mutant_ID_Selector==73?
(x+y >= z):
(klee_semu_GenMu_Mutant_ID_Selector==72?
( z):
(klee_semu_GenMu_Mutant_ID_Selector==71?
(x+y != z):
(klee_semu_GenMu_Mutant_ID_Selector==70?
(x+y <= z):
(klee_semu_GenMu_Mutant_ID_Selector==69?
(x+y ):
((klee_semu_GenMu_Mutant_ID_Selector==68?
(x/y):
(klee_semu_GenMu_Mutant_ID_Selector==67?
(x):
(klee_semu_GenMu_Mutant_ID_Selector==66?
(x*y):
(klee_semu_GenMu_Mutant_ID_Selector==65?
(y):
(klee_semu_GenMu_Mutant_ID_Selector==64?
(x-y):
((klee_semu_GenMu_Mutant_ID_Selector==63?
((x--)):
(klee_semu_GenMu_Mutant_ID_Selector==62?
((x++)):
(klee_semu_GenMu_Mutant_ID_Selector==61?
((--x)):
(klee_semu_GenMu_Mutant_ID_Selector==60?
(-(x)):
(klee_semu_GenMu_Mutant_ID_Selector==59?
((++x)):
(x))))))+(klee_semu_GenMu_Mutant_ID_Selector==80?
(-(y)):
(y)))))))) > (klee_semu_GenMu_Mutant_ID_Selector==81?
(-(z)):
(z))))))))))
            && (klee_semu_GenMu_Mutant_ID_Selector==104?
(2*x+y<=7):
(klee_semu_GenMu_Mutant_ID_Selector==103?
(2*x+y>=7):
(klee_semu_GenMu_Mutant_ID_Selector==102?
(2*x+y!=7):
(klee_semu_GenMu_Mutant_ID_Selector==101?
(2*x+y<7):
(klee_semu_GenMu_Mutant_ID_Selector==100?
(2*x+y):
(klee_semu_GenMu_Mutant_ID_Selector==99?
(2*x+y>7):
((klee_semu_GenMu_Mutant_ID_Selector==98?
(2*x*y):
(klee_semu_GenMu_Mutant_ID_Selector==97?
(y):
(klee_semu_GenMu_Mutant_ID_Selector==96?
(2*x):
(klee_semu_GenMu_Mutant_ID_Selector==95?
(2*x-y):
(klee_semu_GenMu_Mutant_ID_Selector==94?
(2*x/y):
((klee_semu_GenMu_Mutant_ID_Selector==93?
(2):
(klee_semu_GenMu_Mutant_ID_Selector==92?
(x):
(klee_semu_GenMu_Mutant_ID_Selector==91?
(2+x):
(klee_semu_GenMu_Mutant_ID_Selector==90?
(2-x):
(klee_semu_GenMu_Mutant_ID_Selector==89?
(2/x):
(klee_semu_GenMu_Mutant_ID_Selector==88?
(2%x):
((klee_semu_GenMu_Mutant_ID_Selector==87?
((2 - 1)):
(klee_semu_GenMu_Mutant_ID_Selector==86?
(0):
(klee_semu_GenMu_Mutant_ID_Selector==85?
((2 + 1)):
(klee_semu_GenMu_Mutant_ID_Selector==84?
(-(2)):
(klee_semu_GenMu_Mutant_ID_Selector==83?
((-1)):
(klee_semu_GenMu_Mutant_ID_Selector==82?
(1):
(2)))))))*(klee_semu_GenMu_Mutant_ID_Selector==109?
(-(x)):
(klee_semu_GenMu_Mutant_ID_Selector==108?
((--x)):
(klee_semu_GenMu_Mutant_ID_Selector==107?
((++x)):
(klee_semu_GenMu_Mutant_ID_Selector==106?
((x++)):
(klee_semu_GenMu_Mutant_ID_Selector==105?
((x--)):
(x)))))))))))))+(klee_semu_GenMu_Mutant_ID_Selector==110?
(-(y)):
(y))))))))==(klee_semu_GenMu_Mutant_ID_Selector==116?
((7 - 1)):
(klee_semu_GenMu_Mutant_ID_Selector==115?
(1):
(klee_semu_GenMu_Mutant_ID_Selector==114?
(-(7)):
(klee_semu_GenMu_Mutant_ID_Selector==113?
((7 + 1)):
(klee_semu_GenMu_Mutant_ID_Selector==112?
(0):
(klee_semu_GenMu_Mutant_ID_Selector==111?
((-1)):
(7)))))))))))))))))))) {
        switch (klee_semu_GenMu_Mutant_ID_Selector) {
    case 117: { break;}
    default: {x++; break;}
};
        switch (klee_semu_GenMu_Mutant_ID_Selector) {
    case 118: { break;}
    default: {if ((klee_semu_GenMu_Mutant_ID_Selector==127?
( v):
(klee_semu_GenMu_Mutant_ID_Selector==126?
(x ):
(klee_semu_GenMu_Mutant_ID_Selector==125?
(x && v):
(klee_semu_GenMu_Mutant_ID_Selector==124?
(!(x || v)):
((klee_semu_GenMu_Mutant_ID_Selector==123?
((++x)):
(klee_semu_GenMu_Mutant_ID_Selector==122?
((x--)):
(klee_semu_GenMu_Mutant_ID_Selector==121?
((--x)):
(klee_semu_GenMu_Mutant_ID_Selector==120?
((x++)):
(klee_semu_GenMu_Mutant_ID_Selector==119?
(-(x)):
(x)))))) || (klee_semu_GenMu_Mutant_ID_Selector==132?
((++v)):
(klee_semu_GenMu_Mutant_ID_Selector==131?
((v++)):
(klee_semu_GenMu_Mutant_ID_Selector==130?
((--v)):
(klee_semu_GenMu_Mutant_ID_Selector==129?
((v--)):
(klee_semu_GenMu_Mutant_ID_Selector==128?
(-(v)):
(v)))))))))))){
            switch (klee_semu_GenMu_Mutant_ID_Selector) {
    case 133: { break;}
    default: {ret = (klee_semu_GenMu_Mutant_ID_Selector==136?
((-1)):
(klee_semu_GenMu_Mutant_ID_Selector==135?
(0):
(klee_semu_GenMu_Mutant_ID_Selector==134?
(2):
(1)))); break;}
};
        }else
            ret = 2; break;}
};
    }; break;}
}
klee_semu_GenMu_Post_Mutation_Point_Func(0,0);
klee_semu_GenMu_Post_Mutation_Point_Func(58,116);


    
klee_semu_GenMu_Mutant_ID_Selector_Func(137,178);
switch (klee_semu_GenMu_Mutant_ID_Selector) {
    case 137: { break;}
    default: {for (int i=(klee_semu_GenMu_Mutant_ID_Selector==139?
(1):
(klee_semu_GenMu_Mutant_ID_Selector==138?
((-1)):
(0))); (klee_semu_GenMu_Mutant_ID_Selector==154?
(i<y+z ):
(klee_semu_GenMu_Mutant_ID_Selector==153?
(i<y+z || i<5):
(klee_semu_GenMu_Mutant_ID_Selector==152?
( i<5):
((klee_semu_GenMu_Mutant_ID_Selector==151?
(i==y+z):
(klee_semu_GenMu_Mutant_ID_Selector==150?
(i):
(klee_semu_GenMu_Mutant_ID_Selector==149?
(i>y+z):
(klee_semu_GenMu_Mutant_ID_Selector==148?
(y+z):
(klee_semu_GenMu_Mutant_ID_Selector==147?
(i>=y+z):
(klee_semu_GenMu_Mutant_ID_Selector==146?
(i!=y+z):
(klee_semu_GenMu_Mutant_ID_Selector==145?
(i<=y+z):
((klee_semu_GenMu_Mutant_ID_Selector==144?
((--i)):
(klee_semu_GenMu_Mutant_ID_Selector==143?
((++i)):
(klee_semu_GenMu_Mutant_ID_Selector==142?
((i--)):
(klee_semu_GenMu_Mutant_ID_Selector==141?
((i++)):
(klee_semu_GenMu_Mutant_ID_Selector==140?
(-(i)):
(i))))))<(klee_semu_GenMu_Mutant_ID_Selector==160?
(y*z):
(klee_semu_GenMu_Mutant_ID_Selector==159?
(y/z):
(klee_semu_GenMu_Mutant_ID_Selector==158?
(z):
(klee_semu_GenMu_Mutant_ID_Selector==157?
(y-z):
(klee_semu_GenMu_Mutant_ID_Selector==156?
(y):
((klee_semu_GenMu_Mutant_ID_Selector==155?
(-(y)):
(y))+(klee_semu_GenMu_Mutant_ID_Selector==161?
(-(z)):
(z)))))))))))))))) && (klee_semu_GenMu_Mutant_ID_Selector==172?
(i>=5):
(klee_semu_GenMu_Mutant_ID_Selector==171?
(i!=5):
(klee_semu_GenMu_Mutant_ID_Selector==170?
(i>5):
(klee_semu_GenMu_Mutant_ID_Selector==169?
(i<=5):
(klee_semu_GenMu_Mutant_ID_Selector==168?
(i==5):
(klee_semu_GenMu_Mutant_ID_Selector==167?
(i):
((klee_semu_GenMu_Mutant_ID_Selector==166?
((i--)):
(klee_semu_GenMu_Mutant_ID_Selector==165?
((++i)):
(klee_semu_GenMu_Mutant_ID_Selector==164?
(-(i)):
(klee_semu_GenMu_Mutant_ID_Selector==163?
((i++)):
(klee_semu_GenMu_Mutant_ID_Selector==162?
((--i)):
(i))))))<(klee_semu_GenMu_Mutant_ID_Selector==178?
((5 + 1)):
(klee_semu_GenMu_Mutant_ID_Selector==177?
(1):
(klee_semu_GenMu_Mutant_ID_Selector==176?
(-(5)):
(klee_semu_GenMu_Mutant_ID_Selector==175?
((5 - 1)):
(klee_semu_GenMu_Mutant_ID_Selector==174?
((-1)):
(klee_semu_GenMu_Mutant_ID_Selector==173?
(0):
(5)))))))))))))))))); ++i) 
    {
        // do stuffz
        switch (klee_semu_GenMu_Mutant_ID_Selector) {
    case 179: { break;}
    default: {ret =(klee_semu_GenMu_Mutant_ID_Selector==185?
((-1)):
(klee_semu_GenMu_Mutant_ID_Selector==184?
(0):
(klee_semu_GenMu_Mutant_ID_Selector==183?
((3 + 1)):
(klee_semu_GenMu_Mutant_ID_Selector==182?
(-(3)):
(klee_semu_GenMu_Mutant_ID_Selector==181?
((3 - 1)):
(klee_semu_GenMu_Mutant_ID_Selector==180?
(1):
(3))))))); break;}
};
        switch (klee_semu_GenMu_Mutant_ID_Selector) {
    case 186: { break;}
    case 187: {y*=z+i; break;}
    case 188: {y/=z+i; break;}
    case 189: {y-=z+i; break;}
    default: {y+=(klee_semu_GenMu_Mutant_ID_Selector==195?
(z-i):
(klee_semu_GenMu_Mutant_ID_Selector==194?
(z):
(klee_semu_GenMu_Mutant_ID_Selector==193?
(i):
(klee_semu_GenMu_Mutant_ID_Selector==192?
(z*i):
(klee_semu_GenMu_Mutant_ID_Selector==191?
(z/i):
((klee_semu_GenMu_Mutant_ID_Selector==190?
(-(z)):
(z))+(klee_semu_GenMu_Mutant_ID_Selector==200?
((i++)):
(klee_semu_GenMu_Mutant_ID_Selector==199?
((++i)):
(klee_semu_GenMu_Mutant_ID_Selector==198?
(-(i)):
(klee_semu_GenMu_Mutant_ID_Selector==197?
((i--)):
(klee_semu_GenMu_Mutant_ID_Selector==196?
((--i)):
(i)))))))))))); break;}
};
    }; break;}
}
klee_semu_GenMu_Post_Mutation_Point_Func(0,0);
klee_semu_GenMu_Post_Mutation_Point_Func(137,178);

    
klee_semu_GenMu_Mutant_ID_Selector_Func(201,231);
switch (klee_semu_GenMu_Mutant_ID_Selector) {
    case 201: {switch(ret) {
        case 3:
            ret = -(y); break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 202: {switch(ret) {
        case -(3):
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 203: {switch(ret) {
        case 3:
            ret = y; break;
        case -(2):
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 204: {switch(ret) {
        case 3:
            ret = y; break;
        case (-1):
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 205: {switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + (x--); break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 206: {switch(ret) {
        case (3 + 1):
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 207: {switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case (-1): 
            ret = mytype_var->y + x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 208: {switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + (--x); break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 209: {switch(ret) {
        case 0:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 210: {switch(ret) {
        ; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 211: {switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y * x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 212: {switch(ret) {
        case 3:
            ret = y; break;
        case 0:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 213: {switch(ret) {
        case (-1):
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 214: {switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 0: 
            ret = mytype_var->y + x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 215: {switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + x; break;
        default:
            mytype_var->z[1] = 'a';
    }; break;}
    case 216: {switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + (++x); break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 217: {switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y ; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 218: {switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y / x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 219: {switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret =  x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 220: {switch(ret) {
        case 3:
            ret = y; break;
        ; break;
        case 1: 
            ret = mytype_var->y + x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 221: {switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y % x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 222: {switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + x; break;
        ;
    }; break;}
    case 223: {switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + -(x); break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 224: {switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y + (x++); break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 225: {switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        ; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    case 226: {switch(ret) {
        case 3:
            ret = y; break;
        case 2:
            ret = mytype_var->x; break;
        case 1: 
            ret = mytype_var->y - x; break;
        default:
            mytype_var->z[0] = 'a';
    }; break;}
    default: {switch((klee_semu_GenMu_Mutant_ID_Selector==231?
((ret--)):
(klee_semu_GenMu_Mutant_ID_Selector==230?
(-(ret)):
(klee_semu_GenMu_Mutant_ID_Selector==229?
((ret++)):
(klee_semu_GenMu_Mutant_ID_Selector==228?
((--ret)):
(klee_semu_GenMu_Mutant_ID_Selector==227?
((++ret)):
(ret))))))) {
        case 3:
            ret = y; switch (klee_semu_GenMu_Mutant_ID_Selector) {
    case 232: { break;}
    default: {break; break;}
};
        case 2:
            ret = mytype_var->x; switch (klee_semu_GenMu_Mutant_ID_Selector) {
    case 233: { break;}
    default: {break; break;}
};
        case 1: 
            ret = mytype_var->y + x; switch (klee_semu_GenMu_Mutant_ID_Selector) {
    case 234: { break;}
    default: {break; break;}
};
        default:
            mytype_var->z[0] = 'a';
    }; break;}
}
klee_semu_GenMu_Post_Mutation_Point_Func(0,0);
klee_semu_GenMu_Post_Mutation_Point_Func(201,231);

    
klee_semu_GenMu_Mutant_ID_Selector_Func(235,239);
return (klee_semu_GenMu_Mutant_ID_Selector==239?
(-(ret)):
(klee_semu_GenMu_Mutant_ID_Selector==238?
((++ret)):
(klee_semu_GenMu_Mutant_ID_Selector==237?
((--ret)):
(klee_semu_GenMu_Mutant_ID_Selector==236?
((ret++)):
(klee_semu_GenMu_Mutant_ID_Selector==235?
((ret--)):
(ret))))));
klee_semu_GenMu_Post_Mutation_Point_Func(0,0);
klee_semu_GenMu_Post_Mutation_Point_Func(235,239);

}

/* Wrapping main template for the function func1 defined in the file /home/ubuntu/faqas_semu/case_studies/tests/WORKSPACE/DOWNLOADED/src.c */
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */

#include <stdio.h>
#include <string.h>


#include "klee/klee.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    // Declare variable to hold function returned value
    int result_faqas_semu;

    // Declare arguments and make input ones symbolic
    int v;
    MyType mytype_var;
    memset(&v, 0, sizeof(v));
    memset(&mytype_var, 0, sizeof(mytype_var));
    klee_make_symbolic(&v, sizeof(v), "v"); //int
    klee_make_symbolic(&mytype_var, sizeof(mytype_var), "mytype_var"); //MyType

    // Call function under test
    result_faqas_semu = func1(v, &mytype_var);

    // Make some output
    printf("FAQAS-SEMU-TEST_OUTPUT: x=%d, y=%d, z=%s\n", mytype_var.x, mytype_var.y, mytype_var.z);
    printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\n", result_faqas_semu);
    return (int)result_faqas_semu;
}
