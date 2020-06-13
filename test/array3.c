int test(int(*a)[10], int x)
 {
    int i;
    int j;
    for(i =0;i<10;i++)
    for(j =0;j<10;j++)
        print(a[i][j]);
//print(a[9][8]);

return 0;
}
int main()
{
    int a[10][10];
        int i;
    int j;
    for(i =0;i<10;i++)
    for(j =0;j<10;j++)
        a[i][j] = i*10+j;
   print(test(a,10));

    return 0;
}