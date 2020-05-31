int dump(int x)
{
    print(x);
    int p;
    p = x+12;
    print(p);
}
int dump2(int x, int y)
{
    print(x);
    print(y);
}

int main()
{
    int x ;
    x =12;
    int y;
    y =66;
    dump(x);
    dump2(33,44);
}
/*
Expected output:
12
24
33
44
*/