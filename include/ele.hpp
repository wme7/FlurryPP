/*!
 * \file ele.hpp
 * \brief Header file for ele class
 *
 * \author - Jacob Crabill
 *           Aerospace Computing Laboratory (ACL)
 *           Aero/Astro Department. Stanford University
 *
 * \version 0.0.1
 *
 * Flux Reconstruction in C++ (Flurry++) Code
 * Copyright (C) 2014 Jacob Crabill.
 *
 */
#pragma once

#include "global.hpp"
//#include "mesh.hpp"
#include "matrix.hpp"
#include "geo.hpp"

#include <array>

class ele
{
friend class face;
friend class solver;

public:
  int ID, IDg; //! IDg will be for MPI (if I ever get to that; for now, just a reminder!)
  int eType;
  int order;
  int nDims; // should probably just go in Geo? Or not needed once eType set?
  int nNodes;
  int nSpts, nFpts; //! # of solution points, flux points

  vector<point> loc_spts; //! Location of solution points in parent domain
  vector<point> loc_fpts; //! Location of flux points in parent domain
  vector<point> nodes; //! Location of mesh nodes in physical space
  vector<int> nodeID; //! Global ID's of element's nodes
  vector<int> faceID; //! Global ID's of element's faces
  vector<bool> bndFace; //! Tag for faces on a boundary

//  mesh *Mesh; //! Pointer to mesh object to which ele 'belongs'
  geo* Geo; //! same as ^ - need to get rid of one
  input* params; //! Input parameters for simulation

  //! Default constructor
  ele();

  //! Alternate constructor (think I'll delete this)
  ele(int in_eType, int in_order, int in_ID, vector<point> &in_nodes, geo *in_Geo);

  void initialize(void);

  void setup(input *inParams, geo *inGeo);
  //void setup(int in_eType, int in_order, int in_ID, vector<double> &xy);

  void calcTransforms(void);

  void calcInviscidFlux_spts(void);

  void calcViscousFlux_spts(void);

private:
  // Solution Variables
  // Still undecided on how this will be stored - double*, vector<double>, something custom?
  matrix<double> U_spts;  //! Solution at solution points
  matrix<double> U_fpts;  //! Solution at flux points
  vector<matrix<double> > F_spts;  //! Flux at solution points
  vector<matrix<double> > F_fpts;  //! Flux at flux points
  matrix<double> Fn_fpts; //! Interface flux at flux points

  // Gradients
  vector<matrix<double> > dU_spts;  //! Gradient of solution at solution points
  vector<matrix<double> > dU_fpts;  //! Gradient of solution at flux points
  vector<matrix<double> > dF_spts;  //! Gradient of flux at solution points
  vector<matrix<double> > dF_fpts;  //! Gradient of flux at flux points

  // Transform Variables
  vector<double> detJac_spts;  //! Determinant of transformation Jacobian at each solution point
  vector<double> detJac_fpts;  //! Determinant of transformation Jacobian at each solution point
  vector<matrix<double> > Jac_spts;  //! Transformation Jacobian [matrix] at each solution point
  vector<matrix<double> > Jac_fpts;  //! Transformation Jacobian [matrix] at each flux point
  
  // Misc.
  /* NOTE: Can't have a pointer to an array<>, but can get a double* using
   * double* blah = norm_fpts.data() */
  vector<array<double,3> > norm_fpts;
  vector<array<double,3> > tNorm_fpts;
};