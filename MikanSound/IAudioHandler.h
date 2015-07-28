#pragma once

#include "IFile.h"

#include <memory>
#include <string>

namespace Thaleia
{
	/// Waveデータのフォーマットを定義する構造体.
	/// 引用: http://msdn.microsoft.com/ja-jp/library/cc371559.aspx
	struct WaveFormatEx
	{
		/// (wFormatTag)
		/// ウェーブフォーム オーディオ フォーマットのタイプ. 
		/// 多くの圧縮アルゴリズムのフォーマット タグが, Microsoft で登録されている. 
		/// フォーマット タグの完全なリストは, ヘッダー ファイル Mmreg.h に記述されている. 
		/// 1 チャンネルまたは 2 チャンネルの PCM データの場合, この値は WAVE_FORMAT_PCM でなければならない. 
		unsigned short format_tag;

		/// (nChannels)
		/// ウェーブフォーム オーディオ データのチャンネル数. 
		/// モノラル データは 1 つのチャンネルを使い, ステレオ データは 2 つのチャンネルを使う.  
		unsigned short channels;

		/// (nSamplesPerSec)
		/// サンプリング レート. 1 秒あたりのサンプル数 (Hz) で表す. 
		/// wFormatTag が WAVE_FORMAT_PCM の場合, nSamplesPerSec の一般的な値は 8.0 kHz, 11.025 kHz, 22.05 kHz, 44.1 kHz である. 
		/// PCM フォーマットでない場合は, メーカーのフォーマット タグの仕様に従ってこのメンバを計算する必要がある.  
		unsigned long  samples_per_second;

		/// (nAvgBytesPerSec)
		/// フォーマット タグで必要な平均データ転送速度. 単位は, 1 秒あたりのバイト数. 
		/// wFormatTag が WAVE_FORMAT_PCM である場合, nAvgBytesPerSec は nSamplesPerSec と nBlockAlign の積に等しくなければならない. 
		/// PCM フォーマットでない場合は, メーカーのフォーマット タグの仕様に従ってこのメンバを計算する必要がある.  
		unsigned long  average_bytes_per_second;

		/// (nBlockAlign)
		/// ブロック アライメント (バイト単位). 
		/// ブロック アライメントは, wFormatTag フォーマット タイプのデータの最小単位である. 
		/// wFormatTag が WAVE_FORMAT_PCM または WAVE_FORMAT_EXTENSIBLE である場合, 
		/// nBlockAlign は nChannels と wBitsPerSample の積を 8 で割った値 (バイトあたりのビット数) に等しくなければならない. 
		/// PCM フォーマットでない場合は, メーカーのフォーマット タグの仕様に従ってこのメンバを計算する必要がある. 
		///
		/// ソフトウェアは, データの複数の nBlockAlign バイトを, 一度に処理しなければならない. 
		/// デバイスに書き込むデータ, デバイスから読み込むデータは, 常にブロックの先頭から開始しなければならない. 
		/// たとえば, PCM データの再生を, サンプルの中間 (つまり, 非ブロック アライメントの境界) で開始することは不正である.  
		unsigned short block_align;

		/// (wBitsPerSample)
		/// wFormatTag フォーマット タイプの, サンプリングあたりのビット数. 
		/// wFormatTag が WAVE_FORMAT_PCM である場合, wBitsPerSample は 8 または 16 でなければならない. 
		/// PCM フォーマットでない場合は, メーカーのフォーマット タグの仕様に従ってこのメンバを設定する必要がある. 
		/// wFormatTag が WAVE_FORMAT_EXTENSIBLE である場合は, 任意の 8 の倍数を設定できる. 
		/// 圧縮スキームが wBitsPerSample の値を定義できないこともあるので, このメンバは 0 となる. 
		unsigned short bits_per_sample;

		/// (cbSize)
		/// WAVEFORMATEX 構造体の後ろに追加されるフォーマット情報のサイズ (バイト単位). 
		/// この情報を非 PCM フォーマットで使って, wFormatTag の追加属性を格納することができる. 
		/// wFormatTag で必要な追加情報がない場合, このメンバは 0 に設定しなければならない. 
		/// WAVE_FORMAT_PCM フォーマットでは (そして WAVE_FORMAT_PCM フォーマットの場合に限り), このメンバは無視される.  
		unsigned short extend_size;
	};

	/// オーディオファイルを扱うクラスのインターフェース
	///
	/// IFileインターフェースをもつインスタンスからオーディオファイルを読み込み,
	/// PCMデータ(波形データ)や再生に必要なパラメータを取得するためのインターフェースを提供する.
	class IAudioHandler
	{
	public:
		virtual ~IAudioHandler() {}

	public:
		/// オーディオファイルをこのインスタンスに結びつける.
		/// 以降, ファイルの所有権はこのインスタンスが保持する.
		/// 
		/// file	このクラスと結びつけるファイル. 読み取り準備が完了している必要がある.
		///
		/// returns	結び付けに成功すればture, さもなくばfalse.
		virtual bool bind( std::unique_ptr<IFile>&& file ) = 0;

		/// ファイルとの結びつけを解除する.
		/// ファイルの所有権は返却される.
		virtual std::unique_ptr<IFile> unbind() = 0;

		/// PCMデータを読み取る.
		///
		/// buffer	読み取ったデータの書き込み先.
		/// length	最大の読み取りバイト数.
		///
		/// returns	実際に読み取ったバイト数. 必ずlength以下の値になる.
		virtual unsigned long read( void* buffer, unsigned long length ) = 0;

		/// 現在のPCMデータ読み取り位置を得る
		///
		/// returns	現在の読み取り位置
		virtual unsigned long tell() const = 0;

		/// 現在のPCMデータ読み取り位置を変更する
		///
		/// offset		移動先.
		/// elatively	この引数がtureのとき, 現在の読み取り位置から相対移動する.
		virtual void seek( int offset, bool relatively = false ) = 0;

		/// PCMのサイズを得る
		///
		/// returns	PCMのサイズ
		virtual unsigned long size() const = 0;

		/// WAVEFORMATEX形式でフォーマットデータを得る
		virtual size_t getWaveFormatEx( void* format, size_t size ) const = 0;

		/// タグの数を取得する
		///
		/// returns	タグの数.
		virtual int getTagLength() const = 0;

		/// タグを取得する.
		///
		/// index	タグの番号. 0が最小, getTagLength()-1が最大.
		///
		/// returns	タグ. 属性と値の対応を持つタグは"属性=値"のように"="を区切り文字とした文字列を返す.
		virtual std::string getTag( int index ) const = 0;
	};

};