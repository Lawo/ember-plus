/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol
   Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __LIBEMBER_SLIM_GLOW_H
#define __LIBEMBER_SLIM_GLOW_H

#include "ember.h"


// ====================================================================
//
// Glow definitions
//
// ====================================================================

/**
  * The glow dtd version this library complies to as an
  * uint16.
  * The upper byte is the major version number, the
  * lower byte is the minor version number.
  */
#define GLOW_SCHEMA_VERSION (0x021E)


/**
  * Defines the structure of all ber tags used by the glow dtd.
  */
typedef struct
{
   struct
   {
      BerTag number;
      BerTag contents;
      BerTag children;
   } parameter;

   struct
   {
      BerTag path;
      BerTag contents;
      BerTag children;
   } qualifiedParameter;

   struct
   {
      BerTag identifier;
      BerTag description;
      BerTag value;
      BerTag minimum;
      BerTag maximum;
      BerTag access;
      BerTag format;
      BerTag enumeration;
      BerTag factor;
      BerTag isOnline;
      BerTag formula;
      BerTag step;
      BerTag defaultValue;
      BerTag type;
      BerTag streamIdentifier;
      BerTag enumMap;
      BerTag streamDescriptor;
      BerTag schemaIdentifier;
   } parameterContents;

   struct
   {
      BerTag number;
      BerTag contents;
      BerTag children;
   } node;

   struct
   {
      BerTag path;
      BerTag contents;
      BerTag children;
   } qualifiedNode;

   struct
   {
      BerTag identifier;
      BerTag description;
      BerTag isRoot;
      BerTag isOnline;
      BerTag schemaIdentifier;
   } nodeContents;

   struct
   {
      BerTag number;
      BerTag dirFieldMask;
      BerTag invocation;
   } command;

   struct
   {
      BerTag element;
   } elementCollection;

   struct
   {
      BerTag streamIdentifier;
      BerTag streamValue;
   } streamEntry;

   struct
   {
      BerTag streamEntry;
   } streamCollection;

   struct
   {
      BerTag entryString;
      BerTag entryInteger;
   } stringIntegerPair;

   struct
   {
      BerTag stringIntegerPair;
   } stringIntegerCollection;

   struct
   {
      BerTag format;
      BerTag offset;
   } streamDescription;

   struct
   {
      BerTag number;
      BerTag contents;
      BerTag children;
      BerTag targets;
      BerTag sources;
      BerTag connections;
   } matrix;
   
   struct
   {
      BerTag path;
      BerTag contents;
      BerTag children;
      BerTag targets;
      BerTag sources;
      BerTag connections;
   } qualifiedMatrix;
   
   struct
   {
      BerTag identifier;
      BerTag description;
      BerTag type;
      BerTag addressingMode;
      BerTag targetCount;
      BerTag sourceCount;
      BerTag maximumTotalConnects;
      BerTag maximumConnectsPerTarget;
      BerTag parametersLocation;
      BerTag gainParameterNumber;
      BerTag labels;
      BerTag schemaIdentifier;
   } matrixContents;

   struct
   {
      BerTag basePath;
      BerTag description;
   } label;
   
   struct 
   {
      BerTag number;
   } signal;

   struct 
   {
      BerTag target;
      BerTag sources;
      BerTag operation;
      BerTag disposition;
   } connection;

   struct 
   {
      BerTag item;
   } collection;

   struct 
   {
      BerTag number;
      BerTag contents;
      BerTag children;
   } function;

   struct 
   {
      BerTag path;
      BerTag contents;
      BerTag children;
   } qualifiedFunction;

   struct 
   {
      BerTag identifier;
      BerTag description;
      BerTag arguments;
      BerTag result;
   } functionContents;

   struct 
   {
      BerTag type;
      BerTag name;
   } tupleItemDescription;

   struct 
   {
      BerTag invocationId;
      BerTag arguments;
   } invocation;

   struct 
   {
      BerTag invocationId;
      BerTag success;
      BerTag result;
   } invocationResult;

   BerTag root;

} __GlowTags;


/**
  * Constant to hold all ber tags used by the glow dtd
  */
extern const __GlowTags glowTags;


