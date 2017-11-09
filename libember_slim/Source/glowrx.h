/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_SLIM_GLOWRX_H
#define __LIBEMBER_SLIM_GLOWRX_H

#include "api.h"
#include "glow.h"

// ====================================================================
//
// NonFramingGlowReader
//
// ====================================================================

/**
* Function type used by NonFramingGlowReader to notify the application
* of an incoming glow template.
* @param pTemplate pointer to the read template.
* @param fields flags indicating which fields of @p pParameter have been
*     read.
* @param entering
*       true, if the template is now being entered. All subsequent calls to other on... calls are now
*       within the context of this template until this callback is invoked again with entering
*       set to false.
*
*       Please note that all elements reported within the template context contain the full path, not the
*       path relative to the template. Typically, only the number of the template element is of importance.
*       However, if the relative path to the template is needed, it must be computed separately.
* @param pPath pointer to the first number in the node path, which is
*     the number of the tree's root node. May be NULL only if
*     pathLength is 0.
*     See documentation of glow_writeQualifiedParameter for an example.
* @param pathLength number of node numbers at @p pPath.
* @param state application-defined state as stored in NonFramingGlowReader.
*/
typedef void(*onTemplate_t)(const GlowTemplate *pTemplate, GlowFieldFlags fields, bool entering, const berint *pPath, int pathLength, voidptr state);

/**
  * Function type used by NonFramingGlowReader to notify the application
  * of an incoming glow parameter.
  * @param pParameter pointer to the read parameter.
  * @param fields flags indicating which fields of @p pParameter have been
  *     read.
  * @param pPath pointer to the first number in the node path, which is
  *     the number of the tree's root node. May be NULL only if
  *     pathLength is 0.
  *     See documentation of glow_writeQualifiedParameter for an example.
  * @param pathLength number of node numbers at @p pPath.
  * @param state application-defined state as stored in NonFramingGlowReader.
  */
typedef void (*onParameter_t)(const GlowParameter *pParameter, GlowFieldFlags fields, const berint *pPath, int pathLength, voidptr state);

/**
  * Function type used by NonFramingGlowReader to notify the application
  * of an incoming glow node.
  * @param pNode pointer to the read node.
  * @param fields flags indicating which fields of @p pNode have been
  *     read.
  * @param pPath pointer to the first number in the node path, which is
  *     the number of the tree's root node. May be NULL only if
  *     pathLength is 0.
  *     See documentation of glow_writeQualifiedNode for an example.
  * @param pathLength number of node numbers at @p pPath.
  * @param state application-defined state as stored in NonFramingGlowReader.
  */
typedef void (*onNode_t)(const GlowNode *pNode, GlowFieldFlags fields, const berint *pPath, int pathLength, voidptr state);

/**
  * Function type used by NonFramingGlowReader to notify the application
  * of an incoming glow command.
  * @param pCommand pointer to the read command.
  * @param pPath pointer to the first number in the node path, which is
  *     the number of the tree's root node. May be NULL only if
  *     pathLength is 0.
  *     See documentation of glow_writeQualifiedCommand for an example.
  * @param pathLength number of node numbers at @p pPath.
  * @param state application-defined state as stored in NonFramingGlowReader.
  */
typedef void (*onCommand_t)(const GlowCommand *pCommand, const berint *pPath, int pathLength, voidptr state);

/**
  * Function type used by NonFramingGlowReader to notify the application
  * of an incoming glow stream entry.
  * @param pStreamEntry pointer to the read stream entry.
  * @param state application-defined state as stored in NonFramingGlowReader.
  */
typedef void (*onStreamEntry_t)(const GlowStreamEntry *pStreamEntry, voidptr state);

/**
  * Function type used by NonFramingGlowReader to notify the application
  * of an incoming matrix.
  * @param pMatrix pointer to the read matrix.
  * @param pPath pointer to the first number in the node path, which is
  *     the number of the tree's root node. May be NULL only if
  *     pathLength is 0.
  *     See documentation of glow_writeQualifiedNode for an example.
  * @param pathLength number of node numbers at @p pPath.
  * @param state application-defined state as stored in NonFramingGlowReader.
  */
typedef void (*onMatrix_t)(const GlowMatrix *pMatrix, const berint *pPath, int pathLength, voidptr state);

/**
  * Function type used by NonFramingGlowReader to notify the application
  * of an incoming signal.
  * @param pSignal pointer to the read signal.
  * @param pPath pointer to the first number in the node path, which is
  *     the number of the tree's root node. May be NULL only if
  *     pathLength is 0.
  *     This path identifies the element containing the signal (usually a matrix).
  * @param pathLength number of node numbers at @p pPath.
  * @param state application-defined state as stored in NonFramingGlowReader.
  */
