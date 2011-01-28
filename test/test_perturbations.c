/** @file class.c 
 * Julien Lesgourgues, 18.04.2010    
 */

#include "class.h"

main(int argc, char **argv) {

  struct precision pr;        /* for precision parameters */
  struct background ba;       /* for cosmological background */
  struct thermo th;           /* for thermodynamics */
  struct perturbs pt;         /* for source functions */
  struct bessels bs;          /* for bessel functions */
  struct transfers tr;        /* for transfer functions */
  struct primordial pm;       /* for primordial spectra */
  struct spectra sp;          /* for output spectra */
  struct nonlinear nl;        /* for non-linear spectra */
  struct lensing le;          /* for lensed spectra */
  struct output op;           /* for output files */

  ErrorMsg errmsg;

  if (input_init_from_arguments(argc, argv,&pr,&ba,&th,&pt,&bs,&tr,&pm,&sp,&nl,&le,&op,errmsg) == _FAILURE_) {
    printf("\n\nError running input_init_from_arguments \n=>%s\n",errmsg); 
    return _FAILURE_;
  }

  if (background_init(&pr,&ba) == _FAILURE_) {
    printf("\n\nError running background_init \n=>%s\n",ba.error_message);
    return _FAILURE_;
  }

  if (thermodynamics_init(&pr,&ba,&th) == _FAILURE_) {
    printf("\n\nError in thermodynamics_init \n=>%s\n",th.error_message);
    return _FAILURE_;
  }

  if (perturb_init(&pr,&ba,&th,&pt) == _FAILURE_) {
    printf("\n\nError in perturb_init \n=>%s\n",pt.error_message);
    return _FAILURE_;
  }

  /****** here you can output the source functions ******/

  if (pt.has_source_g == _TRUE_) {

    FILE * output;
    int index_k,index_eta;
    int index_mode=pt.index_md_scalars;
    int index_type=pt.index_tp_g;
    int index_ic=pt.index_ic_ad;

    output=fopen("output/source1.dat","w");

    for (index_k=0; index_k < pt.k_size[index_mode]; index_k++) {
/*     for (index_k=pt.k_size[index_mode]-1; index_k < pt.k_size[index_mode]; index_k++) { */
      for (index_eta=0; index_eta < pt.eta_size; index_eta++) { 
	
	fprintf(output,"%e %e %e\n",
		pt.k[index_mode][index_k],
		pt.eta_sampling[index_eta],
		pt.sources[index_mode]
		[index_ic * pt.tp_size[index_mode] + index_type]
		[index_eta * pt.k_size[index_mode] + index_k]
		);
      }
      fprintf(output,"\n");
    }
    
    fclose(output);
  }

  if (pt.has_matter_transfers == _TRUE_) {

    FILE * output;
    int index_k,index_eta;
    int index_mode=pt.index_md_scalars;

    int index_ic=pt.index_ic_ad;

    output=fopen("output/transfers.dat","w");

    for (index_eta=0; index_eta < pt.eta_size; index_eta++) { 
      for (index_k=0; index_k < pt.k_size[index_mode]; index_k++) {

	fprintf(output,"%e %e %e %e %e %e\n",
		pt.eta_sampling[index_eta],
		pt.k[index_mode][index_k],
		pt.sources[index_mode]
		[index_ic * pt.tp_size[index_mode] + pt.index_tp_delta_g]
		[index_eta * pt.k_size[index_mode] + index_k],
		pt.sources[index_mode]
		[index_ic * pt.tp_size[index_mode] + pt.index_tp_delta_b]
		[index_eta * pt.k_size[index_mode] + index_k],
		pt.sources[index_mode]
		[index_ic * pt.tp_size[index_mode] + pt.index_tp_delta_cdm]
		[index_eta * pt.k_size[index_mode] + index_k],
		pt.sources[index_mode]
		[index_ic * pt.tp_size[index_mode] + pt.index_tp_delta_nur]
		[index_eta * pt.k_size[index_mode] + index_k]
		);
      }
      fprintf(output,"\n");
    }
    
    fclose(output);
  }

  /******************************************************/

  if (perturb_free(&pt) == _FAILURE_) {
    printf("\n\nError in perturb_free \n=>%s\n",pt.error_message);
    return _FAILURE_;
  }

  if (thermodynamics_free(&th) == _FAILURE_) {
    printf("\n\nError in thermodynamics_free \n=>%s\n",th.error_message);
    return _FAILURE_;
  }

  if (background_free(&ba) == _FAILURE_) {
    printf("\n\nError in background_free \n=>%s\n",ba.error_message);
    return _FAILURE_;
  }

}
