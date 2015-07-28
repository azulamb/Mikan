#pragma once

#include <dsound.h>

#ifndef _UNLINK_LIBRARY
#  pragma comment(lib, "dsound.lib")
//#pragma comment(lib, "dxguid.lib")
#  ifdef _MIKANDEV
#    ifndef _DEBUG
#      pragma comment(lib, "Dev/MikanSound.lib")
#    else
#      pragma comment(lib, "Dev/MikanSoundD.lib")
#    endif
#  else
#    ifndef _DEBUG
#      pragma comment(lib, "MikanSound.lib")
#    else
#      pragma comment(lib, "MikanSoundD.lib")
#    endif
#  endif
#endif

#ifdef _MIKANDEV
#ifdef USEMIKANLIBRARY
#include "Dev/Thaleia.h"
#else
#include "Thaleia.h"
#endif
#else
#include "Thaleia.h"
#endif
#define CLASSDEF_DIRECTX_SOUND class ThaleiaSound

//公開Soundクラス
class MikanSoundPub
{
public:
	virtual ~MikanSoundPub() {}

public:
	//!サウンドを読み込む
	/*!
		\param sound_number 読込先のサウンド番号
		\param file_path 読込元のサウンドファイル
		\param streaming ストリーミング再生を行うかどうか
		\retval true 成功
		\retval false 失敗
		*/
	virtual bool Load( int sound_number, const char* file_path, bool streaming = false );
	//!サウンドを読み込む
	/*!
		\param sound_number 読込先のサウンド番号
		\param module モジュールハンドル。
		\param resource 読込元のサウンドファイルリソース名(リソースタイプはSOUND)。
		\param streaming ストリーミング再生を行うかどうか
		\retval true 成功
		\retval false 失敗
		*/
	virtual bool Load( int sound_number, HMODULE module, const char* resource );

	//!サウンドを破棄する
	/*!
		\param sound_number 破棄するサウンド番号
		*/
	virtual void Unload( int sound_number );

	//!全サウンドを破棄する
	virtual void UnloadAll();

	//!再生する
	/*!
		\param sound_number 再生するサウンド番号
		\param loop ループ再生するかどうか
		\retval true 成功
		\retval false 失敗
		*/
	virtual bool Play( int sound_number, bool loop );
	//!再生しているかどうか調べる
	/*!
		\param sound_number 再生を確認するサウンド番号
		\retval true 再生している
		\retval false 再生していない
		*/
	virtual bool IsPlaying( int sound_number );

	//!一時停止する
	/*!
		\param sound_number 一時停止するサウンド番号
		*/
	virtual void Pause( int sound_number );

	//!全てのサウンドを一時停止する
	virtual void PauseAll();

	//!停止する
	/*!
		\param sound_number 停止するサウンド番号
		*/
	virtual void Stop( int sound_number );

	//!全てのサウンドを停止する
	virtual void StopAll( void );

	//!音量を取得する
	/*!
		\param sound_number 音量を取得するサウンド番号
		\retval 0(無音)～100(最大)で表現される音量
		*/
	virtual int GetVolume( int sound_number );

	//!音量を設定する
	/*!
		\param sound_number 音量を設定するサウンド番号
		\param volume 0(無音)～100(最大)で表現される音量
		*/
	virtual void SetVolume( int sound_number, int volume );

	//!定位を取得する
	/*!
		\param sound_number 定位を取得するサウンド番号
		\retval -100(最左)～0(中央)～100(最右)で表現される定位
		*/
	virtual int GetPan( int sound_number );

	//!定位を設定する
	/*!
		\param sound_number 定位を設定するサウンド番号
		\param pan -100(最左)～0(中央)～100(最右)で表現される定位
		*/
	virtual void SetPan( int sound_number, int pan );

	//!再生周波数を取得する
	/*!
		\param sound_number 再生周波数を取得するサウンド番号
		\retval 再生周波数
		*/
	virtual unsigned long GetFrequency( int sound_number );

	//!再生周波数を設定する
	/*!
		\param sound_number 再生周波数を取得するサウンド番号
		\param frequency 再生周波数
		*/
	virtual void SetFrequency( int sound_number, unsigned long frequency );

	//!再生位置を取得する
	/*!
		\param sound_number 再生時間を取得するサウンド番号
		\retval 再生位置（ミリ秒単位）
		*/
	virtual unsigned long Tell( int sound_number );

