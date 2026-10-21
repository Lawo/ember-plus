using System;
using System.Linq;
using BerLib;
static class Program {
 static int Main(string[] args) {
  byte[][] encoded={new byte[]{0,3},new byte[]{39,3},new byte[]{40,3},new byte[]{79,3},new byte[]{80,3},new byte[]{119,3},new byte[]{120,3},new byte[]{0x81,0x34,3},new byte[]{0x88,0x37,3}};
  int[][] expected={new[]{0,0,3},new[]{0,39,3},new[]{1,0,3},new[]{1,39,3},new[]{2,0,3},new[]{2,39,3},new[]{2,40,3},new[]{2,100,3},new[]{2,999,3}};
  int failures=0;
  for(int i=0;i<encoded.Length;i++) {
   var actual=BerEncoding.DecodeObjectIdentifier(new BerMemoryInput(encoded[i]),encoded[i].Length);
   if(!actual.SequenceEqual(expected[i]))failures++;
   Console.WriteLine(string.Join(".",expected[i])+" => "+string.Join(".",actual));
  }
  Console.WriteLine("Exact public decoder + native memory input: failures="+failures+"/9");
  return failures == 0 ? 0 : 1;
 }
}
