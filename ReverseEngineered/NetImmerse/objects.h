#pragma once
#include "skse/GameTypes.h"
#include "skse/NiNodes.h"
#include "skse/NiObjects.h"
#include "skse/Utilities.h"
#include "ReverseEngineered/Shared.h"
#include "ReverseEngineered/Types.h"

class BGSPrimitive;
class bhkAabbPhantom;
class BSMultiBoundShape;
namespace RE {
   class bhkCollisionObject;
   class bhkRigidBody;
   class BSFadeNode;
   class BSMultiBoundNode;
   class NiCollisionObject;
   class NiExtraData;
   class NiGeometry;
   class NiNode;
   class TESObjectREFR;

   struct NiCloningProcess {
      BSTHashMap<uint32_t, uint32_t> unk00;
      BSTHashMap<uint32_t, uint32_t> unk20;
      UInt32 unk40;
      UInt8  unk44 = '$';
      UInt8  pad45[3];
      float  unk48; // initialized to constructor arg
      float  unk4C; // initialized to constructor arg
      float  unk50; // initialized to constructor arg

      MEMBER_FN_PREFIX(NiCloningProcess);
      DEFINE_MEMBER_FN(Constructor, NiCloningProcess&, 0x0046DF40, float);
      DEFINE_MEMBER_FN(Destructor, void, 0x00677F00);

      NiCloningProcess(float a) {
         CALL_MEMBER_FN(this, Constructor)(a);
      }
   };

   class NiObject : public NiRefObject {
      //
      // Notes:
      //  - NiNode->unk1C instanceof bhkCollisionObject (or anything that can cast to it)
      //
      public:
         static constexpr uint32_t vtbl = 0x011173FC;
         //
      public:
         //
         // Standard NetImmerse:
         //
         virtual NiRTTI*		GetRTTI(void); // 02
         // 
         // Casts:
         //
         virtual NiNode*			GetAsNiNode(); // 03
         virtual NiSwitchNode*	GetAsNiSwitchNode(); // 04
         virtual BSFadeNode*		GetAsBSFadeNode(); // 05
         virtual BSMultiBoundNode* GetAsBSMultiBoundNode(); // 06
         virtual NiGeometry*		GetAsNiGeometry(); // 07
         virtual NiTriBasedGeom*	GetAsNiTriBasedGeom(); // 08
         virtual NiTriStrips*		GetAsNiTriStrips(); // 09
         virtual NiTriShape*		GetAsNiTriShape(); // 0A
         virtual BSSegmentedTriShape* GetAsBSSegmentedTriShape(); // 0B
         virtual UInt32			Unk_0C(void);
         virtual UInt32			Unk_0D(void);
         virtual UInt32			Unk_0E(void);
         virtual bhkCollisionObject* GetAsBhkCollisionObject(); // 0F // or a superclass? // virtual method 0x0F
         virtual UInt32			Unk_10(void);
         virtual bhkRigidBody* GetAsBhkRigidBody(); // 11 // or a superclass?
         virtual UInt32        Unk_12(void);
         //
         // Back to NetImmerse:
         //
         virtual NiObject*		CreateClone(NiCloningProcess*); // 13 // internal; you should call the non-virtual Clone member function instead
         //
         virtual void			LoadBinary(NiStream * stream); // 14
         virtual void			LinkObject(NiStream * stream); // 15
         virtual bool			RegisterStreamables(NiStream * stream); // 16
         virtual void			SaveBinary(NiStream * stream); // 17
         virtual bool			IsEqual(NiObject * object); // 18
         //
         virtual void			ProcessClone(NiCloningProcess*); // 19 // internal; you should call the non-virtual Clone member function instead
         virtual void			PostLinkObject(NiStream * stream); // 1A
         virtual bool			StreamCanSkip(void); // 1B
         virtual const NiRTTI*	GetStreamableRTTI(void) const; // 1C
         virtual UInt32         GetBlockAllocationSize(void) const; // 1D
         virtual NiObjectGroup* GetGroup(void) const; // 1E
         virtual void			SetGroup(NiObjectGroup * group); // 1F
         //
         // Begin bethesda extensions?
         //
         virtual UInt32			Unk_20(void); // 20
         //
         MEMBER_FN_PREFIX(NiObject);
         DEFINE_MEMBER_FN(Constructor,   NiObject&, 0x00AAFD30);
         DEFINE_MEMBER_FN(Clone,         NiObject*, 0x00AAFC00, NiCloningProcess*);
         DEFINE_MEMBER_FN(Clone_Default, NiObject*, 0x00AAFF30); // literally just constructs its own NiCloningProcess and does the same things as Clone
         DEFINE_MEMBER_FN(VerifyType,    void*,     0x004202E0, const NiRTTI*); // If object is of type, returns self; otherwise returns NULL.
   };
   static_assert(sizeof(NiObject) == 0x08, "RE::NiObject is not the right size!");

