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
#include "..\Helpers\FileHelper.h"
#include "MagickReader.h"

namespace GraphicsMagick
{
	//==============================================================================================
	int MagickReader::GetExpectedLength(MagickReadSettings^ readSettings)
	{
		int length = readSettings->Width.Value * readSettings->Height.Value * readSettings->PixelStorage->Mapping->Length;
		switch (readSettings->PixelStorage->StorageType)
		{
		case StorageType::Char:
			return length;
		case StorageType::Double:
			return length * sizeof(double);
		case  StorageType::Float:
			return length * sizeof(float);
		case StorageType::Integer:
			return length * sizeof(int);
		case StorageType::Long:
			return length * sizeof(long);
		case StorageType::Short:
			return length * sizeof(short);
		default:
			throw gcnew NotImplementedException();
		}
	}
	//==============================================================================================
	void MagickReader::ReadPixels(Magick::Image* image, MagickReadSettings^ readSettings,
		array<Byte>^ pixels)
	{
		Throw::IfNull("readSettings", readSettings->Width, "Width should be defined when pixel storage is set.");
		Throw::IfNull("readSettings", readSettings->Height, "Height should be defined when pixel storage is set.");
		Throw::IfNullOrEmpty("readSettings", readSettings->PixelStorage->Mapping, "Pixel storage mapping should be defined.");

		int length = GetExpectedLength(readSettings);
		Throw::IfTrue("pixels", pixels->Length != length, "The array length is " + pixels->Length + " but should be " + length + ".");

		void* convertedPixels = NULL;
		try
		{
			convertedPixels = Marshaller::Marshal(pixels, readSettings->PixelStorage->StorageType);

			std::string map;
			Marshaller::Marshal(readSettings->PixelStorage->Mapping, map);

			Magick::Geometry size = Magick::Geometry();
			image->read(readSettings->Width.Value, readSettings->Height.Value, map,
				(MagickLib::StorageType)readSettings->PixelStorage->StorageType, convertedPixels);
		}
		finally
		{
			delete[] convertedPixels;
		}
	}
	//==============================================================================================
	array<Byte>^ MagickReader::ReadUnchecked(String^ filePath)
	{
		FileStream^ stream = File::OpenRead(filePath);
		array<Byte>^ result = Read(stream);
		delete stream;

		return result;
	}
	//==============================================================================================
	MagickException^ MagickReader::Read(Magick::Image* image, array<Byte>^ bytes,
		MagickReadSettings^ readSettings)
	{
		Throw::IfNullOrEmpty("bytes", bytes);

		Magick::Blob blob;

		try
		{
			if (readSettings != nullptr)
			{
				if (readSettings->Ping)
				{
					Marshaller::Marshal(bytes, &blob);
					image->ping(blob);
					return nullptr;
				}
				else if (readSettings->PixelStorage != nullptr)
				{
					ReadPixels(image, readSettings, bytes);
					return nullptr;
				}

				readSettings->Apply(image);
			}

			Marshaller::Marshal(bytes, &blob);
			image->read(blob);

			return nullptr;
		}
		catch (Magick::Exception& exception)
		{
			return MagickException::Create(exception);
		}
	}
	//==============================================================================================
	MagickException^ MagickReader::Read(Magick::Image* image, MagickColor^ color, int width, int height)
	{
		Throw::IfNull("color", color);

		const Magick::Geometry* geometry = new Magick::Geometry(width, height);
		const Magick::Color* background = color->CreateColor();
		try
		{
			image->read(*geometry, "xc:" + (std::string)*background);

			return nullptr;
		}
		catch (Magick::Exception& exception)
		{
			return MagickException::Create(exception);
		}
		finally
		{
			delete geometry;
			delete background;
		}
	}
	//==============================================================================================
	MagickException^ MagickReader::Read(Magick::Image* image, Stream^ stream,
		MagickReadSettings^ readSettings)
	{
		return Read(image, Read(stream), readSettings);
	}
	//==============================================================================================
	MagickException^ MagickReader::Read(Magick::Image* image, String^ fileName, int width, int height)
	{
		String^ filePath = FileHelper::CheckForBaseDirectory(fileName);
		Throw::IfInvalidFileName(filePath);

		const Magick::Geometry* geometry = new Magick::Geometry(width, height);
		try
		{
			std::string imageSpec;
			Marshaller::Marshal(filePath, imageSpec);

			image->read(*geometry, imageSpec);

			return nullptr;
		}
		catch (Magick::Exception& exception)
		{
			return MagickException::Create(exception);
		}
		finally
		{
			delete geometry;
		}
	} 
	//==============================================================================================
	MagickException^ MagickReader::Read(Magick::Image* image, String^ fileName,
		MagickReadSettings^ readSettings)
	{
		String^ filePath = FileHelper::CheckForBaseDirectory(fileName);
		Throw::IfInvalidFileName(filePath);

		unsigned char *pixels = NULL;

		try
		{
			std::string imageSpec;
			Marshaller::Marshal(filePath, imageSpec);

			if (readSettings != nullptr)
			{
				if (readSettings->Ping)
				{
					image->ping(imageSpec);
					return nullptr;
				}
				else if (readSettings->PixelStorage != nullptr)
				{
					array<Byte>^ bytes = ReadUnchecked(filePath);
					ReadPixels(image, readSettings, bytes);
					return nullptr;
				}

				readSettings->Apply(image);
			}

			image->read(imageSpec);

			return nullptr;
		}
		catch (Magick::Exception& exception)
		{
			return MagickException::Create(exception);
		}
		finally
		{
			if (pixels != NULL)
				delete[] pixels;
		}
	}
	//==============================================================================================
	MagickException^ MagickReader::Read(std::list<Magick::Image>* imageList, array<Byte>^ bytes,
		MagickReadSettings^ readSettings)
	{
		Throw::IfNull("readSettings", readSettings);
		Throw::IfNullOrEmpty("bytes", bytes);

		MagickLib::ImageInfo* imageInfo = MagickLib::CloneImageInfo((MagickLib::ImageInfo *) NULL);
		MagickLib::GetImageInfo(imageInfo);

		MagickLib::ExceptionInfo exceptionInfo;
		MagickLib::GetExceptionInfo(&exceptionInfo);

		unsigned char* data = NULL;

		try
		{
			Throw::IfTrue("readSettings", readSettings->PixelStorage != nullptr,
				"PixelStorage is not supported for images with multiple frames/layers.");

			if (!readSettings->Ping)
				readSettings->Apply(imageInfo);

			data = Marshaller::Marshal(bytes);
			MagickLib::Image *images;

			if (readSettings->Ping)
				images = MagickLib::PingBlob(imageInfo, data, bytes->Length, &exceptionInfo);
			else
				images = MagickLib::BlobToImage(imageInfo, data, bytes->Length, &exceptionInfo);

			Magick::insertImages(imageList, images);
			Magick::throwException(exceptionInfo, readSettings->IgnoreWarnings);
			MagickLib::DestroyExceptionInfo(&exceptionInfo);

			return nullptr;
		}
		catch (Magick::Exception& exception)
		{
			return MagickException::Create(exception);
		}
		finally
		{
			MagickLib::DestroyImageInfo(imageInfo);
			if (data != NULL)
				delete[] data;
		}
	}
	//==============================================================================================
	MagickException^ MagickReader::Read(std::list<Magick::Image>* imageList, Stream^ stream,
		MagickReadSettings^ readSettings)
	{
		return Read(imageList, Read(stream), readSettings);
	}	
	//==============================================================================================
	MagickException^ MagickReader::Read(std::list<Magick::Image>* imageList, String^ fileName,
		MagickReadSettings^ readSettings)
	{
		Throw::IfNull("readSettings", readSettings);

		String^ filePath = FileHelper::CheckForBaseDirectory(fileName);
		Throw::IfInvalidFileName(filePath);

		MagickLib::ImageInfo* imageInfo = MagickLib::CloneImageInfo((MagickLib::ImageInfo *) NULL);
		MagickLib::GetImageInfo(imageInfo);

		MagickLib::ExceptionInfo exceptionInfo;
		MagickLib::GetExceptionInfo(&exceptionInfo);

		try
		{
			Throw::IfTrue("readSettings", readSettings->PixelStorage != nullptr,
				"PixelStorage is not supported for images with multiple frames/layers.");

			if (!readSettings->Ping)
				readSettings->Apply(imageInfo);

			std::string imageSpec;
			Marshaller::Marshal(filePath, imageSpec);

			MagickLib::MagickStrlCpy(imageInfo->filename, imageSpec.c_str(), MaxTextExtent - 1);

			MagickLib::Image* images;
			if (readSettings->Ping)
				images = MagickLib::PingImage(imageInfo, &exceptionInfo);
			else
				images = MagickLib::ReadImage(imageInfo, &exceptionInfo);
			Magick::insertImages(imageList, images);
			Magick::throwException(exceptionInfo, readSettings->IgnoreWarnings);
			MagickLib::DestroyExceptionInfo(&exceptionInfo);

			return nullptr;
		}
		catch (Magick::Exception& exception)
		{
			return MagickException::Create(exception);
		}
		finally
		{
			MagickLib::DestroyImageInfo(imageInfo);
		}
	}
	//==============================================================================================
	array<Byte>^ MagickReader::Read(Stream^ stream)
	{
		Throw::IfNull("stream", stream);

		if (stream->CanSeek)
		{
			int length = (int)stream->Length;
			if (length == 0)
				return gcnew array<Byte>(0);

			array<Byte>^ result = gcnew array<Byte>(length);
			stream->Read(result, 0, length);
			return result;
		}
		else
		{
			const int bufferSize = 8192;
			MemoryStream^ memStream = gcnew MemoryStream();

			array<Byte>^ buffer =  gcnew array<Byte>(bufferSize);
			int length;
			while ((length = stream->Read(buffer, 0, bufferSize)) != 0)
			{
				memStream->Write(buffer, 0, length);
			}

			array<Byte>^ result = memStream->ToArray();
			delete memStream;

			return result;
		}
	}
	//==============================================================================================
	array<Byte>^ MagickReader::Read(String^ fileName)
	{
		String^ filePath = FileHelper::CheckForBaseDirectory(fileName);
		Throw::IfInvalidFileName(filePath);

		return ReadUnchecked(fileName);
	}
	//==============================================================================================
}