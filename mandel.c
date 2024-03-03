#include "ppm.h"
#include <complex.h>
#include <math.h>
#include <stdio.h>

#define TRSH 2.0
#define ITER 1024ull

#define SIZEX 1500
#define SIZEY 1500

struct col
{
    int r;
    int g;
    int b;
};
 
struct col getcol( int val , int max )  //Créer une couleur grâce aux coefficients de c.r, c.g, c.b qui correspondent à un mélange de rouge, de vert et de bleu
{
    double q = (double)val/(double)max;
 
    struct col c = { 0, 0, 0 };
 
    if( q < 0.25 )
    {
            c.r = ( q * 4.0 ) * 255.0;
            c.b = 255;
        }
    else if( q < 0.5 )
    {
            c.b = 255;
            c.g = 255;
            c.r = (q-0.25)*4.0*255.0;
 
        }
    else if( q < 0.75 )
    {
            c.b = 255;
            c.r = 255;
            c.g = 255.0 - (q-0.5)*4.0*255.0;
        }
    else
    {
            c.b = 255-(q-0.75)*4.0*255.0;
            c.g = 0;
            c.r = 255;
        }
 
    return c;
}


double cx(int x) {
  /* -2 ---> 1 */
  static const double qx = 3.0 / (double)SIZEX;
  return -2.0 + x * qx;
}

double cy(int y) {
  /* -1 ---> 1 */
  static const double qy = 2.0 / (double)SIZEY;
  return -1.0 + y * qy;
}

int main(int argc, char *argv[]) {
  struct ppm_image im;
  ppm_image_init(&im, SIZEX, SIZEY);        //Création d'une image de taille SIZEX x SIZEY

  int i, j;
  int colref = log(ITER);

for (i = 0; i < SIZEX; ++i) {              //Initialisation de la boucle utile horizontalement dans l'image
        for (j = 0; j < SIZEY; ++j) {      //Initialisation de la boucle utile verticalement dans l'image
 
            unsigned long int iter = 0;
 
            double complex c =  cx(i) + cy(j) * I;
            double complex z = 0;
 
            while(iter < ITER)
            {
                double mod = cabs(z);
 
                if( TRSH < mod )
                {
                    break;
                }
 
                z = z*z + c;
 
                iter++;
            }
 
            struct col cc = getcol( log(iter), colref );        //Création de la couleur
            ppm_image_setpixel(&im, i,j, cc.r, cc.g , cc.b );  //Modification du pixel de coordonnées i,j avec la couleur cc
        }
    }

  ppm_image_dump(&im, "m.ppm");     //Nomme l'image créée précédemment m.ppm
  ppm_image_release(&im);           //Libération de l'espace mémoire utilisé par im

  return 0;
}