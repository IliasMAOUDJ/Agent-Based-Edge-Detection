#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "LibImages.h"

typedef struct {
                Image im;
                Image originale;
                Image preprocessed;
                Image resultat;
                Image postprocessed;
               }                                  System;

#endif // _SYSTEM_H_
