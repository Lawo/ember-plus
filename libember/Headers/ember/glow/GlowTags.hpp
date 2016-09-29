/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWTAGS_HPP
#define __LIBEMBER_GLOW_GLOWTAGS_HPP

#include "../util/Api.hpp"
#include "MatrixProperty.hpp"
#include "NodeProperty.hpp"
#include "ParameterProperty.hpp"

namespace libember { namespace glow
{ 
    /**
     * Lists all tags that are used by glow to store properties of nodes, parameters and other object types.
     */
    struct LIBEMBER_API GlowTags
    {
        struct LIBEMBER_API Command
        {
            /**
             * Returns the context-specific tag identifying a command number.
             * @return The context-specific tag identifying a command number.
             */
            static ber::Tag Number();

            /**
             * Returns the tag of the dir field mask, which indicates the fields a consumer is interested in.
             * @return The tag of the dir field mask.
             */
            static ber::Tag DirFieldMask();

            /**
             * Returns the tag of the invocation property, which contains a function that shall be executed
             * by the provider.
             * @return The tag of the invocation property.
             */
            static ber::Tag Invocation();
        };

        struct LIBEMBER_API NodeContents
        {
            /**
             * Returns the context-specific tag identifying an identifier.
             * @return Returns the context-specific tag identifying an identifier.
             */
            static ber::Tag Identifier();

            /**
             * Returns the context-specific tag identifying a number.
             * @return Returns the context-specific tag identifying a number.
             */
            static ber::Tag Description();

            /**
             * Returns the context-specific tag identifying the root flag.
             * @return Returns the context-specific tag identifying the root flag.
             */
            static ber::Tag IsRoot();

            /**
             * Returns the context-specific tag identifying the node's online state.
             * @return Returns the context-specific tag identifying the node's online state.
             */
            static ber::Tag IsOnline();

            /**
             * Returns the context-specific tag identifying the node's schema identifier.
             * @return The context-specific tag identifying the node's schema identifier.
             */
            static ber::Tag SchemaIdentifiers();

            /**
             * Returns the context-specific tag identifying the template reference.
             * @return The context-specific tag identifying the template reference.
             */
            static ber::Tag TemplateReference();
        };

        struct LIBEMBER_API Node 
        {
            /**
             * Returns the context-specific tag identifying a number.
             * @return Returns the context-specific tag identifying a number.
             */
            static ber::Tag Number(); 

            /**
             * Returns the context-specific tag identifying the contents of a node. 
             * @return Returns the context-specific tag identifying the node contents.
             */
            static ber::Tag Contents();

            /**
             * Returns the context-specific tag identifying the collection containing the child elements.
             * @return Returns the context-specific tag identifying the collection containing the child elements.
             */
            static ber::Tag Children();
        };

        struct LIBEMBER_API QualifiedNode
        {
            /**
             * Returns the context-specific tag identifying the path of the node.
             * @return Returns the context-specific tag identifying the path.
             */
            static ber::Tag Path(); 

            /**
             * Returns the context-specific tag identifying the contents of a node. 
             * @return Returns the context-specific tag identifying the node contents.
             */
            static ber::Tag Contents();

            /**
             * Returns the context-specific tag identifying the collection containing the child elements.
             * @return Returns the context-specific tag identifying the collection containing the child elements.
             */
            static ber::Tag Children();
        };

        struct LIBEMBER_API Parameter
        {
            /**
             * Returns the context-specific tag identifying a number.
             * @return Returns the context-specific tag identifying a number.
             */
            static ber::Tag Number(); 

            /**
             * Returns the context-specific tag identifying the contents of a parameter. 
             * @return Returns the context-specific tag identifying the parameter contents.
             */
            static ber::Tag Contents();

            /**
             * Returns the context-specific tag identifying the child collection of a node or parameter.
             * @return Returns the context-specific tag identifying the child collection.
             */
            static ber::Tag Children();
        };

        struct LIBEMBER_API QualifiedParameter
        {
            /**
             * Returns the context-specific tag identifying the path of a parameter.
             * @return Returns the context-specific tag identifying the path.
             */
            static ber::Tag Path(); 

