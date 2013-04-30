using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Win32;
using EmberLib.Glow;

namespace EmberPlusRegistryProvider
{
   class RegistryProvider
   {
      public void ReadGlow(GlowContainer glow, Client client)
      {
         var rootItem = new Item(null, _rootKey.Name, false, _rootKey);

         var walker = new Walker(this, client, rootItem);
         walker.Walk(glow);
      }

      #region Implementation
      RegistryKey _rootKey = Registry.CurrentUser;
      Dictionary<string, int> _streamIdentifiers = new Dictionary<string,int>();

      static Item[] GetSubItems(Item item)
      {
         var subKeyNames = item.Key.GetSubKeyNames();
         var valueNames = item.Key.GetValueNames();

         return (from keyName in subKeyNames
                 orderby keyName
                 select new Item(item, keyName, false, null))
                 .Concat(
                 from valueName in valueNames
                 orderby valueName
                 select new Item(item, valueName, true, null))
                 .ToArray();
      }
      #endregion

      #region Class Item
      class Item
      {
         public Item(Item parent, string name, bool isValue, RegistryKey key)
         {
            Parent = parent;
            Name = name;
            IsValue = isValue;
            Key = key;
         }

         public Item Parent { get; private set; }
         public string Name { get; private set; }
         public bool IsValue { get; private set; }
         public RegistryKey Key { get; private set; }
         public Item[] Children { get; set; }

         public string Identifier
         {
            get
            {
               if(_ident == null)
               {
                  _ident = Name;

                  if(String.IsNullOrEmpty(_ident))
                  {
                     _ident = "__default";
                  }
                  else
                  {
                     if(Char.IsLetter(_ident[0]) == false
                     && _ident[0] != '_')
                        _ident = "_" + _ident;

                     _ident = _ident.Replace('/', '_');
                  }
               }

               return _ident;
            }
         }

         public string IdentPath
         {
            get
            {
               if(_identPath == null)
               {
                  var list = new LinkedList<string>();
                  var item = this;

                  while(item.Parent != null)
                  {
                     list.AddFirst(item.Identifier);
                     item = item.Parent;
                  }

                  _identPath = String.Join(@"\", list);
               }

               return _identPath;
            }
         }

         public void Open(bool isWriteable)
         {
            if(Parent != null)
               Key = Parent.Key.OpenSubKey(Name, isWriteable);
         }

         public void Close()
         {
            if(Key != null)
               Key.Close();
         }

         #region Implementation
         string _identPath;
         string _ident;
         #endregion
      }
      #endregion

      #region Class Walker
      class Walker : GlowWalker
      {
         public Walker(RegistryProvider registry, Client client, Item rootItem)
         {
            _registry = registry;
            _client = client;
            _rootItem = rootItem;
         }

         protected override void OnCommand(GlowCommand glow, int[] path)
         {
            if(glow.Number == GlowCommandType.GetDirectory)
            {
               var glowRoot = GlowRootElementCollection.CreateRoot();
               var fields = glow.DirFieldMask ?? GlowFieldFlags.All;
               var item = OpenItem(path, false);

               if(item.IsValue)
               {
                  CreateGlowParameter(item, path, null, fields, glowRoot);
               }
               else
               {
                  var children = item.Children;

                  if(children == null)
                  {
                     children = GetSubItems(item);
                     item.Children = children;
                  }

                  if(children.Any())
                  {
                     var number = 1;

                     foreach(var child in children)
                     {
                        if(child.IsValue)
                           CreateGlowParameter(child, path, number, fields, glowRoot);
                        else
                           CreateGlowNode(child, path, number, fields, glowRoot);

                        number++;
                     }
                  }
                  else
                  {
                     CreateGlowNode(item, path, null, fields, glowRoot);
                  }
               }

               _client.Write(glowRoot);
            }
         }

