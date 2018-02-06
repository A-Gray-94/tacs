/*
  This file is part of TACS: The Toolkit for the Analysis of Composite
  Structures, a parallel finite-element code for structural and
  multidisciplinary design optimization.

  Copyright (C) 2014 Georgia Tech Research Corporation

  TACS is licensed under the Apache License, Version 2.0 (the
  "License"); you may not use this software except in compliance with
  the License.  You may obtain a copy of the License at
  
  http://www.apache.org/licenses/LICENSE-2.0 
*/

#ifndef TACS_GIBBS_VECTOR_H
#define TACS_GIBBS_VECTOR_H

/*
  The following class defines a Gibbs vector: A 3D vector in the
  global inertial reference frame.

  The components of this vector can be set as design variables that
  modify either initial conditions or the initial orientation of the
  bodies within a multibody system.  
*/
class TACSGibbsVector : public TACSObject {
 public:
  TACSGibbsVector( const TacsScalar _x[] );
  TACSGibbsVector( const TacsScalar _x[], const int _xDV[] );
  TACSGibbsVector( TacsScalar x1, TacsScalar x2, TacsScalar x3,
                   int n1=-1, int n2=-1, int n3=-1 );
  ~TACSGibbsVector(){}

  void getVector( const TacsScalar **_x );
  void setDesignVars( const TacsScalar *dvs, int numDVs );
  void getDesignVars( TacsScalar *dvs, int numDVs );
  void addPointAdjResProduct( TacsScalar fdvSens[], int numDVs,
                              TacsScalar scale,
                              const TacsScalar psi[] );
  void getVectorDesignVarNums( const int **dvs );

 private:
  TacsScalar x[3];
  int xDV[3];
};

/*
  A vector class for the multibody dynamics code. 

  These vectors are global objects that exist in the global reference
  frame. The purpose of this object is to provide an easier input to
  the initial set up of dynamics problems. All input is specified in a
  global reference frame (except for body-fixed frame inertial
  properties.)

  Reference points within the body-fixed frames are generated by each
  body at the initial point for analysis.

  input:
  x:    the 3-space coordinate of the point
  
  Note: The design variable numbers are initialized to -1
*/
inline TACSGibbsVector::TACSGibbsVector( const TacsScalar _x[] ){
  x[0] = _x[0];
  x[1] = _x[1];
  x[2] = _x[2];
  xDV[0] = xDV[1] = xDV[2] = -1;
}

/*
  The alternate constructor

  Create the vector object using the coordinates of the point within
  the inertial reference frame and the corresponding design variable
  numbers.
  
  input:
  x1, x2, x3:   the 3 coordinates
  n1, n2, n3:   the design variable numbers
*/
inline TACSGibbsVector::TACSGibbsVector( TacsScalar x1, 
                                         TacsScalar x2, 
                                         TacsScalar x3,
                                         int n1, int n2, int n3 ){
  x[0] = x1;
  x[1] = x2;
  x[2] = x3;
  xDV[0] = n1;
  xDV[1] = n2;
  xDV[2] = n3;
}

/*
  Create an point in the inertial reference frame

  input:
  x:    the coordinate of the point
  xDV:  the design variable numbers of the point
*/
inline TACSGibbsVector::TACSGibbsVector( const TacsScalar _x[], 
                                         const int _xDV[] ){
  x[0] = _x[0];
  x[1] = _x[1];
  x[2] = _x[2];
  xDV[0] = _xDV[0];
  xDV[1] = _xDV[1];
  xDV[2] = _xDV[2];
}

/*
  Retrieve the coordinates of the point
*/
inline void TACSGibbsVector::getVector( const TacsScalar **_x ){ 
  *_x = x;
}
 
/*
  Set the design variable values from the input vector into this object

  input:
  dvs:    the design variable values
  numDVs: the number of design variable values in the array
*/
inline void TACSGibbsVector::setDesignVars( const TacsScalar *dvs, int numDVs ){
  if (xDV[0] >= 0 && xDV[0] < numDVs){ x[0] = dvs[xDV[0]]; }
  if (xDV[1] >= 0 && xDV[1] < numDVs){ x[1] = dvs[xDV[1]]; }
  if (xDV[2] >= 0 && xDV[2] < numDVs){ x[2] = dvs[xDV[2]]; }
}

/*
  Retrieve the design variable values from the object
  
  input:
  numDVs:  the number of design variable values in the array

  output:
  dvs:     the design variable values retrieved from the object
*/
inline void TACSGibbsVector::getDesignVars( TacsScalar *dvs, int numDVs ){
  if (xDV[0] >= 0 && xDV[0] < numDVs){ dvs[xDV[0]] = x[0]; }
  if (xDV[1] >= 0 && xDV[1] < numDVs){ dvs[xDV[1]] = x[1]; }
  if (xDV[2] >= 0 && xDV[2] < numDVs){ dvs[xDV[2]] = x[2]; }
}

/*
  Add the derivative associated with the point locations to the vector.

  input:
  numDVs:  the number of design variables
  scale:   the sensitivity is multiplied by this scalar
  psi:     the adjoint vector
  
  input/output:
  fdvSens: the array of derivatives
*/
inline void TACSGibbsVector::addPointAdjResProduct( TacsScalar fdvSens[], 
                                                    int numDVs,
                                                    TacsScalar scale,
                                                    const TacsScalar psi[] ){
  if (xDV[0] >= 0 && xDV[0] < numDVs){ fdvSens[xDV[0]] += scale*psi[0]; }
  if (xDV[1] >= 0 && xDV[1] < numDVs){ fdvSens[xDV[1]] += scale*psi[1]; }
  if (xDV[2] >= 0 && xDV[2] < numDVs){ fdvSens[xDV[2]] += scale*psi[2]; }
}

/*
  Retrieve the design variable numbers from the Gibbs vector object

  output:
  dvs:  the design variable numbers
*/
inline void TACSGibbsVector::getVectorDesignVarNums( const int **dvs ){
  *dvs = xDV;
}

#endif // TACS_GIBBS_VECTOR_H