            /**
             * Returns the context-specific tag identifying the contents of a parameter. 
             * @return Returns the context-specific tag identifying the parameter contents.
             */
            static ber::Tag Contents();

            /**
             * Returns the context-specific tag identifying the child collection of a node or parameter.
             * @return Returns the context-specific tag identifying the child collection.
             */
            static ber::Tag Children();
        };

        struct LIBEMBER_API ParameterContents
        {
            /**
             * Returns the context-specific tag identifying an identifier.
             * @return Returns the context-specific tag identifying an identifier.
             */
            static ber::Tag Identifier();

            /**
             * Returns the context-specific tag identifying a number.
             * @return Returns the context-specific tag identifying a number.
             */
            static ber::Tag Description();

            /**
             * Returns the context-specific tag identifying a value.
             * @return Returns the context-specific tag identifying a value.
             */
            static ber::Tag Value();

            /**
             * Returns the context-specific tag identifying a minimum.
             * @return Returns the context-specific tag identifying a minimum.
             */
            static ber::Tag Minimum();

            /**
             * Returns the context-specific tag identifying a maximum.
             * @return Returns the context-specific tag identifying a maximum.
             */
            static ber::Tag Maximum();

            /**
             * Returns the context-specific tag identifying the access flag.
             * @return Returns the context-specific tag identifying the access flag.
             */
            static ber::Tag Access();

            /**
             * Returns the context-specific tag identifying a format string.
             * @return Returns the context-specific tag identifying a format string.
             */
            static ber::Tag Format();

            /**
             * Returns the context-specific tag identifying an enumeration.
             * @return Returns the context-specific tag identifying an enumeration.
             */
            static ber::Tag Enumeration();

            /**
             * Returns the context-specific tag identifying a factor.
             * @return Returns the context-specific tag identifying a factor.
             */
            static ber::Tag Factor();

            /**
             * Returns the context-specific tag identifying the online flag.
             * @return Returns the context-specific tag identifying the online flag.
             */
            static ber::Tag IsOnline();

            /**
             * Returns the context-specific tag identifying a formula.
             * @return Returns the context-specific tag identifying a formula.
             */
            static ber::Tag Formula();

            /**
             * Returns the context-specific tag identifying a step.
             * @return Returns the context-specific tag identifying a step.
             */
            static ber::Tag Step();

            /**
             * Returns the context-specific tag identifying a defalt value.
             * @return Returns the context-specific tag identifying a default value.
             */
            static ber::Tag Default();

            /**
             * Returns the context-specific tag identifying the value type.
             * @return Returns the context-specific tag identifying the value type.
             */
            static ber::Tag Type();

            /**
             * Returns the context-specific tag identifying a stream identifier.
             * @return Returns the context-specific tag identifying the stream identifier.
             */
            static ber::Tag StreamIdentifier();

            /**
             * Returns the context-specific tag identifying an enumeration map. The map has a higher priority
             * than the enumeration string. It contains a list of pairs, where each pair has an index and a string value.
             * @return Returns the context-specific tag identifying an enumeration map.
             */
            static ber::Tag EnumMap();

            /**
             * Returns the context-specific tag identifying a StreamDescriptor. A StreamDescriptor is only required when 
             * a stream doesn't use the default format.
             * @return Returns the context-specific tag identifying a StreamDescriptor.
             */
            static ber::Tag StreamDescriptor();

            /**
             * Returns the context-specific tag identifying the parameter's schema identifier.
             * @return The context-specific tag identifying the parameter's schema identifier.
             */
            static ber::Tag SchemaIdentifiers();

            /**
             * Returns the context-specific tag identifying the template reference.
             * @return The context-specific tag identifying the template reference.
             */
            static ber::Tag TemplateReference();
        };
        
        struct LIBEMBER_API StreamEntry
        {
            /**
             * Returns the context-specific tag identifying a stream identifier within a stream entry.
             * @return Returns the context-specific tag of a stream identifier
             */
            static ber::Tag StreamIdentifier();

