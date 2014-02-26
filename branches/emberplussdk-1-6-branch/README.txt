--------------------------------------------------------------------------------
  CONTENTS OF THIS SDK
--------------------------------------------------------------------------------

The complete documentation of the Ember+ protocol stack may be found in the
'documentation' directory.

In addition this SDK contains the following components in source code form:

  * EmberLib.Net
    A .Net implementation of the Ember+ protocol with additional helpers and
    numerous example applications demonstrating its use.

  * libember_slim
    An ANSI C implementation of the Ember+ protocol usable as a static
    library or as a shared library.

  * libember
    A standard C++03 implementation of the Ember+ protocol usable as a static
    library, a shared library or as a header only library.

  * libs101
    A standard C++03 implementation of the s101 framing used by Ember+.

  * libformula
    A standard C++03 implementation of an evaluator for UPN expressions as
    used by Ember+.

  * tinyember/TinyEmberPlus
    A libember, libs101 & libformula based generic Ember+ provider.

  * tinyember/TinyEmberPlusRouter
    A libember, libs101 & libformula based Ember+ provider demonstrating the
    use of the specialized matrix extensions.
   
This SDK also contains a premake4 project file that may be used to generate
native project files for various platforms and development environments for the
'libember' and 'libember_slim' projects.

The 'tools' directory contains prebuilt versions of 'TinyEmberPlus' and
'TinyEmberPlusRouter' as well as the 'EmberPlusView' utility, a generic Ember+
consumer.

