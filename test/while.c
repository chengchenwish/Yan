void main(){ 
 int i;
  i=1;
  while (i <= 10) {
    print(i);
    i= i + 1;
  }
  
  do 
  {
      print(i);
      i = i-2;
      
  }while(i >= 3);
}
/*
Expected output:
1
2
3
4
5
6
7
8
9
10
11
9
7
5
3
*/