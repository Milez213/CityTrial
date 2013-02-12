

#include "calcFrustum.h"


extern mat4 g_proj;
extern mat4 g_view;

float fr[6][4];

void ExtractFrustum() {
   float   clip[16];
   float   t;


  mat4 tpose = g_proj * g_view;
  tpose = glm::transpose(tpose);

  // g_model is not used in main. it's a 0 matrix.
  // mat4 g_mView =  tpose * g_model;
  mat4 g_mView =  tpose;


   clip[ 0] = g_mView[ 0][0]; 
   clip[ 1] = g_mView[ 1][0] ;
   clip[ 2] = g_mView[ 2][0] ;
   clip[ 3] = g_mView[ 3][0] ;

   clip[ 4] = g_mView[ 0][1] ;
   clip[ 5] = g_mView[ 1][1] ;
   clip[ 6] = g_mView[ 2][1] ;
   clip[ 7] = g_mView[ 3][1] ;

   clip[ 8] = g_mView[ 0][2];
   clip[ 9] = g_mView[ 1][2] ;
   clip[10] = g_mView[ 2][2];
   clip[11] = g_mView[ 3][2] ;

   clip[12] = g_mView[0][3] ;
   clip[13] = g_mView[1][3] ;
   clip[14] = g_mView[2][3] ;
   clip[15] = g_mView[3][3] ;
   

   // wut
   /* Combine the two matrices (multiply projection by modelview) */
   /*clip[ 0] = g_mView[ 0][0] * g_proj[ 0][0] + g_mView[ 1][0] * g_proj[ 1][0] + g_mView[ 2][0] * g_proj[ 2][0] + g_mView[ 3][0] * g_proj[3][0];
   clip[ 1] = g_mView[ 0][0] * g_proj[ 0][1] + g_mView[ 1][0] * g_proj[ 1][1] + g_mView[ 2][0] * g_proj[ 2][1] + g_mView[ 3][0] * g_proj[3][1];
   clip[ 2] = g_mView[ 0][0] * g_proj[ 0][2] + g_mView[ 1][0] * g_proj[ 1][2] + g_mView[ 2][0] * g_proj[2][2] + g_mView[ 3][0] * g_proj[3][2];
   clip[ 3] = g_mView[ 0][0] * g_proj[ 0][3] + g_mView[ 1][0] * g_proj[ 1][3] + g_mView[ 2][0] * g_proj[2][3] + g_mView[ 3][0] * g_proj[3][3];

   clip[ 4] = g_mView[ 0][1] * g_proj[ 0][0] + g_mView[ 1][1] * g_proj[ 1][0] + g_mView[ 2][1] * g_proj[ 2][0] + g_mView[ 3][1] * g_proj[3][0];
   clip[ 5] = g_mView[ 0][1] * g_proj[ 0][1] + g_mView[ 1][1] * g_proj[ 1][1] + g_mView[ 2][1] * g_proj[ 2][1] + g_mView[ 3][1] * g_proj[3][1];
   clip[ 6] = g_mView[ 0][1] * g_proj[ 0][2] + g_mView[ 1][1] * g_proj[ 1][2] + g_mView[ 2][1] * g_proj[2][2] + g_mView[ 3][1] * g_proj[3][2];
   clip[ 7] = g_mView[ 0][1] * g_proj[ 0][3] + g_mView[ 1][1] * g_proj[ 1][3] + g_mView[ 2][1] * g_proj[2][3] + g_mView[ 3][1] * g_proj[3][3];

   clip[ 8] = g_mView[ 0][2] * g_proj[0][0] + g_mView[ 1][2] * g_proj[ 1][0] + g_mView[2][2] * g_proj[ 2][0] + g_mView[3][2] * g_proj[3][0];
   clip[ 9] = g_mView[ 0][2] * g_proj[0][1] + g_mView[ 1][2] * g_proj[ 1][1] + g_mView[2][2] * g_proj[ 2][1] + g_mView[3][2] * g_proj[3][1];
   clip[10] = g_mView[ 0][2] * g_proj[0][2] + g_mView[ 1][2] * g_proj[ 1][2] + g_mView[2][2] * g_proj[2][2] + g_mView[3][2] * g_proj[3][2];
   clip[11] = g_mView[ 0][2] * g_proj[0][3] + g_mView[ 1][2] * g_proj[ 1][3] + g_mView[2][2] * g_proj[2][3] + g_mView[3][2] * g_proj[3][3];

   clip[12] = g_mView[0][3] * g_proj[0][0] + g_mView[1][3] * g_proj[ 1][0] + g_mView[2][3] * g_proj[ 2][0] + g_mView[3][3] * g_proj[3][0];
   clip[13] = g_mView[0][3] * g_proj[0][1] + g_mView[1][3] * g_proj[ 1][1] + g_mView[2][3] * g_proj[ 2][1] + g_mView[3][3] * g_proj[3][1];
   clip[14] = g_mView[0][3] * g_proj[0][2] + g_mView[1][3] * g_proj[ 1][2] + g_mView[2][3] * g_proj[2][2] + g_mView[3][3] * g_proj[3][2];
   clip[15] = g_mView[0][3] * g_proj[0][ 3] + g_mView[1][3] * g_proj[ 1][3] + g_mView[2][3] * g_proj[2][3] + g_mView[3][3] * g_proj[3][3];
   */
   
   /* Extract the numbers for the RIGHT plane */
   fr[0][0] = clip[ 3] - clip[ 0];
   fr[0][1] = clip[ 7] - clip[ 4];
   fr[0][2] = clip[11] - clip[ 8];
   fr[0][3] = clip[15] - clip[12];

   /* Normalize the result */
   t = sqrt( fr[0][0] * fr[0][0] + fr[0][1] * fr[0][1] + fr[0][2] * fr[0][2] );
   fr[0][0] /= t;
   fr[0][1] /= t;
   fr[0][2] /= t;
   fr[0][3] /= t;
   
   /* Extract the numbers for the LEFT plane */
   fr[1][0] = clip[ 3] + clip[ 0];
   fr[1][1] = clip[ 7] + clip[ 4];
   fr[1][2] = clip[11] + clip[ 8];
   fr[1][3] = clip[15] + clip[12];

   /* Normalize the result */
   t = sqrt( fr[1][0] * fr[1][0] + fr[1][1] * fr[1][1] + fr[1][2] * fr[1][2] );
   fr[1][0] /= t;
   fr[1][1] /= t;
   fr[1][2] /= t;
   fr[1][3] /= t;

   /* Extract the BOTTOM plane */
   fr[2][0] = clip[ 3] + clip[ 1];
   fr[2][1] = clip[ 7] + clip[ 5];
   fr[2][2] = clip[11] + clip[ 9];
   fr[2][3] = clip[15] + clip[13];

   /* Normalize the result */
   t = sqrt( fr[2][0] * fr[2][0] + fr[2][1] * fr[2][1] + fr[2][2] * fr[2][2] );
   fr[2][0] /= t;
   fr[2][1] /= t;
   fr[2][2] /= t;
   fr[2][3] /= t;

   /* Extract the TOP plane */
   fr[3][0] = clip[ 3] - clip[ 1];
   fr[3][1] = clip[ 7] - clip[ 5];
   fr[3][2] = clip[11] - clip[ 9];
   fr[3][3] = clip[15] - clip[13];

   /* Normalize the result */
   t = sqrt( fr[3][0] * fr[3][0] + fr[3][1] * fr[3][1] + fr[3][2] * fr[3][2] );
   fr[3][0] /= t;
   fr[3][1] /= t;
   fr[3][2] /= t;
   fr[3][3] /= t;

   /* Extract the FAR plane */
   fr[4][0] = clip[ 3] - clip[ 2];
   fr[4][1] = clip[ 7] - clip[ 6];
   fr[4][2] = clip[11] - clip[10];
   fr[4][3] = clip[15] - clip[14];

   /* Normalize the result */
   t = sqrt( fr[4][0] * fr[4][0] + fr[4][1] * fr[4][1] + fr[4][2] * fr[4][2] );
   fr[4][0] /= t;
   fr[4][1] /= t;
   fr[4][2] /= t;
   fr[4][3] /= t;

   /* Extract the NEAR plane */
   fr[5][0] = clip[ 3] + clip[ 2];
   fr[5][1] = clip[ 7] + clip[ 6];
   fr[5][2] = clip[11] + clip[10];
   fr[5][3] = clip[15] + clip[14];

   /* Normalize the result */
   t = sqrt( fr[5][0] * fr[5][0] + fr[5][1] * fr[5][1] + fr[5][2] * fr[5][2] );
   fr[5][0] /= t;
   fr[5][1] /= t;
   fr[5][2] /= t;
   fr[5][3] /= t;


/*for (int i = 0; i<6;i++)
{
   for (int d = 0; d<4; d++)
   {
      printf("%f ",fr[i][d]);
   }
   printf("\n");
}
printf("\n");*/
}












bool checkFrust(vec3 pos)
{
    int p;

    for( p = 0; p < 6; p++ ) {
        if( fr[p][0] * pos.x + fr[p][1] * pos.y + fr[p][2] * pos.z + fr[p][3] < 0.0 ){
            //printf("%d\n",p);
            return false;
        }
    }
    return true;
}



float SphereInFrustum( vec3 pos, float radius )
{
    int p;
    float d;
    int c = 0;

    for( p = 0; p < 6; p++ )
    {
        d = fr[p][0] * pos.x + fr[p][1] * pos.y + fr[p][2] * pos.z + fr[p][3];
        if( d <= -radius )
            return 0;
        if( d > radius )
            c++;
    }
    return (c == 6) ? 2 : 1;
}