	//!再生位置を設定する
	/*!
		\param sound_number 再生位置を設定するサウンド番号
		\param time 再生位置（ミリ秒単位）
		*/
	virtual void Seek( int sound_number, unsigned long time );

	//!ループ位置を取得する
	/*!
		\param sound_number ループ位置を取得するサウンド番号
		\param loop_start ループ開始位置を格納する変数へのポインタ NULLを指定してもよい
		\param loop_end ループ終了位置を格納する変数へのポインタ NULLを指定してもよい
		\retval true 成功
		\retval false 失敗
		*/
	virtual bool GetLoop( int sound_number, unsigned long* loop_start, unsigned long* loop_end );

	//!ループ位置を設定する
	/*!
		\param sound_number ループ位置を取得するサウンド番号
		\param loop_start ループ開始位置
		\param loop_end ループ終了位置
		\retval true 成功
		\retval false 失敗
		*/
	virtual bool SetLoop( int sound_number, unsigned long loop_start, unsigned long loop_end );
};

//非公開Soundクラス(実装)
//  ただのラッパーなのでヘッダに直書きする。
//  Thaleiaを使いたい人がいればこれをサンプルとして欲しい。
class ThaleiaSound
{
private:
	static const int MAX_SOUND_LENGTH = 50; //サウンドの最大数

private:
	IDirectSound8* m_directsound; //DirectSoundへのポインタ
	//Thaleia::IPcmPlayer* m_players[MAX_SOUND_LENGTH]; //サウンドリスト
	std::unique_ptr<Thaleia::IVoice> m_players[ MAX_SOUND_LENGTH ];
public:
	ThaleiaSound() :
		m_directsound( NULL )
	{
		//サウンド初期化
		for ( int i = 0; i < MAX_SOUND_LENGTH; i++ )
			m_players[ i ] = NULL;
	}

	~ThaleiaSound()
	{
		StopAll();
		UnloadAll();
	}

	//MikanSound
public:
	bool Load( int sound_number, const char* file_path, bool streaming )
	{
		if ( m_directsound == NULL )
			return false;

		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return false;

		//ファイルパスからファイル生成
		//Thaleia::IFile* file = Thaleia::createFile(file_path, !streaming);
		auto file = Thaleia::createFile( file_path, !streaming );
		if ( file == NULL )
			return false;

		//ファイルタイプ推定
		Thaleia::FileType file_type = Thaleia::suggestFileType( file );

		//PCMハンドラ生成
		//Thaleia::IPcmHandler* pcm = Thaleia::createPcmHandler(file_type);
		auto pcm = Thaleia::createPcmHandler( file_type );
		if ( pcm == NULL )
		{
			//delete file;
			return false;
		}

		//PCMハンドラのソースとして先ほどのファイルを指定する
		//if(!pcm->bind(file))
		if ( !pcm->bind( std::move( file ) ) )
		{
			//delete pcm;
			//delete file;
			return false;
		}

		Unload( sound_number );

		//PCMプレイヤー生成
		//WAVEファイルに対しては非リングバッファプレイヤーを、
		//それ以外のファイルに対してはリングバッファプレイヤーを用意する。
		//Thaleia::IPcmPlayer* player = Thaleia::createPlayer(m_directsound, (file_type!=Thaleia::FILETYPE_WAVE));
		//std::unique_ptr<Thaleia::IVoice> player
		m_players[ sound_number ] = Thaleia::createPlayer( m_directsound, ( file_type != Thaleia::FILETYPE_WAVE ) );
		if ( m_players[ sound_number ] == NULL )
		{
			//delete pcm; //この時点でfileはpcmの管理下にあるので、pcmのみdeleteすればよい
			return false;
		}

		//タグ情報からループを設定する
		{
			int loop_start = -1;
			int loop_end = -1;
			int loop_length = -1;
			int tag_length = pcm->getTagLength(); //タグの個数を取得
			for ( int i = 0; i < tag_length; i++ ) //全てのタグを走査
			{
				//"属性=値"の形式から値を読み取る
				/*const char* str = pcm->getTag(i);
				if(strstr(str, "LOOPSTART=") == str)
				loop_start = atoi(str+10);
				else if(strstr(str, "LOOPEND=") == str)
				loop_end = atoi(str+8);
				else if(strstr(str, "LOOPLENGTH=") == str)
				loop_length = atoi(str+11);*/
				std::string tag = pcm->getTag( i );
				int equal_position = tag.find( "=" );
				if ( equal_position == std::string::npos )
					continue;

				std::string element = tag.substr( 0, equal_position );
				std::string value = tag.substr( equal_position + 1 );
				if ( element == "LOOPSTART" )
					loop_start = atoi( value.c_str() );
				else if ( element == "LOOPEND" )
					loop_end = atoi( value.c_str() );
				else if ( element == "LOOPLENGTH" )
					loop_length = atoi( value.c_str() );
			}

			//LOOPSTARTとLOOPENDが指定されているケース
			if ( loop_start >= 0 && loop_end > loop_start )
			{
				m_players[ sound_number ]->setLoopTime( loop_start, loop_end ); //ループ位置設定
				//        m_players[sound_number]->enableLoop(true);
			}
			//LOOPSTARTとLOOPLENGTHが指定されているケース（RPGツクールの指定フォーマット）
			else if ( loop_start >= 0 && loop_length > 0 )
			{
				m_players[ sound_number ]->setLoopTime( loop_start, loop_start + loop_length );
				//        m_players[sound_number]->enableLoop(true);
			}
		}

		//PCMプレイヤーのソースとして先ほどのPCMハンドラを指定する
		//if(!player->bind(pcm))
		if ( !m_players[ sound_number ]->bind( std::move( pcm ) ) )
		{
			//delete player;
			//delete pcm;
			return false;
		}

		//サウンドリストに反映
		//Unload(sound_number); //先に指定番号にあったサウンドはUnloadされる
		//m_players[sound_number] = player;

		//再生準備完了
		return true;
	}

