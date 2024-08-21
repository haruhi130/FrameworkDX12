#pragma once

namespace ConstantBufferData
{
	struct Camera
	{
		Math::Matrix mView;
		Math::Matrix mProj;
		Math::Matrix mProjInv;

		Math::Vector3 CamPos;
		int blank = 0;
	};
}