/**
  * Lists all application-defined ber types defined by the
  * Glow DTD, fitting for the library defined bertype.
  */
typedef enum EGlowType
{
   GlowType_Parameter             = BerType_ApplicationFlag | 1,
   GlowType_Command               = BerType_ApplicationFlag | 2,
   GlowType_Node                  = BerType_ApplicationFlag | 3,
   GlowType_ElementCollection     = BerType_ApplicationFlag | 4,
   GlowType_StreamEntry           = BerType_ApplicationFlag | 5,
   GlowType_StreamCollection      = BerType_ApplicationFlag | 6,
   GlowType_EnumEntry             = BerType_ApplicationFlag | 7,
   GlowType_EnumCollection        = BerType_ApplicationFlag | 8,
   GlowType_QualifiedParameter    = BerType_ApplicationFlag | 9,
   GlowType_QualifiedNode         = BerType_ApplicationFlag | 10,
   GlowType_RootElementCollection = BerType_ApplicationFlag | 11,
   GlowType_StreamDescription     = BerType_ApplicationFlag | 12,
   GlowType_Matrix                = BerType_ApplicationFlag | 13,
   GlowType_Target                = BerType_ApplicationFlag | 14,
   GlowType_Source                = BerType_ApplicationFlag | 15,
   GlowType_Connection            = BerType_ApplicationFlag | 16,
   GlowType_QualifiedMatrix       = BerType_ApplicationFlag | 17,
   GlowType_Label                 = BerType_ApplicationFlag | 18,
   GlowType_Function              = BerType_ApplicationFlag | 19,
   GlowType_QualifiedFunction     = BerType_ApplicationFlag | 20,
   GlowType_TupleItemDescription  = BerType_ApplicationFlag | 21,
   GlowType_Invocation            = BerType_ApplicationFlag | 22,
   GlowType_InvocationResult      = BerType_ApplicationFlag | 23,
} GlowType;


/**
  * possible values of the "type" field of a GlowParameter
  */
typedef enum EGlowParameterType
{
   GlowParameterType_Integer = 1,
   GlowParameterType_Real    = 2,
   GlowParameterType_String  = 3,
   GlowParameterType_Boolean = 4,
   GlowParameterType_Trigger = 5,
   GlowParameterType_Enum    = 6,
   GlowParameterType_Octets  = 7,
} GlowParameterType;


/**
  * possible values of the "access" field of a GlowParameter
  */
typedef enum EGlowAccess
{
   GlowAccess_None      = 0,
   GlowAccess_Read      = 1,
   GlowAccess_Write     = 2,
   GlowAccess_ReadWrite = 3,
} GlowAccess;


/**
  * possible values of the "number" field of a GlowCommand
  */
typedef enum EGlowCommandType
{
   GlowCommandType_Subscribe    = 30,
   GlowCommandType_Unsubscribe  = 31,
   GlowCommandType_GetDirectory = 32,
   GlowCommandType_Invoke       = 33,
} GlowCommandType;


/**
  * Lists flags used to describe which fields in
  * a GlowParameter or GlowNode struct are set.
  */
typedef enum EGlowFieldFlags
{
   GlowFieldFlag_None               = 0,
   GlowFieldFlag_Identifier         = 0x00000001,
   GlowFieldFlag_Description        = 0x00000002,
   GlowFieldFlag_Value              = 0x00000004,
   GlowFieldFlag_Minimum            = 0x00000008,
   GlowFieldFlag_Maximum            = 0x00000010,
   GlowFieldFlag_Access             = 0x00000020,
   GlowFieldFlag_Format             = 0x00000040,
   GlowFieldFlag_Factor             = 0x00000080,
   GlowFieldFlag_IsOnline           = 0x00000100,
   GlowFieldFlag_Step               = 0x00000200,
 //GlowFieldFlag_DefaultValue       = 0x00000400,
   GlowFieldFlag_Type               = 0x00000800,
   GlowFieldFlag_StreamIdentifier   = 0x00001000,
   GlowFieldFlag_Formula            = 0x00002000,
   GlowFieldFlag_Enumeration        = 0x00004000,
 //GlowFieldFlag_EnumMap            = 0x00008000,
   GlowFieldFlag_StreamDescriptor   = 0x00010000,
   GlowFieldFlag_IsRoot             = 0x00020000,
   GlowFieldFlag_SchemaIdentifier   = 0x00040000,
   GlowFieldFlag_Connections        = 0x00000005,

   GlowFieldFlag_All                = 0xFFFFFFFF,
} GlowFieldFlags;