	bool Load( int sound_number, HMODULE module, const char* resource )
	{
		if ( m_directsound == NULL )
			return false;

		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return false;

		//ファイルパスからファイル生成
		auto file = Thaleia::createResourceFile( module, resource );
		if ( file == NULL )
			return false;

		//ファイルタイプ推定
		Thaleia::FileType file_type = Thaleia::suggestFileType( file );

		//PCMハンドラ生成
		auto pcm = Thaleia::createPcmHandler( file_type );
		if ( pcm == NULL )
		{
			return false;
		}

		//PCMハンドラのソースとして先ほどのファイルを指定する
		if ( !pcm->bind( std::move( file ) ) )
		{
			return false;
		}

		Unload( sound_number );

		//PCMプレイヤー生成
		//WAVEファイルに対しては非リングバッファプレイヤーを、
		//それ以外のファイルに対してはリングバッファプレイヤーを用意する。
		m_players[ sound_number ] = Thaleia::createPlayer( m_directsound, ( file_type != Thaleia::FILETYPE_WAVE ) );
		if ( m_players[ sound_number ] == NULL )
		{
			//delete pcm; //この時点でfileはpcmの管理下にあるので、pcmのみdeleteすればよい
			return false;
		}

		//タグ情報からループを設定する
		{
			int loop_start = -1;
			int loop_end = -1;
			int loop_length = -1;
			int tag_length = pcm->getTagLength(); //タグの個数を取得
			for ( int i = 0; i < tag_length; i++ ) //全てのタグを走査
			{
				std::string tag = pcm->getTag( i );
				int equal_position = tag.find( "=" );
				if ( equal_position == std::string::npos )
					continue;

				std::string element = tag.substr( 0, equal_position );
				std::string value = tag.substr( equal_position + 1 );
				if ( element == "LOOPSTART" )
					loop_start = atoi( value.c_str() );
				else if ( element == "LOOPEND" )
					loop_end = atoi( value.c_str() );
				else if ( element == "LOOPLENGTH" )
					loop_length = atoi( value.c_str() );
			}

			//LOOPSTARTとLOOPENDが指定されているケース
			if ( loop_start >= 0 && loop_end > loop_start )
			{
				m_players[ sound_number ]->setLoopTime( loop_start, loop_end ); //ループ位置設定
			}
			//LOOPSTARTとLOOPLENGTHが指定されているケース（RPGツクールの指定フォーマット）
			else if ( loop_start >= 0 && loop_length > 0 )
			{
				m_players[ sound_number ]->setLoopTime( loop_start, loop_start + loop_length );
			}
		}

		//PCMプレイヤーのソースとして先ほどのPCMハンドラを指定する
		if ( !m_players[ sound_number ]->bind( std::move( pcm ) ) )
		{
			return false;
		}

		//再生準備完了
		return true;
	}

	void Unload( int sound_number )
	{
		//引数の範囲検査
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return;

		if ( m_players[ sound_number ] != NULL )
		{
			//deleteしてNULLを代入しておく
			//delete m_players[sound_number];
			m_players[ sound_number ] = NULL;
		}
	}

