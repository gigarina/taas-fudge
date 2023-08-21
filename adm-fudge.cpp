/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
=================================================================================================================
Name : adm-fudge.cpp
Author : Carina Benzin
Version : 1.0
Copyright : GPL3
Description : The adm-fudge solver for abstract argumentation.
              Is based on taas-fudge by Matthias Thimm, Federico Cerutti and Mauro Vallati
===================================================================================================================
*/

// The following definitions are taken from taas-fudge.cpp
#define COMPUTATION_FINISHED 0
#define COMPUTATION_ABORTED__ANSWER_YES 1
#define COMPUTATION_ABORTED__ANSWER_NO  2
#define COMPUTATION_ABORTED__ANSWER_EMPTYSET  3
#define COMPUTATION_ABORTED__ANSWER_EMPTYEMPTYSET  4
#define COMPUTATION_FINISHED__EMPTY_GROUNDED  5
#define COMPUTATION_FINISHED__NONEMPTY_GROUNDED  6
#define COMPUTATION_FINISHED__ANSWER_NO  7

#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <ctype.h>
#include <cmath>
#include <glib.h>
#include <assert.h>
#include <stdbool.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "lib/pstreams-1.0.3/pstream.h"

#include "util/bitset.c"
#include "util/raset.c"
#include "util/miscutil.c"

#include "taas/taas_aaf.c"
#include "taas/taas_inout.c"
#include "taas/taas_labeling.c"
#include "adm-carina/adm_util/adm_defended.cpp"
#include "adm-carina/adm_util/adm_tried.cpp"

#include "adm-carina/tasks/task_dc-adm.cpp"


/**
 * @brief Main flow of the solver. 
 * Structure based on taas-fudge taas_basics.c taas__solve function.
*/
int adm__solve(int argc, char *argv[], struct SolverInformation* info){
	// Handle command parameters
	struct TaskSpecification *task = taas__cmd_handle(argc,argv,info);
	// if only solver information was asked, terminate
	if(task != NULL){
	// read file
		struct AAF *aaf = (struct AAF*) malloc(sizeof(struct AAF));
		// check format
		if(task->format != NULL && strcmp(task->format,"tgf") == 0)
			taas__readFile_tgf(task->file,aaf);
		taas__update_arg_param(task,aaf);
		// Solve the Problem with solve_dcadm
		solve_dcadm(task,aaf);

		taas__aaf_destroy(aaf);
	}
	taas__solverinformation_destroy(info);
	taas__cmd_destroy(task);
	return 0;
}

/**
 * @brief The function that starts the solver. 
 * Structure based on taas-fudge.cpp main function.
*/
int main(int argc, char *argv[]){
    struct SolverInformation *info = taas__solverinformation(
			"adm-fudge v1.0 (2023-08-17)\nCarina Benzin (carinabenzin@outlook.com)",
			"[tgf]",
            "[DC-CO,DC-PR]"
		);
        return adm__solve(argc, argv, info);
}
