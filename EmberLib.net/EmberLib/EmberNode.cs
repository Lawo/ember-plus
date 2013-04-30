/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol
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

using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Diagnostics;
using BerLib;


namespace EmberLib
{
   /// <summary>
   /// Base class of all DOM nodes. Enumerates its child nodes.
   /// </summary>
   public abstract class EmberNode : IEnumerable<EmberNode>
   {
      /// <summary>
      /// Creates a new instance of EmberNode with the specified tag.
      /// </summary>
      /// <param name="tag">The tag of the new EmberNode.</param>
      protected EmberNode(BerTag tag)
      {
         Tag = tag;
      }

      /// <summary>
      /// Gets the BerType of this node. Must be one of the constants
      /// defined in the BerType class.
      /// </summary>
      public uint BerTypeNumber { get; protected internal set; }

      /// <summary>
      /// Gets the tag of this node.
      /// </summary>
      public BerTag Tag { get; protected set; }

      /// <summary>
      /// Gets the parent node of this node.
      /// </summary>
      public EmberContainer Parent { get; internal set; }

      /// <summary>
      /// Gets the child node of this container with the specified tag.
      /// Throws an exception if this EmberNode is not derived from EmberContainer.
      /// </summary>
      /// <param name="tag">The tag of the child node to get.</param>
      /// <returns>The child EmberNode object with the specified tag or null
      /// if no child with the tag exists.</returns>
      public EmberNode this[BerTag tag]
      {
         get { return GetChildNode(tag); }
      }

      /// <summary>
      /// Gets the number of child nodes this node contains.
      /// </summary>
      public abstract int Count { get; }

      /// <summary>
      /// Gets the length in bytes of this node and its child nodes.
      /// When decoding, may be <see cref="BerLib.BerDefinitions.IndefiniteLength" /> if
      /// the indefinite length form has been used to encode the data.
      /// </summary>
      internal int EncodedLength { get; set; }

      /// <summary>
      /// Gets a value indicating whether this container needs to be pre-encoded
      /// before being written out.
      /// </summary>
      internal abstract bool IsDirty { get; }

      /// <summary>
      /// Writes the binary BER representaion of this node and its children to <paramref name="output"/>.
      /// </summary>
      /// <param name="output">The output to write to.</param>
      internal abstract void EncodeOverride(IBerOutput output);

      /// <summary>
      /// Removes the child node with the specified tag.
      /// Throws an exception if this node is not a container.
      /// </summary>
      /// <param name="tag">The tag of the child node to remove.</param>
      /// <returns>True if the child node was found and removed, otherwise false.</returns>
      public abstract bool Remove(BerTag tag);

      /// <summary>
      /// Inserts the specified node into this node's collection of children.
      /// </summary>
      /// <param name="node">The node to insert.</param>
      public void Insert(EmberNode node)
      {
         InsertChildNode(node);
      }

      /// <summary>
      /// Writes the binary BER representaion of this node and its children to <paramref name="output"/>.
      /// </summary>
      /// <param name="output">The output to write to.</param>
      public void Encode(IBerOutput output)
      {
         EncodeOverride(output);
      }

      /// <summary>
      /// Gets the child node with the specified tag.
      /// Throws an exception if this node is not a container.
      /// </summary>
      /// <param name="tag">The tag of the child node to get.</param>
      /// <returns>The child node with the specified tag or null if no such
      /// node exists.</returns>
      internal abstract EmberNode GetChildNode(BerTag tag);

      /// <summary>
      /// Inserts a node into this node's collection of children.
      /// Throws an exception if this node is not a container.
      /// Overridable so that subclasses can verify the node to insert.
      /// </summary>
      /// <param name="node">The node to insert.</param>
      protected internal abstract void InsertChildNode(EmberNode node);

      /// <summary>
      /// Recursively builds a node tree from the specified reader, advancing the reader in the process.
      /// Throws an exception if the first node on the reader (which will become the tree's root cannot
      /// be decoded or is not a container.
      /// </summary>
      /// <param name="reader">The BER reader to build the tree from.</param>
      /// <param name="application">The application interface responsible for creating nodes
      /// with application-defined types. If null, containers with application-defined types
      /// will be decoded to objects of type EmberContainer.</param>
      /// <returns>The root node of the decoded tree.</returns>
      public static EmberNode Decode(EmberReader reader, EmberApplicationInterface application)
      {
         var root = null as EmberContainer;

         if(reader.Read())
         {
            root = FromReader(reader, application) as EmberContainer;

            if(root == null)
               throw new BerException(4, "Root node is not a container");

            var childReader = new EmberReader(reader);

            Decode_Recurse(childReader, root, application);
         }

         return root;
      }

