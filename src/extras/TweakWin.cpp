//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////


#include "TweakWin.h"
#include "Tweak/Tweakable.h"
#include "imgui.h"

#include "../rwbase.h"
#include "../rwplg.h"
#include "../rwengine.h"
#include "../rwpipeline.h"
#include "../rwobjects.h"
#include "../rwrender.h"
#include "../vulkan/vklighting.h"
#include "../vulkan/rwvk.h"
#include "vulkan/VulkanDebug.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace maple
{
	auto TweakWin::render() -> void
	{
		if (ImGui::Begin("Tweakables"))
		{
			for (auto& values : getValues())
			{
				if (ImGui::CollapsingHeader(values.first.c_str()))
				{
					for (auto& value : values.second)
					{
						switch (value->varType)
						{
						case TweakType::TweakInt32:
						{
							auto tweak = static_cast<TweakInt32*>(value);
							ImGui::SliderInt(tweak->varName, &tweak->var, tweak->lowerBound, tweak->upperBound);
						}
						break;
						case TweakType::TweakBool: {
							auto tweak = static_cast<TweakBool*>(value);
							ImGui::Checkbox(tweak->varName, &tweak->var);
						}
												 break;
						case TweakType::TweakFloat: {
							auto tweak = static_cast<TweakFloat*>(value);
							ImGui::SliderFloat(tweak->varName, &tweak->var, tweak->lowerBound, tweak->upperBound);
						}
												  break;
						case TweakType::TweakDouble: {
							auto tweak = static_cast<TweakDouble*>(value);
							ImGui::InputDouble(tweak->varName, &tweak->var);
						}
												   break;
						case TweakType::LENGTH:
							break;
						default:
							break;
						}
					}
				}
			}
		}
		ImGui::End();
	}

#define POINT_LIGHT_RADIUS_FACTOR 0.05f
	const float LightRadius = 100.0f;
	float LightConeAngle = 45.0f;
	rw::RGBA LightSolidColor = { 255, 255, 0, 255 };

	static maple::TweakBool EnableDebug = {"Lighting:Draw Debug", false};

	inline auto drawPointLight(const rw::LightData& light)
	{
		enum { NUMVERTS = 50 };

		rw::vulkan::Im3DVertex shape[NUMVERTS];
		rw::int32 i;
		rw::V3d point;

		const rw::V3d* pos = &light.position;
		for (i = 0; i < NUMVERTS; i++) {
			point.x = pos->x +
				cosf(i / (NUMVERTS / 2.0f) * M_PI) * light.radius;// *POINT_LIGHT_RADIUS_FACTOR;
			point.y = pos->y +
				sinf(i / (NUMVERTS / 2.0f) * M_PI) * light.radius;// *POINT_LIGHT_RADIUS_FACTOR;
			point.z = pos->z;

			shape[i].setColor(LightSolidColor.red, LightSolidColor.green,
				LightSolidColor.blue, LightSolidColor.alpha);
			shape[i].setX(point.x);
			shape[i].setY(point.y);
			shape[i].setZ(point.z);
		}

		rw::im3d::Transform(shape, NUMVERTS, nil, rw::im3d::ALLOPAQUE);
		rw::im3d::RenderPrimitive(rw::PRIMTYPEPOLYLINE);
		//rw::im3d::RenderLine(NUMVERTS - 1, 0);
		rw::im3d::End();
	}

	auto ObjectsWin::render() -> void
	{
		if (!EnableDebug.var)
			return;

		auto z = rw::GetRenderState(rw::ZTESTENABLE);
		auto cullMode = rw::GetRenderState(rw::CULLMODE);

		rw::SetRenderStatePtr(rw::TEXTURERASTER, nil);
		rw::SetRenderState(rw::CULLMODE, rw::CULLNONE);
		rw::SetRenderState(rw::ZTESTENABLE, 1);
		auto lights = rw::lighting::enumerateLights();

		maple::debug_utils::cmdBeginLabel("Lighting Debug");

		for (auto& light : lights)
		{
			drawPointLight(light);
		}

		maple::debug_utils::cmdEndLabel();

		rw::SetRenderState(rw::CULLMODE, cullMode);
		rw::SetRenderState(rw::ZTESTENABLE, z);
		rw::vulkan::endLastPipeline();
	}
};