/* -*- c++ -*- (enableMbars emacs c++ mode) */
/*======================================================================
    "Mixed Finite Element Methods for Coupled 3D/1D Transport Problems"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. A.Y. 2015-2016
                  
                Copyright (C) 2016 Stefano Brambilla
======================================================================*/
/*! 
  @file   transport3d1d.hpp
  @author Stefano Brambilla <s.brambilla93@gmail.com>
  @date   September 2016 - May 2018.
  @brief  Declaration of the main class for the 3D/1D coupled transport problem.
 */
 
#ifndef M3D1D_TRANSPORT3D1D_HPP_
#define M3D1D_TRANSPORT3D1D_HPP_


// GetFem++ libraries
#include <gmm/gmm_matrix.h>
#include <getfem/dal_bit_vector.h>

// Project headers
#include <problem3d1d.hpp>

#include <assembling1d_transp.hpp>          
#include <assembling3d_transp.hpp> 
#include <assembling3d1d_transp.hpp>        
#include <dof3d1d_transp.hpp>
#include <descr3d1d_transp.hpp>
#include <param3d1d_transp.hpp>
#include <utilities_transp.hpp>
#include <node_transp.hpp>



 
 namespace getfem {

//!	Main class defining the coupled 3D/1D transport problem.
class transport3d1d: public problem3d1d { 

public:
	transport3d1d(void) : 
		mf_Ct(mesht), mf_Cv(meshv){} 
	
	//! Initialize the transport problem
	void init_transp (int argc, char *argv[]);
	
	//! Assemble the transport problem
	void assembly_transp (void);

	//! Solve the transport problem
	bool solve_transp (void);
	
	//! Export the transport solution
	void export_vtk_transp (const string & time_suff = "",const string & suff = ""); 

	//! Compute residuals for mass balance at each junction
	void mass_balance (void);

	//Aux methods for interface with problem3d1d class
	//! Initialize the fluid problem
	void init_fluid (int argc, char *argv[]);
	//! Assemble the fluid problem
	void assembly_fluid (void);
	//! Solve the fluid problem
	bool solve_fluid (void);
	//! Export the fluid solution
	void export_vtk_fluid (const string & suff = "");
	
protected:
	 
	//! Finite Element Method for the tissue concentration @f$c_t@f$
	mesh_fem mf_Ct; 
	//! Finite Element Method for the vessel concentration @f$c_v@f$
	mesh_fem mf_Cv; 

	
	//! Algorithm description strings (mesh files, FEM types, solver info, ...) 
	descr3d1d_transp descr_transp;
	//! Physical parameters
	param3d1d_transp param_transp;
	//! Number of degrees of freedom
	dof3d1d_transp dof_transp;
		
	
	//! List of BC nodes of the network
	vector< node > BCv_transp;	
	//! List of BC nodes of the tissue
	vector< node > BCt_transp;
	//! List of junction nodes of the network
	vector< node_transp > Jv_transp;
		
	//! Monolithic matrix for the coupled problem
	sparse_matrix_type AM_transp;
	//! Monolithic array of unknowns for the coupled problem
	vector_type        UM_transp;
	//! Monolithic right hand side for the coupled problem
	vector_type        FM_transp;
	
	
	//Monolithic temporary matrix for update
	sparse_matrix_type AM_temp;
	//Monolithic temporary right hand side for update
	vector_type        FM_temp; 
	
	

	// Aux methods for init
	//! Import algorithm specifications
	void import_data_transp(void);
	//! Import mesh for tissue (3D) and vessel (1D)  
	void build_mesh_transp(void); 
	//! Set finite elements methods and integration methods 
	void set_im_and_fem_transp(void);
	//! Build problem parameters
	void build_param_transp(void);
	//! Build the list of tissue boundary data 
	/*!	Face numbering:
		  0 : {x = 0 }  "back"
		  1 : {x = Lx}  "front"
		  2 : {y = 0 }  "left"
		  3 : {y = Ly}  "right"
		  4 : {z = 0 }  "bottom"
		  5 : {z = Lz}  "top"
	 */
	void build_tissue_boundary_transp(void);
	//! Build the list of vessel boundary (and junctions) data 
	void build_vessel_boundary_transp(void);
	
	//Aux method for assembly
	//! Build the monolithic matrix AM_transp by blocks
	void assembly_mat_transp(void);
	//! Build the monolithic rhs FM_transp by blocks
	void assembly_rhs_transp(void);
	
	//Aux method for solve 
	//! Aux function for update of rhs at each time step
	void update_transp(void);
	

	
}; //end of class trasport3d1d

}  //end of namespace

#endif
