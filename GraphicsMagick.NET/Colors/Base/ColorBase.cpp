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
#include "ColorBase.h"
#include "..\..\Quantum.h"

namespace GraphicsMagick
{
	//==============================================================================================
	ColorBase::ColorBase(bool hasAlpha)
	{
		_Value = gcnew MagickColor();
		_Value->A = Quantum::Max;

		_HasAlpha = hasAlpha;
	}
	//==============================================================================================
	ColorBase::ColorBase(bool hasAlpha, MagickColor^ color)
	{
		Throw::IfNull("color", color);

		_Value = gcnew MagickColor(color);

		if (!hasAlpha)
			_Value->A = Quantum::Max;

		_HasAlpha = hasAlpha;
	}
	//==============================================================================================
	MagickColor^ColorBase::Value::get()
	{
		return _Value;
	}
	//==============================================================================================
	void ColorBase::UpdateValue()
	{
	}
	//==============================================================================================
	bool ColorBase::operator == (ColorBase^ left, ColorBase^ right)
	{
		return Object::Equals(left, right);
	}
	//==============================================================================================
	bool ColorBase::operator != (ColorBase^ left, ColorBase^ right)
	{
		return !Object::Equals(left, right);
	}
	//==============================================================================================
	bool ColorBase::operator > (ColorBase^ left, ColorBase^ right)
	{
		if (ReferenceEquals(left, nullptr))
			return ReferenceEquals(right, nullptr);

		return left->CompareTo(right) == 1;
	}
	//==============================================================================================
	bool ColorBase::operator < (ColorBase^ left, ColorBase^ right)
	{
		if (ReferenceEquals(left, nullptr))
			return !ReferenceEquals(right, nullptr);

		return left->CompareTo(right) == -1;
	}
	//==============================================================================================
	bool ColorBase::operator >= (ColorBase^ left, ColorBase^ right)
	{
		if (ReferenceEquals(left, nullptr))
			return ReferenceEquals(right, nullptr);

		return left->CompareTo(right) >= 0;
	}
	//==============================================================================================
	bool ColorBase::operator <= (ColorBase^ left, ColorBase^ right)
	{
		if (ReferenceEquals(left, nullptr))
			return !ReferenceEquals(right, nullptr);

		return left->CompareTo(right) <= 0;
	}
	//==============================================================================================
	ColorBase::operator MagickColor^ (ColorBase^ color)
	{
		if (color == nullptr)
			return nullptr;

		return color->ToMagickColor();
	}
	//==============================================================================================
	int ColorBase::CompareTo(ColorBase^ other)
	{
		if (ReferenceEquals(other, nullptr))
			return 1;

		UpdateValue();
		other->UpdateValue();

		return Value->CompareTo(other->Value);
	}
	//==============================================================================================
	bool ColorBase::Equals(Object^ obj)
	{
		return Equals(dynamic_cast<ColorBase^>(obj));
	}
	//==============================================================================================
	bool ColorBase::Equals(ColorBase^ other)
	{
		if (ReferenceEquals(other, nullptr))
			return false;

		if (ReferenceEquals(this, other))
			return true;

		UpdateValue();
		other->UpdateValue();

		return Value->Equals(other->Value);
	}
	//==============================================================================================
	int ColorBase::GetHashCode()
	{
		return Value->GetHashCode();
	}
	//==============================================================================================
	MagickColor^ ColorBase::ToMagickColor()
	{	
		UpdateValue();

		MagickColor^ magickColor = gcnew MagickColor(_Value);

		if (!_HasAlpha)
			magickColor->A = Quantum::Max;

		return magickColor;
	}
	//==============================================================================================
}