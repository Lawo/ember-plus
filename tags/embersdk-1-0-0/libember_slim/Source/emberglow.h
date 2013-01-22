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
#define GLOW_SCHEMA_VERSION (0x0205)


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
   } nodeContents;

   struct
   {
      BerTag number;
      BerTag dirFieldMask;
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

   GlowFieldFlag_All                = 0xFFFFFFFF,
} GlowFieldFlags;


/**
  * possible values of the "format" field of a GlowStreamDescription
  */
typedef enum EGlowStreamFormat
{
   GlowStreamFormat_UnsignedInt8                  =  0,
   GlowStreamFormat_UnsignedInt16BigEndian        =  2,
   GlowStreamFormat_UnsignedInt16LittleEndian     =  3,
   GlowStreamFormat_UnsignedInt32BigEndian        =  4,
   GlowStreamFormat_UnsignedInt32LittleEndian     =  5,
   GlowStreamFormat_UnsignedInt64BigEndian        =  6,
   GlowStreamFormat_UnsignedInt64LittleEndian     =  7,
   GlowStreamFormat_SignedInt8                    =  8,
   GlowStreamFormat_SignedInt16BigEndian          = 10,
   GlowStreamFormat_SignedInt16LittleEndian       = 11,
   GlowStreamFormat_SignedInt32BigEndian          = 12,
   GlowStreamFormat_SignedInt32LittleEndian       = 13,
   GlowStreamFormat_SignedInt64BigEndian          = 14,
   GlowStreamFormat_SignedInt64LittleEndian       = 15,
   GlowStreamFormat_IeeeFloat32BigEndian          = 20,
   GlowStreamFormat_IeeeFloat32LittleEndian       = 21,
   GlowStreamFormat_IeeeFloat64BigEndian          = 22,
   GlowStreamFormat_IeeeFloat64LittleEndian       = 23,
} GlowStreamFormat;


// ====================================================================
//
// Glow Schema
//
// ====================================================================

#ifndef GLOW_MAX_IDENTIFIER_LENGTH
/**
  * Maximum length of Parameter.identifier and Node.identifier.
  */
#define GLOW_MAX_IDENTIFIER_LENGTH (64)
#endif

#ifndef GLOW_MAX_DESCRIPTION_LENGTH
/**
  * Maximum length of Parameter.description and Node.description.
  */
#define GLOW_MAX_DESCRIPTION_LENGTH (64)
#endif

#ifndef GLOW_MAX_VALUE_LENGTH
/**
  * Maximum length of Parameter.value.string and Parameter.value.octets
  */
#define GLOW_MAX_VALUE_LENGTH (64)
#endif

/**
  * Holds information about a Glow Node or Glow QualifiedNode.
  * @note see GlowDTD definition of type Node and QualifiedNode.
  */
typedef struct SGlowNode
{
   /**
     * The "identifier" field.
     */
   char identifier[GLOW_MAX_IDENTIFIER_LENGTH];

   /**
     * The "description" field.
     */
   char description[GLOW_MAX_DESCRIPTION_LENGTH];

   /**
     * The "isRoot" field.
     */
   bool isRoot;
} GlowNode;


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
   };
} GlowMinMax;


/**
  * Union struct to hold the value of a
  * parameter's "value" or "default" field.
  */
