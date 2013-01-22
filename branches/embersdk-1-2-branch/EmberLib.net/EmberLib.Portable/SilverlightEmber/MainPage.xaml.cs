using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Xml;
using System.Xml.Linq;
using BerLib;
using EmberLib;
using EmberLib.Glow;
using EmberLib.Glow.Formula;
using EmberLib.Glow.Framing;
using EmberLib.Glow.PowerPack.Xml;

namespace SilverlightEmber
{
   public partial class MainPage : UserControl
   {
      public MainPage()
      {
         InitializeComponent();
      }

      #region XmlText DependencyProperty
      public string XmlText
      {
         get { return (string)GetValue(XmlTextProperty); }
         set { SetValue(XmlTextProperty, value); }
      }

      // Using a DependencyProperty as the backing store for XmlText.  This enables animation, styling, binding, etc...
      public static readonly DependencyProperty XmlTextProperty =
         DependencyProperty.Register("XmlText",
            typeof(string),
            typeof(MainPage),
            new PropertyMetadata("Click \"Load\" to convert Glow to XML..."));
      #endregion

      #region FormulaSource DependencyProperty
      public string FormulaSource
      {
         get { return (string)GetValue(FormulaSourceProperty); }
         set { SetValue(FormulaSourceProperty, value); }
      }

      // Using a DependencyProperty as the backing store for FormulaSource.  This enables animation, styling, binding, etc...
      public static readonly DependencyProperty FormulaSourceProperty =
         DependencyProperty.Register("FormulaSource",
            typeof(string),
            typeof(MainPage),
            new PropertyMetadata("1 + 1"));
      #endregion

      #region FormulaResult DependencyProperty
      public string FormulaResult
      {
         get { return (string)GetValue(FormulaResultProperty); }
         set { SetValue(FormulaResultProperty, value); }
      }

      // Using a DependencyProperty as the backing store for FormulaResult.  This enables animation, styling, binding, etc...
      public static readonly DependencyProperty FormulaResultProperty =
         DependencyProperty.Register("FormulaResult",
            typeof(string),
            typeof(MainPage),
            new PropertyMetadata(null));
      #endregion

      #region Implementation
      const string EmberFileDialogFilter = "Ember File|*.EmBER|All Files|*.*";
      const string S101FileDialogFilter = "S101 File|*.s101|All Files|*.*";

      void DecodeGlow(GlowContainer glow)
      {
         var buffer = new StringBuilder();
         var settings = new XmlWriterSettings
         {
            OmitXmlDeclaration = true,
            Indent = true,
            IndentChars = "  ",
         };

         using(var writer = XmlWriter.Create(buffer, settings))
            GlowXmlExport.Export(glow, writer);

         XmlText = buffer.ToString();
      }

      GlowContainer EncodeGlow()
      {
         using(var reader = XmlReader.Create(new StringReader(XmlText)))
            return GlowXmlImport.Import(reader);
      }
      #endregion

      #region Event Handlers
      void loadButton_Click(object sender, RoutedEventArgs e)
      {
         var dialog = new OpenFileDialog
         {
            Filter = EmberFileDialogFilter,
         };

         if(dialog.ShowDialog() == true)
         {
            using(var stream = dialog.File.OpenRead())
            {
               var input = new BerStreamInput(stream);
               var reader = new EmberReader(input);
               var glow = EmberNode.Decode(reader, new GlowApplicationInterface()) as GlowContainer;

               if(glow != null)
                  DecodeGlow(glow);
            }
         }
      }

      void loadWithFramingButton_Click(object sender, RoutedEventArgs e)
      {
         var dialog = new OpenFileDialog
         {
            Filter = S101FileDialogFilter,
         };

         if(dialog.ShowDialog() == true)
         {
            using(var stream = dialog.File.OpenRead())
            {
               var buffer = new byte[1024];
               int read;

               var reader = new GlowReader(
                  (_, rootReadyArgs) =>
                  {
                     var root = rootReadyArgs.Root as GlowContainer;

                     if(root != null)
                        DecodeGlow(root);
                  },
                  (_, keepAliveArgs) => { });

               while((read = stream.Read(buffer, 0, buffer.Length)) > 0)
                  reader.ReadBytes(buffer, 0, read);

               if(reader.BaseReader.IsEmpty == false)
                  MessageBox.Show("Incomplete data! Position: " + reader.BaseReader.GetPositionString());
            }
         }
      }

      void saveButton_Click(object sender, RoutedEventArgs e)
      {
         var dialog = new SaveFileDialog
         {
            Filter = EmberFileDialogFilter,
         };

         if(dialog.ShowDialog() == true)
         {
            try
            {
               var glow = EncodeGlow();

               if(glow != null)
               {
                  using(var stream = dialog.OpenFile())
                  {
                     var output = new BerStreamOutput(stream);

                     glow.Encode(output);
                  }
               }
            }
            catch(Exception ex)
            {
               MessageBox.Show("Error: " + ex.Message);
            }
         }
      }

      void saveWithFramingButton_Click(object sender, RoutedEventArgs e)
      {
         var dialog = new SaveFileDialog
         {
            Filter = S101FileDialogFilter,
         };

         if(dialog.ShowDialog() == true)
         {
            try
            {
               var glow = EncodeGlow();

               if(glow != null)
               {
                  using(var stream = dialog.OpenFile())
                  {
                     var output = new GlowOutput(
                        0,
                        (_, packageArgs) =>
                        {
                           stream.Write(packageArgs.FramedPackage, 0, packageArgs.FramedPackageLength);
                        });

                     using(output)
                     {
                        glow.Encode(output);

                        output.Finish();
                     }
                  }
               }
            }
            catch(Exception ex)
            {
               MessageBox.Show("Error: " + ex.Message);
            }
         }
      }

      void evalFormulaButton_Click(object sender, RoutedEventArgs e)
      {
         var result = Compiler.Compile(FormulaSource, false);

         if(result.Success)
         {
            var value = result.Formula.Eval(0);

            FormulaResult = value.ToString(CultureInfo.CurrentCulture);
         }
         else
         {
            FormulaResult = result.Error.Message;
         }
      }

      void TextBox_KeyDown(object sender, KeyEventArgs e)
      {
         if(e.Key == Key.Enter)
            evalFormulaButton_Click(sender, e);
      }
      #endregion
   }
}