typedef void (*onSignal_t)(const GlowSignal *pSignal, const berint *pPath, int pathLength, voidptr state);

/**
  * Function type used by NonFramingGlowReader to notify the application
  * of an incoming connection.
  * @param pConnection pointer to the read connection.
  * @param pPath pointer to the first number in the node path, which is
  *     the number of the tree's root node. May be NULL only if
  *     pathLength is 0.
  *     This path identifies the element containing the connection (usually a matrix).
  * @param pathLength number of node numbers at @p pPath.
  * @param state application-defined state as stored in NonFramingGlowReader.
  */
typedef void (*onConnection_t)(const GlowConnection *pConnection, const berint *pPath, int pathLength, voidptr state);

/**
  * Function type used by NonFramingGlowReader to notify the application
  * of an incoming function.
  * @param pFunction pointer to the read function.
  * @param pPath pointer to the first number in the node path, which is
  *     the number of the tree's root node. May be NULL only if
  *     pathLength is 0.
  *     This path identifies the element containing the connection (usually a matrix).
  * @param pathLength number of node numbers at @p pPath.
  * @param state application-defined state as stored in NonFramingGlowReader.
  */
typedef void (*onFunction_t)(const GlowFunction *pFunction, const berint *pPath, int pathLength, voidptr state);

/**
  * Function type used by NonFramingGlowReader to notify the application
  * of an incoming invocation result.
  * @param pInvocationResult pointer to the read function.
  * @param state application-defined state as stored in NonFramingGlowReader.
  */
typedef void (*onInvocationResult_t)(const GlowInvocationResult *pFunction, voidptr state);

/**
  * Lists the possible states of a GlowReader when calling the
  * unsupportedTtlv callback.
  */
typedef enum EGlowReaderPosition
{
   GlowReaderPosition_NodeContents,
   GlowReaderPosition_ParameterContents,
   GlowReaderPosition_Command,
   GlowReaderPosition_StreamEntry,
   GlowReaderPosition_MatrixContents,
   GlowReaderPosition_Target,
   GlowReaderPosition_Source,
   GlowReaderPosition_Connection,
   GlowReaderPosition_FunctionContents,
   GlowReaderPosition_InvocationResult,
   GlowReaderPosition_Template
} GlowReaderPosition;

/**
  * Function type used by NonFramingGlowReader to notify the application
  * of a read field that is not supported by the reader.
  * @param pReader pointer to the reader object which is positioned on the
  *     unkown TTLV.
  * @param pPath pointer to the first number in the node path, which is
  *     the number of the tree's root node. May be NULL only if
  *     pathLength is 0.
  *     See documentation of glow_writeQualifiedNode for an example.
  * @param pathLength number of node numbers at @p pPath.
  * @param position describes the type of container the NonFramingGlowReader
  *     is positioned on.
  * @param state application-defined state as stored in NonFramingGlowReader.
  */
typedef void (*onUnsupportedTltlv_t)(const BerReader *pReader, const berint *pPath, int pathLength, GlowReaderPosition position, voidptr state);

/**
  * Aggregates EmberAsyncReader, adding functionality for the 
  * Glow DTD.
  * Overrides EmberAsyncReader.onItemReady and
  * EmberAsyncReader.onNewContainer.
  * @note This structure is huge, therefore it is strongly recommended
  *     to put it on the heap.
  */
