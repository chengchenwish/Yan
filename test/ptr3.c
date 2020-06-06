int test(int *a)
{
    if(*a == 9)
    {
        *a = 90;
    }
    print(*a);
    return 88;
}

int main()
{
    
    
   // int *a;
    int b;
    int*a;
    int c;
    b = 9;

    
     a = &b;
    // print(*a);
    // int c;
     //c =*a;

   // b= 9;
    //*a = 90;
    print(*a);
    
    print(test(a));
    print(*a);
     return 0;
     
}

/*
Expected output:
9
90
88
90
*/