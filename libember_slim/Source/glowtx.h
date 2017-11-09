/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_SLIM_GLOWTX_H
#define __LIBEMBER_SLIM_GLOWTX_H

#include "api.h"
#include "glow.h"

// ====================================================================
//
// GlowOutput
//
// ====================================================================

/**
  * Aliases an BerFramingOutput, specializing the
  * initialization function.
  */
typedef struct SGlowOutput
{
   /**
     * base aggregation
     */
   BerFramingOutput base;

   /**
     * private field
     */
   bool hasLastPackage;

   /**
     * private field
     */
   int packageCount;

   /**
     * private field
     */
   int positionHint;
} GlowOutput;

#ifdef _DEBUG
#define __GLOWOUTPUT_POSITION_TARGETS (1)
#define __GLOWOUTPUT_POSITION_SOURCES (2)
#define __GLOWOUTPUT_POSITION_CONNECTIONS (3)
#endif

/**
  * Initializes a GlowOutput instance.
  * Must be called before any other operations on the
  * GlowOutput instance are invoked.
  * @param pThis pointer to the object to process.
  * @param pMemory pointer to the memory location to write
  *     the framed package to.
  * @param size number of bytes at @p pMemory.
  * @param slotId the slot id as described in the
  *     framing protocol documentation.
  */
LIBEMBER_API void glowOutput_init(GlowOutput *pThis,
                     byte *pMemory,
                     unsigned int size,
                     byte slotId);

/**
* Initializes a GlowOutput instance which uses the non-escaping variant of the S101 framing.
* Must be called before any other operations on the
* GlowOutput instance are invoked.
* @param pThis pointer to the object to process.
* @param pMemory pointer to the memory location to write
*     the framed package to.
* @param size number of bytes at @p pMemory.
* @param slotId the slot id as described in the
*     framing protocol documentation.
*/
LIBEMBER_API void glowOutput_initWithoutEscaping(GlowOutput *pThis,
    byte *pMemory,
    unsigned int size,
    byte slotId);

/**
  * Begins a new package by writing the framing header and the start tag
  * of the root container.
  * Must be called before any Glow payload is written to @p pThis.
  * @param pThis pointer to the object to process.
  * @param isLastPackage if true, the EmberFramingFlag_LastPackage flag
  *     is set in the header.
  */
LIBEMBER_API void glowOutput_beginPackage(GlowOutput *pThis, bool isLastPackage);

/**
  * Begins a new package by writing the framing header and the start tag
  * of the stream root container.
  * Must be called before any Glow payload is written to @p pThis.
  * @param pThis pointer to the object to process.
  * @param isLastPackage if true, the EmberFramingFlag_LastPackage flag
  *     is set in the header.
  */
LIBEMBER_API void glowOutput_beginStreamPackage(GlowOutput *pThis, bool isLastPackage);

/**
  * Finishes the framed package. After this function
  * is called, the output at @p pThis can be re-used
  * to build another package.
  * @param pThis pointer to the object to process.
  * @return the length of the framed package located
  *     at the address passed to glowOutput_init.
  * @note you need to call this function when you have
  *     written a complete glow tree to @p pThis.
  */
LIBEMBER_API unsigned int glowOutput_finishPackage(GlowOutput *pThis);


// ====================================================================
//
// Writer
//
// ====================================================================

/**
  * Writes a Node to the passed GlowOutput, encoding @p pNode
  * with type QualifiedNode.
  * The last integer at @p pPath is the number of the node.
  * @param pOut pointer to the output to be used for in-memory framing.
  * @param pNode pointer to the node to write.
  * @param fields flags indicating which fields of @p pNode should
  *     be written.
  * @param pPath pointer to the first number in the node path, which is
  *     the number of the tree's root node. The last number is the node's
  *     number. May be NULL only if pathLength is 0.
  * @param pathLength number of node numbers at @p pPath.
  */
LIBEMBER_API void glow_writeQualifiedNode(GlowOutput *pOut,
                             const GlowNode *pNode,
                             GlowFieldFlags fields,
                             const berint *pPath,
                             int pathLength);