/**
  * possible values of the "format" field of a GlowStreamDescription
  */
typedef enum EGlowStreamFormat
{
   GlowStreamFormat_UnsignedInt8               =  0,
   GlowStreamFormat_UnsignedInt16BigEndian     =  2,
   GlowStreamFormat_UnsignedInt16LittleEndian  =  3,
   GlowStreamFormat_UnsignedInt32BigEndian     =  4,
   GlowStreamFormat_UnsignedInt32LittleEndian  =  5,
   GlowStreamFormat_UnsignedInt64BigEndian     =  6,
   GlowStreamFormat_UnsignedInt64LittleEndian  =  7,
   GlowStreamFormat_SignedInt8                 =  8,
   GlowStreamFormat_SignedInt16BigEndian       = 10,
   GlowStreamFormat_SignedInt16LittleEndian    = 11,
   GlowStreamFormat_SignedInt32BigEndian       = 12,
   GlowStreamFormat_SignedInt32LittleEndian    = 13,
   GlowStreamFormat_SignedInt64BigEndian       = 14,
   GlowStreamFormat_SignedInt64LittleEndian    = 15,
   GlowStreamFormat_IeeeFloat32BigEndian       = 20,
   GlowStreamFormat_IeeeFloat32LittleEndian    = 21,
   GlowStreamFormat_IeeeFloat64BigEndian       = 22,
   GlowStreamFormat_IeeeFloat64LittleEndian    = 23,
} GlowStreamFormat;


/**
  * possible values of the "addressingMode" field of a GlowMatrix
  */
typedef enum EGlowMatrixAddressingMode
{
   GlowMatrixAddressingMode_Linear     = 0,
   GlowMatrixAddressingMode_NonLinear  = 1,
} GlowMatrixAddressingMode;


/**
  * possible values of the "type" field of a GlowMatrix
  */
typedef enum EGlowMatrixType
{
   GlowMatrixType_OneToN    = 0,
   GlowMatrixType_OneToOne  = 1,
   GlowMatrixType_NToN      = 2,
} GlowMatrixType;


/**
  * possible values of the "operation" field of a GlowConnection
  */
typedef enum EGlowConnectionOperation
{
   GlowConnectionOperation_Absolute    = 0,
   GlowConnectionOperation_Connect     = 1,
   GlowConnectionOperation_Disconnect  = 2,
} GlowConnectionOperation;


/**
  * possible values of the "disposition" field of a GlowConnection
  */
typedef enum EGlowConnectionDisposition
{
   GlowConnectionDisposition_Tally      = 0,
   GlowConnectionDisposition_Modified   = 1,
   GlowConnectionDisposition_Pending    = 2,
   GlowConnectionDisposition_Locked     = 3,
} GlowConnectionDisposition;


// ====================================================================
//
// Glow Schema
//
// ====================================================================

/**
  * Maximum depth of the glow tree (nodes and parameters).
  * Dependent on the maximum depth of the ember tree. Since
  * each glow node consists of 2 nested containers (node, children),
  * half of the maximum ember tree depth is a good approximation.
  */
#define GLOW_MAX_TREE_DEPTH (EMBER_MAX_TREE_DEPTH / 2)


/**
  * Type that can be used to identify a type of
  * parent element (see ASN.1 definition of EmberPlus-Glow.Element).
  */
typedef enum EGlowElementType
{
   GlowElementType_Node      = 0,
   GlowElementType_Parameter = 1,
   GlowElementType_Matrix    = 2,
   GlowElementType_Function  = 3,
} GlowElementType;

/**
  * Holds information about a Glow Node or Glow QualifiedNode.
  * @note see GlowDTD definition of type Node and QualifiedNode.
  */