typedef struct SNonFramingGlowReader
{
   /**
     * Base async reader.
     */
   EmberAsyncReader base;

   /**
     * Private field.
     */
   berint path[GLOW_MAX_TREE_DEPTH];

   /**
     * Private field.
     */
   int pathLength;

   union
   {
      /**
        * Private field.
        */
      GlowNode node;

      /**
        * Private field.
        */
      GlowParameter parameter;

      /**
        * Private field.
        */
      GlowCommand command;

      /**
        * Private field.
        */
      GlowStreamEntry streamEntry;

      /**
        * Private field.
        */
      GlowMatrix matrix;

      /**
        * Private field.
        */
      GlowSignal signal;

      /**
        * Private field.
        */
      GlowConnection connection;

      /**
        * Private field.
        */
      GlowFunction function;

      /**
        * Private field.
        */
      GlowInvocationResult invocationResult;

      /**
       * Private field.
       */
      GlowTemplate template_;
   } glow;

   /**
     * Private field.
     */
   GlowFieldFlags fields;

   /**
     * Private field.
     */
   void (*onItemReadyState)(struct SNonFramingGlowReader *pThis);

   /**
   * May be set to a callback function called when
   * a glow template has been read.
   */
   onTemplate_t onTemplate;

   /**
     * May be set to a callback function called when
     * a glow node has been read.
     */
   onNode_t onNode;

   /**
     * May be set to a callback function called when
     * a glow parameter has been read.
     */
   onParameter_t onParameter;

   /**
     * May be set to a callback function called when
     * a glow command has been read.
     */
   onCommand_t onCommand;

   /**
     * May be set to a callback function called when
     * a glow stream entry has been read.
     */
   onStreamEntry_t onStreamEntry;

   /**
     * May be set to a callback function called when
     * a glow matrix has been read.
     */
   onMatrix_t onMatrix;

   /**
     * May be set to a callback function called when
     * a glow target has been read.
     */
   onSignal_t onTarget;

   /**
     * May be set to a callback function called when
     * a glow source has been read.
     */
   onSignal_t onSource;

   /**
     * May be set to a callback function called when
     * a glow target has been read.
     */
   onConnection_t onConnection;

   /**
     * May be set to a callback function called when
     * a glow function has been read.
     */
   onFunction_t onFunction;

   /**
     * May be set to a callback function called when
     * a glow invocation result has been read.
     */
   onInvocationResult_t onInvocationResult;

   /**
     * May be set to a callback function called when an unsupported TTLV
     * has been read.
     * The application code can evaluate the TTLV using the passed Arguments.
     */
   onUnsupportedTltlv_t onUnsupportedTltlv;

   /**
     * Application-defined argument passed to callback
     * functions.
     */
   voidptr state;
} NonFramingGlowReader;

/**
  * Initializes a NonFramingGlowReader instance.
  * Must be called before any other operations on the
  * NonFramingGlowReader instance are invoked.
  * @param pThis pointer to the object to process.
  * @param onNode callback function to called by the reader
  *     when a glow node has been read.
  * @param onParameter callback function to called by the reader
  *     when a glow parameter has been read.
  * @param onCommand callback function to called by the reader
  *     when a glow command has been read.
  * @param state application-defined argument passed to
  *     callback functions.
  * @note you need to call nonFramingGlowReader_free to release
  *     all memory allocated by nonFramingGlowReader_init.
  */
LIBEMBER_API void nonFramingGlowReader_init(NonFramingGlowReader *pThis,
                               onNode_t onNode,
                               onParameter_t onParameter,
                               onCommand_t onCommand,
                               onStreamEntry_t onStreamEntry,
                               voidptr state);

/**
* Initializes a NonFramingGlowReader instance.
* Must be called before any other operations on the
* NonFramingGlowReader instance are invoked.
* @param pThis pointer to the object to process.
* @param onNode callback function to called by the reader
*     when a glow node has been read.
* @param onParameter callback function to called by the reader
*     when a glow parameter has been read.
* @param onCommand callback function to called by the reader
*     when a glow command has been read.
* @param onFunction callback function called by the reader
*     when a glow function has been read.
* @param onTemplate callback function called by the reader
*     when a glow template has been read.
* @param state application-defined argument passed to
*     callback functions.
* @note you need to call nonFramingGlowReader_free to release
*     all memory allocated by nonFramingGlowReader_init.
*/
LIBEMBER_API void nonFramingGlowReader_initEx(NonFramingGlowReader *pThis,
    onNode_t onNode,
    onParameter_t onParameter,
    onCommand_t onCommand,
    onStreamEntry_t onStreamEntry,
    onFunction_t onFunction,
    onTemplate_t onTemplate,
    voidptr state);

/**
  * Frees all dynamically allocated memory used by the
  * passed NonFramingGlowReader. The memory has been allocated
  * by a prior call to nonFramingGlowReader_init.
  * @param pThis pointer to the object to process.
  */
LIBEMBER_API void nonFramingGlowReader_free(NonFramingGlowReader *pThis);

/**
  * Resets the internal state of the passedNonFramingGlowReader.
  * @param pThis pointer to the object to process.
  */
LIBEMBER_API void nonFramingGlowReader_reset(NonFramingGlowReader *pThis);


// ====================================================================
//
// GlowReader
//
// ====================================================================

/**
  * Aggregates both NonFramingGlowReader and EmberFramingReader
  * to provide ember reading functionality specialized on the Glow DTD,
  * unframing incoming packages on the fly.
  * @note This structure is huge, therefore it is strongly recommended
  *     to put it on the heap.
  */
