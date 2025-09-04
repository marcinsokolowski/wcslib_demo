#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wcslib-7.1/wcs.h> // Assuming WCSLIB is installed and wcs.h is accessible
#include <wcslib-7.1/wcserr.h>
#include <wcslib-7.1/wcshdr.h>

#include <fitsio.h>
#include <string>

using namespace std;

void print_usage()
{
   printf("wcslib_fits FITS_FILE X Y\n");
   exit(-1);
}

int main( int argc, char* argv[] ) {
    char* header;
    int ncard,nreject,nwcs,status=0;
    fitsfile *fptr;
    
    if( argc>=2 && strcmp(argv[1],"-h")==0 ){
       print_usage();
    }
    
    // READ FITS:
//    string fits_file{ "avg_fits" };
//    const double pixcrd[2] = {477.0,977.0};
//    string fits_file{ "1199603608.ms_briggs-1_TH300mJy_CF10_clark__XX_0.28arcmin_1000px_UV_niter10000.fits" };

    const char* infile = argv[1];
    double x = atof( argv[2] ); 
    double y = atof( argv[3] ); 
    
   
    string fits_file{ argv[1] };
    const double pixcrd[2] = {x,y}; // radec2deg.sh 20:59:59.2 -14:40:42.3  -> ra_deg = 314.99666667 dec_deg = -14.67841667
//   const double pixcrd[2] = { 4096, 4096 }; // radec2deg.sh 22:14:27.5 -17:02:05.15 -> ra_deg = 333.61458333 dec_deg = -17.03476389

    
    fits_open_file(&fptr, fits_file.c_str(), READONLY, &status);
    if(status==0){
       printf("FITS file opened OK\n");
    }
    
    fits_hdr2str(fptr,0,NULL,0,&header,&ncard,&status);
    if(status==0){
       printf("FITS Header read OK\n");
    }

    // 1. Initialize a wcsprm struct (e.g., from a FITS header)
    // This part involves reading FITS header keywords and setting up the
    // wcsprm struct. For brevity, we'll assume it's already done.
    // In a real application, you'd use functions like wcsset() or wcspih().
    struct wcsprm* wcs = nullptr;
    status = 0; // WCSLIB functions return a status code

    // Placeholder for WCS initialization (replace with actual code)
    // For example, if you have a FITS header string:
    // char header[] = "SIMPLE  = T / Standard FITS format\n...";
    // status = wcspih(header, &wcs);
    // if (status != 0) { /* handle error */ }
    
    // http://www-wfau.roe.ac.uk/www-data/vmc/heasoft-6.19/heacore/wcslib/html/structwcsprm.html#a35bff8de85e5a8892e1b68db69ca7a68
    // init arrays 
//    wcs->flag = -1; // to force memory initialisation
    wcserr_enable(1);
//    wcsini(1,2,wcs);
    
    int ret = wcspih(header,ncard,WCSHDR_all,2,&nreject,&nwcs,&wcs);
//    printf("DEBUG : ret = %d , wcs = %x\n",ret,wcs);    

    // 2. Define pixel coordinates
    int ncoord = 1; // Number of coordinate sets to transform


    // 3. Prepare arrays for output
    double imgcrd[2]; // Intermediate image coordinates
    double world[2] = {-1,-1};  // Output world coordinates (e.g., RA, Dec)
    double phi=-1, theta=-1; // Spherical coordinates (longitude, latitude)
    int stat[1] = {0};      // Status array for each coordinate set

// printf("OK3 ???\n");
    printf("Pixel coordinates (%.2f, %.2f) transform to:\n", pixcrd[0], pixcrd[1]);


    // 4. Call wcsp2s to perform the transformation
    ret = wcsp2s(wcs, ncoord, 2, pixcrd, imgcrd, &phi, &theta, world, stat);
//    wcsxy2s(&wcs, pixcrd, world); 
    
// printf("OK4 ??? ret = %d, stat = %d\n",ret,stat[0]);
    

    // 5. Check status and print results
    if (ret == 0 ) {
        if( world[0] < 0 ){
           world[0] += 360.00;
        }    
    
        printf("Pixel coordinates (%.2f, %.2f) transform to:\n", pixcrd[0], pixcrd[1]);
        printf("  World coordinates (RA, Dec): (%.6f, %.6f)\n", world[0], world[1]);
        printf("  Spherical coordinates (phi, theta): (%.6f, %.6f)\n", phi, theta);
    } else {
// CRASH:       fprintf(stderr, "Error transforming coordinates: %d , error = %s\n", stat[0],wcs.err->msg);
        fprintf(stderr, "Error transforming coordinates: %d\n",stat[0]);
//        wcserr_prt(wcs->err,"ABC");
    }
    

    // 6. Free WCSLIB resources
    wcsfree(wcs);

    return 0;
}


