int main()
{
    char *s;
    s = "hello world";

    {
        char * p;
        p = "again";
    }
    printstr(s);
    return 0;
}
/*
Expected output:
hello world
*/