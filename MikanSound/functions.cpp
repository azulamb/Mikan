#include <stdio.h>
#include <string.h>

#include <windows.h>
#include <dsound.h>
#include "vorbis/vorbisfile.h"

#include "debug.h"

#include "Thaleia.h"

#include "DiskFile.h"
#include "MemoryFile.h"
#include "ResourceFile.h"

#include "WaveHandler.h"
#include "OggHandler.h"

#include "DirectSoundVoice.h"
#include "DirectSoundRingVoice.h"


namespace Thaleia
{

	std::unique_ptr<IFile> createFile( const char* file_path, bool cache )
	{
		if ( cache )
		{
			std::unique_ptr<MemoryFile> file( new MemoryFile() );
			if ( !file->load( file_path ) )
				return nullptr;

			return std::move( file );
		} else
		{
			std::unique_ptr<DiskFile> file( new DiskFile() );
			if ( !file->open( file_path ) )
				return nullptr;

			return std::move( file );
		}
	}

	std::unique_ptr<IFile> createResourceFile( HMODULE module, const char* resource )
	{

		std::unique_ptr<ResourceFile> file( new ResourceFile() );
		if ( !file->open( module, resource ) )
			return nullptr;

		return std::move( file );
	}

	std::unique_ptr<IAudioHandler> createPcmHandler( FileType file_type )
	{
		std::unique_ptr<IAudioHandler> pcm = nullptr;

		switch ( file_type )
		{
		case FILETYPE_WAVE:
			pcm = std::unique_ptr<IAudioHandler>( new WaveHandler() );
			break;

		case FILETYPE_OGG:
			pcm = std::unique_ptr<IAudioHandler>( new OggHandler() );
			break;
		}

		return pcm;
	}

	std::unique_ptr<IVoice> createPlayer( IDirectSound* directsound, bool ring )
	{
		std::unique_ptr<IVoice> player = nullptr;
		if ( ring )
			player = std::unique_ptr<IVoice>( new DirectSoundRingVoice( directsound ) );
		else
			player = std::unique_ptr<IVoice>( new DirectSoundVoice( directsound ) );

		return player;
	}

	FileType suggestFileType( const std::unique_ptr<IFile>& file )
	{
		if ( file == nullptr )
		{
			LOG( "Argument 'file' is nullptr." );
			return FILETYPE_UNKNOWN;
		}

		FileType suggested_type = FILETYPE_UNKNOWN;

		//現在位置保存
		unsigned long cursor = file->tell();

		//ヘッダ読み取り
		{
			char buffer[ 17 ];
			buffer[ 16 ] = '\0'; //オーバーランの保険
			char* str = buffer;

			file->seek( 0 );
			file->read( str, 16 );

			if ( strstr( str, "RIFF" ) == str )
			{
				//wave
				str += 8;
				if ( strstr( str, "WAVE" ) == str )
					suggested_type = FILETYPE_WAVE;
			} else if ( strstr( str, "OggS" ) == str )
			{
				//ogg
				suggested_type = FILETYPE_OGG;
			}
		}

		//元の位置に戻る
		file->seek( cursor );

		//完了
		return suggested_type;
	}

};