   class NiObjectNET : public NiObject { // sizeof == 0x18
      public:
         static constexpr uint32_t vtbl = 0x01117824;
         //
      public:
         const char*       m_name; // 08
         NiTimeController* m_controller; // 0C next pointer at +0x30 // NiPointer?
         NiExtraData**     m_extraData;  // 10 extra data
         UInt16 m_extraDataLen;      // 14 max valid entry
         UInt16 m_extraDataCapacity; // 16 array len

         // UNTESTED:
         void AddExtraData(NiExtraData * extraData);
         bool RemoveExtraData(NiExtraData * extraData);
         SInt32 GetIndexOf(NiExtraData * extraData);
         NiExtraData* GetExtraData(BSFixedString name);

         MEMBER_FN_PREFIX(NiObjectNET);
         DEFINE_MEMBER_FN(Constructor, NiObjectNET&, 0x00AB4180);
         DEFINE_MEMBER_FN(AddExtraData, bool, 0x00AB44C0, NiExtraData*);
         DEFINE_MEMBER_FN(DynamicCastController, void*, 0x00AB40C0, const NiRTTI*); // dynamic-casts this->m_controller
         DEFINE_MEMBER_FN(SetName, void, 0x00AB4020, StringCache::Ref*);
         DEFINE_MEMBER_FN(ControllerCount, uint32_t, 0x00ABF6D0); // returns the number of NiControllers present
   };
   static_assert(sizeof(NiObjectNET) == 0x18, "RE::NiObjectNET is not the right size!");

   class NiAVObject : public NiObjectNET { // sizeof == 0xA8
      public:
         struct flag {
            flag() = delete;
            enum type : uint32_t {
               none = 0,
               //
               culled                        = 0x00000001, // used by Skyrim to hide nodes
               selective_update              = 0x00000002,
               update_property_controllers   = 0x00000004,
               selective_update_rigid        = 0x00000010,
               display_object                = 0x00000020, // UI?
               disable_sorting               = 0x00000040,
               override_selective_transforms = 0x00000080,
               //
               save_external_geom_data       = 0x00000200,
               no_decals                     = 0x00000400,
               always_draw                   = 0x00000800,
               mesh_lod                      = 0x00001000,
               fixed_bound                   = 0x00002000,
               top_fade_node                 = 0x00004000,
               ignore_fade                   = 0x00008000,
               no_anim_sync_x                = 0x00010000,
               no_anim_sync_y                = 0x00020000,
               no_anim_sync_z                = 0x00040000,
               no_anim_sync_s                = 0x00080000,
               no_dismember                  = 0x00100000,
               no_dismember_validity         = 0x00200000,
               render_use                    = 0x00400000,
               materials_applied             = 0x00800000,
               high_detail                   = 0x01000000,
               force_update                  = 0x02000000,
               preprocessed_node             = 0x04000000,
            };
         };
         using flags_t = std::underlying_type_t<flag::type>;
         //
         struct ControllerUpdateContext {
            struct flag {
               flag() = delete;
               enum type : uint32_t {
                  none = 0,
                  //
                  dirty = 0x00000001,
               };
            };
            using flags_t = std::underlying_type_t<flag::type>;
            //
            float   delta = 0.0F;
            flags_t flags = flag::none;
         };
         //
         virtual void UpdateControllers(ControllerUpdateContext * ctx); // 21	// calls virtual NiTimeController::Update
         virtual void UpdateNodeBound(ControllerUpdateContext * ctx); // 22
         virtual void ApplyTransform(NiMatrix33 * mtx, NiPoint3 * translate, bool postTransform); // 23
         virtual void SetNiProperty(NiProperty * prop); // 24
         virtual void Unk_25(UInt32 arg0); // no-op on NiNode
         virtual void Unk_26(UInt32 arg0); // no-op on NiNode
         virtual NiAVObject* GetObjectByName(const BSFixedString&); // 27	// BSFixedString? alternatively BSFixedString is a typedef of a netimmerse type
         virtual void SetSelectiveUpdateFlags(bool * selectiveUpdate, bool selectiveUpdateTransforms, bool * rigid); // 28
         virtual void UpdateDownwardPass(ControllerUpdateContext * ctx, UInt32 unk1); // 29
         virtual void UpdateSelectedDownwardPass(ControllerUpdateContext * ctx, UInt32 unk1); // 2A
         virtual void UpdateRigidDownwardPass(ControllerUpdateContext * ctx, UInt32 unk1); // 2B
         virtual void UpdateWorldBound(void); // 2C
         virtual void UpdateWorldData(ControllerUpdateContext * ctx);
         virtual void UpdateNoControllers(ControllerUpdateContext * ctx);
         virtual void UpdateDownwardPassTempParent(NiNode * parent, ControllerUpdateContext * ctx);
         virtual void Unk_30(void);	// calls virtual function on parent
         virtual void Unk_31(UInt32 arg0); // NiNode: calls Unk_12 on all non-culled children
         virtual void Unk_32(void* arg0); // NiNode: calls Unk_32 on all children
         //
         NiNode*     parent = nullptr; // 18
         NiCollisionObject* collision = nullptr; // 1C
         NiTransform	localTransform;  // 20
         NiTransform	worldTransform;  // 54
         NiBound  unk88;        // 88 // world bound?
         flags_t  flags = 0;    // 98 // bitfield
         float		unk9C = 1.0F; // 9C
         TESObjectREFR* unkA0;				// A0 // per 004D7C90
         UInt8		unkA4;				// A4
         UInt8		unkA5;				// A5 - bitfield
         //
         MEMBER_FN_PREFIX(NiAVObject);
         DEFINE_MEMBER_FN(SetCollisionObject, void, 0x0046AC60, NiCollisionObject*);
         DEFINE_MEMBER_FN(UpdateNode, void, 0x00AAF320, ControllerUpdateContext * ctx);
         DEFINE_MEMBER_FN(UpdateWorldTransformFromParent, void, 0x00AB5370);
         DEFINE_MEMBER_FN(SetIsCulled, void, 0x0042A5E0, bool); // just here for documentation purposes. don't bother using it; faster to just set the flag yourself
         //
         void RemoveFromNodeTree();
   };
   static_assert(sizeof(NiAVObject) == 0xA8, "RE::NiAVObject is not the right size!");

