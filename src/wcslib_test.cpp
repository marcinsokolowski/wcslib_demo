#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wcslib-7.1/wcs.h> // Assuming WCSLIB is installed and wcs.h is accessible
#include <wcslib-7.1/wcserr.h>

int main() {
    // 1. Initialize a wcsprm struct (e.g., from a FITS header)
    // This part involves reading FITS header keywords and setting up the
    // wcsprm struct. For brevity, we'll assume it's already done.
    // In a real application, you'd use functions like wcsset() or wcspih().
    struct wcsprm wcs;
    int status = 0; // WCSLIB functions return a status code

    // Placeholder for WCS initialization (replace with actual code)
    // For example, if you have a FITS header string:
    // char header[] = "SIMPLE  = T / Standard FITS format\n...";
    // status = wcspih(header, &wcs);
    // if (status != 0) { /* handle error */ }
    
    // http://www-wfau.roe.ac.uk/www-data/vmc/heasoft-6.19/heacore/wcslib/html/structwcsprm.html#a35bff8de85e5a8892e1b68db69ca7a68
    // init arrays 
    wcs.flag = -1; // to force memory initialisation
    wcserr_enable(1);
    wcsini(1,2,&wcs);
    

    // For this example, let's manually set some basic WCS parameters
    // (This is a simplified example and may not be fully functional
    // without more complete WCS setup).
/*    wcs.naxis = 2;
    wcs.crpix[0] = 100.0; // Reference pixel X
    wcs.crpix[1] = 100.0; // Reference pixel Y
    wcs.crval[0] = 200.0; // Reference value X (e.g., Right Ascension)
    wcs.crval[1] = 30.0;  // Reference value Y (e.g., Declination)
    wcs.cdelt[0] = -0.1;  // Pixel scale X
    wcs.cdelt[1] = 0.1;   // Pixel scale Y
//    wcs.ctype[0][0] = 'R'; wcs.ctype[0][1] = 'A'; // CTYPE1 = 'RA---TAN'
//    wcs.ctype[1][0] = 'D'; wcs.ctype[1][1] = 'E'; // CTYPE2 = 'DEC--TAN'
    strcpy(wcs.ctype[0],"RA---SIN");
    strcpy(wcs.ctype[1],"DEC--SIN");
    wcs.equinox = 2000.0; // J2000 equinox*/

    // CRAB IMAGE : /media/msok/f5ce6064-9dc3-40c3-bc9c-7314e8594519/pacer/processing/1192477696/1192477696_new_images
    wcs.naxis = 2;
    wcs.crpix[0] = 601.0; // Reference pixel X
    wcs.crpix[1] = 601.0; // Reference pixel Y
    wcs.crval[0] = 84.3841; // Reference value X (e.g., Right Ascension)
    wcs.crval[1] = 18.63321;  // Reference value Y (e.g., Declination)
    wcs.cdelt[0] = 0.005649384;  // Pixel scale X
    wcs.cdelt[1] = 0.00831817;   // Pixel scale Y
    strcpy(wcs.cunit[0],"deg");
    strcpy(wcs.cunit[1],"deg");
//    wcs.ctype[0][0] = 'R'; wcs.ctype[0][1] = 'A'; // CTYPE1 = 'RA---TAN'
//    wcs.ctype[1][0] = 'D'; wcs.ctype[1][1] = 'E'; // CTYPE2 = 'DEC--TAN'
    strcpy(wcs.ctype[0],"RA---SIN");
    strcpy(wcs.ctype[1],"DEC--SIN");
//    strcpy(wcs.ctype[0],"RA---TAN");
//    strcpy(wcs.ctype[1],"DEC--TAN");
    wcs.equinox = 2000.0; // J2000 equinox

printf("OK0 ???\n");

    // Call wcsset() to finalize the WCS setup after manually setting parameters
    status = wcsset(&wcs);
    if (status != 0) {
        fprintf(stderr, "Error setting WCS: %d\n", status);
        return 1;
    }

printf("OK ???\n");

    // 2. Define pixel coordinates
    int ncoord = 1; // Number of coordinate sets to transform
    double pixcrd[2]; // Input pixel coordinates (x, y)
    pixcrd[0] = 477.0; // Example pixel X
    pixcrd[1] = 977.0; // Example pixel Y

printf("OK2 ???\n");


    // 3. Prepare arrays for output
    double imgcrd[2]; // Intermediate image coordinates
    double world[2] = {-1,-1};  // Output world coordinates (e.g., RA, Dec)
    double phi=-1, theta=-1; // Spherical coordinates (longitude, latitude)
    int stat[1] = {0};      // Status array for each coordinate set

printf("OK3 ???\n");


    // 4. Call wcsp2s to perform the transformation
    int ret = wcsp2s(&wcs, ncoord, 2, pixcrd, imgcrd, &phi, &theta, world, stat);
//    wcsxy2s(&wcs, pixcrd, world); 
    
printf("OK4 ???\n");
    

    // 5. Check status and print results
    if (ret == 0 ) {
        printf("Pixel coordinates (%.2f, %.2f) transform to:\n", pixcrd[0], pixcrd[1]);
        printf("  World coordinates (RA, Dec): (%.6f, %.6f)\n", world[0], world[1]);
        printf("  Spherical coordinates (phi, theta): (%.6f, %.6f)\n", phi, theta);
    } else {
// CRASH:       fprintf(stderr, "Error transforming coordinates: %d , error = %s\n", stat[0],wcs.err->msg);
        fprintf(stderr, "Error transforming coordinates: %d\n",stat[0]);
        wcserr_prt(wcs.err,"ABC");
    }
    
printf("OK5 ???\n");
    

    // 6. Free WCSLIB resources
    wcsfree(&wcs);

    return 0;
}