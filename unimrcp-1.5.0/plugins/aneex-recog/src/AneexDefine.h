// This namespace contains name of functions, using in library
// for UniMRCP project

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define ANEEXDEFINE_PUBLIC __attribute__ ((dllexport))
    #else
      #define ANEEXDEFINE_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define ANEEXDEFINE_PUBLIC __attribute__ ((dllimport))
    #else
      #define ANEEXDEFINE_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define ANEEXDEFINE_LOCAL
#else
  #if __GNUC__ >= 4
    #define ANEEXDEFINE_PUBLIC __attribute__ ((visibility ("default")))
    #define ANEEXDEFINE_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define ANEEXDEFINE_PUBLIC
    #define ANEEXDEFINE_LOCAL
  #endif
#endif

extern "C" ANEEXDEFINE_PUBLIC void function(int a);
class ANEEXDEFINE_PUBLIC AudioIndexingTask
{
   int c;
   ANEEXDEFINE_LOCAL void privateMethod();  // Only for use within this DSO
public:
   AudioIndexingTask();
   ~AudioIndexingTask();
};