   class LoadedAreaBound : public NiRefObject {
      public:
         virtual ~LoadedAreaBound();
         //
         bhkAabbPhantom* unk08; // bhkAabbPhantom*
         bhkAabbPhantom* unk0C; // bhkAabbPhantom*
         bhkAabbPhantom* unk10; // bhkAabbPhantom*
         bhkAabbPhantom* unk14; // bhkAabbPhantom*
         bhkAabbPhantom* unk18; // bhkAabbPhantom*
         bhkAabbPhantom* unk1C; // bhkAabbPhantom*
         TESObjectCELL* unk20; // TESObjectCELL*
         TESObjectCELL* cell;  // 24
         UInt32 unk28;
         UInt32 unk2C;
         UInt32 unk30;
         UInt32 unk34;
         UInt32 unk38; // inited 0xDEADBEEF
         UInt32 unk3C;
         UInt32 unk40;
         NiPoint3 boundsMax; // 44
         NiPoint3 boundsMin; // 50
         float  unk5C; // ==   20.0 when observed
         float  unk60; // ==  600.0 when observed
         float  unk64; // == 1000.0 when observed
         float  unk68; // ==    0.3 when observed
   };
   static_assert(sizeof(LoadedAreaBound) == 0x6C, "LoadedAreaBound is the wrong size.");
   static_assert(offsetof(LoadedAreaBound, boundsMax) == 0x44, "LoadedAreaBound::boundsMax is at the wrong offset.");

