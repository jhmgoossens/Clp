// Copyright (C) 2003, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef ClpGubDynamicMatrix_H
#define ClpGubDynamicMatrix_H


#include "CoinPragma.hpp"

#include "ClpGubMatrix.hpp"
#include "CoinFactorization.hpp"
/** This implements Gub rows plus a ClpPackedMatrix.
    This a dynamic version which stores the gub part and dynamically creates matrix.
    All bounds are assumed to be zero and infinity

    This is just a simple example for real column generation
*/

class ClpGubDynamicMatrix : public ClpGubMatrix {
  
public:
  /// Partial pricing 
  virtual void partialPricing(ClpSimplex * model, int start, int end,
			      int & bestSequence, int & numberWanted);
  /** This is local to Gub to allow synchronization:
      mode=0 when status of basis is good 
      mode=1 when variable is flagged
      mode=2 when all variables unflagged (returns number flagged)
      mode=3 just reset costs (primal)
  */
  virtual int synchronize(ClpSimplex * model,int mode);
  /// Sets up an effective RHS and does gub crash if needed
  virtual void useEffectiveRhs(ClpSimplex * model,bool cheapest=true);
  /** 
     update information for a pivot (and effective rhs)
  */
  virtual int updatePivot(ClpSimplex * model,double oldInValue, double oldOutValue);
  /// Add a new variable to a set
  void insertNonBasic(int sequence, int iSet);
  //@}

  
  
  /**@name Constructors, destructor */
  //@{
  /** Default constructor. */
  ClpGubDynamicMatrix();
  /** Destructor */
  virtual ~ClpGubDynamicMatrix();
  //@}
  
  /**@name Copy method */
  //@{
  /** The copy constructor. */
  ClpGubDynamicMatrix(const ClpGubDynamicMatrix&);
  /** This is the real constructor.
      It assumes factorization frequency will not be changed.
      This resizes model !!!!
   */
  ClpGubDynamicMatrix(ClpSimplex * model, int numberSets,
		      int numberColumns, const int * starts,
		      const double * lower, const double * upper,
		      const int * startColumn, const int * row,
		      const double * element, const double * cost,
		      const unsigned char * status=NULL);
  
  ClpGubDynamicMatrix& operator=(const ClpGubDynamicMatrix&);
  /// Clone
  virtual ClpMatrixBase * clone() const ;
  //@}
  /**@name gets and sets */
  //@{
  /// Whether flagged
  inline bool flagged(int i) const {
    int word = i >> COINFACTORIZATION_SHIFT_PER_INT;
    int bit = i & COINFACTORIZATION_MASK_PER_INT;
    return (flagged_[word]&(1<<bit))!=0;
  }
  inline void setFlagged(int i) {
    int word = i >> COINFACTORIZATION_SHIFT_PER_INT;
    int bit = i & COINFACTORIZATION_MASK_PER_INT;
    flagged_[word] |= (1<<bit);
  }
  inline void unsetFlagged(int i) {
    int word = i >> COINFACTORIZATION_SHIFT_PER_INT;
    int bit = i & COINFACTORIZATION_MASK_PER_INT;
    flagged_[word]  &= ~(1<<bit);;
  }
  //@}
   
    
protected:
  /**@name Data members
     The data members are protected to allow access for derived classes. */
  //@{
  /// Starts of each column
  int * startColumn_;
  /// rows
  int * row_;
  /// elements
  float * element_;
  /// costs
  float * cost_;
  /// full starts
  int * fullStart_;
  /// ids of active columns (just index here)
  int * id_;
  /// for flagging variables
  unsigned int * flagged_;
  /// size
  int numberGubColumns_;
  /// first free
  int firstAvailable_;
  /// first dynamic
  int firstDynamic_;
  /// number of columns in dynamic model
  int lastDynamic_;
  /// size of working matrix (max)
  int numberElements_;
   //@}
};

#endif