            /**
             * Returns the context-specific tag identifying a stream value withing a stream entry.
             * The tag returned is ContextSpecific - 0.
             * @return Returns the context-specific tag identifying a stream value.
             */
            static ber::Tag StreamValue();
        };

        struct LIBEMBER_API StringIntegerPair
        {
            /**
             * Returns the context-specific tag identifying a String-Integer pair's name
             * Returns the context-specific tag identifying a String-Integer pair's name
             */
            static ber::Tag Name();

            /**
             * Returns the context-specific tag identifying a String-Integer pair's value
             * Returns the context-specific tag identifying a String-Integer pair's value
             */
            static ber::Tag Value();
        };
        
        struct LIBEMBER_API StreamDescriptor
        {
            /**
             * Returns the context-specific tag identifying the stream format within a StreamDescriptor.
             * @return The context-specific tag identifying the stream format within a StreamDescriptor.
             */
            static ber::Tag Format();

            /**
             * Returns the context-specific tag identifying the stream-value offset within a StreamDescriptor.
             * @return The context-specific tag identifying the stream-value offset within a StreamDescriptor.
             */
            static ber::Tag Offset();
        };

        /**
         * Returns the tag identifying a root element collection. 
         * @return Returns the tag identifying a root.
         */
        static ber::Tag Root();

        /**
         * Returns the tag that is used for a glow element when it is listed within
         * a sequence. The tag returned is ContextSpecific - 0.
         * @return Returns the tag for listed glow elements.
         */
        static ber::Tag ElementDefault();

        struct LIBEMBER_API Matrix
        {
            /**
             * Returns the context-specific tag identifying a number.
             * @return Returns the context-specific tag identifying a number.
             */
            static ber::Tag Number();

            /**
             * Returns the context-specific tag identifying the contents of a matrix.
             * @return Returns the context-specific tag identifying the matrix contents.
             */
            static ber::Tag Contents();

            /**
             * Returns the context-specific tag identifying the child collection of a matrix.
             * @return Returns the context-specific tag identifying the child collection.
             */
            static ber::Tag Children();

            /**
             * Returns the context-specific tag identifying the target collection of a matrix.
             * @return Returns the context-specific tag identifying the target collection.
             */
            static ber::Tag Targets();

            /**
             * Returns the context-specific tag identifying the source collection of a matrix.
             * @return Returns the context-specific tag identifying the source collection.
             */
            static ber::Tag Sources();

            /**
             * Returns the context-specific tag identifying the connection collection of a matrix.
             * @return Returns the context-specific tag identifying the connection collection.
             */
            static ber::Tag Connections();
        };

        struct LIBEMBER_API QualifiedMatrix
        {
            /**
             * Returns the context-specific tag identifying the path of a qualified matrix.
             * @return Returns the context-specific tag identifying the path of a qualified matrix.
             */
            static ber::Tag Path();

            /**
             * Returns the context-specific tag identifying the contents of a qualified matrix.
             * @return Returns the context-specific tag identifying the matrix contents.
             */
            static ber::Tag Contents();

            /**
             * Returns the context-specific tag identifying the child collection of a qualified matrix.
             * @return Returns the context-specific tag identifying the child collection.
             */
            static ber::Tag Children();

            /**
             * Returns the context-specific tag identifying the target collection of a qualified matrix.
             * @return Returns the context-specific tag identifying the target collection.
             */
            static ber::Tag Targets();

            /**
             * Returns the context-specific tag identifying the source collection of a qualified matrix.
             * @return Returns the context-specific tag identifying the source collection.
             */
            static ber::Tag Sources();

            /**
             * Returns the context-specific tag identifying the connection collection of a qualified matrix.
             * @return Returns the context-specific tag identifying the connection collection.
             */
            static ber::Tag Connections();
        };

        struct LIBEMBER_API MatrixContents
        {
            /**
             * Returns the context-specific tag identifying an identifier.
             * @return Returns the context-specific tag identifying an identifier.
             */
            static ber::Tag Identifier();

