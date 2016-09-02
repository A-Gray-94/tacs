#include "TACSIntegrator.h"
#include "TACSAssembler.h"
#include "RigidBody.h"

/*
  Function to test the rigid body dynamics implementation
*/
int main( int argc, char *argv[] ){
  // Initialize MPI
  MPI_Init(&argc, &argv);

  // Construct the frame of reference
  const TacsScalar r0[3] = {0.0, 0.0, 0.0}; // The base point
  const TacsScalar r1[3] = {1.0, 0.0, 0.0}; // The first coordinate direction
  const TacsScalar r2[3] = {0.0, 1.0, 0.0}; // The second coordinate direction
  TACSGibbsVector *r0Vec = new TACSGibbsVector(r0); r0Vec->incref();
  TACSGibbsVector *r1Vec = new TACSGibbsVector(r1); r1Vec->incref();
  TACSGibbsVector *r2Vec = new TACSGibbsVector(r2); r2Vec->incref();

  TACSRefFrame *refFrameA = new TACSRefFrame(r0Vec, r1Vec, r2Vec);

  // Define the inertial properties
  const TacsScalar mass = 4.0;
  const TacsScalar c[]  = {0.5, 0.2, -0.1};
  const TacsScalar J[]  = {1.0, -0.1, 0.25,
                           2.0, 0.1,
                           0.75};

  // Define dynamics properties
  const TacsScalar grav[3]      = {0.0, 0.0, -10.0}; // The acceleration due to gravity
  const TacsScalar rInit[3]     = {0.0, 0.0, 0.0}; // The initial position
  const TacsScalar vInit[3]     = {0.0, 1.0, 0.0}; // The initial velocity
  const TacsScalar omegaInit[3] = {0.0, 1.0, 0.0}; // The initial angular velocity
  TACSGibbsVector *gravVec      = new TACSGibbsVector(grav); gravVec->incref();
  TACSGibbsVector *rInitVec     = new TACSGibbsVector(rInit); rInitVec->incref();
  TACSGibbsVector *vInitVec     = new TACSGibbsVector(vInit); vInitVec->incref();
  TACSGibbsVector *omegaInitVec = new TACSGibbsVector(omegaInit); omegaInitVec->incref();

  // Construct a rigid body
  TACSRigidBody *bodyA = new  TACSRigidBody(refFrameA, 
                                            mass, c, J,
                                            rInitVec, vInitVec, omegaInitVec, gravVec);

  /*
    TACSRigidBody *bodyA = TACSRigidBody::getInstance(r0, r1, r2,
    mass, c, J, 
    rInit, vInit, omegaInit, grav);
    bodyA->incref();
  */

  // Modify the inertial properties to use with the second body
  //  mass *= 2.0;
  //   J[0] += 1.0;
  
  TACSRigidBody *bodyB = new  TACSRigidBody(refFrameA, 
                                            mass, c, J,
                                            rInitVec, vInitVec, omegaInitVec, gravVec);
  bodyB->incref();
  
  // Create the constraint
  TacsScalar point[3]  = {0.0, 0.0, 1.0};
  TACSGibbsVector *pointVec = new TACSGibbsVector(point); pointVec->incref();

  // Construct the spherical constraint
  TACSSphericalConstraint *con = new TACSSphericalConstraint(bodyA, bodyB, pointVec);
  con->incref();
 
  // Set up the TACSAssembler object
  int num_nodes     = 3;
  int vars_per_node = 8;
  int num_elems     = 3;

  TACSAssembler *tacs = new TACSAssembler(MPI_COMM_WORLD, vars_per_node,
                                          num_nodes, num_elems);
  tacs->incref();

  // Set the elements
  TACSElement *elements[] = {bodyA, bodyB, con};
  tacs->setElements(elements);

  // Set the connectivity
  int conn[] = {0, 1, 0, 1, 2};
  int ptr[]  = {0, 1, 2, 5};
  tacs->setElementConnectivity(conn, ptr);

  tacs->initialize();

  // Create the TACSIntegrator object
  double tinit = 0.0, tfinal = 0.1;
  int steps_per_second = 1000; int num_stages = 2;


  TACSBDFIntegrator *bdf = new TACSBDFIntegrator(tacs, tinit, tfinal,
                                                 steps_per_second, 2);
  bdf->incref();
  
  // Set optional parameters
  bdf->setRelTol(1.0e-10);
  bdf->setAbsTol(1.0e-14);
  bdf->setMaxNewtonIters(24);
  bdf->setPrintLevel(1);
  bdf->setJacAssemblyFreq(1);
  bdf->setUseLapack(0);

  // Integrate and write solution to file
  bdf->integrate();
  bdf->writeSolution("solutionBDF.dat");
  bdf->decref();
  /*
  TACSDIRKIntegrator *dirk = 
    new TACSDIRKIntegrator(tacs, tinit, tfinal,
                           steps_per_second, num_stages);
  dirk->incref();

  // Set optional parameters
  dirk->setRelTol(1.0e-10);
  dirk->setAbsTol(1.0e-14);
  dirk->setMaxNewtonIters(24);
  dirk->setPrintLevel(1);
  dirk->setJacAssemblyFreq(1);
  dirk->setUseLapack(0);
  
  // Integrate and write solution to file
  dirk->integrate();
  dirk->writeSolution("solutionDIRK.dat");
  dirk->decref();
  */

  // Delete objects
  bodyA->decref();
  bodyB->decref();
  con->decref();

  r0Vec->decref();  
  r1Vec->decref();
  r2Vec->decref();

  gravVec->decref();
  rInitVec->decref();
  vInitVec->decref();

  tacs->decref();

  MPI_Finalize();
  return (0);
}


/*

// bodyA->testResidual(1e-4);
  
// Modify the inertial properties
mass *= 2.0;
J[0] += 1.0;
TACSRigidBody *bodyB;// = new TACSRigidBody(mass, c, J);
bodyB->incref();

// Create the constraint
TACSSphericalConstraint *con;// = new TACSSphericalConstraint();
*/
//  con->incref();

//  TACSRevoluteConstraint *rev = new TACSRevoluteConstraint();

// TestElement *test = new TestElement(rev);
/*
  TestElement *test = new TestElement(rev);
  test->setPrintLevel(2);
  for ( int i = 0; i < 24; i++ ){
  test->testJacobian(i);
  }
*/
