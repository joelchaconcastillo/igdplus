#include <bits/stdc++.h>
#define MAX_N 1000
using namespace std;

string filenamereference, filenamedata;

double reference[100*MAX_N], data[MAX_N], data_nd[MAX_N];
int nobj, nd, nnd=0, nr;//objectives, data size, reference size
double p = 2.0;

double dist(double *r, double *a)
{
   double d = 0.0;
   for(int i = 0; i < nobj; i++)
     d += max(a[i]-r[i], 0.0)*max(a[i]-r[i], 0.0);
   return sqrt(d);
}
double IGD(double *reference, int size_r, double  *point, int size_p)
{
   double totaldist = 0.0;
   for(int i = 0; i < size_r; i++)
   {
      double mind = DBL_MAX;
      for(int  j = 0; j < size_p; j++)
	 mind = min(mind, pow(dist(reference + i*nobj, point + j*nobj), p) );
      totaldist +=mind;
   } 
   return pow((totaldist/size_r), 1.0/p);
}
void readpoints(double *points, int &size, FILE *fin)
{
   nobj=0;
   int end=1, r=0, c=0;
   double num;
   char trimm[2];
   do //each row
   {
      c = 0;
      do //each column
      {
        fscanf(fin, "%lf", points + r*nobj+c);
        fscanf(fin, "%*[ \t]");
        end = fscanf (fin, "%1[\r\n]", trimm);
	c++;
      }while(!end);
      nobj = max(nobj, c);
      //skip over successive empty lines
  do { 
       if(!fscanf (fin, "%1[#]%*[^\n\r]", trimm))
            fscanf (fin, "%*[ \t]");
       end = fscanf (fin, "%1[\r\n]", trimm);
     } while (end == 1);
     r++;
   }while(end!= EOF);
   nobj = c;
   size = r;
}
bool dominate(double *a, double *b)
{	
   bool dominated = true;
   int cont = 0;
   for(int n=0; n<nobj; n++)
   {
      if(b[n]<a[n]) return false;
      if(a[n] == b[n]) cont++;
   }
    if(cont>0) return false;
   return dominated;
}
void take_non_dominated_set(double *points, int &n_points)
{
  for(int i = 0; i < n_points; i++)
  {
     int cont = 0;
     for(int j = 0; j < n_points; j++)
       if(i!=j && dominate(points + nobj*j, points + nobj*i)) cont++;

     if( cont == 0 )
     {
       for(int m = 0; m < nobj; m++)  data_nd[nnd*nobj + m ] = data[i*nobj+m];
       nnd++;
     }
  } 
}
void readdata()
{
   //FILE *fin_ref = fopen(filenamereference.c_str(), "r"), *fin_point = stdin;
   FILE *fin_ref = fopen(filenamereference.c_str(), "r"), *fin_point = fopen(filenamedata.c_str(), "r");
   readpoints(reference, nr, fin_ref);
   readpoints(data, nd, fin_point);
   take_non_dominated_set(data, nd);
}
void inputArg(int argc, char **argv)
{
   for(int i = 1; i < argc ; i++)
   {
      string Terminal(argv[i]);
      if(Terminal == "--r" )
         filenamereference = string(argv[++i]);
      else if(Terminal == "--p" )
         p = atof(argv[++i]);
      else if(Terminal == "--d" )
         filenamedata = string(argv[++i]);
   }
}
int main(int argc, char **argv)
{
   inputArg(argc, argv);
   readdata();
   cout << IGD(reference, nr, data_nd, nnd)<<endl;   
   return 0;
}
