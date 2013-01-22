using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.IO;
using System.Diagnostics;

namespace EmberPlusProxy
{
   [DataContract(Namespace = "http://EmberPlusProxy")]
   class Settings
   {
      public IEnumerable<GlowEndPointDesc> EndPoints
      {
         get { return _endPoints; }
      }

      public static Settings Load()
      {
         var dcs = new DataContractSerializer(typeof(Settings));
         Settings settings;

         try
         {
            using(var stream = File.OpenRead(FilePath))
               settings = (Settings)dcs.ReadObject(stream);
         }
         catch
         {
            settings = new Settings
            {
               _endPoints = new List<GlowEndPointDesc>
               {
                  new GlowEndPointDesc
                  {
                     HostName = "localhost",
                     Port = 9000,
                     LocalNumber = 1,
                     SlotId = 0,
                  },
               },
            };

            settings.Save();
         }

         return settings;
      }

      #region Implementation
      [DataMember(Name = "EndPoints")]
      List<GlowEndPointDesc> _endPoints;

      static string FilePath
      {
         get
         {
            var directory = Path.GetDirectoryName(typeof(Settings).Assembly.Location);

            return Path.Combine(directory, "EmberPlusProxy.settings.xml");
         }
      }

      void Save()
      {
         var dcs = new DataContractSerializer(typeof(Settings));

         try
         {
            using(var stream = File.Create(FilePath))
               dcs.WriteObject(stream, this);
         }
         catch(Exception ex)
         {
            Debug.WriteLine(ex.Message);
         }
      }
      #endregion
   }

   [DataContract(Namespace = "http://EmberPlusProxy")]
   class GlowEndPointDesc
   {
      [DataMember]
      public string HostName { get; set; }

      [DataMember]
      public int Port { get; set; }

      [DataMember]
      public int SlotId { get; set; }

      [DataMember]
      public int LocalNumber { get; set; }

      [DataMember]
      public string Description { get; set; }
   }
}
