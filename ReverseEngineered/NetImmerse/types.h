#pragma once
#include "skse/NiTypes.h"
#include "skse/Utilities.h"

namespace RE {
   struct NiPoint3 : public ::NiPoint3 {
      public:
         NiPoint3() {
            this->x = 0.0F;
            this->y = 0.0F;
            this->z = 0.0F;
         };
         NiPoint3(float X, float Y, float Z) {
            this->x = X;
            this->y = Y;
            this->z = Z;
         };
         NiPoint3(const ::NiPoint3& other) {
            *this = other;
         }
         //
         inline static NiPoint3* as(::NiPoint3& other) { return (NiPoint3*) &other; };
         //
         inline NiPoint3& operator=(const ::NiPoint3& other) {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
            return *this;
         }
         //
         MEMBER_FN_PREFIX(NiPoint3);
         DEFINE_MEMBER_FN(Add,              NiPoint3*, 0x00420320, NiPoint3& out, const NiPoint3& other);
         DEFINE_MEMBER_FN(Add_B,            NiPoint3&, 0x00420470, const NiPoint3& other); // modifies and returns self
         DEFINE_MEMBER_FN(CompareDistance,  int,       0x004ADDF0, const NiPoint3& other, float compareTo); // return (*this - other).CompareLength(compareTo);
         DEFINE_MEMBER_FN(CompareLength,    int,       0x004ADD70, float length); // if this->GetLength() < length, returns -1; if greater, returns 1; if equal, returns 0
         DEFINE_MEMBER_FN(GetLength,        float,     0x0040A4B0);
         DEFINE_MEMBER_FN(GetLengthSquared, float,     0x004310E0); // if you're comparing the length to a constant, it's faster to compare the squared length to the squared constant
         DEFINE_MEMBER_FN(Multiply,         NiPoint3*, 0x004203A0, NiPoint3& out, const float other);
         DEFINE_MEMBER_FN(Negate,           NiPoint3*, 0x0045C680, NiPoint3& out);
         DEFINE_MEMBER_FN(NotEqualTo,       bool,      0x0040B7D0, const NiPoint3& other); // Bethesda operator!=
         DEFINE_MEMBER_FN(Subtract,         NiPoint3*, 0x00420360, NiPoint3& out, const NiPoint3& other);
         DEFINE_MEMBER_FN(VectorNormalize,  void,      0x00420500);
         DEFINE_MEMBER_FN(VectorNormalize2, float,     0x00431120); // returns old length

         inline NiPoint3 operator-(const NiPoint3& other) const noexcept {
            NiPoint3 out = *this;
            out.x -= other.x;
            out.y -= other.y;
            out.z -= other.z;
            return out;
         }
         inline NiPoint3& operator+=(const NiPoint3& other) noexcept {
            this->x += other.x;
            this->y += other.y;
            this->z += other.z;
            return *this;
         }
         inline bool operator==(const NiPoint3& other) const noexcept {
            return this->x == other.x && this->y == other.y && this->z == other.z;
         }
         inline bool operator!=(const NiPoint3& other) const noexcept { return !(*this == other); }

         inline float length_squared() const noexcept {
            return x*x + y*y + z*z;
         }
   };

   class NiBound { // sizeof == 0x10
      public:
         NiPoint3	pos;
         float    radius;
         //
         MEMBER_FN_PREFIX(NiBound);
         DEFINE_MEMBER_FN(Recalculate, void, 0x00AC4370, uint32_t vertexCount, NiPoint3* vertices);
   };

   class NiQuaternion {
      public:
         float w;
         float x;
         float y;
         float z;

         MEMBER_FN_PREFIX(NiQuaternion);
         DEFINE_MEMBER_FN(FromMatrix, void, 0x00AC0260, const NiMatrix33&);
         DEFINE_MEMBER_FN(ToMatrix, NiMatrix33&, 0x004719A0, NiMatrix33& out);
   };

