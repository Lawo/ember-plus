/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWMATRIXBASE_HPP
#define __LIBEMBER_GLOW_GLOWMATRIXBASE_HPP

#include "GlowContentElement.hpp"
#include "GlowElementCollection.hpp"
#include "MatrixProperty.hpp"
#include "MatrixType.hpp"
#include "MatrixAddressingMode.hpp"
#include "ParametersLocation.hpp"
#include "util/CompliesWithSchema.hpp"
#include "util/TypeFilter.hpp"
#include "GlowLabel.hpp"
#include "GlowTarget.hpp"
#include "GlowSource.hpp"
#include "GlowConnection.hpp"

namespace libember { namespace glow
{
    /**
     * Class containing the common properties of a matrix.
     * The methods to access a property of this object return a default value if a property doesn't exist.
     * To assure that the property exists, the contains method should be used.
     */
    class LIBEMBER_API GlowMatrixBase : public GlowContentElement
    {
        public:
            /**
             * Tests if the matrix contains the passed property.
             * @param property The property the look for.
             * @return Returns true if the property exists, false otherwise.
             */
            bool contains(MatrixProperty const& property) const;

            /**
             * Sets the identifier string.
             * @param identifier The identifier string.
             */
            void setIdentifier(std::string const& identifier);

            /**
             * Sets the description string.
             * @param description The description string to set.
             */
            void setDescription(std::string const& description);

            /**
             * Sets the string containing the schema identifiers. The identifiers
             * are separated by the linefeed character (0x0A, '\n')
             * @param identifiers The names of the schema identifiers this matrix uses.
             */
            void setSchemaIdentifiers(std::string const& identifiers);

            /**
             * Sets the matrix type.
             * @param type The matrix type to set.
             */
            void setType(MatrixType const& type);

            /**
             * Sets the addressing mode.
             * @param addressingMode The addressing mode to set.
             */
            void setAddressingMode(MatrixAddressingMode const& addressingMode);

            /**
             * Sets the target count.
             * @param targetCount The target count to set.
             */
            void setTargetCount(int targetCount);

            /**
             * Sets the source count.
             * @param sourceCount The source count string to set.
             */
            void setSourceCount(int sourceCount);

            /**
             * Sets the total maximum number of matrix connects.
             * @param maximumTotalConnects The total maximum number of matrix connects to set.
             */
            void setMaximumTotalConnects(int maximumTotalConnects);

            /**
             * Sets the maximum number of connects per target.
             * @param maximumConnectsPerTargets The maximum number of connects per target to set.
             */
            void setMaximumConnectsPerTarget(int maximumConnectsPerTargets);

            /**
             * Sets the parameters location as an inline sub-identifier encoded as INTEGER.
             * @param inlineSubid The parameters location as an inline sub-identifier to set.
             */
            void setParametersLocation(int inlineSubid);

            /**
             * Sets the parameters location as a base path encoded as RELATIVE-OID.
             * @param basePath The parameters location as a base path to set.
             */
            void setParametersLocation(ber::ObjectIdentifier const& basePath);

            /**
             * Sets the gain parameter number.
             * @param gainParameterNumber The gain parameter number to set.
             */
            void setGainParameterNumber(int gainParameterNumber);

            /**
             * Inserts the object identifier to the template describing the structure
             * of this element.
             */
            void setTemplateReference(ber::ObjectIdentifier const& path);

            /**
             * Returns a modifiable sequence collection that contains the labels.
             * The element will be inserted if it doesn't already exist.
             * @return The label collection.
             */
            dom::Sequence* labels();

            /**
             * Returns a modifiable element collection that contains the children.
             * The element will be inserted if it doesn't already exist.
             * @return The element collection.
             */
            GlowElementCollection* children();

            /**
             * Returns a modifiable sequence collection that contains the targets.
             * The element will be inserted if it doesn't already exist.
             * @return The target collection.
             */
            dom::Sequence* targets();

            /**
             * Returns a modifiable sequence collection that contains the sources.
             * The element will be inserted if it doesn't already exist.
             * @return The source collection.
             */
            dom::Sequence* sources();

            /**
             * Returns a modifiable sequence collection that contains the connections.
             * The element will be inserted if it doesn't already exist.
             * @return The connection collection.
             */
            dom::Sequence* connections();

            /**
             * Returns the identifier string.
             * @return The identifier string.
             */
            std::string identifier() const;

            /**
             * Returns the description string.
             * @return The description string.
             */
            std::string description() const;

            /**
             * Returns the string containing the schema identifiers. The identifiers
             * are separated with the line feed character (0x0A, '\n').
             * @return The string containing the schema identifiers.
             */
            std::string schemaIdentifiers() const;

            /**
             * Tests if the matrix complies with the specified schema.
             * @param schemaIdentifier The identifier of the schema to test.
             * @return true, if the schema is supported. Otherwise, this method
             *      return false.
             */
            bool compliesWithSchema(std::string const& schemaIdentifier) const;

            /**
             * Returns the matrix type.
             * @return The matrix type.
             */
            MatrixType type() const;

            /**
             * Returns the matrix addressing mode.
             * @return The matrix addressing mode.
             */
            MatrixAddressingMode addressingMode() const;