typedef struct SGlowOctetsValue
{
   byte octets[GLOW_MAX_VALUE_LENGTH];
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
     * If flag is GlowParameterType_String, GlowValue.string is valid.
     * If flag is GlowParameterType_Boolean, GlowValue.boolean is valid.
     * If flag is GlowParameterType_Octets, GlowValue.octets is valid.
     */
   GlowParameterType flag;
   union
   {
      berlong integer;
      double real;
      bool boolean;
      char string[GLOW_MAX_VALUE_LENGTH];
      GlowOctetsValue octets;
   };
} GlowValue;


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
   char identifier[GLOW_MAX_IDENTIFIER_LENGTH];

   /**
     * the "description" field.
     */
   char description[GLOW_MAX_DESCRIPTION_LENGTH];

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
   pcstr format;

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
     */
   pcstr formula;

   /**
     * the "enumeration" field.
     * @note TX only, this field will never be set by a GlowReader!
     */
   pcstr enumeration;
} GlowParameter;


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

   /**
     * the "fielFieldMask" field.
     */
   GlowFieldFlags dirFieldMask;
} GlowCommand;


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
} GlowOutput;

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
void glowOutput_init(GlowOutput *pThis,
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
void glowOutput_beginPackage(GlowOutput *pThis, bool isLastPackage);

/**
  * Begins a new package by writing the framing header and the start tag
  * of the stream root container.
  * Must be called before any Glow payload is written to @p pThis.
  * @param pThis pointer to the object to process.
  * @param isLastPackage if true, the EmberFramingFlag_LastPackage flag
  *     is set in the header.
  */
void glowOutput_beginStreamPackage(GlowOutput *pThis, bool isLastPackage);

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
unsigned int glowOutput_finishPackage(GlowOutput *pThis);


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
void glow_writeQualifiedNode(GlowOutput *pOut,
                             const GlowNode *pNode,
                             GlowFieldFlags fields,
                             const berint *pPath,
                             int pathLength);

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
void glow_writeQualifiedParameter(GlowOutput *pOut,
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
  * @param isNestedInParameter indicates that the command is a direct
  *     child of a parameter instead of a node.
  *     If this is true, the glow tree from the above example will look
  *     like this:
  *     pPath points to {1, 5}, isNestedInParameter is true -> writes
  *     <qualifiedParameter path="1,5"><pCommand... /></qualifiedParameter>
  *     This is needed for commands that directly affect a single parameter,
  *     like the "subscribe" command.
  */
void glow_writeQualifiedCommand(GlowOutput *pOut,
                                const GlowCommand *pCommand,
                                const berint *pPath,
                                int pathLength,
                                bool isNestedInParameter);


/**
  * Writes a StreamEntry to the passed GlowOutput.
  * @param pOut pointer to the output to be used for in-memory framing.
  * @param pEntry pointer to GlowStreamEntry object to write.
  */
void glow_writeStreamEntry(GlowOutput *pOut, const GlowStreamEntry *pEntry);


// ====================================================================
//
// NonFramingGlowReader
//
// ====================================================================

/**
  * Maximum depth of the glow tree (nodes and parameters).
  * Dependent on the maximum depth of the ember tree. Since
  * each glow node consists of 2 nested containers (node, children),
  * half of the maximum ember tree depth is a good approximation.
  */
#define MAX_GLOW_TREE_DEPTH (MAX_EMBER_TREE_DEPTH / 2)

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
  * @param pPath pointer to the first number in the node path, which is
  *     the number of the tree's root node. May be NULL only if
  *     pathLength is 0.
  *     See documentation of glow_writeQualifiedNode for an example.
  * @param pathLength number of node numbers at @p pPath.
  * @param state application-defined state as stored in NonFramingGlowReader.
  */
typedef void (*onNode_t)(const GlowNode *pNode, const berint *pPath, int pathLength, voidptr state);

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
  * @param pPath pointer to the first number in the node path, which is
  *     the number of the tree's root node. May be NULL only if
  *     pathLength is 0.
  *     See documentation of glow_writeQualifiedNode for an example.
  * @param pathLength number of node numbers at @p pPath.
  * @param state application-defined state as stored in NonFramingGlowReader.
  */
typedef void (*onStreamEntry_t)(const GlowStreamEntry *pStreamEntry, const berint *pPath, int pathLength, voidptr state);

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
   berint path[MAX_GLOW_TREE_DEPTH];

   /**
     * Private field.
     */
   int pathLength;

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
   GlowFieldFlags paramFields;

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
   void (*onItemReadyState)(struct SNonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase);

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
  * @param pBuffer pointer to the memory location to store
  *     encoded values to.
  * @param bufferSize maximum length of a encoded value.
  *      If this value is exceeded, the throwError callback
  *      is invoked.
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
void nonFramingGlowReader_init(NonFramingGlowReader *pThis,
                               byte *pBuffer,
                               int bufferSize,
                               onNode_t onNode,
                               onParameter_t onParameter,
                               onCommand_t onCommand,
                               onStreamEntry_t onStreamEntry,
                               voidptr state);

/**
  * Frees all dynamically allocated memory used by the
  * passed NonFramingGlowReader. The memory has been allocated
  * by a prior call to nonFramingGlowReader_init.
  * @param pThis pointer to the object to process.
  */
void nonFramingGlowReader_free(NonFramingGlowReader *pThis);

/**
  * Resets the internal state of the passedNonFramingGlowReader.
  * @param pThis pointer to the object to process.
  */
void nonFramingGlowReader_reset(NonFramingGlowReader *pThis);


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
} GlowReader;

/**
  * Initializes a GlowReader instance.
  * Must be called before any other operations on the
  * GlowReader instance are invoked.
  * @param pThis pointer to the object to process.
  * @param pValueBuffer pointer to the memory location to store
  *     encoded primitive values to.
  * @param valueBufferSize maximum length of a primitive encoded
  *      value. If this value is exceeded, the throwError callback
  *      is invoked.
  * @param onNode callback function to called by the reader
  *     when a glow node has been read.
  * @param onParameter callback function to called by the reader
  *     when a glow parameter has been read.
  * @param onCommand callback function to called by the reader
  *     when a glow command has been read.
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
void glowReader_init(GlowReader *pThis,
                     byte *pValueBuffer,
                     int valueBufferSize,
                     onNode_t onNode,
                     onParameter_t onParameter,
                     onCommand_t onCommand,
                     onStreamEntry_t onStreamEntry,
                     voidptr state,
                     byte *pRxBuffer,
                     unsigned int rxBufferSize);

/**
  * Frees all dynamically allocated memory used by the
  * passed GlowReader. The memory has been allocated
  * by a prior call to GlowReader_init.
  * @param pThis pointer to the object to process.
  */
void glowReader_free(GlowReader *pThis);

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
void glowReader_readBytes(GlowReader *pThis, const byte *pBytes, int count);

/**
  * Resets the internal state of the passed GlowReader.
  * @param pThis pointer to the object to process.
  */
void glowReader_reset(GlowReader *pThis);

#endif
