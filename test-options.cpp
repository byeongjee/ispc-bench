/*
  Copyright (c) 2010-2011, Intel Corporation
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.


   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
   IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
   TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define NOMINMAX

#include <algorithm>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using std::max;

#include "timing.h"
#include "options_serial.h"

//using namespace ispc;


int main() {
    int nOptions = 128 * 1024;

    float *S = new float[nOptions];
    float *X = new float[nOptions];
    float *T = new float[nOptions];
    float *r = new float[nOptions];
    float *v = new float[nOptions];
    float *result = new float[nOptions];

    MemRefDescriptor<float, 1> S_desc = {
        .allocated = S,
        .aligned = S,
        .offset = 0,
        .sizes = {nOptions},
        .strides = {1}
    };
    MemRefDescriptor<float, 1> X_desc = {
        .allocated = X,
        .aligned = X,
        .offset = 0,
        .sizes = {nOptions},
        .strides = {1}
    };

    MemRefDescriptor<float, 1> T_desc = {
        .allocated = T,
        .aligned = T,
        .offset = 0,
        .sizes = {nOptions},
        .strides = {1}
    };
    MemRefDescriptor<float, 1> r_desc = {
        .allocated = r,
        .aligned = r,
        .offset = 0,
        .sizes = {nOptions},
        .strides = {1}
    };
    MemRefDescriptor<float, 1> v_desc = {
        .allocated = v,
        .aligned = v,
        .offset = 0,
        .sizes = {nOptions},
        .strides = {1}
    };
    MemRefDescriptor<float, 1> result_desc = {
        .allocated = result,
        .aligned = result,
        .offset = 0,
        .sizes = {nOptions},
        .strides = {1}
    };




    for (int i = 0; i < nOptions; ++i) {
        S[i] = 100; // stock price
        X[i] = 98;  // option strike price
        T[i] = 2;   // time (years)
        r[i] = .02; // risk-free interest rate
        v[i] = 5;   // volatility
    }

    double sum;

    //
    // Binomial options, serial implementation
    //
    reset_and_start_timer();
    #ifdef INTRINSIC_COMPILER
        _mlir_ciface_options_serial(&S_desc, &X_desc, &T_desc, &r_desc, &v_desc, &result_desc, nOptions, false);
    #else
        options_serial(S, X, T, r, v, result, nOptions, false);
    #endif
    double binomial_serial = get_elapsed_mcycles();
    sum = 0.;
    for (int i = 0; i < nOptions; ++i) {
        sum += result[i];
    }
    // printf("[binomial serial]:\t\t[%.3f] million cycles (avg %f)\n", binomial_serial, sum / nOptions);


    reset_and_start_timer();
    #ifdef INTRINSIC_COMPILER
        _mlir_ciface_options_serial(&S_desc, &X_desc, &T_desc, &r_desc, &v_desc, &result_desc, nOptions, true);
    #else
        options_serial(S, X, T, r, v, result, nOptions, true);
    #endif
    double bs_serial = get_elapsed_mcycles();
    sum = 0.;
    for (int i = 0; i < nOptions; ++i) {
        sum += result[i];
    }

    double total = bs_serial;
    printf ("[execution time] %0.6f\n", total);


    return 0;
}
