//Movie class originally written by James Taylor jrt@gwu.edu
//Modified by Lindsey DiAntonio 
//lindseydiantonio@gmail.com

#ifndef _MOVIE_H_
#define _MOVIE_H_

#include <stdio.h>
#include <string>
#include "tiffio.h"

class Movie {
public:

    void write_frame( const std::string& name,
                      const unsigned int& frame,
                      const unsigned int& width,
                      const unsigned int& height )
    {
        char file[128];

        sprintf( file, "%s_%.04d.tif",name.c_str(), frame );
        //printf( "%s\n", file );
       writetiff( file, "movie", 0, 0, width, height, COMPRESSION_NONE );
    }

	int writetiff(const char *f_name, const char *description, int x, int y, int width, int height, int compression) {
        TIFF *file;
        GLubyte *image, *p;
        int i;

        file = TIFFOpen(f_name, "w");
        if (file == NULL) {
            return 1;
        }
        //image = (GLubyte *) malloc(width * height * sizeof(GLubyte) * 3);
        image = new GLubyte[width * height * sizeof(GLubyte) * 3];
        glPixelStorei(GL_PACK_ALIGNMENT, 1);

        glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
        TIFFSetField(file, TIFFTAG_IMAGEWIDTH, (uint32) width);
        TIFFSetField(file, TIFFTAG_IMAGELENGTH, (uint32) height);
        TIFFSetField(file, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(file, TIFFTAG_COMPRESSION, compression);
        TIFFSetField(file, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
        TIFFSetField(file, TIFFTAG_SAMPLESPERPIXEL, 3);
        TIFFSetField(file, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(file, TIFFTAG_ROWSPERSTRIP, 1);
        TIFFSetField(file, TIFFTAG_IMAGEDESCRIPTION, description);
        p = image;
        for (i = height - 1; i >= 0; i--) {
            if (TIFFWriteScanline(file, p, i, 0) < 0) {
                //free(image);
                delete [] image;
                TIFFClose(file);
                return 1;
            }
            p += width * sizeof(GLubyte) * 3;
        }
        TIFFClose(file);
        return 0;
	}
};

#endif // _MOVIE_H_