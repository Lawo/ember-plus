#ifndef __TINYEMBERROUTER_MODEL_MATRIX_DETAIL_CONNECT_H
#define __TINYEMBERROUTER_MODEL_MATRIX_DETAIL_CONNECT_H

#include "../Signal.h"
#include "../../../util/Types.h"

namespace model { namespace matrix { namespace detail
{
   /**
     * Connects the first source in @p sources to target at @p target
     * (1:N connection semantics).
     * @param target Pointer to the target to connect to.
     * @param sources Collection of pointers to sources to connect to @p target.
     * @param state Caller-defined state to pass through.
     * @param operation The desired connection operation.
     * @return True if the connection could be made, false otherwise.
     */
   bool connectOneToN(Signal* target, Signal::Vector const& sources, void* state, util::ConnectOperation const& operation);

   /**
     * Connects or disconnects the sources in @p sources to/from target at @p target
     * (N:N connection semantics).
     * @param target Pointer to the target to connect to.
     * @param sources Collection of pointers to sources to connect to @p target.
     * @param state Caller-defined state to pass through.
     * @param operation The desired connection operation.
     * @return True if the connection could be made, false otherwise.
     */
   bool connectNToN(Signal* target, Signal::Vector const& sources, void* state, util::ConnectOperation const& operation);
}}}

#endif
