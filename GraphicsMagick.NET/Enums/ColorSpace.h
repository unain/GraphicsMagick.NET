//=================================================================================================
// Copyright 2014-2015 Dirk Lemstra <https://graphicsmagick.codeplex.com/>
//
// Licensed under the ImageMagick License (the "License"); you may not use this file except in 
// compliance with the License. You may obtain a copy of the License at
//
//   http://www.imagemagick.org/script/license.php
//
// Unless required by applicable law or agreed to in writing, software distributed under the
// License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
// express or implied. See the License for the specific language governing permissions and
// limitations under the License.
//=================================================================================================
#pragma once

#include "Stdafx.h"

namespace GraphicsMagick
{
	///=============================================================================================
	///<summary>
	/// Specifies a kind of color space.
	///</summary>
	public enum class ColorSpace
	{
		Undefined = Magick::UndefinedColorspace,
		RGB = Magick::RGBColorspace,
		GRAY = Magick::GRAYColorspace,
		Transparent = Magick::TransparentColorspace,
		OHTA = Magick::OHTAColorspace,
		XYZ = Magick::XYZColorspace,
		YCbCr = Magick::YCbCrColorspace,
		YCC = Magick::YCCColorspace,
		YIQ = Magick::YIQColorspace,
		YPbPr = Magick::YPbPrColorspace,
		YUV = Magick::YUVColorspace,
		CMYK = Magick::CMYKColorspace,
		sRGB = Magick::sRGBColorspace,
		HSL = Magick::HSLColorspace,
		HWB = Magick::HWBColorspace,
		Rec601Luma = Magick::Rec601LumaColorspace,
		Rec601YCbCr = Magick::Rec601YCbCrColorspace,
		Rec709Luma = Magick::Rec709LumaColorspace,
		Rec709YCbCr = Magick::Rec709YCbCrColorspace,
	};
	//==============================================================================================
}