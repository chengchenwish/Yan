
struct Point
{
    int x;
    int y;
};
int main()
{
   struct Point p;
   p.x = 100;
   p.y = 200;
    print(p.x+p.y);
   print(p.x);
    return 0;
}
/*
Expected output:
300
100
*/