typedef struct SGlowNode
{
   /**
     * The "identifier" field.
     */
   pstr pIdentifier;

   /**
     * The "description" field.
     */
   pstr pDescription;

   /**
     * The "isRoot" field.
     */
   bool isRoot;

   /**
     * The "isOnline" field.
     */
   bool isOnline;

   /**
     * the "schemaIdentifier" field.
     */
   pstr pSchemaIdentifier;
} GlowNode;


/**
  * Frees all memory allocated by a GlowNode instance.
  * @param pThis pointer to the object to process.
  */
void glowNode_free(GlowNode *pThis);


/**
  * Union struct to hold the value of a
  * parameter's "minimum" or "maximum" field.
  */
typedef struct SGlowMinMax
{
   /**
     * The type of the stored value.
     * If flag is GlowParameterType_Integer, GlowMinMax.integer is valid.
     * If flag is GlowParameterType_Real, GlowMinMax.real is valid.
     */
   GlowParameterType flag;
   union
   {
      berlong integer;
      double real;
   } choice;
} GlowMinMax;


/**
  * Union struct to hold the value of a
  * parameter's "value" or "default" field.
  */
typedef struct SGlowOctetsValue
{
   byte *pOctets;
   int length;
} GlowOctetsValue;


/**
  * Union struct to hold the value of a
  * parameter's "value" or "default" field.
  */
typedef struct SGlowValue
{
   /**
     * The type of the stored value.
     * If flag is GlowParameterType_Integer, GlowValue.integer is valid.
     * If flag is GlowParameterType_Real, GlowValue.real is valid.
     * If flag is GlowParameterType_String, GlowValue.pString is valid.
     * If flag is GlowParameterType_Boolean, GlowValue.boolean is valid.
     * If flag is GlowParameterType_Octets, GlowValue.octets is valid.
     */
   GlowParameterType flag;
   union
   {
      berlong integer;
      double real;
      bool boolean;
      pstr pString;
      GlowOctetsValue octets;
   } choice;
} GlowValue;


/**
  * Deep-Copies the contents from another GlowValue.
  * @param pThis pointer to the object to process.
  * @param pDest pointer to the source value.
  */
void glowValue_copyFrom(GlowValue *pThis, const GlowValue *pSource);

#define glowValue_copyTo(pSource, pDest) glowValue_copyFrom(pDest, pSource)


/**
  * Frees all memory allocated by a GlowValue instance.
  * @param pThis pointer to the object to process.
  */
void glowValue_free(GlowValue *pThis);


/**
  * Struct to hold the value of a parameter's "streamDescriptor" field.
  * Projects the ASN.1 type "GlowingEmber.StreamDescription".
  */
typedef struct SGlowStreamDescription
{
   /**
     * the "format" field.
     */
   GlowStreamFormat format;

   /**
     * the "offset" field.
     */
   berint offset;
} GlowStreamDescription;


/**
  * Holds information about a Glow Parameter or a Glow QualifiedParameter.
  * @note see GlowDTD definition of type Parameter and QualifiedParameter.
  */
typedef struct SGlowParameter
{
   /**
     * the "identifier" field.
     */
   pstr pIdentifier;

   /**
     * the "description" field.
     */
   pstr pDescription;

   /**
     * the "value" field.
     */
   GlowValue value;

   /**
     * the "minimum" field.
     */
   GlowMinMax minimum;

   /**
     * the "maximum" field.
     */
   GlowMinMax maximum;

   /**
     * the "access" field.
     */
   GlowAccess access;

   /**
     * the "format" field.
     * @note TX only, this field will never be set by a GlowReader!
     */
   pcstr pFormat;

   /**
     * the "factor" field.
     */
   berint factor;

   /**
     * the "isOnline" field.
     */
   bool isOnline;

   /**
     * the "step" field.
     */
   berint step;

   /**
     * the "type" field.
     */
   GlowParameterType type;

   /**
     * the "streamIdentifier" field.
     */
   berint streamIdentifier;

   /**
     * the "streamDescription" field.
     */
   GlowStreamDescription streamDescriptor;

   /**
     * the "formula" field.
     * @note TX only, this field will never be set by a GlowReader!
     *     This field is ignored by glowParameter_free().
     */
   pcstr pFormula;

   /**
     * the "enumeration" field.
     * @note TX only, this field will never be set by a GlowReader!
     *     This field is ignored by glowParameter_free().
     */
   pcstr pEnumeration;

   /**
     * the "schemaIdentifier" field.
     */
   pstr pSchemaIdentifier;
} GlowParameter;