/**
* Writes a Template to the passed GlowOutput, encoding @p pTemplate
* with type QualifiedTemplate.
* The last integer at @p pPath is the number of the template.
* To write the structure the element defines, pass a valid function to
* the writeElement callback. Within this callback, the functions
*   @see glow_writeTemplateElementNodeBegin
*   @see glow_writeTemplateElementNodeEnd
*   @see glow_writeTemplateElementNodeChildrenBegin
*   @see glow_writeTemplateElementNodeChildrenEnd
*   @see glow_writeTemplateElementNode
*   @see glow_writeTemplateElementParameter
*   @see glow_writeTemplateElementFunction
*
* can be used to write the structure.
* @param pOut pointer to the output to be used for in-memory framing.
* @param pTemlate pointer to the template to write.
* @param fields flags indicating which fields of @p pTemplate should
*     be written.
* @param writeElement A callback which will be invoked when the structure of
*     the template can be written. The tag passed to this callback must be used
*     for the root element of the tag. For all children, NULL can be passed
*     when writing the structure.
* @param pPath pointer to the first number in the template path, which is
*     the number of the tree's root node. The last number is the template's
*     number. May be NULL only if pathLength is 0.
* @param pathLength number of template numbers at @p pPath.
*/
LIBEMBER_API void glow_writeQualifiedTemplate(GlowOutput *pOut,
    const GlowTemplate *pTemplate,
    GlowFieldFlags fields,
    void (*writeElement)(GlowOutput *pOut, const GlowTemplate *pTemplate, const BerTag * elementTag),
    const berint *pPath,
    int pathLength);


/**
 * Writes an element of a template. This function must only be called within the context of
 * the callback invoked by @see glow_writeQualifiedTemplate.
 * When using this function children can be added by calling 
 *      @see glow_writeTemplateElementNodeChildrenBegin.
 * When done, @see glow_writeTemplateElementNodeChildrenEnd must be called to terminate
 * the container.
 * When the node does not contain any children, the @see glow_writeTemplateElementNode
 * function can be used for convenience.
 * @param pOut pointer to the output to be used for in-memory framing.
 * @param tag The application tag of this element. For the root element of the template, the
 *      tag passed to the callback must be used. For all subsequent elements, NULL or
 *      glowTags.elementCollection.element can be used.
 * @param pNode The node to encode.
 * @param fields The fields of the node to encode.
 * @param number The number of the node.
 */
LIBEMBER_API void glow_writeTemplateElementNodeBegin(
    GlowOutput *pOut,
    const BerTag *tag,
    const GlowNode *pNode,
    GlowFieldFlags fields,
    berint number);

/**
 * Terminates a node structure within a template. Must be called once per call
 * to @see glow_writeTemplateElementNodeBegin.
 *
 * This function must only be called within the context of
 * the callback invoked by @see glow_writeQualifiedTemplate.
 * @param pOut pointer to the output to be used for in-memory framing.
 */
LIBEMBER_API void glow_writeTemplateElementNodeEnd(GlowOutput *pOut);

/**
 * Starts a new container which may contain the subelements of a templated node.
 * Once all elements have been written, @see glow_writeTemplateElementNodeChildrenEnd
 * must be called.
 *
 * This function must only be called within the context of
 * the callback invoked by @see glow_writeQualifiedTemplate.
 *
 * @param pOut pointer to the output to be used for in-memory framing.
 */
LIBEMBER_API void glow_writeTemplateElementNodeChildrenBegin(GlowOutput *pOut);

/**
 * Terminates the current child-element container.
 *
 * This function must only be called within the context of
 * the callback invoked by @see glow_writeQualifiedTemplate.
 * 
 * @param pOut pointer to the output to be used for in-memory framing.
 */
LIBEMBER_API void glow_writeTemplateElementNodeChildrenEnd(GlowOutput *pOut);

/**
 * Writes a node of a template. This function must only be called within the context of
 * the callback invoked by @see glow_writeQualifiedTemplate.
 * @param pOut pointer to the output to be used for in-memory framing.
 * @param tag The application tag of this element. For the root element of the template, the
 *      tag passed to the callback must be used. For all subsequent elements, NULL or
 *      glowTags.elementCollection.element can be used.
 * @param pNode The node to encode.
 * @param fields The fields to encode.
 * @param number The number of the node.
 */
LIBEMBER_API void glow_writeTemplateElementNode(
    GlowOutput *pOut,
    const BerTag *tag,
    const GlowNode *pNode,
    GlowFieldFlags fields,
    berint number);

/**
 * Writes a parameter of a template. This function must only be called within the context of
 * the callback invoked by @see glow_writeQualifiedTemplate.
 * @param pOut pointer to the output to be used for in-memory framing.
 * @param tag The application tag of this element. For the root element of the template, the
 *      tag passed to the callback must be used. For all subsequent elements, NULL or
 *      glowTags.elementCollection.element can be used.
 * @param pParameter The parameter to encode.
 * @param fields The fields of to encode.
 * @param number The number of the parameter.
 */
LIBEMBER_API void glow_writeTemplateElementParameter(
    GlowOutput *pOut,
    const BerTag *tag,
    const GlowParameter *pParameter,
    GlowFieldFlags fields,
    berint number);


