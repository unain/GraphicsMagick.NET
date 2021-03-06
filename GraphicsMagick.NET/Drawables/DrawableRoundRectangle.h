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

#include "Base\DrawableWrapper.h"

using namespace System::Drawing;

namespace GraphicsMagick
{
	///=============================================================================================
	///<summary>
	/// Encapsulation of the DrawableRoundRectangle object.
	///</summary>
	public ref class DrawableRoundRectangle sealed : DrawableWrapper<Magick::DrawableRoundRectangle>
	{
		//===========================================================================================
	public:
		///==========================================================================================
		///<summary>
		/// Creates a new DrawableRoundRectangle instance.
		///</summary>
		///<param name="centerX">The center X coordinate.</param>
		///<param name="centerY">The center Y coordinate.</param>
		///<param name="width">The width.</param>
		///<param name="height">The height.</param>
		///<param name="cornerWidth">The corner width.</param>
		///<param name="cornerHeight">The corner height.</param>
		DrawableRoundRectangle(double centerX, double centerY, double width, double height,
			double cornerWidth, double cornerHeight);
		///==========================================================================================
		///<summary>
		/// The center X coordinate.
		///</summary>
		property double CenterX
		{
			double get();
			void set(double value);
		}
		///==========================================================================================
		///<summary>
		/// The center Y coordinate.
		///</summary>
		property double CenterY
		{
			double get();
			void set(double value);
		}
		///==========================================================================================
		///<summary>
		/// The corner height.
		///</summary>
		property double CornerHeight
		{
			double get();
			void set(double value);
		}
		///==========================================================================================
		///<summary>
		/// The corner width.
		///</summary>
		property double CornerWidth
		{
			double get();
			void set(double value);
		}
		///==========================================================================================
		///<summary>
		/// The height.
		///</summary>
		property double Height
		{
			double get();
			void set(double value);
		}
		///==========================================================================================
		///<summary>
		/// The width.
		///</summary>
		property double Width
		{
			double get();
			void set(double value);
		}
		//===========================================================================================
	};
	//==============================================================================================
}