      /// <summary>
      /// Accepts a visitor
      /// </summary>
      /// <typeparam name="TState">The type of the visitor's state</typeparam>
      /// <typeparam name="TResult">The type the visitor methods return.</typeparam>
      /// <param name="visitor">The visitor</param>
      /// <param name="state">The visitor's initial state</param>
      /// <returns>Whatever the visitor methods return</returns>
      public abstract TResult Accept<TState, TResult>(IEmberVisitor<TState, TResult> visitor, TState state);

      /// <summary>
      /// Creates a new EmberNode from the TLV the passed BER reader is currently positioned on.
      /// </summary>
      /// <param name="reader">The BER reader to create the node from.</param>
      /// <param name="application">The application interface responsible for creating nodes
      /// with application-defined types.</param>
      /// <returns>A new instance of EmberNode representing the read TLV or null
      /// if the TLV could not be decoded into an EmberNode.
      /// This happens if the TLV has an unsupported type.</returns>
      internal static EmberNode FromReader(BerReaderBase reader, EmberApplicationInterface application)
      {
         var node = null as EmberNode;
         var type = reader.Type;
         var tag = reader.Tag;

         if(reader.IsContainer)
         {
            switch(type)
            {
#pragma warning disable 618 // EmberFrame is obsolete
               case BerType.Sequence:
                  node = tag == Legacy.EmberFrame.FrameTag
                         ? new Legacy.EmberFrame()
                         : new EmberSequence(tag, null);
                  break;
#pragma warning restore 618

               case BerType.Set:
                  node = new EmberSet(tag);
                  break;

               default:
                  {
                     if(application != null)
                        node = application.CreateNodeFromReader(type, reader);

                     if(node == null)
                     {
                        Debug.WriteLine("WARNING: Unknown BER container type: " + type);

                        node = new EmberContainer(tag, null, type);
                     }
                     break;
                  }
            }
         }
         else
         {
            switch(type)
            {
               case BerType.Boolean:
                  node = new BooleanEmberLeaf(tag, reader.GetBoolean());
                  break;

               case BerType.Integer:
                  if(reader.Length > 4)
                     node = new LongEmberLeaf(tag, reader.GetLong());
                  else
                     node = new IntegerEmberLeaf(tag, reader.GetInteger());
                  break;

               case BerType.Real:
                  node = new RealEmberLeaf(tag, reader.GetReal());
                  break;

               case BerType.UTF8String:
                  node = new StringEmberLeaf(tag, reader.GetString());
                  break;

               case BerType.OctetString:
                  node = new OctetStringEmberLeaf(tag, reader.GetOctetString());
                  break;

               case BerType.RelativeOid:
                  node = new RelativeOidEmberLeaf(tag, reader.GetRelativeOid());
                  break;

               case BerType.ObjectIdentifier:
                  node = new ObjectIdentifierEmberLeaf(tag, reader.GetObjectIdentifier());
                  break;

               default:
                  Debug.WriteLine("Unknown BER value type: " + type);
                  break;
            }
         }

         return node;
      }

      /// <summary>
      /// Clears the dirty flag, pre-encodes the BER header and returns the byte length of the encoded node.
      /// </summary>
      /// <returns>The length of the encoded node.</returns>
      internal abstract int Update();

      /// <summary>
      /// Calls ValidateAfterDecodeOverride and raises the DecoderValidation.ValidationError
      /// event if validation failed.
      /// </summary>
      internal void ValidateAfterDecode()
      {
         var validation = DecoderValidation.Instance;

         if(validation.HasSubscriptions)
         {
            string message;

            if(ValidateAfterDecodeOverride(out message) == false)
               validation.RaiseValidationError(this, message);
         }
      }

      /// <summary>
      /// Override this in application-defined node types to provide validation.
      /// Called when a node has been decoded.
      /// </summary>
      /// <param name="message">When returning false, set this to a string describing the kind of error.</param>
      /// <returns>False if validation failed, otherwise true.</returns>
      protected virtual bool ValidateAfterDecodeOverride(out string message)
      {
         message = null;
         return true;
      }

      #region Overrides
      /// <summary>
      /// Overridden for easier debugging.
      /// </summary>
      /// <returns>A string containing tag and type of this node.</returns>
      public override string ToString()
      {
         return String.Format("Tag: {0} Type: {1}", Tag.ToString(), BerDefinitions.GetTypeName(BerTypeNumber));
      }
      #endregion

      #region Implementation
      static void Decode_Recurse(EmberReader reader, EmberContainer parent, EmberApplicationInterface application)
      {
         while(reader.Read())
         {
            var node = FromReader(reader, application);

            if(node != null)
            {
               var container = node as EmberContainer;

               if(container != null)
               {
                  var childReader = new EmberReader(reader);

                  Decode_Recurse(childReader, container, application);
               }

               node.ValidateAfterDecode();
               parent.InsertChildNode(node);
            }
         }
      }
      #endregion

      #region IEnumerable<EmberNode> Members
      public abstract IEnumerator<EmberNode> GetEnumerator();
      #endregion

      #region IEnumerable Members
      System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
      {
         return GetEnumerator();
      }
      #endregion
   }
}