/**
  * Frees all memory allocated by a GlowValue instance.
  * @param pThis pointer to the object to process.
  */
void glowParameter_free(GlowParameter *pThis);


/**
  * Holds information about a Glow Function Invocation.
  * @note see GlowDTD definition of type Invocation.
  */
typedef struct SGlowInvocation
{
   /**
     * the "invocationId" field.
     */
   berint invocationId;

   /**
     * the "arguments" field.
     */
   GlowValue *pArguments;

   /**
     * number of GlowValues in the array at pArguments.
     */
   int argumentsLength;
   
} GlowInvocation;


/**
  * Frees all memory allocated by a GlowInvocation instance.
  * @param pThis pointer to the object to process.
  */
void glowInvocation_free(GlowInvocation *pThis);


/**
  * Holds information about a Glow Command.
  * @note see GlowDTD definition of type Parameter.
  */
typedef struct SGlowCommand
{
   /**
     * the "number" field.
     */
   berint number;

   union
   {
      /**
        * the "dirFieldMask" field.
        * @note only valid if number is GlowCommandType_GetDirectory!
        */
      GlowFieldFlags dirFieldMask;

      /**
        * the "invocation" field.
        * @note only valid if number is GlowCommandType_Invoke!
        */
      GlowInvocation invocation;
   } options;
} GlowCommand;


/**
  * Frees all memory allocated by a GlowCommand instance.
  * @param pThis pointer to the object to process.
  */
void glowCommand_free(GlowCommand *pThis);


/**
  * Holds information about a Glow StreamEntry.
  * @note see GlowDTD definition of type StreamEntry.
  */
typedef struct SGlowStreamEntry
{
   /**
     * the "streamIdentifier" field.
     */
   berint streamIdentifier;

   /**
     * the "streamValue" field.
     */
   GlowValue streamValue;
} GlowStreamEntry;


/**
  * Holds information about a Glow Label.
  * @note see GlowDTD definition of type Label.
  */
typedef struct SGlowLabel
{
   /**
     * the "basePath" field.
     */
   berint basePath[GLOW_MAX_TREE_DEPTH];

   /**
     * length of the "basePath" field.
     */
   int basePathLength;

   /**
     * the "description" field.
     */
   pcstr pDescription;
} GlowLabel;


typedef enum EGlowParametersLocationKind
{
   GlowParametersLocationKind_BasePath = 0,
   GlowParametersLocationKind_Inline = 1,
} GlowParametersLocationKind;


/**
  * Holds the value of the GlowingEmber.MatrixContents.parametersLocation field
  */
typedef struct SGlowParametersLocation
{
   /**
     * The kind of the stored value.
     * BasePath: basePath is valid
     * Inline: inlineId is valid
     */
   GlowParametersLocationKind kind;

   union
   {
      struct
      {
         berint ids[GLOW_MAX_TREE_DEPTH];
         int length;
      } basePath;

      berint inlineId;
   } choice;
} GlowParametersLocation;


/**
  * Returns true if pThis points to a valid (initialized) GlowParametersLocation struct.
  */
bool glowParametersLocation_isValid(const GlowParametersLocation *pThis);


/**
  * Holds information about a Glow Matrix or Glow QualifiedMatrix.
  * @note see GlowDTD definition of type Matrix and QualifiedMatrix.
  */