/**
 * Writes a function of a template. This function must only be called within the context of
 * the callback invoked by @see glow_writeQualifiedTemplate.
 * @param pOut pointer to the output to be used for in-memory framing.
 * @param tag The application tag of this element. For the root element of the template, the
 *      tag passed to the callback must be used. For all subsequent elements, NULL or
 *      glowTags.elementCollection.element can be used.
 * @param pFunction The function to encode.
 * @param fields The fields to encode.
 * @param number The number of the function.
 */
LIBEMBER_API void glow_writeTemplateElementFunction(
    GlowOutput *pOut,
    const BerTag *tag,
    const GlowFunction *pFunction,
    GlowFieldFlags fields,
    berint number);

/**
  * Writes a Parameter to the passed GlowOutput, encoding @p pParameter
  * with type QualifiedParameter.
  * The last integer at @p pPath is the number of the parameter.
  * @param pOut pointer to the output to be used for in-memory framing.
  * @param pParameter pointer to the parameter to write.
  * @param fields flags indicating which fields of @p pParameter should
  *     be written.
  * @param pPath pointer to the first number in the node path, which is
  *     the number of the tree's root node. The last number is the parameter's
  *     number. May be NULL only if pathLength is 0.
  * @param pathLength number of node numbers at @p pPath.
  */
LIBEMBER_API void glow_writeQualifiedParameter(GlowOutput *pOut,
                                  const GlowParameter *pParameter,
                                  GlowFieldFlags fields,
                                  const berint *pPath,
                                  int pathLength);

/**
  * Writes a glow tree to the passed GlowOutput, nesting @p pCommand
  * in either a QualifiedNode or QualifiedParameter container.
  * The last integer at @p pPath is the number of the node or parameter
  * enclosing the command.
  * Example:
  * pPath points to {1, 5}, isNestedInParameter is false -> writes
  * <qualifiedNode path="1,5"><pCommand... /></qualifiedNode>
  * @param pOut pointer to the output to be used for in-memory framing.
  * @param pCommand pointer to the command to write.
  * @param pPath pointer to the first number in the node path, which is
  *     the number of the tree's root node. May be NULL only if
  *     pathLength is 0.
  * @param pathLength number of node numbers at @p pPath.
  * @param parent indicates the type of parent element to nest
  *     the command in.
  *     - parentType = GlowElementType_Node:
  *       command is nested in a QualifiedNode
  *     - parentType = GlowElementType_Parameter:
  *       command is nested in a QualifiedParameter
  *     - parentType = GlowElementType_Matrix:
  *       command is nested in a QualifiedMatrix
  */
LIBEMBER_API void glow_writeQualifiedCommand(GlowOutput *pOut,
                                const GlowCommand *pCommand,
                                const berint *pPath,
                                int pathLength,
                                GlowElementType parentType);

/**
  * Writes a StreamEntry to the passed GlowOutput.
  * @param pOut pointer to the output to be used for in-memory framing.
  * @param pEntry pointer to GlowStreamEntry object to write.
  */
LIBEMBER_API void glow_writeStreamEntry(GlowOutput *pOut, const GlowStreamEntry *pEntry);

/**
  * Writes a Matrix to the passed GlowOutput, encoding @p pMatrix
  * with type QualifiedMatrix.
  * Only writes the "contents" set of the matrix. The sequences "targets",
  * "sources" and "connections" are written with dedicated functions.
  * The last integer at @p pPath is the number of the matrix.
  * @param pOut pointer to the output to be used for in-memory framing.
  * @param pMatrix pointer to the matrix to write.
  * @param fields flags indicating which fields of @p pMatrix should
  *     be written.
  * @param pPath pointer to the first number in the node path, which is
  *     the number of the tree's root node. The last number is the parameter's
  *     number. May be NULL only if pathLength is 0.
  *     This is encoded in the "path" field of the QualifiedMatrix.
  * @param pathLength number of node numbers at @p pPath.
  */
LIBEMBER_API void glow_writeQualifiedMatrix(GlowOutput *pOut,
                               const GlowMatrix *pMatrix,
                               GlowFieldFlags fields,
                               const berint *pPath,
                               int pathLength);

/**
  * Writes the prefix for the "targets" sequence of a QualifiedMatrix.
  * (in XML, this would be: <QualifiedMatrix path="pMatrixPath"><Targets>).
  * @param pOut pointer to the output to be used for in-memory framing.
  * @param pMatrixPath pointer to the first number in the node path to the
  *     enclosing matrix. May be NULL only if pathLength is 0.
  *     This is encoded in the "path" field of the QualifiedMatrix.
  * @param matrixPathLength number of node numbers at @p pPath.
  */
LIBEMBER_API void glow_writeTargetsPrefix(GlowOutput *pOut,
                             const berint *pMatrixPath,
                             int matrixPathLength);

/**
  * Writes a Target to the passed GlowOutput. Only valid if preceeded by a
  * call to glow_writeTargetsPrefix.
  * @param pOut pointer to the output to be used for in-memory framing.
  * @param pConnection pointer to the target to write.
  */