   class NiMatrix33 : public ::NiMatrix33 { // sizeof == 0x24
      public:
         MEMBER_FN_PREFIX(NiMatrix33);
         DEFINE_MEMBER_FN(ConstructFromEuler, void,        0x00AAB810, float x, float y, float z);
         DEFINE_MEMBER_FN(ConstructFromPitch, void,        0x004A7E70, float);
         DEFINE_MEMBER_FN(ConstructFromYaw,   void,        0x0045C6C0, float);
         DEFINE_MEMBER_FN(ConvertPositionToThisFrame, NiPoint3*, 0x00474CD0, NiPoint3& out, const NiPoint3& in);
         DEFINE_MEMBER_FN(Multiply,           NiMatrix33*, 0x00447ED0, NiMatrix33& out, const NiMatrix33& other);

         inline NiMatrix33& operator=(const NiMatrix33& other) { memcpy(this, &other, sizeof(NiMatrix33)); return *this; }
         inline NiMatrix33& operator=(const ::NiMatrix33& other) { memcpy(this, &other, sizeof(NiMatrix33)); return *this; }

         static NiMatrix33 ConstructFromEuler(float x, float y, float z) {
            /*//
            NiMatrix33 mX = ConstructFromX(x);
            NiMatrix33 mY = ConstructFromY(y);
            NiMatrix33 mZ = ConstructFromZ(z);
            NiMatrix33 scrap;
            CALL_MEMBER_FN(&mY, Multiply)(scrap, mZ);
            return *(CALL_MEMBER_FN(&mX, Multiply)(mY, scrap));
            //*/
            NiMatrix33 out;
            CALL_MEMBER_FN(&out, ConstructFromEuler)(x, y, z);
            return out;
         }
         inline static NiMatrix33 ConstructFromEuler(const RE::NiPoint3& rot) {
            return ConstructFromEuler(rot.x, rot.y, rot.z);
         }
         static NiMatrix33 ConstructFromX(float x);
         static NiMatrix33 ConstructFromY(float y);
         static NiMatrix33 ConstructFromZ(float z);

         //
         // Relative direction getters, given a matrix that represents a reference frame 
         // (e.g. an object rotation, camera rotation, trajectory, etc.):
         //
         inline NiPoint3 Forward() const {
            NiPoint3 dir;
            dir.x = data[0][1]; // Y+ == forward
            dir.y = data[1][1];
            dir.z = data[2][1];
            CALL_MEMBER_FN(RE::NiPoint3::as(dir), VectorNormalize)();
            return dir;
         };
         inline NiPoint3 Up() const {
            NiPoint3 dir;
            dir.x = data[0][2]; // Z+ == up
            dir.y = data[1][2];
            dir.z = data[2][2];
            CALL_MEMBER_FN(RE::NiPoint3::as(dir), VectorNormalize)();
            return dir;
         };
         //
         // could add getters for down, left, right, etc.; maybe later
         //
   };
   class NiTransform { // sizeof == 0x34
      public:
	      NiMatrix33 rot;   // 00
	      NiPoint3   pos;   // 24
	      float      scale; // 30
         //
         NiTransform() { CALL_MEMBER_FN(this, Constructor)(); }
         void Apply(const NiTransform& other) {
            NiTransform scratch;
            *this = CALL_MEMBER_FN(this, Apply)(scratch, other);
         }
         //
         MEMBER_FN_PREFIX(NiTransform);
         DEFINE_MEMBER_FN(Constructor, NiTransform&, 0x00ABD300);
         DEFINE_MEMBER_FN(Apply, NiTransform&, 0x004EC9C0, NiTransform& out, const NiTransform& other); // parent_world.Apply(scrap, child_local);
         //
         inline static NiTransform& from(::NiTransform& out) { return *(RE::NiTransform*)(&out); }
         inline static NiTransform& from(NiTransform& out) { return out; }
   };
}