// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME srcdIExoticDictionary
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// The generated code does not explicitly qualifies STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/opt/anaExotic/include/rootana/Application.h"
#include "/opt/anaExotic/include/rootana/DataPointer.h"
#include "/opt/anaExotic/include/rootana/Cut.h"
#include "/opt/anaExotic/include/rootana/Globals.h"

// Header files passed via #pragma extra_include

namespace exotic {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *exotic_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("exotic", 0 /*version*/, "utils/VariableStructs.h", 4,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &exotic_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *exotic_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace rootana {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *rootana_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("rootana", 0 /*version*/, "rootana/Directory.h", 12,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &rootana_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *rootana_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static TClass *exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLInfogR_Dictionary();
   static void exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLInfogR_TClassManip(TClass*);
   static void delete_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLInfogR(void *p);
   static void deleteArray_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLInfogR(void *p);
   static void destruct_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLInfogR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::exotic::utils::DelayedMessagePrinter<exotic::utils::Info>*)
   {
      ::exotic::utils::DelayedMessagePrinter<exotic::utils::Info> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::exotic::utils::DelayedMessagePrinter<exotic::utils::Info>));
      static ::ROOT::TGenericClassInfo 
         instance("exotic::utils::DelayedMessagePrinter<exotic::utils::Info>", "utils/ErrorExotic.h", 331,
                  typeid(::exotic::utils::DelayedMessagePrinter<exotic::utils::Info>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLInfogR_Dictionary, isa_proxy, 4,
                  sizeof(::exotic::utils::DelayedMessagePrinter<exotic::utils::Info>) );
      instance.SetDelete(&delete_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLInfogR);
      instance.SetDeleteArray(&deleteArray_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLInfogR);
      instance.SetDestructor(&destruct_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLInfogR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::exotic::utils::DelayedMessagePrinter<exotic::utils::Info>*)
   {
      return GenerateInitInstanceLocal((::exotic::utils::DelayedMessagePrinter<exotic::utils::Info>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::exotic::utils::DelayedMessagePrinter<exotic::utils::Info>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLInfogR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::exotic::utils::DelayedMessagePrinter<exotic::utils::Info>*)0x0)->GetClass();
      exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLInfogR_TClassManip(theClass);
   return theClass;
   }

   static void exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLInfogR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLWarninggR_Dictionary();
   static void exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLWarninggR_TClassManip(TClass*);
   static void delete_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLWarninggR(void *p);
   static void deleteArray_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLWarninggR(void *p);
   static void destruct_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLWarninggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::exotic::utils::DelayedMessagePrinter<exotic::utils::Warning>*)
   {
      ::exotic::utils::DelayedMessagePrinter<exotic::utils::Warning> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::exotic::utils::DelayedMessagePrinter<exotic::utils::Warning>));
      static ::ROOT::TGenericClassInfo 
         instance("exotic::utils::DelayedMessagePrinter<exotic::utils::Warning>", "utils/ErrorExotic.h", 331,
                  typeid(::exotic::utils::DelayedMessagePrinter<exotic::utils::Warning>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLWarninggR_Dictionary, isa_proxy, 4,
                  sizeof(::exotic::utils::DelayedMessagePrinter<exotic::utils::Warning>) );
      instance.SetDelete(&delete_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLWarninggR);
      instance.SetDeleteArray(&deleteArray_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLWarninggR);
      instance.SetDestructor(&destruct_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLWarninggR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::exotic::utils::DelayedMessagePrinter<exotic::utils::Warning>*)
   {
      return GenerateInitInstanceLocal((::exotic::utils::DelayedMessagePrinter<exotic::utils::Warning>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::exotic::utils::DelayedMessagePrinter<exotic::utils::Warning>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLWarninggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::exotic::utils::DelayedMessagePrinter<exotic::utils::Warning>*)0x0)->GetClass();
      exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLWarninggR_TClassManip(theClass);
   return theClass;
   }

   static void exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLWarninggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLErrorgR_Dictionary();
   static void exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLErrorgR_TClassManip(TClass*);
   static void delete_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLErrorgR(void *p);
   static void deleteArray_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLErrorgR(void *p);
   static void destruct_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLErrorgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::exotic::utils::DelayedMessagePrinter<exotic::utils::Error>*)
   {
      ::exotic::utils::DelayedMessagePrinter<exotic::utils::Error> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::exotic::utils::DelayedMessagePrinter<exotic::utils::Error>));
      static ::ROOT::TGenericClassInfo 
         instance("exotic::utils::DelayedMessagePrinter<exotic::utils::Error>", "utils/ErrorExotic.h", 331,
                  typeid(::exotic::utils::DelayedMessagePrinter<exotic::utils::Error>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLErrorgR_Dictionary, isa_proxy, 4,
                  sizeof(::exotic::utils::DelayedMessagePrinter<exotic::utils::Error>) );
      instance.SetDelete(&delete_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLErrorgR);
      instance.SetDeleteArray(&deleteArray_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLErrorgR);
      instance.SetDestructor(&destruct_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLErrorgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::exotic::utils::DelayedMessagePrinter<exotic::utils::Error>*)
   {
      return GenerateInitInstanceLocal((::exotic::utils::DelayedMessagePrinter<exotic::utils::Error>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::exotic::utils::DelayedMessagePrinter<exotic::utils::Error>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLErrorgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::exotic::utils::DelayedMessagePrinter<exotic::utils::Error>*)0x0)->GetClass();
      exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLErrorgR_TClassManip(theClass);
   return theClass;
   }

   static void exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLErrorgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *exoticcLcLDsssd_Dictionary();
   static void exoticcLcLDsssd_TClassManip(TClass*);
   static void *new_exoticcLcLDsssd(void *p = 0);
   static void *newArray_exoticcLcLDsssd(Long_t size, void *p);
   static void delete_exoticcLcLDsssd(void *p);
   static void deleteArray_exoticcLcLDsssd(void *p);
   static void destruct_exoticcLcLDsssd(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::exotic::Dsssd*)
   {
      ::exotic::Dsssd *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::exotic::Dsssd));
      static ::ROOT::TGenericClassInfo 
         instance("exotic::Dsssd", "utils/Exotic.h", 44,
                  typeid(::exotic::Dsssd), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &exoticcLcLDsssd_Dictionary, isa_proxy, 4,
                  sizeof(::exotic::Dsssd) );
      instance.SetNew(&new_exoticcLcLDsssd);
      instance.SetNewArray(&newArray_exoticcLcLDsssd);
      instance.SetDelete(&delete_exoticcLcLDsssd);
      instance.SetDeleteArray(&deleteArray_exoticcLcLDsssd);
      instance.SetDestructor(&destruct_exoticcLcLDsssd);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::exotic::Dsssd*)
   {
      return GenerateInitInstanceLocal((::exotic::Dsssd*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::exotic::Dsssd*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *exoticcLcLDsssd_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::exotic::Dsssd*)0x0)->GetClass();
      exoticcLcLDsssd_TClassManip(theClass);
   return theClass;
   }

   static void exoticcLcLDsssd_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_rootanacLcLApp(void *p);
   static void deleteArray_rootanacLcLApp(void *p);
   static void destruct_rootanacLcLApp(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::rootana::App*)
   {
      ::rootana::App *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::rootana::App >(0);
      static ::ROOT::TGenericClassInfo 
         instance("rootana::App", ::rootana::App::Class_Version(), "rootana/Application.h", 28,
                  typeid(::rootana::App), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::rootana::App::Dictionary, isa_proxy, 4,
                  sizeof(::rootana::App) );
      instance.SetDelete(&delete_rootanacLcLApp);
      instance.SetDeleteArray(&deleteArray_rootanacLcLApp);
      instance.SetDestructor(&destruct_rootanacLcLApp);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::rootana::App*)
   {
      return GenerateInitInstanceLocal((::rootana::App*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::rootana::App*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *rootanacLcLDataPointer_Dictionary();
   static void rootanacLcLDataPointer_TClassManip(TClass*);
   static void delete_rootanacLcLDataPointer(void *p);
   static void deleteArray_rootanacLcLDataPointer(void *p);
   static void destruct_rootanacLcLDataPointer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::rootana::DataPointer*)
   {
      ::rootana::DataPointer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::rootana::DataPointer));
      static ::ROOT::TGenericClassInfo 
         instance("rootana::DataPointer", "rootana/DataPointer.h", 16,
                  typeid(::rootana::DataPointer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &rootanacLcLDataPointer_Dictionary, isa_proxy, 4,
                  sizeof(::rootana::DataPointer) );
      instance.SetDelete(&delete_rootanacLcLDataPointer);
      instance.SetDeleteArray(&deleteArray_rootanacLcLDataPointer);
      instance.SetDestructor(&destruct_rootanacLcLDataPointer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::rootana::DataPointer*)
   {
      return GenerateInitInstanceLocal((::rootana::DataPointer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::rootana::DataPointer*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *rootanacLcLDataPointer_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::rootana::DataPointer*)0x0)->GetClass();
      rootanacLcLDataPointer_TClassManip(theClass);
   return theClass;
   }

   static void rootanacLcLDataPointer_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *rootanacLcLCut_Dictionary();
   static void rootanacLcLCut_TClassManip(TClass*);
   static void delete_rootanacLcLCut(void *p);
   static void deleteArray_rootanacLcLCut(void *p);
   static void destruct_rootanacLcLCut(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::rootana::Cut*)
   {
      ::rootana::Cut *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::rootana::Cut));
      static ::ROOT::TGenericClassInfo 
         instance("rootana::Cut", "rootana/Cut.h", 194,
                  typeid(::rootana::Cut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &rootanacLcLCut_Dictionary, isa_proxy, 4,
                  sizeof(::rootana::Cut) );
      instance.SetDelete(&delete_rootanacLcLCut);
      instance.SetDeleteArray(&deleteArray_rootanacLcLCut);
      instance.SetDestructor(&destruct_rootanacLcLCut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::rootana::Cut*)
   {
      return GenerateInitInstanceLocal((::rootana::Cut*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::rootana::Cut*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *rootanacLcLCut_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::rootana::Cut*)0x0)->GetClass();
      rootanacLcLCut_TClassManip(theClass);
   return theClass;
   }

   static void rootanacLcLCut_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace rootana {
//______________________________________________________________________________
atomic_TClass_ptr App::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *App::Class_Name()
{
   return "rootana::App";
}

//______________________________________________________________________________
const char *App::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::rootana::App*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int App::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::rootana::App*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *App::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::rootana::App*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *App::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::rootana::App*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace rootana
namespace ROOT {
   // Wrapper around operator delete
   static void delete_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLInfogR(void *p) {
      delete ((::exotic::utils::DelayedMessagePrinter<exotic::utils::Info>*)p);
   }
   static void deleteArray_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLInfogR(void *p) {
      delete [] ((::exotic::utils::DelayedMessagePrinter<exotic::utils::Info>*)p);
   }
   static void destruct_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLInfogR(void *p) {
      typedef ::exotic::utils::DelayedMessagePrinter<exotic::utils::Info> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::exotic::utils::DelayedMessagePrinter<exotic::utils::Info>

namespace ROOT {
   // Wrapper around operator delete
   static void delete_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLWarninggR(void *p) {
      delete ((::exotic::utils::DelayedMessagePrinter<exotic::utils::Warning>*)p);
   }
   static void deleteArray_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLWarninggR(void *p) {
      delete [] ((::exotic::utils::DelayedMessagePrinter<exotic::utils::Warning>*)p);
   }
   static void destruct_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLWarninggR(void *p) {
      typedef ::exotic::utils::DelayedMessagePrinter<exotic::utils::Warning> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::exotic::utils::DelayedMessagePrinter<exotic::utils::Warning>

namespace ROOT {
   // Wrapper around operator delete
   static void delete_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLErrorgR(void *p) {
      delete ((::exotic::utils::DelayedMessagePrinter<exotic::utils::Error>*)p);
   }
   static void deleteArray_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLErrorgR(void *p) {
      delete [] ((::exotic::utils::DelayedMessagePrinter<exotic::utils::Error>*)p);
   }
   static void destruct_exoticcLcLutilscLcLDelayedMessagePrinterlEexoticcLcLutilscLcLErrorgR(void *p) {
      typedef ::exotic::utils::DelayedMessagePrinter<exotic::utils::Error> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::exotic::utils::DelayedMessagePrinter<exotic::utils::Error>

namespace ROOT {
   // Wrappers around operator new
   static void *new_exoticcLcLDsssd(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::exotic::Dsssd : new ::exotic::Dsssd;
   }
   static void *newArray_exoticcLcLDsssd(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::exotic::Dsssd[nElements] : new ::exotic::Dsssd[nElements];
   }
   // Wrapper around operator delete
   static void delete_exoticcLcLDsssd(void *p) {
      delete ((::exotic::Dsssd*)p);
   }
   static void deleteArray_exoticcLcLDsssd(void *p) {
      delete [] ((::exotic::Dsssd*)p);
   }
   static void destruct_exoticcLcLDsssd(void *p) {
      typedef ::exotic::Dsssd current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::exotic::Dsssd

namespace rootana {
//______________________________________________________________________________
void App::Streamer(TBuffer &R__b)
{
   // Stream an object of class rootana::App.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(rootana::App::Class(),this);
   } else {
      R__b.WriteClassBuffer(rootana::App::Class(),this);
   }
}

} // namespace rootana
namespace ROOT {
   // Wrapper around operator delete
   static void delete_rootanacLcLApp(void *p) {
      delete ((::rootana::App*)p);
   }
   static void deleteArray_rootanacLcLApp(void *p) {
      delete [] ((::rootana::App*)p);
   }
   static void destruct_rootanacLcLApp(void *p) {
      typedef ::rootana::App current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::rootana::App

namespace ROOT {
   // Wrapper around operator delete
   static void delete_rootanacLcLDataPointer(void *p) {
      delete ((::rootana::DataPointer*)p);
   }
   static void deleteArray_rootanacLcLDataPointer(void *p) {
      delete [] ((::rootana::DataPointer*)p);
   }
   static void destruct_rootanacLcLDataPointer(void *p) {
      typedef ::rootana::DataPointer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::rootana::DataPointer

namespace ROOT {
   // Wrapper around operator delete
   static void delete_rootanacLcLCut(void *p) {
      delete ((::rootana::Cut*)p);
   }
   static void deleteArray_rootanacLcLCut(void *p) {
      delete [] ((::rootana::Cut*)p);
   }
   static void destruct_rootanacLcLCut(void *p) {
      typedef ::rootana::Cut current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::rootana::Cut

namespace {
  void TriggerDictionaryInitialization_ExoticDictionary_Impl() {
    static const char* headers[] = {
"include/rootana/Application.h",
"include/rootana/DataPointer.h",
"include/rootana/Cut.h",
"include/rootana/Globals.h",
0
    };
    static const char* includePaths[] = {
"/opt/midas/include",
"/opt/root/include/",
"/opt/anaExotic/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "ExoticDictionary dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace exotic{namespace utils{struct __attribute__((annotate("$clingAutoload$utils/ErrorExotic.h")))  __attribute__((annotate("$clingAutoload$include/rootana/Application.h")))  Info;}}
namespace exotic{namespace utils{template <class ErrType_t> class __attribute__((annotate("$clingAutoload$utils/ErrorExotic.h")))  __attribute__((annotate("$clingAutoload$include/rootana/Application.h")))  DelayedMessagePrinter;
}}
namespace exotic{namespace utils{struct __attribute__((annotate("$clingAutoload$utils/ErrorExotic.h")))  __attribute__((annotate("$clingAutoload$include/rootana/Application.h")))  Warning;}}
namespace exotic{namespace utils{struct __attribute__((annotate("$clingAutoload$utils/ErrorExotic.h")))  __attribute__((annotate("$clingAutoload$include/rootana/Application.h")))  Error;}}
namespace exotic{class __attribute__((annotate("$clingAutoload$utils/Exotic.h")))  __attribute__((annotate("$clingAutoload$include/rootana/Application.h")))  Dsssd;}
namespace rootana{class __attribute__((annotate(R"ATTRDUMP(rootana)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(rootana)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(rootana)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(rootana)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$include/rootana/Application.h")))  App;}
namespace rootana{class __attribute__((annotate("$clingAutoload$include/rootana/DataPointer.h")))  DataPointer;}
namespace rootana{class __attribute__((annotate("$clingAutoload$include/rootana/Cut.h")))  Cut;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "ExoticDictionary dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "include/rootana/Application.h"
#include "include/rootana/DataPointer.h"
#include "include/rootana/Cut.h"
#include "include/rootana/Globals.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"exotic::Dsssd", payloadCode, "@",
"exotic::utils::DelayedMessagePrinter<exotic::utils::Error>", payloadCode, "@",
"exotic::utils::DelayedMessagePrinter<exotic::utils::Info>", payloadCode, "@",
"exotic::utils::DelayedMessagePrinter<exotic::utils::Warning>", payloadCode, "@",
"rootana::App", payloadCode, "@",
"rootana::Cut", payloadCode, "@",
"rootana::DataPointer", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("ExoticDictionary",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_ExoticDictionary_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_ExoticDictionary_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_ExoticDictionary() {
  TriggerDictionaryInitialization_ExoticDictionary_Impl();
}