            /**
             * Returns the context-specific tag identifying a number.
             * @return Returns the context-specific tag identifying a number.
             */
            static ber::Tag Description();

            /**
             * Returns the context-specific tag identifying the matrix type.
             * @return Returns the context-specific tag identifying the matrix type.
             */
            static ber::Tag Type();

            /**
             * Returns the context-specific tag identifying the matrix addressing mode.
             * @return Returns the context-specific tag identifying the matrix addressing mode.
             */
            static ber::Tag AddressingMode();

            /**
             * Returns the context-specific tag identifying the matrix target count.
             * @return Returns the context-specific tag identifying the matrix target count.
             */
            static ber::Tag TargetCount();

            /**
             * Returns the context-specific tag identifying the matrix source count.
             * @return Returns the context-specific tag identifying the matrix source count.
             */
            static ber::Tag SourceCount();

            /**
             * Returns the context-specific tag identifying the total maximum connects of a matrix.
             * @return Returns the context-specific tag identifying the total maximum connects of a matrix.
             */
            static ber::Tag MaximumTotalConnects();

            /**
             * Returns the context-specific tag identifying the maximum connects per target of a matrix.
             * @return Returns the context-specific tag identifying the maximum connects per target of a matrix.
             */
            static ber::Tag MaximumConnectsPerTarget();

            /**
             * Returns the context-specific tag identifying the matrix parameters location.
             * @return Returns the context-specific tag identifying the matrix parameters location.
             */
            static ber::Tag ParametersLocation();

            /**
             * Returns the context-specific tag identifying the matrix gain parameter number.
             * @return Returns the context-specific tag identifying the matrix gain parameter number.
             */
            static ber::Tag GainParameterNumber();

            /**
             * Returns the context-specific tag identifying the matrix labels.
             * @return Returns the context-specific tag identifying the matrix labels.
             */
            static ber::Tag Labels();

            /**
             * Returns the context-specific tag identifying the matrix' schema identifier.
             * @return The context-specific tag identifying the matrix' schema identifier.
             */
            static ber::Tag SchemaIdentifiers();

            /**
             * Returns the context-specific tag identifying the template reference.
             * @return The context-specific tag identifying the template reference.
             */
            static ber::Tag TemplateReference();
        };

        struct LIBEMBER_API Signal
        {
            /**
             * Returns the context-specific tag identifying the signal number.
             * @return Returns the context-specific tag identifying the signal number.
             */
            static ber::Tag Number();
        };

        struct LIBEMBER_API Connection
        {
            /**
             * Returns the context-specific tag identifying the number of the target the connection refers to.
             * @return Returns the context-specific tag identifying the number of the target the connection refers to.
             */
            static ber::Tag Target();

            /**
             * Returns the context-specific tag identifying the connected sources.
             * @return Returns the context-specific tag identifying the connected sources.
             */
            static ber::Tag Sources();

            /**
             * Returns the context-specific tag identifying the connection operation.
             * @return Returns the context-specific tag identifying the connection operation.
             */
            static ber::Tag Operation();

            /**
             * Returns the context-specific tag identifying the connection disposition.
             * @return Returns the context-specific tag identifying the connection disposition.
             */
            static ber::Tag Disposition();
        };

        struct LIBEMBER_API Label
        {
            /**
             * Returns the context-specific tag identifying the label base path.
             * @return Returns the context-specific tag identifying the label base path.
             */
            static ber::Tag BasePath();

            /**
             * Returns the context-specific tag identifying the label description.
             * @return Returns the context-specific tag identifying the label description.
             */
            static ber::Tag Description();
        };

        struct LIBEMBER_API Function
        {
            /**
             * Returns the context-specific tag identifying the function number.
             * @return The context-specific tag identifying the function number.
             */
            static ber::Tag Number();

            /**
             * Returns the context-specific tag identifying the function contents.
             * @return The context-specific tag identifying the function contents.
             */
            static ber::Tag Contents();

            /**
             * Returns the context-specific tag identifying the function children.
             * @return The context-specific tag identifying the function children.
             */
            static ber::Tag Children();
        };

