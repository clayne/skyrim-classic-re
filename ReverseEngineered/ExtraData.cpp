#include "ReverseEngineered/ExtraData.h"
#include "ReverseEngineered/Shared.h"
#include "ReverseEngineered/Types.h"

#include "skse/GameBSExtraData.h"

namespace RE {
   static const UInt32 VTBL_ExtraCollisionData = 0x010794B8;
   static const UInt32 VTBL_ExtraLightData     = 0x010794D8;
   static const UInt32 VTBL_ExtraPrimitive     = 0x01079128;
   static const UInt32 VTBL_ExtraRadius        = 0x01079438;

   ExtraCollisionData* ExtraCollisionData::Create() {
      auto extra    = (ExtraCollisionData*) BSExtraData::Create(sizeof(ExtraCollisionData), VTBL_ExtraCollisionData);
      ExtraCollisionData::Data* contents = (ExtraCollisionData::Data*) HeapAllocate(sizeof(ExtraCollisionData::Data));
      extra->data   = contents;
      return extra;
   };
   ExtraLightData*   ExtraLightData::Create() {
      auto data = (ExtraLightData*) BSExtraData::Create(sizeof(ExtraLightData), VTBL_ExtraLightData);
      {
         data->modifiedFOV = 0;
         data->modifiedFade = 0;
         data->unk10 = 0;
         data->depthBias = 0;
         data->unk18 = 0;
         data->pad19[0] = 0;
         data->pad19[1] = 0;
         data->pad19[2] = 0;
      }
      return data;
   };
   ExtraPrimitive* ExtraPrimitive::Create() {
      auto data = (ExtraPrimitive*) BSExtraData::Create(sizeof(ExtraPrimitive), VTBL_ExtraPrimitive);
      return data;
   };
   ExtraRadius*      ExtraRadius::Create() {
      auto data = (ExtraRadius*) BSExtraData::Create(sizeof(ExtraRadius), VTBL_ExtraRadius);
      {
         data->radiusModifier = 0.0;
      }
      return data;
   };
};