         protected override void OnParameter(GlowParameterBase glow, int[] path)
         {
            var glowValue = glow.Value;

            if(glowValue != null)
            {
               var item = OpenItem(path, true);

               if(item.IsValue)
               {
                  var valueKind = item.Parent.Key.GetValueKind(item.Name);

                  switch(valueKind)
                  {
                     case RegistryValueKind.DWord:
                        item.Parent.Key.SetValue(item.Name, (int)glowValue.Integer);
                        break;
                     case RegistryValueKind.QWord:
                        item.Parent.Key.SetValue(item.Name, glowValue.Integer);
                        break;
                     case RegistryValueKind.String:
                        item.Parent.Key.SetValue(item.Name, glowValue.String);
                        break;
                  }

                  var glowRoot = GlowRootElementCollection.CreateRoot();
                  CreateGlowParameter(item, path, null, GlowFieldFlags.Value, glowRoot);
                  _client.Write(glowRoot);
               }

               CloseItem(item);
            }
         }

         protected override void OnNode(GlowNodeBase glow, int[] path)
         {
         }

         protected override void OnMatrix(GlowMatrixBase glow, int[] path)
         {
         }

         protected override void OnStreamEntry(GlowStreamEntry glow)
         {
         }

         protected override void OnFunction(GlowFunctionBase glow, int[] path)
         {
         }

         protected override void OnInvocationResult(GlowInvocationResult glow)
         {
         }

         #region Implementation
         RegistryProvider _registry;
         Client _client;
         Item _rootItem;

         void CreateGlowNode(Item item, int[] path, int? number, int fields, GlowElementCollectionBase parent)
         {
            if(number != null)
               path = path.Concat(new[] { number.Value }).ToArray();

            var glow = new GlowQualifiedNode(path);

            if((fields & GlowFieldFlags.Identifier) != 0)
               glow.Identifier = item.Identifier;

            if((fields & GlowFieldFlags.Description) != 0
            && String.IsNullOrEmpty(item.Name) == false)
               glow.Description = item.Name;

            parent.Insert(glow);
         }

         void CreateGlowParameter(Item item, int[] path, int? number, int fields, GlowElementCollectionBase parent)
         {
            var glowValue = null as GlowValue;
            var isWriteable = false;

            if((fields & GlowFieldFlags.Value) != 0)
            {
               var valueKind = item.Parent.Key.GetValueKind(item.Name);
               var value = item.Parent.Key.GetValue(item.Name);

               switch(valueKind)
               {
                  case RegistryValueKind.Binary:
                     glowValue = new GlowValue((byte[])value);
                     break;
                  case RegistryValueKind.DWord:
                     glowValue = new GlowValue((long)(int)value);
                     isWriteable = true;
                     break;
                  case RegistryValueKind.ExpandString:
                     glowValue = new GlowValue((string)value);
                     break;
                  case RegistryValueKind.MultiString:
                     glowValue = new GlowValue(String.Join("\n", (string[])value));
                     break;
                  case RegistryValueKind.QWord:
                     glowValue = new GlowValue((long)value);
                     isWriteable = true;
                     break;
                  case RegistryValueKind.String:
                     glowValue = new GlowValue((string)value);
                     isWriteable = true;
                     break;
               }
            }

            if(number != null)
               path = path.Concat(new[] { number.Value }).ToArray();

            var glow = new GlowQualifiedParameter(path);

            if((fields & GlowFieldFlags.Identifier) != 0)
               glow.Identifier = item.Identifier;

            if((fields & GlowFieldFlags.Description) != 0
            && String.IsNullOrEmpty(item.Name) == false)
               glow.Description = item.Name;

            if(fields == GlowFieldFlags.All
            && isWriteable)
               glow.Access = GlowAccess.ReadWrite;

            if(glowValue != null)
               glow.Value = glowValue;

            parent.Insert(glow);
         }

         Item OpenItem(int[] path, bool isWriteable)
         {
            var item = _rootItem;

            foreach(var number in path)
            {
               var children = item.Children;

               if(children == null)
               {
                  children = GetSubItems(item);
                  item.Children = children;
               }

               item = children[number - 1];
               item.Open(isWriteable);
            }

            return item;
         }

         void CloseItem(Item leaf)
         {
            while(leaf != _rootItem)
            {
               leaf.Close();
               leaf = leaf.Parent;
            }
         }
         #endregion
      }
      #endregion
   }
}