        struct LIBEMBER_API QualifiedFunction
        {
            /**
             * Returns the context-specific tag identifying the function path.
             * @return The context-specific tag identifying the function path.
             */
            static ber::Tag Path();

            /**
             * Returns the context-specific tag identifying the function contents.
             * @return The context-specific tag identifying the function contents.
             */
            static ber::Tag Contents();

            /**
             * Returns the context-specific tag identifying the function children.
             * @return The context-specific tag identifying the function children.
             */
            static ber::Tag Children();
        };

        struct LIBEMBER_API FunctionContents
        {
            /**
             * Returns the context-specific tag identifying the identifier of a function.
             * @return The context-specific tag identifying the identifier of a function.
             */
            static ber::Tag Identifier();

            /**
             * Returns the context-specific tag identifying the description of a function.
             * @return The context-specific tag identifying the description of a function.
             */
            static ber::Tag Description();

            /**
             * Returns the context-specific tag identifying the arguments of a function.
             * @return The context-specific tag identifying the arguments of a function.
             */
            static ber::Tag Arguments();

            /**
             * Returns the context-specific tag identifying the result of a function.
             * @return The context-specific tag identifying the result of a function.
             */
            static ber::Tag Result();

            /**
             * Returns the context-specific tag identifying the template reference.
             * @return The context-specific tag identifying the template reference.
             */
            static ber::Tag TemplateReference();
        };

        struct LIBEMBER_API TupleItemDescription
        {
            /**
             * Returns the context-specific tag identifying the type of a TupleItemDescription.
             * @return The context-specific tag identifying the type of a TupleItemDescription.
             */
            static ber::Tag Type();

            /**
             * Returns the context-specific tag identifying the name of a TupleItemDescription.
             * @return The context-specific tag identifying the name of a TupleItemDescription.
             */
            static ber::Tag Name();
        };

        struct LIBEMBER_API Invocation
        {
            /**
             * Returns the context-specific tag identifying the identifier of an Invocation.
             * @return The context-specific tag identifying the identifier of an Invocation.
             */
            static ber::Tag InvocationId();

            /**
             * Returns the context-specific tag identifying the arguments of an Invocation.
             * @return The context-specific tag identifying the arguments of an Invocation.
             */
            static ber::Tag Arguments();
        };

        struct LIBEMBER_API InvocationResult
        {
            /**
             * Returns the context-specific tag identifying the identifier of an InvocationResult.
             * @return The context-specific tag identifying the identifier of an InvocationResult.
             */
            static ber::Tag InvocationId();

            /**
             * Returns the context-specific tag identifying the success field of an InvocationResult.
             * @return The context-specific tag identifying the success field of an InvocationResult.
             */
            static ber::Tag Success();

            /**
             * Returns the context-specific tag identifying the result value of an InvocationResult.
             * @return The context-specific tag identifying the result value of an InvocationResult.
             */
            static ber::Tag Result();
        };

        struct LIBEMBER_API Template 
        {
            /**
             * Returns the context-specific tag identifying a number.
             * @return Returns the context-specific tag identifying a number.
             */
            static ber::Tag Number(); 

            /**
             * Returns the context-specific tag identifying the description. 
             * @return Returns the context-specific tag identifying the description.
             */
            static ber::Tag Description();

            /**
             * Returns the context-specific tag identifying the element described by the template.
             * @return Returns the context-specific tag identifying the element described by the template.
             */
            static ber::Tag Element();
        };

        struct LIBEMBER_API QualifiedTemplate
        {
            /**
             * Returns the context-specific tag identifying the path of the template.
             * @return Returns the context-specific tag identifying the path.
             */
            static ber::Tag Path(); 

            /**
             * Returns the context-specific tag identifying the description. 
             * @return Returns the context-specific tag identifying the description.
             */
            static ber::Tag Description();

            /**
             * Returns the context-specific tag identifying the element described by the template.
             * @return Returns the context-specific tag identifying the element described by the template.
             */
            static ber::Tag Element();
        };
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowTags.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWTAGS_HPP