LIBEMBER_API void glow_writeTarget(GlowOutput *pOut, const GlowSignal *pTarget);

/**
  * Writes the suffix for the "targets" sequence of a QualifiedMatrix.
  * (in XML, this would be: </Targets></QualifiedMatrix>).
  * Only valid if preceeded by a call to glow_writeTargetsPrefix.
  * @param pOut pointer to the output to be used for in-memory framing.
  */
LIBEMBER_API void glow_writeTargetsSuffix(GlowOutput *pOut);

/**
  * Writes the prefix for the "sources" sequence of a QualifiedMatrix.
  * (in XML, this would be: <QualifiedMatrix path="pMatrixPath"><Sources>).
  * @param pOut pointer to the output to be used for in-memory framing.
  * @param pMatrixPath pointer to the first number in the node path to the
  *     enclosing matrix. May be NULL only if pathLength is 0.
  *     This is encoded in the "path" field of the QualifiedMatrix.
  * @param matrixPathLength number of node numbers at @p pPath.
  */
LIBEMBER_API void glow_writeSourcesPrefix(GlowOutput *pOut,
                             const berint *pMatrixPath,
                             int matrixPathLength);

/**
  * Writes a Source to the passed GlowOutput. Only valid if preceeded by a
  * call to glow_writeSourcesPrefix.
  * @param pOut pointer to the output to be used for in-memory framing.
  * @param pConnection pointer to the source to write.
  */
LIBEMBER_API void glow_writeSource(GlowOutput *pOut, const GlowSignal *pSource);

/**
  * Writes the suffix for the "sources" sequence of a QualifiedMatrix.
  * (in XML, this would be: </Sources></QualifiedMatrix>).
  * Only valid if preceeded by a call to glow_writeSourcesPrefix.
  * @param pOut pointer to the output to be used for in-memory framing.
  */
LIBEMBER_API void glow_writeSourcesSuffix(GlowOutput *pOut);

/**
  * Writes the prefix for the "connections" sequence of a QualifiedMatrix.
  * (in XML, this would be: <QualifiedMatrix path="pMatrixPath"><Connections>).
  * @param pOut pointer to the output to be used for in-memory framing.
  * @param pMatrixPath pointer to the first number in the node path to the
  *     enclosing matrix. May be NULL only if pathLength is 0.
  *     This is encoded in the "path" field of the QualifiedMatrix.
  * @param matrixPathLength number of node numbers at @p pPath.
  */
LIBEMBER_API void glow_writeConnectionsPrefix(GlowOutput *pOut,
                                 const berint *pMatrixPath,
                                 int matrixPathLength);

/**
  * Writes a Connection to the passed GlowOutput. Only valid if preceeded by a
  * call to glow_writeConnectionsPrefix.
  * @param pOut pointer to the output to be used for in-memory framing.
  * @param pConnection pointer to the connection to write.
  */
LIBEMBER_API void glow_writeConnection(GlowOutput *pOut, const GlowConnection *pConnection);

/**
  * Writes the suffix for the "connections" sequence of a QualifiedMatrix.
  * (in XML, this would be: </Connections></QualifiedMatrix>).
  * Only valid if preceeded by a call to glow_writeConnectionsPrefix.
  * @param pOut pointer to the output to be used for in-memory framing.
  */
LIBEMBER_API void glow_writeConnectionsSuffix(GlowOutput *pOut);

/**
  * Writes a Function to the passed GlowOutput, encoding @p pFunction
  * with type QualifiedFunction.
  * The last integer at @p pPath is the number of the function.
  * @param pOut pointer to the output to be used for in-memory framing.
  * @param pFunction pointer to the function to write.
  * @param fields flags indicating which fields of @p pFunction should
  *     be written.
  * @param pPath pointer to the first number in the node path, which is
  *     the number of the tree's root node. The last number is the parameter's
  *     number. May be NULL only if pathLength is 0.
  *     This is encoded in the "path" field of the QualifiedFunction.
  * @param pathLength number of node numbers at @p pPath.
  */
LIBEMBER_API void glow_writeQualifiedFunction(GlowOutput *pOut,
                                 const GlowFunction *pFunction,
                                 GlowFieldFlags fields,
                                 const berint *pPath,
                                 int pathLength);

/**
  * Writes a complete glow package containing an invocation result with
  * root tag to the passed GlowOutput.
  * @param pOut pointer to the output to be used for in-memory framing.
  * @param pInvocationResult pointer to the InvocationResult to write.
  * @return the number of bytes written to the output.
  */
LIBEMBER_API unsigned int glow_writeInvocationResultPackage(GlowOutput *pOut, const GlowInvocationResult *pRoot);

#endif//__LIBEMBER_SLIM_GLOWTX_H
