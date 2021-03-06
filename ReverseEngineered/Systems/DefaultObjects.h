#pragma once
#include "skse/GameForms.h"
#include "ReverseEngineered/Shared.h"

namespace RE {
   enum DefaultObjectIndex : UInt32 {
      kDOBJIndex_DLC1WerewolfRace    = 0x142, // used for non-DLC vampires; the DOBJ itself was added with Dawnguard
      kDOBJIndex_DLC1VampireLordRace = 0x143,
   };

   class DefaultObjectList {
      public:
         static constexpr int count = 0x15A; // number of DOBJs
         //
         struct DefaultObject {
            const char* description; // 00 // human-readable name
            UInt32		formType;    // 04
            UInt32		signature;   // 08 // four-character string
            UInt32		unk0C;       // 0C
         };
         //
         static DefaultObjectList* GetSingleton(void);
         //
         DefaultObject objects[count];
         //
         TESForm* GetBySignature(UInt32 signature) const;
         SInt32 GetIndexOfSignature(UInt32 signature) const;
   };
   class BGSDefaultObjectManager : public TESForm {
      //
      // Constructor: 0x004F8FB0
      //
      public:
         enum { kTypeID = kFormType_DOBJ };

         // cleared to 0 in ctor
         TESForm* objects[DefaultObjectList::count]; // 14
         UInt8    pad[DefaultObjectList::count];     // just pad out the rest of the space

         static BGSDefaultObjectManager* GetSingleton(void);
   };

   static DEFINE_SUBROUTINE(TESForm*, GetDOBJByIndex, 0x00401EA0, UInt32 index);
};