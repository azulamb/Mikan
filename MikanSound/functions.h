#pragma once

struct IDirectSound; //IDirectSoundëOï˚êÈåæ

namespace Thaleia
{

	enum FileType
	{
		FILETYPE_UNKNOWN,
		FILETYPE_WAVE,
		FILETYPE_MP3,
		FILETYPE_OGG,
		FILETYPE_MIDI,
		FILETYPE_BMP,
		FILETYPE_JPEG,
		FILETYPE_GIF,
		FILETYPE_PNG
	};

	extern std::unique_ptr<IFile> createFile( const char* file_path, bool cache );
	extern std::unique_ptr<IFile> createResourceFile( HMODULE module, const char* resource );
	extern std::unique_ptr<IAudioHandler> createPcmHandler( FileType file_type );
	extern std::unique_ptr<IVoice> createPlayer( IDirectSound* directsound, bool ring );

	extern FileType suggestFileType( const std::unique_ptr<IFile>& file );

};