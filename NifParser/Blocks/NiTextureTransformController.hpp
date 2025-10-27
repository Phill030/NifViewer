#pragma once
#include <cstdint>
#include "../../Types/Ref.hpp"
#include "../NifHeader.hpp"
#include "NiFloatInterpController.hpp"

enum class TexType : uint32_t
{
    BASE_MAP = 0,  // The basic texture used by most meshes.
    DARK_MAP = 1,  // Used to darken the model with false lighting.
    DETAIL_MAP = 2,  // Combined with base map for added detail. Usually tiled many times.
    GLOSS_MAP = 3,  // Controls surface specularity (glossiness).
    GLOW_MAP = 4,  // Creates a glowing/incandescence effect.
    BUMP_MAP = 5,  // Adds apparent detail through height perturbation.
    NORMAL_MAP = 6,  // Adds apparent detail via normal perturbation.
    PARALLAX_MAP = 7, // Parallax mapping for depth illusion.
    DECAL_0_MAP = 8,  // Sticker-like overlay on surface.
    DECAL_1_MAP = 9,  // Sticker-like overlay on surface.
    DECAL_2_MAP = 10, // Sticker-like overlay on surface.
    DECAL_3_MAP = 11  // Sticker-like overlay on surface.
};

enum class TransformMember : uint32_t
{
    TT_TRANSLATE_U = 0, // Controls the translation of the U coordinates.
    TT_TRANSLATE_V = 1, // Controls the translation of the V coordinates.
    TT_ROTATE = 2, // Controls the rotation of the coordinates.
    TT_SCALE_U = 3, // Controls the scale of the U coordinates.
    TT_SCALE_V = 4  // Controls the scale of the V coordinates.
};

/*
Used to animate a single member of an NiTextureTransform.
NiInterpController::GetCtlrID() string formats:
['%1-%2-TT_TRANSLATE_U', '%1-%2-TT_TRANSLATE_V', '%1-%2-TT_ROTATE', '%1-%2-TT_SCALE_U', '%1-%2-TT_SCALE_V']
(Depending on "Operation" enumeration, %1 = Value of "Shader Map", %2 = Value of "Texture Slot")

*/
struct NiTextureTransformController : NiFloatInterpController
{
public:
	bool shaderMap;
	TexType textureSlot;
	TransformMember operation;
    NiTextureTransformController(Reader& reader) : NiFloatInterpController(reader) {
        shaderMap = reader.read<bool>();
        textureSlot = static_cast<TexType>(reader.read<uint32_t>());
        operation = static_cast<TransformMember>(reader.read<uint32_t>());
	}
};