typedef struct SGlowReader
{
   /**
     * The base glow reader.
     */
   NonFramingGlowReader base;

   /**
     * The frame reader used for unframing.
     */
   EmberFramingReader framing;

   /**
     * Callback invoked everytime a valid framing package has been
     * received.
     */
   onPackageReceived_t onPackageReceived;

   /**
     * Callback invoked everytime a non-ember packet has been received.
     * set.
     */
   onPackageReceived_t onOtherPackageReceived;

   /**
     * Callback invoked everytime a valid framing package has been
     * received with the @see EmberFramingFlag_FirstPackage flag
     * set.
     */
   onPackageReceived_t onFirstPackageReceived;

   /**
     * Callback invoked everytime a valid framing package has been
     * received with the @see EmberFramingFlag_LastPackage flag
     * set.
     */
   onPackageReceived_t onLastPackageReceived;
} GlowReader;

/**
  * Initializes a GlowReader instance.
  * Must be called before any other operations on the
  * GlowReader instance are invoked.
  * @param pThis pointer to the object to process.
  * @param onNode callback function to called by the reader
  *     when a glow node has been read.
  * @param onParameter callback function to called by the reader
  *     when a glow parameter has been read.
  * @param onCommand callback function to called by the reader
  *     when a glow command has been read.
  * @param onStreamEntry callback function called when a stream
  *     entry has been read.
  * @param state application-defined argument passed to
  *     callback functions.
  * @param pRxBuffer pointer to the memory location to unframe
  *     packages to.
  * @param rxBufferSize number of available bytes at @p pRxBuffer.
  *     If this value is exceeded, the throwError callback
  *     is invoked.
  *     This value has to be greater than or equal to the size of
  *     the biggest package you expect to receive.
  *     This value is usually much bigger for applications which
  *     act as ember consumers than for applications which act as
  *     ember providers.
  * @note you need to call glowReader_free to release
  *     all memory allocated by glowReader_init.
  */
LIBEMBER_API void glowReader_init(GlowReader *pThis,
   onNode_t onNode,
   onParameter_t onParameter,
   onCommand_t onCommand,
   onStreamEntry_t onStreamEntry,
   voidptr state,
   byte *pRxBuffer,
   unsigned int rxBufferSize);

/**
* Initializes a GlowReader instance.
* Must be called before any other operations on the
* GlowReader instance are invoked.
* @param pThis pointer to the object to process.
* @param onNode callback function to called by the reader
*     when a glow node has been read.
* @param onParameter callback function to called by the reader
*     when a glow parameter has been read.
* @param onCommand callback function to called by the reader
*     when a glow command has been read.
* @param onStreamEntry callback function called when a stream
*     entry has been read.
* @param onFunction callback function called when a function has
*     been read.
* @param onTemplate callback function called when a template
*     has been read.
* @param state application-defined argument passed to
*     callback functions.
* @param pRxBuffer pointer to the memory location to unframe
*     packages to.
* @param rxBufferSize number of available bytes at @p pRxBuffer.
*     If this value is exceeded, the throwError callback
*     is invoked.
*     This value has to be greater than or equal to the size of
*     the biggest package you expect to receive.
*     This value is usually much bigger for applications which
*     act as ember consumers than for applications which act as
*     ember providers.
* @note you need to call glowReader_free to release
*     all memory allocated by glowReader_init.
*/
LIBEMBER_API void glowReader_initEx(GlowReader *pThis,
   onNode_t onNode,
   onParameter_t onParameter,
   onCommand_t onCommand,
   onStreamEntry_t onStreamEntry,
   onFunction_t onFunction,
   onTemplate_t onTemplate,
   voidptr state,
   byte *pRxBuffer,
   unsigned int rxBufferSize);

/**
  * Frees all dynamically allocated memory used by the
  * passed GlowReader. The memory has been allocated
  * by a prior call to GlowReader_init.
  * @param pThis pointer to the object to process.
  */
LIBEMBER_API void glowReader_free(GlowReader *pThis);

/**
  * Feeds multiple bytes of framed Ember data using the Glow DTD into
  * the passed GlowReader.
  * The reader then unframes the data and feeds the unframed data into
  * a NonFramingGlowReader, which decodes the ember data and invokes
  * the glow callbacks (onNode, onParameter, etc.).
  * @param pThis pointer to the object to process.
  * @param pBytes pointer to the first byte to feed.
  * @param count the number of bytes at @p pBytes to feed.
  */
LIBEMBER_API void glowReader_readBytes(GlowReader *pThis, const byte *pBytes, int count);

/**
  * Resets the internal state of the passed GlowReader.
  * @param pThis pointer to the object to process.
  */
LIBEMBER_API void glowReader_reset(GlowReader *pThis);

#endif//__LIBEMBER_SLIM_GLOWRX_H