	void UnloadAll()
	{
		for ( int i = 0; i < MAX_SOUND_LENGTH; i++ )
		{
			//Unload()を毎回呼ぶと引数の範囲検査が無駄なので自前で書く
			if ( m_players[ i ] != NULL )
			{
				//delete m_players[i];
				m_players[ i ] = NULL;
			}
		}
	}

	bool Play( int sound_number, bool loop )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return false;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return false;

		//player->setLoop(loop);
		m_players[ sound_number ]->enableLoop( loop );
		m_players[ sound_number ]->play();

		return true;
	}

	bool IsPlaying( int sound_number )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return false;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return false;

		return m_players[ sound_number ]->isPlaying();
	}

	void Pause( int sound_number )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] != NULL )
			m_players[ sound_number ]->pause();
	}

	void PauseAll()
	{
		for ( int i = 0; i < MAX_SOUND_LENGTH; i++ )
		{
			//Thaleia::IPcmPlayer* player = m_players[i];
			//std::unique_ptr<Thaleia::IVoice> player = m_players[i];
			if ( m_players[ i ] != NULL )
				m_players[ i ]->pause();
		}
	}

	void Stop( int sound_number )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] != NULL )
			m_players[ sound_number ]->stop();
	}

	void StopAll()
	{
		for ( int i = 0; i < MAX_SOUND_LENGTH; i++ )
		{
			//Thaleia::IPcmPlayer* player = m_players[i];
			//std::unique_ptr<Thaleia::IVoice> player = m_players[i];
			if ( m_players[ i ] != NULL )
				m_players[ i ]->stop();
		}
	}

	int GetVolume( int sound_number )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return 0;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return 0;

		return (int)( m_players[ sound_number ]->getVolume() * 100 );
	}

	void SetVolume( int sound_number, int volume )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return;

		m_players[ sound_number ]->setVolume( (double)volume / 100 );
	}

	int GetPan( int sound_number )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return 0;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return 0;

		return (int)( m_players[ sound_number ]->getPan() * 100 );
	}

	void SetPan( int sound_number, int pan )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return;

		m_players[ sound_number ]->setPan( (double)pan / 100 );
	}

	unsigned long GetFrequency( int sound_number )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return 0;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return 0;

		return m_players[ sound_number ]->getFrequency();
	}

	void SetFrequency( int sound_number, unsigned long frequency )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return;

		m_players[ sound_number ]->setFrequency( frequency );
	}

	/*  unsigned long Size( int sound_number )
	  {
	  if(sound_number < 0 || sound_number >= MAX_SOUND_LENGTH)
	  return 0;

	  if(m_players[sound_number] == NULL)
	  return 0;

	  return m_players[sound_number]->size();
	  }*/

	unsigned long Tell( int sound_number )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return 0;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return 0;

		return m_players[ sound_number ]->tell();
	}

	void Seek( int sound_number, unsigned long time )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return;

		m_players[ sound_number ]->seek( time );
	}

	bool GetLoop( int sound_number, unsigned long* loop_start, unsigned long* loop_end )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return false;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return false;

		return m_players[ sound_number ]->getLoopTime( loop_start, loop_end );
	}

	bool SetLoop( int sound_number, unsigned long loop_start, unsigned long loop_end )
	{
		if ( sound_number < 0 || sound_number >= MAX_SOUND_LENGTH )
			return false;

		//Thaleia::IPcmPlayer* player = m_players[sound_number];
		//std::unique_ptr<Thaleia::IVoice> player = m_players[sound_number];
		if ( m_players[ sound_number ] == NULL )
			return false;

		return m_players[ sound_number ]->setLoopTime( loop_start, loop_end );
	}

	//ThaleiaSound
public:
	//!初期化関数
	/*!
		\param window_handle メインウィンドウのハンドル
		\retval true 成功
		\retval false 失敗
		*/
	bool Initialize( HWND window_handle )
	{
		if ( m_directsound != NULL )
			return false;

		HRESULT ret;

		//DirectSound生成
		ret = DirectSoundCreate8( NULL, &m_directsound, NULL );
		if ( ret != DS_OK )
			return false;

		//協調レベル設定
		ret = m_directsound->SetCooperativeLevel( window_handle, DSSCL_PRIORITY );
		if ( ret != DS_OK )
			return false;

		//完了
		return true;
	}

};