typedef struct SGlowMatrix
{
   /**
     * the "identifier" field.
     */
   pstr pIdentifier;

   /**
     * the "description" field.
     */
   pstr pDescription;

   /**
     * the "type" field.
     */
   GlowMatrixType type;

   /**
     * the "addressingMode" field.
     */
   GlowMatrixAddressingMode addressingMode;

   /**
     * the "targetCount" field.
     * linear: matrix X size; nonLinear: number of targets
     */
   berint targetCount;

   /**
     * the "sourceCount" field.
     * linear: matrix Y size; nonLinear: number of sources
     */
   berint sourceCount;

   /**
     * the "maximumTotalConnects" field.
     * nToN: max number of set connections
     */
   berint maximumTotalConnects;

   /**
     * the "maximumConnectsPerTarget" field.
     * nToN: max number of sources connected to one target
     */
   berint maximumConnectsPerTarget;

   /**
     * the "parametersLocation" field.
     * nToN: path to node containing parameters for targets, sources and connections
     */
   GlowParametersLocation parametersLocation;

   /**
     * the "labels" field.
     * @note TX only, this field will never be set by a GlowReader!
     *     This field is ignored by glowParameter_free().
     */
   const GlowLabel *pLabels;

   /**
     * number of labels at the location pLabels.
     * @note TX only, this field will never be set by a GlowReader!
     */
   int labelsLength;

   /**
     * the "schemaIdentifier" field.
     */
   pstr pSchemaIdentifier;
} GlowMatrix;


/**
  * Frees all memory allocated by a GlowMatrix instance.
  * @param pThis pointer to the object to process.
  */
void glowMatrix_free(GlowMatrix *pThis);


/**
  * Holds information about a Glow Connection.
  * @note see GlowDTD definition of type Connection.
  */
typedef struct SGlowConnection
{
   /**
     * the "target" field.
     */
   berint target;

   /**
     * the "sources" field.
     */
   berint *pSources;

   /**
     * length of the "sources" field.
     */
   int sourcesLength;

   /**
     * the "operation" field.
     */
   GlowConnectionOperation operation;

   /**
     * the "disposition" field.
     */
   GlowConnectionDisposition disposition;
} GlowConnection;


/**
  * Frees all memory allocated by a GlowMatrix instance.
  * @param pThis pointer to the object to process.
  */
void glowConnection_free(GlowConnection *pThis);


/**
  * Holds information about a Glow Signal.
  * @note see GlowDTD definition of type Signal.
  */
typedef struct SGlowSignal
{
   /**
     * the "number" field. 
     */
   berint number;
} GlowSignal;


/**
  * Holds information about a Glow Tuple.
  * @note see GlowDTD definition of type GlowTupleItemDescription.
  */
typedef struct SGlowTupleItemDescription
{
   /**
     * the "type" field. 
     */
   GlowParameterType type;

   /**
     * the "name" field. 
     */
   pstr pName;
} GlowTupleItemDescription;

/**
  * Frees all memory allocated by a GlowTupleItemDescription instance.
  * @param pThis pointer to the object to process.
  */
void glowTupleItemDescription_free(GlowTupleItemDescription *pThis);


/**
  * Holds information about a Glow Function.
  * @note see GlowDTD definition of type Function.
  */
typedef struct SGlowFunction
{
   /**
     * the "identifier" field.
     */
   pstr pIdentifier;

   /**
     * the "description" field.
     */
   pstr pDescription;

   /**
     * the "arguments" field.
     */
   GlowTupleItemDescription *pArguments;

   /**
     * number of arguments (number of GlowTupleItemDescriptions at pArguments)
     */
   int argumentsLength;

   /**
     * the "result" field.
     */
   GlowTupleItemDescription *pResult;

   /**
     * number of fields in the result. (number of GlowTupleItemDescriptions
     * at pResult)
     */
   int resultLength;
} GlowFunction;


/**
  * Frees all memory allocated by a GlowFunction instance.
  * @param pThis pointer to the object to process.
  */
void glowFunction_free(GlowFunction *pThis);


/**
  * Holds information about a Glow InvocationResult.
  * @note see GlowDTD definition of type InvocationResult.
  */
typedef struct SGlowInvocationResult
{
   /**
     * the "invocationId" field.
     */
   berint invocationId;

   /**
     * inversion of the "success" field.
     */
   bool hasError;

   /**
     * the "result" field.
     */
   GlowValue *pResult;

   /**
     * number of GlowValues in the array at pResult.
     */
   int resultLength;
} GlowInvocationResult;


/**
  * Frees all memory allocated by a GlowInvocationResult instance.
  * @param pThis pointer to the object to process.
  */
void glowInvocationResult_free(GlowInvocationResult *pThis);

#endif// __LIBEMBER_SLIM_GLOW_H
