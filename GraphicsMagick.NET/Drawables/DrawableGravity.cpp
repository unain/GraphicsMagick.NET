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
#include "Stdafx.h"
#include "DrawableGravity.h"

namespace GraphicsMagick
{
	//==============================================================================================
	DrawableGravity::DrawableGravity(GraphicsMagick::Gravity gravity)
	{
		BaseValue = new Magick::DrawableGravity((Magick::GravityType)gravity);
	}
	//==============================================================================================
	GraphicsMagick::Gravity DrawableGravity::Gravity::get()
	{
		return (GraphicsMagick::Gravity)Value->gravity();
	}
	//==============================================================================================
	void DrawableGravity::Gravity::set(GraphicsMagick::Gravity value)
	{
		Value->gravity((Magick::GravityType)value);
	}
	//==============================================================================================
}