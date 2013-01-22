#ifndef __TINYEMBERROUTER_MODEL_MATRIX_LINEARMATRIX_H
#define __TINYEMBERROUTER_MODEL_MATRIX_LINEARMATRIX_H

#include "Matrix.h"

namespace model { namespace matrix
{
   /**
     * Abstract base class for matrices with linear addressing mode.
     */
   class LinearMatrix : public Matrix
   {
   public:
      /**
        * Creates a new instance of LinearMatrix.
        * @param number The number used for Ember+ automation.
        * @param parent Pointer to the parent of the new object - the ctor
        *     will enter the new object in the parent's collection of children.
        * @param identifier The identifier used for Ember+ automation.
        * @param notificationSink The object to notify of parameter value changes
        *     and matrix connections.
        * @param targetCount The number of targets to create.
        * @param sourceCount The number of sources to create.
        */
      LinearMatrix(int number, Element* parent, std::string const& identifier, NotificationSink* notificationSink, int targetCount, int sourceCount);

      /**
        * Overridden to return number of targets.
        */
      virtual int targetCount() const;

      /**
        * Overridden to return number of sources.
        */
      virtual int sourceCount() const;

      /**
        * Overridden find a target according to the rules
        * of linear addressing.
        */
      virtual Signal* getTarget(int number) const;

      /**
        * Overridden find a source according to the rules
        * of linear addressing.
        */
      virtual Signal* getSource(int number) const;
   };
}}

#endif//__TINYEMBERROUTER_MODEL_MATRIX_LINEARMATRIX_H
