using System; using System.Collections.Generic; using System.Linq; using EmberPlusRouter; using EmberPlusRouter.Model;
class Regression {
 static int failures; static void Check(bool ok,string name){Console.WriteLine(name+": "+(ok?"PASS":"FAIL"));if(!ok)failures++;}
 static IEnumerable<Signal> Once(Signal s, int[] count){if(++count[0]>1)throw new Exception("enumerated twice");yield return s;}
 static int Main(){var d=new Dispatcher();int events=0;d.GlowRootReady+=(s,e)=>events++;var t=new Signal(0,null);var a=new Signal(1,null);var foreign=new Signal(2,null);var m=new NToNMatrix(1,Node.CreateRoot(),"matrix",d,new[]{t},new[]{a},null,null);
 bool rejected=false;try{m.Connect(t,new[]{a,foreign},null);}catch(ArgumentException){rejected=true;}Check(rejected&&events==0&&!t.ConnectedSources.Any(),"foreign later source rejected before mutation/notification");
 m.Connect(t,new[]{a},null);Check(t.ConnectedSources.SequenceEqual(new[]{a}),"valid absolute connection");m.Connect(t,new Signal[0],null);Check(!t.ConnectedSources.Any(),"empty absolute disconnect");
 var count=new[]{0};bool once=true;try{m.Connect(t,Once(a,count),null);}catch(Exception){once=false;}Check(once&&count[0]==1&&t.ConnectedSources.SequenceEqual(new[]{a}),"single enumeration");return failures==0?0:1;}}