   class BSMultiBoundShape : public NiObject {
      public:
         static constexpr uint32_t vtbl = 0x0111EC34;
         virtual uint32_t Unk_21(); // pure // returns an enum; probably "get type"
         virtual float    GetRadius(); // 22 // pure
         virtual uint32_t Unk_23(uint32_t); // pure
         virtual bool     Unk_24(uint32_t); // pure
         virtual bool     Unk_25(uint32_t); // pure
         virtual bool     Unk_26(); // pure
         virtual void     ToBound(NiBound& out); // 27 // pure
         virtual bool     ContainsPoint(const NiPoint3&); // 28
         virtual void     SetPosition(const NiPoint3&); // 29
         virtual bool     Unk_2A(uint32_t);
         //
         uint32_t unk08 = 0;
         //
         MEMBER_FN_PREFIX(BSMultiBoundShape);
         DEFINE_MEMBER_FN(Constructor, BSMultiBoundShape&, 0x00B0A2C0);
   };
   class BSMultiBoundAABB : public BSMultiBoundShape { // sizeof == 0x30
      public:
         static constexpr uint32_t vtbl = 0x0107DB4C;
         //
         uint32_t unk0C;
         NiPoint3 position; // 10
         uint32_t unk1C;
         NiPoint3 halfwidths; // 20
         uint32_t unk2C;
         //
         MEMBER_FN_PREFIX(BSMultiBoundAABB);
         DEFINE_MEMBER_FN(Constructor,  BSMultiBoundAABB&, 0x00AFBC20);
         DEFINE_MEMBER_FN(Constructor2, BSMultiBoundAABB&, 0x00469970, const NiPoint3& position, const NiPoint3& halfwidths);
   };
   class BSMultiBoundOBB : public BSMultiBoundAABB { // sizeof == 0x58
      public:
         static constexpr uint32_t vtbl = 0x0107DBFC;
         //
         NiMatrix33 rotation; // 30
         //
         MEMBER_FN_PREFIX(BSMultiBoundOBB);
         DEFINE_MEMBER_FN(Constructor, BSMultiBoundOBB&, 0x004699D0, const NiPoint3& position, const NiPoint3& halfwidths, const NiMatrix33& rotation);
   };
   class BSMultiBoundSphere : public BSMultiBoundShape { // sizeof == 0x1C
      public:
         static constexpr uint32_t vtbl = 0x0107DA2C;
         //
         NiPoint3 position; // 0C // centerpoint
         float    radius;   // 18
         //
         MEMBER_FN_PREFIX(BSMultiBoundSphere);
         DEFINE_MEMBER_FN(Constructor, BSMultiBoundSphere&, 0x00B0A470);
   };

   class BSMultiBound : public NiObject { // sizeof == 0x10
      public:
         static constexpr uint32_t vtbl = 0x0111D954;
         virtual bool Unk_21(uint32_t);
         virtual void SetPosition(const NiPoint3&); // 22 // no-op
         //
         BSMultiBoundNode* unk08 = nullptr; // possibly "owner"; NOT a smart pointer
         NiPointer<BSMultiBoundShape> shape = nullptr; // 0C
         //
         MEMBER_FN_PREFIX(BSMultiBound);
         DEFINE_MEMBER_FN(Constructor, BSMultiBound&, 0x00AFA620);
         DEFINE_MEMBER_FN(SetShape, void, 0x0046ACC0, BSMultiBoundShape*); // refcounted class?
   };

   DEFINE_SUBROUTINE_EXTERN(bhkCollisionObject*, GetBhkCollisionObjectForNiObject, 0x0046A240, NiObject* obj); // returns obj->unk1C ? obj->unk1C : obj->GetAsBhkCollisionObject();
   DEFINE_SUBROUTINE_EXTERN(bool,                NiObjectIs,                       0x0042A960, const NiRTTI*, const NiObject*);
   DEFINE_SUBROUTINE_EXTERN(bool,                NodeTreeContainsGeomMorpherController, 0x00B06360, NiNode* obj);
   static DEFINE_SUBROUTINE(float, GetMassOfNiObject, 0x00588C40, NiObject*);
   static DEFINE_SUBROUTINE(bool, NiObjectContainsAController, 0x004D79A0, NiObjectNET*); // tests if the argument or any descendants/properties have a controller
   static DEFINE_SUBROUTINE(void, RemoveCollisionFromNiAVObject, 0x00588BB0, NiAVObject*); // removes collision; if object is a node, also removes collision from all descendants

   // calls NiTimeController::Start for the object's controller and any controllers in its tree
   // seems to be called on NiControllerManagers; tests on other objects e.g. nodes don't work
   static DEFINE_SUBROUTINE(void, StartAllNiControllersInObject, 0x00ABF960, NiAVObject*);
   static DEFINE_SUBROUTINE(void, StartAllNiControllersInObjectWithTime, 0x00ABFA20, NiAVObject*, float startTime);

   static DEFINE_SUBROUTINE(void, FlagCollisionForReset, 0x00D171D0, NiAVObject*, bool one); // finds all NiCollisionObjects and sets the "reset" flag on them
};