            /**
             * Returns the target count.
             * @return The target count.
             */
            int targetCount() const;

            /**
             * Returns the source count.
             * @return The source count.
             */
            int sourceCount() const;

            /**
             * Returns the total maximum number of matrix connects.
             * @return The total maximum number of matrix connects.
             */
            int maximumTotalConnects() const;

            /**
             * Returns the maximum number of connects per target.
             * @return The maximum number of connects per target.
             */
            int maximumConnectsPerTarget() const;

            /**
             * Returns the parameters location.
             * @return The parameters location.
             */
            ParametersLocation parametersLocation() const;

            /**
             * Returns the gain parameter number.
             * @return the gain parameter number.
             */
            int gainParameterNumber() const;

            /**
             * Returns a constant sequence that contains the labels.
             * If the labels field is not present, this method returns null.
             * @return The label collection.
             */
            dom::Sequence const* labels() const;

            /**
             * Inserts all GlowLabel objects in the the labels sequence into the passed OutputIterator.
             * @return The number of copied pointers.
             */
            template<typename OutputIterator>
            size_type typedLabels(OutputIterator dest) const;

            /**
             * Returns the constant element collection. If no children are present,
             * this method returns null.
             * @return Element collection containing the children of this node.
             */
            GlowElementCollection const* children() const;

            /**
             * Returns the constant target collection. If no targets are present,
             * this method returns null.
             * @return Sequence containing the targets of this matrix.
             */
            dom::Sequence const* targets() const;

            /**
             * Extracts all GlowTarget objects from the targets sequence into the passed OutputIterator.
             * @return The number of copied pointers.
             */
            template<typename OutputIterator>
            size_type typedTargets(OutputIterator dest) const;

            /**
             * Returns the constant source collection. If no sources are present,
             * this method returns null.
             * @return Sequence containing the sources of this matrix.
             */
            dom::Sequence const* sources() const;

            /**
             * Extracts all GlowSource objects from the the sources sequence into the passed OutputIterator.
             * @return The number of copied pointers.
             */
            template<typename OutputIterator>
            size_type typedSources(OutputIterator dest) const;

            /**
             * Returns the constant connection collection. If no connections are present,
             * this method returns null.
             * @return Sequence containing the connections of this matrix.
             */
            dom::Sequence const* connections() const;

            /**
             * Extracts all GlowConnection objects from the the connections sequence into the passed OutputIterator.
             * @return The number of copied pointers.
             */
            template<typename OutputIterator>
            size_type typedConnections(OutputIterator dest) const;

            /**
             * Returns the object identifier of the template reference. If not present,
             * an empty oid is being returned.
             */
            ber::ObjectIdentifier templateReference() const;

    protected:
            /**
             * This constructor initializes a matrix with the specified content- and children-tags.
             * @param type Object type.
             * @param tag Application tag.
             * @param contentsTag The tag to use for the contents set.
             * @param childrenTag The tag to use for the children.
             * @param targetsTag The tag to use for the targets.
             * @param sourcesTag The tag to use for the sources.
             * @param connectionsTag The tag to use for the connections.
             */
            GlowMatrixBase(
                GlowType const& type,
                ber::Tag const& tag,
                ber::Tag const& contentsTag,
                ber::Tag const& childrenTag,
                ber::Tag const& targetsTag,
                ber::Tag const& sourcesTag,
                ber::Tag const& connectionsTag);

        private:
            ber::Tag m_childrenTag;
            ber::Tag m_targetsTag;
            ber::Tag m_sourcesTag;
            ber::Tag m_connectionsTag;
    };


    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/

    template<typename OutputIterator>
    inline GlowContainer::size_type GlowMatrixBase::typedLabels(OutputIterator dest) const
    {
        dom::Sequence const* labels = this->labels();

        return labels != 0
               ? util::TypeFilter<GlowLabel>::collect(labels->begin(), labels->end(), dest)
               : 0;
    }

    template<typename OutputIterator>
    inline GlowContainer::size_type GlowMatrixBase::typedTargets(OutputIterator dest) const
    {
        dom::Sequence const* targets = this->targets();

        return targets != 0
               ? util::TypeFilter<GlowTarget>::collect(targets->begin(), targets->end(), dest)
               : 0;
    }

    template<typename OutputIterator>
    inline GlowContainer::size_type GlowMatrixBase::typedSources(OutputIterator dest) const
    {
        dom::Sequence const* sources = this->sources();

        return sources != 0
               ? util::TypeFilter<GlowSource>::collect(sources->begin(), sources->end(), dest)
               : 0;
    }

    template<typename OutputIterator>
    inline GlowContainer::size_type GlowMatrixBase::typedConnections(OutputIterator dest) const
    {
        dom::Sequence const* connections = this->connections();

        return connections != 0
               ? util::TypeFilter<GlowConnection>::collect(connections->begin(), connections->end(), dest)
               : 0;
    }

    inline bool GlowMatrixBase::compliesWithSchema(std::string const& schemaIdentifier) const
    {
        return util::complies_with_schema(schemaIdentifiers(), schemaIdentifier);
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowMatrixBase.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWMATRIXBASE_HPP

